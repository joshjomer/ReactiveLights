#include "config.h"

AdafruitIO_Feed *sharedFeed = io.feed("emotion", FEED_OWNER);

#define DATA_PIN 6   // DIN
#define CLOCK_PIN 10  // CLK
#define CS_PIN 9    // LOAD

void sendCommand(uint8_t address, uint8_t data) {
  digitalWrite(CS_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, address);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
  digitalWrite(CS_PIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  io.connect();
  sharedFeed->onMessage(handleMessage);
  sharedFeed->get();
  Serial.print("Connecting to Adafruit IO");
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  // Initialize MAX7219
  sendCommand(0x09, 0x00); // Decode mode: none
  sendCommand(0x0A, 0x03); // Intensity: 3 (range 0x00 to 0x0F)
  sendCommand(0x0B, 0x07); // Scan limit: 0–7 (all rows)
  sendCommand(0x0C, 0x01); // Shutdown register: normal operation
  sendCommand(0x0F, 0x00); // Display test: off

  // Clear all rows
  for (int i = 1; i <= 8; i++) {
    sendCommand(i, 0x00);
  }
  
}

void loop() {
  io.run();
}  


void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <-  ");
  Serial.println(data->toInt());
  if (data->toInt() == 0) {
    //Neutral
    sendCommand(1, 0b00000000); // Row 0
    sendCommand(2, 0b01100110); // Row 1
    sendCommand(3, 0b01100110); // Row 2
    sendCommand(4, 0b00000000); // Row 3
    sendCommand(5, 0b00000000); // Row 4
    sendCommand(6, 0b00000000); // Row 5
    sendCommand(7, 0b00111100); // Row 6
    sendCommand(8, 0b00000000); // Row 7
  } else if (data->toInt() == 1) {
    //Happy
    sendCommand(1, 0b00000000); // Row 0
    sendCommand(2, 0b01100110); // Row 1
    sendCommand(3, 0b01100110); // Row 2
    sendCommand(4, 0b00000000); // Row 3
    sendCommand(5, 0b00000000); // Row 4
    sendCommand(6, 0b00100100); // Row 5
    sendCommand(7, 0b00011000); // Row 6
    sendCommand(8, 0b00000000); // Row 7
  } else if (data->toInt() == 2) {
    //Sad
    sendCommand(1, 0b00000000); // Row 0
    sendCommand(2, 0b01100110); // Row 1
    sendCommand(3, 0b01100110); // Row 2
    sendCommand(4, 0b00000000); // Row 3
    sendCommand(5, 0b00000000); // Row 4
    sendCommand(6, 0b00000000); // Row 5
    sendCommand(7, 0b00011000); // Row 6
    sendCommand(8, 0b00100100); // Row 7
  } else if (data->toInt() == 3) {
    //Surprised
    sendCommand(1, 0b00000000); // Row 0
    sendCommand(2, 0b01100110); // Row 1
    sendCommand(3, 0b01100110); // Row 2
    sendCommand(4, 0b00000000); // Row 3
    sendCommand(5, 0b00011000); // Row 4
    sendCommand(6, 0b00100100); // Row 5
    sendCommand(7, 0b00011000); // Row 6
    sendCommand(8, 0b00000000); // Row 7
  }
  
}
