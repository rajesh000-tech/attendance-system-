#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define SS_PIN  2    // D4
#define RST_PIN 0    // D3
MFRC522 rfid(SS_PIN, RST_PIN);

const char* ssid = "Conference Hall";
const char* password = "Datapro@123$";
const char* server = "api.thingspeak.com";
String apiKey = "W80QUNYN7NLZTNVC";

WiFiClient client;

// optional: associate UIDs to names
String getNameFromUID(String uid) {
  if (uid == "A3BA062F") return "Rocky";
  if (uid == "XXXXXXXX") return "User2";  // add more if needed
  return "Unknown";
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID Attendance System Starting...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected! IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("RC522 ready. Scan your card...");
}

void loop() {
  // Check for card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  // Read UID
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.print("Card UID: ");
  Serial.println(uidString);

  String name = getNameFromUID(uidString);
  Serial.println("User: " + name);

  // Prepare data
  String postStr = apiKey;
  postStr += "&field1=" + uidString;
  postStr += "&field2=" + name;
  postStr += "&field3=" + String(millis() / 1000);  // timestamp
  postStr += "\r\n\r\n";

  // Send to ThingSpeak
  if (client.connect(server, 80)) {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("Data sent to ThingSpeak!");
  }

  client.stop();
  rfid.PICC_HaltA();

  delay(15000); // ThingSpeak allows updates every 15 seconds
}
