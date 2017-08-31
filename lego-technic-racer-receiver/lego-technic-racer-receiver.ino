// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 8
#define CSN_PIN 7

#define motor2In1 5 
#define motor2In2 6

#define motor1In3 9  
#define motor1In4 10

#define PIN_BUZZER 3

#define PIN_LED_BACK_RED 2
#define PIN_LED_SIDE_AND_FRONT_WHITE 4


const byte thisSlaveAddress[5] = {'t','e','s','t','1'};

RF24 radio(CE_PIN, CSN_PIN);

int dataReceived[5]; // this must match dataToSend in the TX
bool newData = false;

//===========

void setup() {

    pinMode(motor2In1, OUTPUT);
    pinMode(motor2In2, OUTPUT);    

    pinMode(motor1In3, OUTPUT);
    pinMode(motor1In4, OUTPUT);  

          analogWrite(motor2In1, 0);
          analogWrite(motor2In2, 0);

          analogWrite(motor1In3, 0);
          analogWrite(motor1In4, 0); 
    
    //Serial.begin(9600);
    //Serial.println("SimpleRx Starting");
    
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();

    pinMode(3, OUTPUT);

    tone(PIN_BUZZER, 4000);
    delay(100);  
    noTone(PIN_BUZZER);
    delay(100);
    tone(PIN_BUZZER, 4000); 
    delay(100); 
    noTone(PIN_BUZZER);

    pinMode(PIN_LED_BACK_RED, OUTPUT);
    pinMode(PIN_LED_SIDE_AND_FRONT_WHITE, OUTPUT);

digitalWrite(PIN_LED_BACK_RED, HIGH);
digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, HIGH);
    
}

//=============

void loop() {
    getData();
    //showData();
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;

        //Serial.print("Data received: ");
        //Serial.println(dataReceived[0]);
        if ( dataReceived[0] == 30 || dataReceived[0] == 1023 ) {


digitalWrite(PIN_LED_BACK_RED, HIGH);
digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, HIGH);

          
tone(3, 20);

analogWrite(motor2In1, 200);
//delay(2000);
analogWrite(motor1In3, 200);
//delay(2000);

          /*for (int i=100; i<=255; i++) {
            analogWrite(motor2In1, i);
            analogWrite(motor2In2, 0);  
            Serial.println(i);   
            delay(10);        
          }
          delay(1000);

          for (int i=100; i<=255; i++) {
            analogWrite(motor1In3, 0);
            analogWrite(motor1In4, i);
            Serial.println(i); 
            delay(10);            
          }       
          delay(1000);*/

        } /*else {
          noTone(3);

          digitalWrite(PIN_LED_BACK_RED, LOW);
          digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
          
          analogWrite(motor2In1, 0);
          analogWrite(motor2In2, 0);

          analogWrite(motor1In3, 0);
          analogWrite(motor1In4, 0);          
        }*/

        if ( dataReceived[4] == 43 ) {
          tone(3, 20);
        } else {
          noTone(3);
        }
        
    }
}
/*
void showData() {
    if (newData == true) {
        //Serial.print("Data received: ");
        //Serial.println(dataReceived[0]);
        if ( dataReceived[0] == 1023 ) {
          digitalWrite(LED, HIGH);

          /*for (int i=100; i<=255; i++) {
            analogWrite(motor2In1, i);
            analogWrite(motor2In2, 0);  
            Serial.println(i);          
          }*/
/*
          for (int i=100; i<=255; i++) {
            analogWrite(motor1In3, i);
            analogWrite(motor1In4, 0);            
          }          

        } else {
          digitalWrite(LED, LOW);
          analogWrite(motor2In1, 0);
          analogWrite(motor2In2, 0);

          analogWrite(motor1In3, 0);
          analogWrite(motor1In4, 0);          
        }
        newData = false;
    }
}*/

