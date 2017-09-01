// The slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN_RF24_CE 8
#define PIN_RF24_CSN 7

#define MOTOR_LEFT_FORWARD_PIN 5 
#define MOTOR_LEFT_REVERSE_PIN 6

#define MOTOR_RIGHT_REVERSE_PIN 9  
#define MOTOR_RIGHT_FORWARD_PIN 10

#define PIN_HORN 3

#define PIN_LED_TAILLIGHTS_RED 4
#define PIN_LED_SIDE_AND_FRONT_WHITE 2


const byte thisSlaveAddress[5] = {'l','e','g','o','1'};

RF24 radio(PIN_RF24_CE, PIN_RF24_CSN);

int dataReceived[5]; // this must match dataToSend in the TX

unsigned int statusFrontSideBeams = 0;
unsigned int statusLights = 0;

unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;




void setup() {

  pinMode(MOTOR_LEFT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_REVERSE_PIN, OUTPUT);    

  pinMode(MOTOR_RIGHT_REVERSE_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD_PIN, OUTPUT);  

  analogWrite(MOTOR_LEFT_FORWARD_PIN, 0);
  analogWrite(MOTOR_LEFT_REVERSE_PIN, 0);

  analogWrite(MOTOR_RIGHT_REVERSE_PIN, 0);
  analogWrite(MOTOR_RIGHT_FORWARD_PIN, 0); 
  
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

  pinMode(3, OUTPUT);

  tone(PIN_HORN, 4000);
  delay(100);  
  noTone(PIN_HORN);
  delay(100);
  tone(PIN_HORN, 4000); 
  delay(100); 
  noTone(PIN_HORN);

  pinMode(PIN_LED_TAILLIGHTS_RED, OUTPUT);
  pinMode(PIN_LED_SIDE_AND_FRONT_WHITE, OUTPUT);

  digitalWrite(PIN_LED_TAILLIGHTS_RED, LOW);
  digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
    
}




void loop() {

  if ( radio.available() ) {
    
    radio.read( &dataReceived, sizeof(dataReceived) );

    // horn
    if ( dataReceived[0] == 1 ) {
      tone(3, 20);
    } else {
      noTone(3);
    }
    
    // switch on/off all lights
    if ( dataReceived[2] != statusLights ) {
      statusLights = dataReceived[2];
    }

    if ( statusLights ) {
      
      digitalWrite(PIN_LED_TAILLIGHTS_RED, HIGH);

      if ( dataReceived[1] != statusFrontSideBeams ) {
        statusFrontSideBeams = dataReceived[1];       
      }

      // switch on/off front high beams + side strong beams
      if ( statusFrontSideBeams == 1 ) {
        digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, HIGH);
        statusFrontSideBeams = dataReceived[1];
      } else {
        digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
      }
      //TODO switch off front dipped lights   
      
    } else {
      switchLightsOff();
    }


    // left motor
    leftMotorSpeed = dataReceived[3];
    if ( leftMotorSpeed < 128 ) { // forward
      leftMotorSpeed = map(leftMotorSpeed, 127, 0, 100, 255);
      analogWrite(MOTOR_LEFT_FORWARD_PIN, leftMotorSpeed);
      analogWrite(MOTOR_LEFT_REVERSE_PIN, 0);
    } else if ( leftMotorSpeed > 128 ) { // reverse
      leftMotorSpeed = map(leftMotorSpeed, 129, 255, 100, 255);
      analogWrite(MOTOR_LEFT_FORWARD_PIN, 0);
      analogWrite(MOTOR_LEFT_REVERSE_PIN, leftMotorSpeed);
    } else { // stop
      analogWrite(MOTOR_LEFT_FORWARD_PIN, 0);
      analogWrite(MOTOR_LEFT_REVERSE_PIN, 0);     
    }
    

    // right motor
    rightMotorSpeed = dataReceived[4];
    if ( rightMotorSpeed < 128 ) { // forward
      rightMotorSpeed = map(rightMotorSpeed, 127, 0, 100, 255);
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, rightMotorSpeed);
      analogWrite(MOTOR_RIGHT_REVERSE_PIN, 0);
    } else if ( rightMotorSpeed > 128 ) { // reverse
      rightMotorSpeed = map(rightMotorSpeed, 129, 255, 100, 255);
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
      analogWrite(MOTOR_RIGHT_REVERSE_PIN, rightMotorSpeed);
    } else { // stop
      analogWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
      analogWrite(MOTOR_RIGHT_REVERSE_PIN, 0);     
    }
        
  }

}


void switchLightsOff() {
  digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
  digitalWrite(PIN_LED_TAILLIGHTS_RED, LOW);
}

