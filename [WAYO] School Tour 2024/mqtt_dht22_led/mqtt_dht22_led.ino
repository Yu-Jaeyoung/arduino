#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LED_PIN 5   // LED STRIP에 연결된 핀 번호
#define NUM_LEDS 4  // LED STRIP의 LED 개수
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// WiFi
const char* ssid = "Hanbat_WLAN_Guest";      // Enter your WiFi name
const char* password = "";  // Enter WiFi password

// MQTT Broker
const char* mqtt_server = "broker.emqx.io";  // Enter your WiFi or Ethernet IP
const int mqtt_port = 1883;
const char* topic = "여기를 바꿔주세요.";


WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(9600);

  dht.begin();

  strip.begin();
  strip.clear();
  strip.show();

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("여기를 바꿔주세요.")) {
      Serial.println("connected");
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  Serial.println(command);



  if (command.equals("ON") || command.equals("on")) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
  }

  if (command.equals("OFF") || command.equals("off")) {
    strip.clear();
    strip.show();
  }

  if (command.equals("MIX") || command.equals("mix")) {
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor((i + 0) % 4, strip.Color(255, 0, 0));      // 빨간색
      strip.setPixelColor((i + 1) % 4, strip.Color(0, 255, 0));      // 초록색
      strip.setPixelColor((i + 2) % 4, strip.Color(0, 0, 255));      // 파란색
      strip.setPixelColor((i + 3) % 4, strip.Color(255, 255, 255));  // 흰색
      strip.show();
      delay(700);
    }
    strip.clear();
    strip.show();
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    char msg[50];
    snprintf(msg, 50, "Humidity: %.2f%%, Temperature: %.2fC", humidity, temperature);
    client.publish(topic, msg);
  }
}