#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ====== Wi-Fi Wokwi ======
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";

// ====== Broker MQTT ======
const char* BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char* ID_MQTT = "esp32_soscidadao";
const char* TOPIC_PUBLISH = "sos/emergencia";

// ====== Pinos ======
#define LED_R 27
#define LED_G 25
#define LED_B 33
#define BUZZER_PIN 26
#define BUTTON_PIN 14
#define RAIN_SENSOR_PIN 34
#define GAS_SENSOR_PIN 35
#define TRIG_PIN 13
#define ECHO_PIN 12

bool alertaAtivo = false;

WiFiClient espClient;
PubSubClient client(espClient);

// ====== Conecta ao Wi-Fi ======
void setupWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void setupMQTT() {
  client.setServer(BROKER_MQTT, BROKER_PORT);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect(ID_MQTT)) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou. Estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void publicarAlerta(bool chuva, int nivelAgua, bool fumaca, bool botao) {
  StaticJsonDocument<256> doc;
  doc["chuva"] = chuva;
  doc["agua"] = nivelAgua;
  doc["fumaca"] = fumaca;
  doc["botao"] = botao;

  char mensagem[256];
  serializeJson(doc, mensagem);

  client.publish(TOPIC_PUBLISH, mensagem);
  Serial.println("\n📤 Alerta enviado: ");
  Serial.println(mensagem);
}

void ligarAlerta() {
  if (!alertaAtivo) {
    alertaAtivo = true;
    tone(BUZZER_PIN, 200);
    setLED(true, false, false); // vermelho
    Serial.println("🔴 Alerta ativado");
  }
}

void desligarAlerta() {
  if (alertaAtivo) {
    alertaAtivo = false;
    noTone(BUZZER_PIN);
    setLED(false, true, false); // verde
    Serial.println("🟢 Alerta desativado");
  }
}

void setLED(bool r, bool g, bool b) {
  digitalWrite(LED_R, !r);
  digitalWrite(LED_G, !g);
  digitalWrite(LED_B, !b);
}

int medirNivelAgua() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH);
  return duracao * 0.034 / 2; // em cm
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("🚀 Setup iniciado");

  desligarAlerta();
  setupWiFi();
  setupMQTT();
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  bool chuva = analogRead(RAIN_SENSOR_PIN) < 3000;
  bool fumaca = analogRead(GAS_SENSOR_PIN) > 2000;
  bool botao = digitalRead(BUTTON_PIN) == LOW;
  int nivelAgua = medirNivelAgua();

  Serial.print("💧 Nível da água: ");
  Serial.print(nivelAgua);
  Serial.println(" cm");

  if (botao) {
    desligarAlerta();
  } else if (chuva || fumaca || nivelAgua < 10) {
    ligarAlerta();
  } else {
    desligarAlerta();
  }

  publicarAlerta(chuva, nivelAgua, fumaca, botao);

  delay(500);
}
