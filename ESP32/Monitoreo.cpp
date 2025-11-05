#include <WiFi.h>
#include <WebServer.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <HTTPClient.h>

// RED
const char* ssid = " ";       // WiFi
const char* password = " "; 
const char* firebaseHost = "carro-esp32-c0050-default-rtdb.firebaseio.com"; // Firebase

// DHT
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// PINES MOTORES
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

// PINES MQ
#define MQ7_PIN 34
#define MQ9_PIN 35
#define MQ135_PIN 32

// WEB SERVER
WebServer server(80);
unsigned long lastSend = 0;
unsigned long lastCmdCheck = 0;

// CONTROL DEL CARRO 
void detenerCarro() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void moverAdelante() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moverAtras() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void moverIzquierda() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moverDerecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

// FIREBASE
void pushToFirebase(int mq7, int mq9, int mq135, float temp, float hum) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = String("https://") + firebaseHost + "/sensores.json";

  String json = "{";
  json += "\"mq7\":" + String(mq7) + ",";
  json += "\"mq9\":" + String(mq9) + ",";
  json += "\"mq135\":" + String(mq135) + ",";
  json += "\"temp\":" + String(temp, 1) + ",";
  json += "\"hum\":" + String(hum, 1) + ",";
  json += "\"timestamp\":" + String(millis());
  json += "}";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(json);
  Serial.print("Firebase -> ");
  Serial.println(code > 0 ? "OK" : "Error");
  http.end();
}

void fetchCommandsFromFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = String("https://") + firebaseHost + "/comandos.json";

  http.begin(url);
  int code = http.GET();

  if (code == 200) {
    String payload = http.getString();
    Serial.println("Comando recibido: " + payload);

    if (payload.indexOf("adelante") >= 0) moverAdelante();
    else if (payload.indexOf("atras") >= 0) moverAtras();
    else if (payload.indexOf("izquierda") >= 0) moverIzquierda();
    else if (payload.indexOf("derecha") >= 0) moverDerecha();
    else if (payload.indexOf("detener") >= 0) detenerCarro();
  }

  http.end();
}

// Funcion Web
void handleRoot() {
  server.send(200, "text/plain", "Servidor ESP32 activo.");
}


void setup() {
  Serial.begin(115200);

  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  detenerCarro();

  // Sensores
  dht.begin();
  pinMode(MQ7_PIN, INPUT);
  pinMode(MQ9_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.println(WiFi.localIP());

  // Servidor local (opcional)
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();

  unsigned long now = millis();

  // Se envian los datos cada 3 segundos 
  if (now - lastSend > 3000) {
    lastSend = now;

    int mq7 = analogRead(MQ7_PIN);
    int mq9 = analogRead(MQ9_PIN);
    int mq135 = analogRead(MQ135_PIN);
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    Serial.printf("MQ7: %d | MQ9: %d | MQ135: %d | T: %.1f | H: %.1f\n",
                  mq7, mq9, mq135, temp, hum);

    pushToFirebase(mq7, mq9, mq135, temp, hum);
  }

  if (now - lastCmdCheck > 1500) {
    lastCmdCheck = now;
    fetchCommandsFromFirebase();
  }
}
