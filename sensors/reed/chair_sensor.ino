/**
   The MySensors Arduino library handles the wireless radio link and protocol
   between your home built sensors/actuators and HA controller of choice.
   The sensors forms a self healing radio network with optional repeaters. Each
   repeater and gateway builds a routing tables in EEPROM which keeps track of the
   network topology allowing messages to be routed to nodes.

   Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
   Copyright (C) 2013-2015 Sensnology AB
   Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors

   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.

 *******************************
   NOTE
   Please use MySensors version 2.2.0 or higher to avoid issues with interrupts and sleep

   DESCRIPTION
   Low power/battery operated contact switch
   - Send heartbeat (battery & sensor status) every 4 hours
   - Send contact switch status after performing 3 checks to make sure it's not someone shifting in the chair
   - Uses approximately 5.8uA when sleeping

   VIDEO
   To watch a video on how to make this sensor go here: https://youtu.be/uz3nBkRKSkk
*/
#define SKETCH_NAME "Chair Sensor"  // reed sensor
#define SKETCH_VERSION "1.0"

// Enable debug prints to serial monitor
//#define MY_DEBUG  // don't enable when is in production

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#define MY_RF24_PA_LEVEL RF24_PA_LOW //Options: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define MY_RF24_CHANNEL  76
#define MY_NODE_ID 1  //Manually set the node ID here. Comment out to auto assign
#define MY_TRANSPORT_WAIT_READY_MS 3000 //This will allow the device to function without draining the battery if it can't find the gateway when it first starts up
#define MY_BAUD_RATE 9600 //Serial monitor will need to be set to 9600 Baud

#include <MySensors.h>

#include <Vcc.h>

#define CONTACT_CHILD_ID 0
#define CONTACT_PIN  3  // Arduino Digital I/O pin for button/reed switch
#define CONTACT_CHILD_NAME "Chair Sensor 1" //The name of this specific child device will be sent to the controller

#define DEBOUNCE_DELAY 1200 //DO NOT SET BELOW 1000! Amount of time to sleep between reading the contact sensor (used for debouncing)
#define BATTERY_DELAY 14400000 //(4 hours) Amount of time in milliseconds to sleep between battery messages (as long as no interrupts occur)
//#define BATTERY_DELAY 60000

uint8_t oldContactVal = 2; //Used to track last contact value sent.  Starting out of bounds value to force an update when the sensor is first powered on
uint8_t contactVal[2]; //Used for storing contact debounce values
uint8_t contactTracker = 0; //Used as a sort of debounce to stop frequent updates when shifting in chair
int8_t wakeVal = 0; //Used to determine if wake was from timer or interrupt

const float VccMin   = 1.9;           // Minimum expected Vcc level, in Volts. (NRF can only go to 1.9V)
const float VccMax   = 3.3;           // Maximum expected Vcc level, in Volts.
const float VccCorrection = 1.0 / 1.0; // Measured Vcc by multimeter divided by reported Vcc

Vcc vcc(VccCorrection);

MyMessage msgContact(CONTACT_CHILD_ID, V_TRIPPED);

void presentation() {
  // Present sketch name & version
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);

  // Register binary input sensor to gw (they will be created as child devices)
  // You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage.
  // If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
  present(CONTACT_CHILD_ID, S_MOTION, CONTACT_CHILD_NAME);
}
void setup()
{
  //Set unused pins low to save a little power (http://gammon.com.au/forum/?id=11497)
  for (uint8_t i = 4; i <= 8; i++)
  {
    pinMode (i, INPUT);
    digitalWrite (i, LOW);
  }

  // Setup the button and activate internal pullup resistor
  //pinMode(CONTACT_PIN, INPUT_PULLUP); //(Uses 100uA in sleep)
  pinMode(CONTACT_PIN, INPUT);  //Use this with an external pullup resistor (uses 10uA in sleep)

  float p = vcc.Read_Perc(VccMin, VccMax);
#ifdef MY_DEBUG
  Serial.print("Batt Level: ");
  Serial.println(p);
#endif
  sendBatteryLevel((uint8_t)p); //Send battery level to gateway
}


void loop()
{
  //Read the value returned by sleep.  If it was a timer send battery info otherwise
  if (wakeVal < 0) {
    //Woke by timer, send battery level and current state of contact sensor
    float p = vcc.Read_Perc(VccMin, VccMax);

#ifdef MY_DEBUG
    float v = vcc.Read_Volts(); //Only using this for debugging so we don't need it taking up resources normally
    Serial.println("Woke by timer.");
    Serial.print("VCC = ");
    Serial.print(v);
    Serial.println(" Volts");
    Serial.print("VCC = ");
    Serial.print(p);
    Serial.println(" %");
#endif
    sendBatteryLevel((uint8_t)p); //Send battery level to gateway
    send(msgContact.set(oldContactVal ? 0 : 1)); //Send current sensor value
    wakeVal = sleep(digitalPinToInterrupt(CONTACT_PIN), CHANGE, BATTERY_DELAY);  //Go back to sleep
  }
  else {
    //Woke by interrupt, send contact value
    if (contactTracker < 2) {
      contactVal[contactTracker] = digitalRead(CONTACT_PIN);
#ifdef MY_DEBUG
      Serial.print("contactVal: ");
      Serial.println(contactVal[contactTracker]);
      Serial.print("contactTracker: ");
      Serial.println(contactTracker);
      Serial.println("Sleeping");
#endif
      contactTracker++; //increment contact tracker
      sleep(DEBOUNCE_DELAY); //sleep until next read
      //wait(DEBOUNCE_DELAY);
    }
    else {
      contactTracker = 0; //Reset contact tracker
      uint8_t readValue = digitalRead(CONTACT_PIN);
#ifdef MY_DEBUG
      Serial.print("contactVal: ");
      Serial.println(readValue);
#endif
      if (readValue == contactVal[0] && readValue == contactVal[1]) {
        //All values are the same, send an update
#ifdef MY_DEBUG
        Serial.println("Values the same");
#endif
        if (oldContactVal != readValue) {
#ifdef MY_DEBUG
          Serial.println("Contact changed. Sending to gateway");
#endif
          send(msgContact.set(readValue == LOW ? 1 : 0));
          oldContactVal = readValue;
        }
      }
      wakeVal = sleep(digitalPinToInterrupt(CONTACT_PIN), CHANGE, BATTERY_DELAY);  //Go back to sleep
    }
  }

}
