#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

// Wi-Fi Credentials
const char* ssid = "Galaxy A32E648";
const char* password = "bsql6036";

// MQTT Broker
const char* mqtt_server = "broker.hivemq.com";

// ThingSpeak API
const char* thingspeak_api_key = "3RB7DRRR00NN0D6L"; // Replace with your ThingSpeak API Key
const char* thingspeak_url = "https://api.thingspeak.com/update";

// Twilio API
const char* twilio_account_sid = "ACfffa4835226fe81c98c72adc88ade92b"; // Replace with your Twilio Account SID
const char* twilio_auth_token = "7f9b3bc83288e90f729b5d819da4232e"; // Replace with your Twilio Auth Token
const char* twilio_phone_number = "+12315005701"; // Replace with your Twilio Phone Number
const char* subscribed_phone_number = "+94719397910"; // Replace with the recipient's phone number

// Pins
#define TRIG_PIN 5
#define ECHO_PIN 18
#define LED_RED 32
#define LED_GREEN 33
#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient espClient;
PubSubClient client(espClient);

bool alert_sent = false; // Flag to track if the alert has been sent

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Connect to Wi-Fi
  connectWiFi();

  // Connect to MQTT Broker
  client.setServer(mqtt_server, 1883);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Measure distance
  long duration;
  float distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Control LEDs based on distance
  if (distance > 0 && distance < 11) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    Serial.println("Status: Object Detected - RED LED ON");

    // Send SMS alert if not already sent
    if (!alert_sent) {
      sendTwilioSMS("Flood Warning: Flooding is imminent or occurring. Move to safey and avoid floodwaters!");
      alert_sent = true; // Set the flag to prevent multiple alerts
    }
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("Status: Safe Distance - GREEN LED ON");

    // Reset alert flag when object moves away
    if (alert_sent && distance >= 11) {
      alert_sent = false;
    }
  }

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  // Prepare JSON payload
  String payload = "{\"distance\":";
  payload += distance;
  payload += ",\"temperature\":";
  payload += temperature;
  payload += ",\"humidity\":";
  payload += humidity;
  payload += "}";

  // Publish data to MQTT topic
  client.publish("sensor/data", payload.c_str());
  Serial.println("Published data to MQTT: " + payload);

  // Send data to ThingSpeak
  sendToThingSpeak(distance, temperature, humidity);

  delay(1000);
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

void sendToThingSpeak(float distance, float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(thingspeak_url) + "?api_key=" + thingspeak_api_key +
                 "&field1=" + String(distance) +
                 "&field2=" + String(temperature) +
                 "&field3=" + String(humidity);

    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.println("ThingSpeak update successful: " + String(httpResponseCode));
    } else {
      Serial.println("Error sending data to ThingSpeak: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Cannot send to ThingSpeak.");
  }
}

void sendTwilioSMS(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(twilio_account_sid) + "/Messages.json";

    String postData = "To=" + String(subscribed_phone_number) + "&From=" + String(twilio_phone_number) + "&Body=" + message;

    http.begin(url);
    http.setAuthorization(twilio_account_sid, twilio_auth_token);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(postData);
    if (httpResponseCode > 0) {
      Serial.println("Twilio SMS sent successfully: " + String(httpResponseCode));
    } else {
      Serial.println("Error sending Twilio SMS: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Cannot send Twilio SMS.");
  }
}
