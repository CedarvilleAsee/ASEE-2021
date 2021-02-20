//This Handles All Radio communication
// Worked on by Josiah Carpenter For Cedarville University Robotics Team


#include <SPI.h>
#include "RF24.h"



bool radioNumber = 0;
RF24 radio(7, 8);
byte addresses[][6] = {"1Node", "2Node"};


// --- Functions ---
void InitializeRadio(){
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    if (radioNumber) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
    } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
    }
    radio.startListening();
    Serial.print("Radio Initialized");
}

void RadioSend(int message) {
  radio.stopListening();

  if (!radio.write( &message, sizeof(int) )) {
    Serial.println(F("failed to send"));
  }

  radio.startListening();
}

int RadioRead() {
  int temp = -1;
  if ( radio.available()) {
    while (radio.available()) {
      radio.read( &temp, sizeof(int));
      return temp;
    }
  }
  return temp;
}
