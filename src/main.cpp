/* 
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 23      
#define DHTTYPE DHT22

BluetoothSerial BT;
DHT dht(DHTPIN, DHTTYPE);

uint8_t enderecoSlave[] = {0x1C, 0x69, 0x20, 0x92, 0x2D, 0x52};

unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 2000;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("=== ESP32 MASTER (DHT22) ===");
  Serial.println("Inicializando Bluetooth...");
  
  dht.begin();

  if (!BT.begin("ESP32_Master", true)) {
    Serial.println("Erro ao iniciar Bluetooth Master!");
    while (true) delay(1000);
  }
  Serial.println("Bluetooth Master iniciado");

  Serial.print("Conectando ao Slave... ");
  if (BT.connect(enderecoSlave)) {
    Serial.println("Conectado ao Slave!");
  } else {
    Serial.println("Falha na conexão ao Slave!");
  }
}

void loop() {
  unsigned long agora = millis();
  
  if (agora - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = agora;

    float h = NAN, t = NAN;
    for (int i = 0; i < 3; i++) {
      h = dht.readHumidity();
      t = dht.readTemperature();
      if (!isnan(h) && !isnan(t)) break;
      Serial.println("Erro ao ler DHT22! Tentando novamente...");
      delay(1000);
    }

    if (isnan(h) || isnan(t)) {
      Serial.println("Falha persistente na leitura do DHT22!");
      return;
    }

    float f = t * 1.8 + 32;

    StaticJsonDocument<128> doc;
    doc["umidade"] = h;
    doc["tempC"] = t;
    doc["tempF"] = f;

    String jsonString;
    serializeJson(doc, jsonString);

    BT.println(jsonString);
    Serial.print("Enviado JSON: ");
    Serial.println(jsonString);
  }
}
*/
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "DHT.h"
int valorGas;
JsonDocument doc;
BluetoothSerial SerialBT;
uint8_t BT[] = {0x1C, 0x69, 0x20, 0x92, 0x2D, 0x52};

void setup() {
  Serial.begin(9600);
    pinMode(27, OUTPUT);

  if(SerialBT.begin("Esp-Recebo", true)){
    Serial.println("Deu certo");
  }
  
  else{
    Serial.println("Deu errado");
  }

   if(SerialBT.connect(BT)){
    Serial.println("Conectado");
    digitalWrite(2, HIGH);
   }
  else{
    Serial.println("Nao conectado");
  }
 }
  String mensagemEnviada;

void loop() {
  delay(2000);
  valorGas = analogRead(27);
  doc["gas"] = valorGas;
  serializeJson(doc,mensagemEnviada);
  SerialBT.print(mensagemEnviada);
  Serial.println(mensagemEnviada);
}
