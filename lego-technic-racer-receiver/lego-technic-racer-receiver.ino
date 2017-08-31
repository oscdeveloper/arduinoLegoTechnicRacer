// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN_RF24_CE 8
#define PIN_RF24_CSN 7

#define motor2In1 5 // TODO change to PIN_xxxx
#define motor2In2 6

#define motor1In3 9  
#define motor1In4 10

#define PIN_HORN 3

#define PIN_LED_BACK_RED 4
#define PIN_LED_SIDE_AND_FRONT_WHITE 2


const byte thisSlaveAddress[5] = {'l','e','g','o','1'};

RF24 radio(PIN_RF24_CE, PIN_RF24_CSN);

int dataReceived[3]; // this must match dataToSend in the TX

unsigned int statusFrontSideBeams = 0;
unsigned int statusLights = 0;




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

  tone(PIN_HORN, 4000);
  delay(100);  
  noTone(PIN_HORN);
  delay(100);
  tone(PIN_HORN, 4000); 
  delay(100); 
  noTone(PIN_HORN);

  pinMode(PIN_LED_BACK_RED, OUTPUT);
  pinMode(PIN_LED_SIDE_AND_FRONT_WHITE, OUTPUT);

  digitalWrite(PIN_LED_BACK_RED, LOW);
  digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
    
}




void loop() {
    
    //showData();

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
      
      digitalWrite(PIN_LED_BACK_RED, HIGH);

      if ( dataReceived[1] != statusFrontSideBeams ) {
        statusFrontSideBeams = dataReceived[1];       
      }
        
      if ( statusFrontSideBeams == 1 ) {
        digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, HIGH);
        statusFrontSideBeams = dataReceived[1];
      } else {
        digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
      }
      //TODO switch off front dipped lights   
      
    } else {
      switchOffLights();
    }

    








 

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


        
    }

    
}


void switchOffLights() {
  digitalWrite(PIN_LED_SIDE_AND_FRONT_WHITE, LOW);
  digitalWrite(PIN_LED_BACK_RED, LOW);
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

