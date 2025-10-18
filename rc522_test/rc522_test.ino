#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  2   // D4
#define RST_PIN 0   // D3
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();       // Init SPI bus
  rfid.PCD_Init();   // Init RC522
  Serial.println("RC522 Ready. Scan your card...");
}

void loop() {
  // Look for new card
  if (!rfid.PICC_IsNewCardPresent()) {
    delay(500);
    return;
  }

  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA(); // Stop reading
}
