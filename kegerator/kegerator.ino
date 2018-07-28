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

/**
  * Routine to setup the PCD (RFID Reader)
  */
void rfid_setup () {
  reader.PCD_Init();
}

//---------------------------------
// OLED SETUP
//---------------------------------

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/**
  * Routine to setup the Physical OLED display
  * The basic setting is:
  *   - Font size '1'
  *   - Color is 'WHITE'
  */
void oled_setup () {
  display.begin();

  // Config
  display.setTextSize(1);
  display.setTextColor(WHITE);
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

/**
  * Display information on every state (i.e., Temperature)
  */
void showHeader() {
  display.setCursor(0, 0);
  display.println("Temp 32C");
}

//------------------------------
// STATE VIEWS
//------------------------------
enum ViewState {
  INITIAL,        // SHOW_USER | SECOND_CARD
  SHOW_USER,      // POUR | INITIAL
  POUR,           // INITIAL
  SECOND_CARD,    // MANAGE (master) | MANAGE (new card)
  MANAGE,         // CONFIRM (delete) | TOKAN_UPDATE
  CONFIRM,        // POUR (if from initial) | INITIAL
  TOKEN_UPDATE    // CONFIRM (update)
};

// What we are going to use to keep track of our current state
ViewState current_state = INITIAL;

void showInitial () {
  // Looking for an RFID card
  display.println("");
  display.print("Waiting for card...");
}

void showPour () {
  display.println("");
  display.println("Pouring Your Drink!");
}

void showWaitForSecondCard() {
  display.println("Detected Master Card:");
  display.println("Scan Another Card");
  display.println("G: Manage | R: Cancel");
}

void showManageCard() {
  display.println("Card Mamagement:");
  display.println("G: tokens | R: delete");
  display.println("B: Cancel");
}

void showUser() {
  display.println("You have X tokens");
  display.println("Do you want to use one?");
  display.println("G: Yes | R: No");
}

void showConfirmation() {
  display.println("Are you sure?");
  display.println("G: Yes | R: No");
}

void showTokenUpdate() {
  display.println("How many tokens?");
  display.println("      1         ");
  display.println("G: Up | R: Dn | B: OK");
}

/**
  * Use to render the current state of the app
  * based off of the 'curren_state'
  *
  * TODO: Could be ecapsulated better
  */
void renderView() {
  display.clearDisplay();
  showHeader();
  switch(current_state){
    case INITIAL:
      showInitial();
      break;
    case POUR:
      showPour();
      break;
    case SECOND_CARD:
      showWaitForSecondCard();
      break;
    case MANAGE:
      showManageCard();
      break;
    case CONFIRM:
      showConfirmation();
      break;
    case TOKEN_UPDATE:
      showTokenUpdate();
      break;
    default:
      showInitial();
  }
  display.display();
}

void collectActions() {

}

void setup () {
  Serial.begin(9600);
  SPI.begin();

  // Initialize the reader
  rfid_setup();

  // Initialize the display
  oled_setup();
}

void loop () {
  renderView();

  current_state = current_state + 1;

  delay(3000);
}
