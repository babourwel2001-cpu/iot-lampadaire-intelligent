#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==================== CONFIGURATION WIFI ====================
const char* ssid = "ssid";              // ← Partage mobile
const char* password = "password";           // ← Mot de passe

// ==================== CONFIGURATION MQTT ====================
const char* mqtt_server = "10.202.1.xxx";   // ← IP du PC
const int mqtt_port = 1883;
const char* mqtt_topic_data = "lampadaire/1/data";
const char* mqtt_topic_command = "lampadaire/1/command";

// ==================== CONFIGURATION LCD ====================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ==================== PINS CAPTEUR ====================
const int trigPin = 18;
const int echoPin = 19;

// ==================== VARIABLES ====================
WiFiClient espClient;
PubSubClient client(espClient);

long duration;
float distance_cm;
unsigned long lastMsg = 0;
const long interval = 2000;

// ==================== CALLBACK MQTT ====================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message recu [");
  Serial.print(topic);
  Serial.print("] : ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Commande:");
  lcd.setCursor(0, 1);
  lcd.print(message.substring(0, 16));
  delay(1500);
}

// ==================== CONNEXION WIFI ====================
void setupWiFi() {
  Serial.print("Connexion WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int tentatives = 0;
  while (WiFi.status() != WL_CONNECTED && tentatives < 40) {
    delay(500);
    Serial.print(".");
    tentatives++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connecte !");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Echec WiFi");
  }
}

// ==================== CONNEXION MQTT ====================
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connexion MQTT...");
    
    String clientId = "ESP32-Lampadaire-1-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connecte !");
      client.subscribe(mqtt_topic_command);
    } else {
      Serial.print("echec, rc=");
      Serial.print(client.state());
      Serial.println(" - nouvelle tentative dans 5s");
      delay(5000);
    }
  }
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test MQTT + Ultrason + LCD");
  
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Init MQTT...");
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  setupWiFi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  lcd.clear();
}

// ==================== LOOP ====================
void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH, 30000);
    
    String payload;
    if (duration == 0) {
      payload = "{\"lampadaire\":\"1\",\"distance_cm\":-1,\"status\":\"ERROR\"}";
      distance_cm = -1;
    } else {
      distance_cm = (duration / 2.0) / 29.1;
      payload = "{\"lampadaire\":\"1\",\"distance_cm\":" + String(distance_cm, 1) + ",\"status\":\"OK\"}";
    }
    
    client.publish(mqtt_topic_data, payload.c_str());
    Serial.print("Publie: ");
    Serial.println(payload);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    if (client.connected()) {
      lcd.print("MQTT OK");
    } else {
      lcd.print("MQTT OFF");
    }
    lcd.setCursor(0, 1);
    if (distance_cm < 0) {
      lcd.print("Hors portee");
    } else {
      lcd.print(distance_cm, 1);
      lcd.print(" cm");
    }
  }
}
