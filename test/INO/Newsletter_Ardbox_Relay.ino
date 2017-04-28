// Industrial Shields by BOOT & WORK CORP.
// Powered by Opiron Electronics -www.opiron.com-
// Feb.2015 technical information for newsletter
// The sketch demonstrates how to control a relay 
// with Ardbox Relay PLC and a Toucbberry Pi.
// Keywords: Ardbox Relay PLC, Touchberry Pi.
//////////////////////////////
//// PLC: ARDBOX RELAY PLC  ////
//// Number of inputs: 0  
//// Number of outputs: 1
//// you need to configurate the correctly switch position for running like digital / analog or PWM mode
//////////////////////////////
//// EXTERNAL BOX PIN NUMBERS:
//// LEFT SIDE (Where Input Voltage lives).
//// Pin 4 (RELAY 1) => (Box Pins: 0 and 1)
//// Pin 5 (RELAY 2) => (Box Pins: 2 and 3)
//// Pin 6 (RELAY 3) => (Box Pins: 4 and 5)
//// Pin 7 (RELAY 4) => (Box Pins: 6 and 7)
//// Pin 8 (RELAY 5) => (Box Pins: 8 and 9)
//// Pin 9 (RELAY 6) => (Box Pins: 10 and 11)
//// Pin 10 (RELAY 7) => (Box Pins: 12 and 13)
//// RIGHT SIDE (The other side).
//// Pin 11 (RELAY 8) => (Box Pins: 0 and A)
////
//// Pin 13 => (Box Pin 1 Arduino Digital => 5Vdc output if pinMode(13,OUTPUT))
////           OR
////           (Box Pin 1 Arduino Digital => 5Vdc input if pinMode(13,INPUT))
//// 
//// Box Pin 2 (GND)
//// Box Pin 3 (GND)
//// 
//// Pin 3 => (Box Pin 4)
//// Pin 2 => (Box Pin 5)
////
//// Pin A0 => (Box Pin 6)
//// Pin A1 => (Box Pin 7)
//// Pin A2 => (Box Pin 8)
//// Pin A3 => (Box Pin 9)
//// Pin A4 => (Box Pin 10)
//// Pin A5 => (Box Pin 11)
////
//// Analog pins 0 to 5. Put the jumper ON in case of digitalRead (24Vdc) otherwise analogRead (10Vdc)
////

//Libraries
#include <Wire.h>  //Library for I2C Master & Slave

//Pins defintion
#define Relay 4   // Relay

//I2C Address for the Ardbox Relay PLC
#define SLAVE_ADDRESS 0x04

// Variables
int number = 0;

// Functions
void receiveData(int byteCount);

void setup() {
    pinMode(Relay, OUTPUT);
    Serial.begin(9600);         // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Serial.println("Ready!");
}

void loop() {
    //Nothing
}

// I2C Slave communication
void receiveData(int byteCount){

    while(Wire.available()) {
        number = Wire.read();

        if (number == 1)
        {
          digitalWrite(Relay, HIGH); // Switch on the Relay
         }
        else if (number == 0)
        {
          digitalWrite(Relay, LOW); // Switch off the Relay
         }
     }
}

