// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PS2X_lib.h> // Bill Porter's PS2 Library

PS2X ps2x;  //The PS2 Controller Class


#define PIN_RF24_CE 8
#define PIN_RF24_CSN 7

#define PIN_PS2_CLK 5
#define PIN_PS2_COM 4
#define PIN_PS2_ATT 3
#define PIN_PS2_DAT 2

// nRF hash string to link up connection between transmitter/receiver
const byte nrf24Address[5] = {'t','e','s','t','1'};

RF24 radio(PIN_RF24_CE, PIN_RF24_CSN); // Create a Radio

unsigned int dataToSend[3]; // only three parameters are sending between tx/rx

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

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures, Rumble)
  ps2x.config_gamepad(PIN_PS2_CLK, PIN_PS2_COM, PIN_PS2_ATT, PIN_PS2_DAT, false, false);
      
  Serial.println("Transmitter Starting...");

  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3,5); // delay, count
  radio.openWritingPipe(nrf24Address);

}

//====================

void loop() {

  ps2x.read_gamepad(); // This needs to be called at least once a second to get data from the controller.

  if ( ps2x.Button(PSB_L1) ) { // horn sound
    Serial.println("L1 pressed - horn");
    stateHorn = !stateHorn;
  }
  dataToSend[0] = stateHorn;

  if ( ps2x.Button(PSB_GREEN) ) { // front high beams + side strong beams
    Serial.println("Triangle 1 pressed - front high beams");
    stateFrontSideBeams = !stateFrontSideBeams;
  } 
  dataToSend[1] = stateFrontSideBeams;

  if ( ps2x.Button(PSB_START) ) { // switch on/off for any lights
    Serial.println("Start pressed - switch on/off for any lights");
    stateLights = !stateLights;
  }
  dataToSend[2] = stateLights;
        

  
  /*currentMillis = millis();
  if (currentMillis - prevMillis >= txIntervalMillis) {
      send();
      prevMillis = millis();
  }*/
    

  send();
  delay(100); // TODO change to 20

}

//====================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );

    Serial.print("Data Sent: ");
    Serial.print(dataToSend[0]);
    Serial.print("\t");
    Serial.print(dataToSend[1]);
    Serial.print("\t");
    Serial.print(dataToSend[2]);
    Serial.print("\t");
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }
}

