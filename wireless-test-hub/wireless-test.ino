/**
 * @author Joshua Rasmussen
 * @version 0.1.0
 */

#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET 4

void echoMessage(void);

RF24 radio(7, 8);
const byte addresses[][6] = {"00001", "00002"};

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(1, addresses[1]);
  radio.openWritingPipe(addresses[0]);
  radio.setPALevel(RF24_PA_MAX);
  radio.setCRCLength(RF24_CRC_8);
  radio.setRetries(15, 15);
}

void loop() {
  // put your main code here, to run repeatedly:
  echoMessage();  
  delay(1000);
}

void echoMessage() {
  char message[6] = "";
  
  radio.startListening();

  while(!radio.available());
 
  radio.read(&message, sizeof(message));
 
  radio.stopListening();

  delay(1000);
  
  radio.write(&message, sizeof(message));

}
