/*#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int SW1 = 2;

void setup()
{

pinMode(SW1, INPUT_PULLUP);
  
 Serial.begin(9600);
 radio.begin();
 radio.openWritingPipe(pipe);
}

void loop()
{
 if (digitalRead(SW1) == LOW) {
 msg[0] = 111;
 radio.write(msg, 1);
 Serial.println("radio puk puk");
}
}*/



// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PS2X_lib.h> // Bill Porter's PS2 Library

PS2X ps2x;  //The PS2 Controller Class


#define CE_PIN 8
#define CSN_PIN 7

//#define BUTTON 2

const byte slaveAddress[5] = {'t','e','s','t','1'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

unsigned int dataToSend[5];

unsigned int stateHorn = 0;
unsigned int stateFrontSideBeams = 0;
unsigned int stateLights = 0;

/*
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second
*/

void setup() {

    Serial.begin(9600);

    ps2x.config_gamepad(5,4,3,2, false, false);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);

    //pinMode(BUTTON, INPUT_PULLUP);
}

//====================

void loop() {

  ps2x.read_gamepad(); //This needs to be called at least once a second to get data from the controller.


    //if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    //{
     
       
         
  if ( ps2x.Button(PSB_L1) ) { // horn sound
    Serial.println("L1 pressed horn");
    stateHorn = !stateHorn;
  }
  dataToSend[0] = stateHorn;

  if ( ps2x.Button(PSB_GREEN) ) { // front high beams + side strong beams
    Serial.println("Triangle 1 pressed front high beams");
    stateFrontSideBeams = !stateFrontSideBeams;
  } 
  dataToSend[1] = stateFrontSideBeams;

  if ( ps2x.Button(PSB_START) ) { // switch on/off for any lights
    Serial.println("Start pressed switch on/off for any lights");
    stateLights = !stateLights;
  }
  dataToSend[2] = stateLights;
        

         
    //}    
    /*currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }*/
    //updateMessage();
    
  /*if (digitalRead(BUTTON) == LOW) {
    dataToSend[0] = 1023;
    Serial.println("radio puk puk");
  } else {
    dataToSend[0] = -1;
    //Serial.println("koniec transmisji");
  }*/
    
    //if ( dataToSend[0] > -1 ) {
      send();
      delay(100); // TODO change to 20
    //}
}

//====================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent: ");
    Serial.print(dataToSend[0]);
    Serial.print("\t");
    Serial.print(dataToSend[1]);
    Serial.print("\t");
    Serial.print(dataToSend[2]);
    Serial.print("\t");
    Serial.print(dataToSend[3]);
    Serial.print("\t");
    Serial.print(dataToSend[4]);
    if (rslt) {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {

  /*if (digitalRead(BUTTON) == LOW) {
    dataToSend[0] = 1023;
    Serial.println("radio puk puk");
  } else {
    dataToSend[0] = -1;
    //Serial.println("koniec transmisji");
  }*/
  
  
}


