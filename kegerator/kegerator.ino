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
// BUTTON ROUTINES
//---------------------------------

#define BUTTON_GREEN 8
#define BUTTON_RED 7

void setup_buttons () {
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_RED, INPUT_PULLUP);
}

//---------------------------------
// MEMORY ROUTINES
//---------------------------------

// 5 bytes
struct CardData {
  byte uid[4];
  bool is_master;
};

// Allocate space upon startup for 10 cards (i.e., 50 bytes)
#define MAX_CARDS 10
struct CardList {
  CardData cards[MAX_CARDS];
  int size;
};

CardList cardList;

//
// Checks to uid to determine if they're the same
//
// @param byte a[]
// @param byte b[]
//
bool idMatches (byte a[], byte b[]) {
  bool match = true;

  for (uint8_t i = 0; i < 4; i++) {
    if (a[i] != b[i]) {
      match = false;
    }
  }

  return match;
}


//
// Searches the PICC list to find if the card exists
//
// @param uid
// @returns bool true if exists, false if not
//
int8_t lookupCard (byte uid[]) {
  int8_t found = -1;

  for (int8_t i = 0; i < cardList.size; i++) {
    if (idMatches(cardList.cards[i].uid, uid)) {
      found = i; 
    }
  }

  return found;
}

//
// Adds the provided card to the end of the list (SET?)
//
// @param CardData
//
void saveCard (CardData data) {
  cardList.cards[cardList.size] = data;
  cardList.size++;
}



//---------------------------------
// MAIN ROUTINES
//---------------------------------

// The UID of a card is 4 bytes long or 32bits
byte readCard[4];

void setup () {
  Serial.begin(9600);

  setup_buttons();

  // Initialize the display
  oled_setup();

  // Initialize the reader
  rfid_setup();

  // Looking for an RFID card
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("Waiting for card...");
  display.display();
}

void loop () {
  if (! reader.PICC_IsNewCardPresent()) {
    return;
  }

  if (! reader.PICC_ReadCardSerial()) {
    return;
  }

  // Read the UID from the read card
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Scanned PICC UID:");

  // Copy the 4 bytes of the id into local mem
  for (uint8_t i = 0; i < 4; i++) {
    readCard[i] = reader.uid.uidByte[i];
    display.print(readCard[i], HEX);
    display.print(" ");
  }
  display.println("");
  display.display();

  reader.PICC_HaltA(); // We are done reading the info from the card
}
