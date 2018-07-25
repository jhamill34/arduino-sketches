/**
  * The main routine for the kegerator system
  *
  * @author Joshua Rasmussen <joshrasmussen34@gmail.com>
  * @version 0.1.0
  */

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//---------------------------------
// RFID SETUP  
//---------------------------------

#define RST_PIN 9
#define SS_PIN 10

MFRC522 reader(SS_PIN, RST_PIN);

void rfid_setup () {
  Serial.begin(9600);
  SPI.begin();
  reader.PCD_Init();
  reader.PCD_DumpVersionToSerial();
}

//---------------------------------
// OLED SETUP  
//---------------------------------

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32) 
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void oled_setup () {
  display.begin();

  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void display_header() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("TEMP: 32C");
  display.display();
}

//---------------------------------
// MAIN ROUTINES 
//---------------------------------


void setup () {
  // Initialize the display 
  oled_setup();
  display_header();
  
  // Initialize the reader
  rfid_setup();
}

void loop () {
  if (! reader.PICC_IsNewCardPresent()) {
    return;
  }

  if (! reader.PICC_ReadCardSerial()) {
    return;
  }

  reader.PICC_DumpToSerial(&(reader.uid));
}
