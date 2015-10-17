#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

int markerPin = 8;

byte lastUid[10];

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  pinMode(markerPin, OUTPUT);
  digitalWrite(markerPin, LOW); 
  memset(lastUid, 0, 10);
  Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  // Look for new cards
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  int uidSize = mfrc522.uid.size;
  byte uid[10];

  memcpy(uid, mfrc522.uid.uidByte, uidSize);

  if (memcmp(lastUid, uid, uidSize)) {
    memcpy(lastUid, uid, uidSize);
    digitalWrite(markerPin, HIGH);

    Serial.print(F("Card UID:"));
    for (byte i = 0; i < uidSize; i++) {
      if(uid[i] < 0x10)
        Serial.print(F(" 0"));
      else
        Serial.print(F(" "));
        
      Serial.print(uid[i], HEX);
    }
    Serial.println("");
  } else {
    digitalWrite(markerPin, LOW);
  }
}
