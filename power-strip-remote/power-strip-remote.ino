/**
 * Transmitter Code: Continually sends the current state to the power strip. 
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

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  
  // We can change this later when we start sending longer distances. 
  radio.setPALevel(RF24_PA_HIGH); 

  radio.stopListening();
}

void loop() {
  byte data = 0x00;

  for (int i = 0; i < 16; i++) {
    radio.write(&data, sizeof(data));
    delay(2000);
    data++;
  }
}
