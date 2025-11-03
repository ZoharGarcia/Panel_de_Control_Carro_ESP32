#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>

const char* ssid     = " ";    // Red
const char* password = " "; 


// Pines Motores M1 = Izquierdo, M2 = Derecho

const int ENA_M1 = 32;  
const int IN1_M1 = 33;  
const int IN2_M1 = 26;  

const int ENB_M2 = 14;  
const int IN3_M2 = 25;  
const int IN4_M2 = 27;  


// PWM (LEDC) 

const int PWM_FREQ = 20000;
const int PWM_RES  = 8;       }
const int CH_M1    = 0;
const int CH_M2    = 1;

// WebSocket

WebServer server(80);
WebSocketsServer ws(81);   // ws://<ip>:81

volatile bool estopActive = false;

static inline float clamp(float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); }
static inline int   pctToPwm(float pct){                
  pct = clamp(pct, 0, 100);
  return (int)roundf(pct * 255.0f / 100.0f);
}

void detenerMotores() {
  ledcWrite(CH_M1, 0);
  ledcWrite(CH_M2, 0);
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, LOW);
  digitalWrite(IN3_M2, LOW);
  digitalWrite(IN4_M2, LOW);
}

void setMotorPair(float left, float right, int pwmCap /*0..255*/) {
  // left/right en [-1..1]
  left  = clamp(left,  -1.0f, 1.0f);
  right = clamp(right, -1.0f, 1.0f);

  // M1 (izquierdo)
  if (left >= 0) {
    digitalWrite(IN1_M1, HIGH);
    digitalWrite(IN2_M1, LOW);
  } else {
    digitalWrite(IN1_M1, LOW);
    digitalWrite(IN2_M1, HIGH);
  }
  int pwmL = (int)roundf(fabs(left) * pwmCap);
  ledcWrite(CH_M1, pwmL);

  // M2 (derecho)
  if (right >= 0) {
    digitalWrite(IN3_M2, HIGH);
    digitalWrite(IN4_M2, LOW);
  } else {
    digitalWrite(IN3_M2, LOW);
    digitalWrite(IN4_M2, HIGH);
  }
  int pwmR = (int)roundf(fabs(right) * pwmCap);
  ledcWrite(CH_M2, pwmR);
}

// Mezcla diferencial a partir de x,y en [-1..1]
void driveFromXY(float x, float y, float maxPct) {
  if (estopActive) {
    detenerMotores();
    return;
  }
  // Mezcla: izq = y + x ; der = y - x
  float left  = y + x;
  float right = y - x;

  // Normaliza si excede 1
  float m = max(fabs(left), fabs(right));
  if (m > 1.0f) { left /= m; right /= m; }

  int cap = pctToPwm(maxPct);   // 0..255
  setMotorPair(left, right, cap);
}

// =====================
// HTTP Handlers
// =====================
void handleStatus() {
  // CORS
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "alive");
}
void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "ESP32 Car Control WS @ :81");
}
void handleOptions() { // preflight CORS por si lo usas
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

float parseFloatField(const String& s, const char* key, float defVal) {
  String pat = String("\"") + key + String("\"");
  int i = s.indexOf(pat);
  if (i < 0) return defVal;
  i = s.indexOf(':', i);
  if (i < 0) return defVal;
  int j = i + 1;
  while (j < (int)s.length() && (s[j] == ' ' || s[j] == '\t')) j++;
  int k = j;
  while (k < (int)s.length() && String("0123456789+-.eE").indexOf(s[k]) >= 0) k++;
  return s.substring(j, k).toFloat();
}

bool hasType(const String& s, const char* val){
  String pat = String("\"type\"");
  int i = s.indexOf(pat);
  if (i < 0) return false;
  i = s.indexOf(':', i);
  if (i < 0) return false;
  int q1 = s.indexOf('"', i+1);
  if (q1 < 0) return false;
  int q2 = s.indexOf('"', q1+1);
  if (q2 < 0) return false;
  String t = s.substring(q1+1, q2);
  t.toLowerCase();
  String vv(val); vv.toLowerCase();
  return t == vv;
}

void onWsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if (type == WStype_CONNECTED) {
    IPAddress ip = ws.remoteIP(num);
    Serial.printf("[WS] Cliente #%u conectado: %s\n", num, ip.toString().c_str());
    return;
  }
  if (type == WStype_DISCONNECTED) {
    Serial.printf("[WS] Cliente #%u desconectado\n", num);
    return;
  }
  if (type == WStype_TEXT) {
    String msg((char*)payload, length);
   
    if (hasType(msg, "STOP")) {
      estopActive = true;
      detenerMotores();
      ws.sendTXT(num, "{\"ok\":true,\"state\":\"ESTOP\"}");
      return;
    }
    if (hasType(msg, "cmd")) {
      float x   = parseFloatField(msg, "x",   0.0f);  // -1..1
      float y   = parseFloatField(msg, "y",   0.0f);  // -1..1
      float max = parseFloatField(msg, "max", 50.0f); // 0..100
      // Si estaba en E-STOP y llegan comandos, ignora hasta rearmar
      if (!estopActive) {
        driveFromXY(x, y, max);
      }
      ws.sendTXT(num, "{\"ok\":true}");
      return;
    }
    if (hasType(msg, "ARM")) {   
      estopActive = false;
      ws.sendTXT(num, "{\"ok\":true,\"state\":\"ARMED\"}");
      return;
    }
    // Esto es "mensaje no reconocido"
    ws.sendTXT(num, "{\"ok\":false,\"err\":\"unknown\"}");
  }
}


void setup() {
  Serial.begin(115200);

  // GPIO
  pinMode(IN1_M1, OUTPUT);
  pinMode(IN2_M1, OUTPUT);
  pinMode(IN3_M2, OUTPUT);
  pinMode(IN4_M2, OUTPUT);

  // PWM
  ledcSetup(CH_M1, PWM_FREQ, PWM_RES);
  ledcSetup(CH_M2, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA_M1, CH_M1);
  ledcAttachPin(ENB_M2, CH_M2);

  detenerMotores();

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(400); Serial.print("."); }
  Serial.println();
  Serial.print("WiFi OK - IP: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32car")) {
    Serial.println("mDNS: http://esp32car.local  |  WS: ws://esp32car.local:81");
  }

  // HTTP
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/", HTTP_OPTIONS, handleOptions);
  server.on("/status", HTTP_OPTIONS, handleOptions);
  server.begin();
  Serial.println("HTTP listo en :80");

  // WebSocket
  ws.begin();
  ws.onEvent(onWsEvent);
  Serial.println("WebSocket listo en :81");
}

void loop() {
  server.handleClient();
  ws.loop();
}
