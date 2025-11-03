#include <WiFi.h>
#include <WebServer.h>

//   WiFi   
const char* ssid     = " ";  // Colocar Red a la que se conectara
const char* password = " ";

//   Pines Motor Izquierdo (M1)   
const int ENA_M1 = 32;  // Enable izquierdo
const int IN1_M1 = 33;
const int IN2_M1 = 26;  

//   Pines Motor Derecho (M2)   
const int ENB_M2 = 14;  // Enable derecho
const int IN3_M2 = 25;
const int IN4_M2 = 27;

WebServer server(80);


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
  // Izquierda: M1 atrás, M2 adelante
  digitalWrite(IN1_M1, HIGH);
  digitalWrite(IN2_M1, LOW);
  digitalWrite(ENA_M1, HIGH);

  digitalWrite(IN3_M2, LOW);
  digitalWrite(IN4_M2, HIGH);
  digitalWrite(ENB_M2, HIGH);
}

void girarDerecha() {
  // Derecha: M1 adelante, M2 atrás
  
  digitalWrite(IN1_M1, LOW);
  digitalWrite(IN2_M1, HIGH);
  digitalWrite(ENA_M1, HIGH);

  digitalWrite(IN3_M2, HIGH);
  digitalWrite(IN4_M2, LOW);
  digitalWrite(ENB_M2, HIGH);
}

void setup() {
  Serial.begin(115200);

  pinMode(ENA_M1,+ OUTPUT);
  pinMode(IN1_M1, OUTPUT);
  pinMode(IN2_M1, OUTPUT);
  pinMode(ENB_M2, OUTPUT);
  pinMode(IN3_M2, OUTPUT);
  pinMode(IN4_M2, OUTPUT);

  detenerMotores();

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(400); Serial.print("."); }
  Serial.println();
  Serial.print("WiFi OK - IP: "); Serial.println(WiFi.localIP());

  server.on("/", [](){
    server.send(200, "text/html", htmlPage());
  });

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
}

void loop() {
  server.handleClient();
}
