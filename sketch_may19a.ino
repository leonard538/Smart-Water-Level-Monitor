#include <WiFi.h>
#include <HTTPClient.h>
#define TRIG_PIN 5
#define ECHO_PIN 18

const char* ssid = "Nothing";
const char* password = "CRLL0639newp@ss";
const char* server = "http://api.thingspeak.com/update";
String apiKey = "HDHYSF5D2QKQ53YQ";

const int tankHeight = 15; // cm
const int ledPin = 15;
unsigned long lastUploadTime = 0;

// Segment pins for one-digit display
int seg[] = {21, 19, 22, 23, 32, 33, 25}; // a, b, c, d, e, f, g

// Segment codes for 0-9 (Common Cathode)
byte digitCode[10][7] = {
  {1,1,1,1,1,1,0}, // 0 
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  for(int i = 0; i < 7; i++) {
    pinMode(seg[i], OUTPUT);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

}

void loop() {
  // Trigger sensor
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // cm

  // Water level calculation
  float level = tankHeight - distance;
  int percent = (level / tankHeight) * 99;

  if (percent > 100) percent = 100;
  if (percent < 0) percent = 0;

  if(percent < 20) {
    digitalWrite(ledPin, HIGH);
    Serial.println("!!Water Level Low!!");
  } else {
    digitalWrite(ledPin, LOW);
  }

  int displayDigit = percent / 10;

  Serial.print("Distance: "); Serial.println(distance);
  Serial.print("Water Level: "); Serial.print(percent); Serial.println("%");

  // Show digit on 7-segment
  for(int i = 0; i < 7; i++) {
    digitalWrite(seg[i], digitCode[displayDigit][i] ? LOW : HIGH);
  }

  // Send data to ThingSpeak every 15 seconds
  if (WiFi.status() == WL_CONNECTED && millis() - lastUploadTime > 15000) {
  HTTPClient http;
  String url = String(server) + "?api_key=" + apiKey;
  url += "&field1=" + String(percent);             // Water level %
  url += "&field2=" + String(distance, 2);         // Distance in cm (2 decimal places)

  http.begin(url);
  int response = http.GET();
  if (response > 0) {
    Serial.print("ThingSpeak response: ");
    Serial.println(response);
  } else {
    Serial.print("Error sending data. Code: ");
    Serial.println(response);
  }
  http.end();
  lastUploadTime = millis();
}

  delay(5000);  // Run other logic every second (safe)
}
