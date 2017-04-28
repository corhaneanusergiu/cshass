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

   DESCRIPTION

   Simple binary switch example
   Connect button or door/window reed switch between
   digitial I/O pin 3 (BUTTON_PIN below) and GND.
   http://www.mysensors.org/build/binary
*/


// Enable debug prints to serial monitor
#define MY_DEBUG

//Fixed NodeID in Domoticz
#define MY_NODE_ID 15

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
#define MY_OTA_FIRMWARE_FEATURE
#include <SPI.h>
#include <MySensors.h>
#include <Bounce2.h>

#define CHILD_ID 2
#define BUTTON_PIN  3  // Arduino Digital I/O pin for button/reed switch

Bounce debouncer = Bounce();
int oldValue = -1;

int BATTERY_SENSE_PIN = A0;  // select the input pin for the battery sense point

unsigned long SLEEP_TIME = 900000;  // sleep time between reads (seconds * 1000 milliseconds)
int oldBatteryPcnt = 0;

// Change to V_LIGHT if you use S_LIGHT in presentation below
MyMessage msg(CHILD_ID, V_TRIPPED);

void setup()
{

  // use the 1.1 V internal reference
  #if defined(__AVR_ATmega2560__)
    analogReference(INTERNAL1V1);
  #else
    analogReference(INTERNAL);
  #endif

  // Setup the button
  pinMode(BUTTON_PIN, INPUT);
  // Activate internal pull-up
  digitalWrite(BUTTON_PIN, HIGH);

  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Sensor-Usa-Intrare", "1.0");

  // Register binary input sensor to gw (they will be created as child devices)
  // You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage.
  // If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
  present(CHILD_ID, S_DOOR);
}


//  Check if digital input has changed and send in new value
void loop()
{
  debouncer.update();
  // Get the update value
  int value = debouncer.read();

  if (value != oldValue) {
    // Send in the new value
    send(msg.set(value == HIGH ? 1 : 0));
    oldValue = value;
    BatteryMeasurment();
  }
 sleep(SLEEP_TIME);    //Can I add it here, in this case does the door sensor response quickly to any state change?
}

void BatteryMeasurment(){
    
  // get the battery Voltage
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  #ifdef MY_DEBUG
    Serial.println(sensorValue);
  #endif
  
    // 1M, 470K divider across battery and using internal ADC ref of 1.1V
    // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
    // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
    // 3.44/1023 = Volts per bit = 0.003363075
  
    int batteryPcnt = sensorValue / 10;
  
  #ifdef MY_DEBUG
    float batteryV  = sensorValue * 0.003363075;
    Serial.print("Battery Voltage: ");
    Serial.print(batteryV);
    Serial.println(" V");
  
    Serial.print("Battery percent: ");
    Serial.print(batteryPcnt);
    Serial.println(" %");
  #endif
  
    if (oldBatteryPcnt != batteryPcnt) {
      // Power up radio after sleep
      sendBatteryLevel(batteryPcnt);
      oldBatteryPcnt = batteryPcnt;
    }

}
