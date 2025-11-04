#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include "ui_page.h"   

// WiFi 
const char* ssid     = " ";   // Red WiFi
const char* password = " ";   // Contraseña

// Motor Izquierdo (M1) 
const int ENA_M1 = 32;  // Enable izquierdo 
const int IN1_M1 = 33;
const int IN2_M1 = 26;

// Pines Motor Derecho (M2)
const int ENB_M2 = 14;  // Enable derecho 
const int IN3_M2 = 25;
const int IN4_M2 = 27;

// Sensores MQ 
const int MQ7_PIN   = 34;  // ADC1_CH6
const int MQ9_PIN   = 35;  // ADC1_CH7
const int MQ135_PIN = 36;  // ADC1_CH0 (VP)

// DHT22 
#define DHTPIN  4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Servidor 
WebServer server(80);

// Muestreo 
unsigned long lastSampleMs = 0;
const uint32_t SAMPLE_PERIOD_MS = 1000;  // cada 1 s

// CMD 
void printCmd(const char* cmd){
  Serial.print("CMD: "); Serial.println(cmd);
}

void detenerMotores() {
  digitalWrite(ENA_M1, LOW);
  digitalWrite(ENB_M2, LOW);
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, LOW);
  digitalWrite(IN3_M2, LOW);
  digitalWrite(IN4_M2, LOW);
}

void moverAdelante() {
  // M1 adelante
  digitalWrite(IN1_M1, HIGH);
  digitalWrite(IN2_M1, LOW);
  digitalWrite(ENA_M1, HIGH);
  // M2 adelante
  digitalWrite(IN3_M2, HIGH);
  digitalWrite(IN4_M2, LOW);
  digitalWrite(ENB_M2, HIGH);
}

void moverAtras() {
  // M1 atrás
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, HIGH);
  digitalWrite(ENA_M1, HIGH);
  // M2 atrás
  digitalWrite(IN3_M2, LOW);
  digitalWrite(IN4_M2, HIGH);
  digitalWrite(ENB_M2, HIGH);
}

void girarIzquierda() {
  
  // Izquierda: M1 adelante, M2 atrás 
  digitalWrite(IN1_M1, HIGH);
  digitalWrite(IN2_M1, LOW);
  digitalWrite(ENA_M1, HIGH);

  digitalWrite(IN3_M2, LOW);
  digitalWrite(IN4_M2, HIGH);
  digitalWrite(ENB_M2, HIGH);
}

void girarDerecha() {
  // Derecha: M1 atrás, M2 adelante
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, HIGH);
  digitalWrite(ENA_M1, HIGH);

  digitalWrite(IN3_M2, HIGH);
  digitalWrite(IN4_M2, LOW);
  digitalWrite(ENB_M2, HIGH);
}

// Lectura de Sensores 
void leerYMostrarSensores() {
  
  int raw_mq7   = analogRead(MQ7_PIN);
  int raw_mq9   = analogRead(MQ9_PIN);
  int raw_mq135 = analogRead(MQ135_PIN);

  const float VREF = 3.3f;
  float v_mq7   = (raw_mq7   * VREF) / 4095.0f;
  float v_mq9   = (raw_mq9   * VREF) / 4095.0f;
  float v_mq135 = (raw_mq135 * VREF) / 4095.0f;

  // DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // En grados C

  // Esto se muestra en el Monitor Serial
  Serial.println(F(" Lectura Sensores "));
  Serial.printf("MQ7   : raw=%4d  V=%.3f V\n", raw_mq7, v_mq7);
  Serial.printf("MQ9   : raw=%4d  V=%.3f V\n", raw_mq9, v_mq9);
  Serial.printf("MQ135 : raw=%4d  V=%.3f V\n", raw_mq135, v_mq135);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("DHT22 : Sin Lectura "));
  } else {
    Serial.printf("DHT22 : T=%.1f °C  H=%.1f %%\n", t, h);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // Motores
  pinMode(ENA_M1, OUTPUT);
  pinMode(IN1_M1, OUTPUT);
  pinMode(IN2_M1, OUTPUT);
  pinMode(ENB_M2, OUTPUT);
  pinMode(IN3_M2, OUTPUT);
  pinMode(IN4_M2, OUTPUT);
  detenerMotores();

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);  // rango ampliado ~3.6V 

  // Esto es opcional si se desea por pin:
  // analogSetPinAttenuation(MQ7_PIN, ADC_11db);
  // analogSetPinAttenuation(MQ9_PIN, ADC_11db);
  // analogSetPinAttenuation(MQ135_PIN, ADC_11db);

  // DHT
  dht.begin();

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(400); Serial.print("."); }
  Serial.println();
  Serial.print("WiFi OK - IP: "); Serial.println(WiFi.localIP());

  // Rutas HTTP (sin HTML por ahora)
  server.on("/", [](){ server.send(200, "text/plain", "ESP32 CAR READY"); });

  server.on("/adelante", [](){
    printCmd("adelante");
    moverAdelante();
    server.send(200, "text/plain", "OK");
  });

  server.on("/atras", [](){
    printCmd("atras");
    moverAtras();
    server.send(200, "text/plain", "OK");
  });

  server.on("/izquierda", [](){
    printCmd("izquierda");
    girarIzquierda();
    server.send(200, "text/plain", "OK");
  });

  server.on("/derecha", [](){
    printCmd("derecha");
    girarDerecha();
    server.send(200, "text/plain", "OK");
  });

  server.on("/detener", [](){
    printCmd("detener");
    detenerMotores();
    server.send(200, "text/plain", "OK");
  });

  server.on("/status", [](){
    server.send(200, "text/plain", "alive");
  });

  server.begin();

  Serial.println(F("Sistema iniciado. Leyendo sensores cada 1s..."));
  Serial.println(F("Nota: Los MQ requieren un precalentamiento para una buena lectura. "));
}

void loop() {
  server.handleClient();

  unsigned long now = millis();
  if (now - lastSampleMs >= SAMPLE_PERIOD_MS) {
    lastSampleMs = now;
    leerYMostrarSensores();
  }
}
