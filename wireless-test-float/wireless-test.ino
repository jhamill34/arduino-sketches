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

#define RF24_CE 7
#define RF24_CSN 8

void sendMessage(void);
void displayMessage(void);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RF24 radio(RF24_CE, RF24_CSN);
const byte addresses [][6] = {"00001", "00002"};
const char message[6] = "abcde";

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  radio.begin();
  radio.openReadingPipe(1, addresses[0]);
  radio.openWritingPipe(addresses[1]);
  radio.setPALevel(RF24_PA_MAX);
  radio.setCRCLength(RF24_CRC_8);
  radio.setRetries(15, 15);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendMessage();

  delay(1000);

  displayMessage();

  delay(1000);
}

void sendMessage() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sending message...");
  display.display();

  radio.stopListening();
  radio.write(&message, sizeof(message));
}

void displayMessage() {
  char recievedMessage[6] = "";

  radio.startListening();
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Waiting for message...");
  display.display();

  while(!radio.available());
  
  radio.read(&recievedMessage, sizeof(recievedMessage));
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (strcmp(recievedMessage, message) == 0) {
    display.println("Success!");
  } else {
    display.println("Error...");
    display.print("R:");
    display.println(recievedMessage);
    display.print("E:");
    display.println(message);
  }
  display.display();
}

