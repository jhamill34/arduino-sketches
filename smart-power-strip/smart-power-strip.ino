/**
 * Reciever Code: Waits for input from the transmitter to 
 * know which relay to turn on. 
 * 
 * @author Joshua Rasmussen <joshrasmussen34@gmail.com>
 */

/**
 * The protocol: 
 * 
 * Each bit in a byte represents the relay state
 * 
 * Example: (0x0B)
 * 
 * We want the 4th, 2nd and 1st relay on
 */

#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(10, 9); // CE, CSN

// This is the agreed upon address to transmit back and forth
const byte address[6] = "000001";

int relay_pins[] = {5, 6, 7, 8};
int relay_state = 0;

void setState(byte state) {
  for (int i = 0; i < sizeof(relay_pins) / sizeof(relay_pins[0]); i++) {
    int val = (state >> i & 0x01);
    digitalWrite(relay_pins[i], val);
  }
}

void setup() {
  for (int i = 0; i < sizeof(relay_pins) / sizeof(relay_pins[0]); i++) {
    pinMode(relay_pins[i], OUTPUT);
  }

  radio.begin();
  radio.openReadingPipe(0, address);
  
  // We can change this later when we start sending longer distances. 
  radio.setPALevel(RF24_PA_HIGH); 

  radio.startListening();
}

void loop() {
  if (radio.available()) {
    byte data;
    radio.read(&data, sizeof(data));
    setState(data);
  }
}
