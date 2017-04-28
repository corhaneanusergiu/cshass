/**
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * This is an example of sensors using RS485 as transport layer
 *
 * Motion Sensor example using HC-SR501
 * http://www.mysensors.org/build/motion
 *
 * The transport uses AltSoftSerial to handle two serial links
 * on one Arduino. Use the following pins for RS485 link
 *
 *  Board          Transmit  Receive   PWM Unusable
 * -----          --------  -------   ------------
 * Teensy 3.0 & 3.1  21        20         22
 * Teensy 2.0         9        10       (none)
 * Teensy++ 2.0      25         4       26, 27
 * Arduino Uno        9         8         10
 * Arduino Leonardo   5        13       (none)
 * Arduino Mega      46        48       44, 45
 * Wiring-S           5         6          4
 * Sanguino          13        14         12 *
 *
 */


// Enable RS485 transport layer
#define MY_RS485

// Define this to enables DE-pin management on defined pin
#define MY_RS485_DE_PIN 2

// Set RS485 baud rate to use
#define MY_RS485_BAUD_RATE 9600

#include <SPI.h>
#include <MySensor.h>

unsigned long SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)
#define DIGITAL_INPUT_SENSOR 3   // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
#define CHILD_ID 1   // Id of the sensor child

// Initialize motion message
MyMessage msg(CHILD_ID, V_TRIPPED);

void setup()
{
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Motion Sensor", "1.0");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID, S_MOTION);
}

void loop()
{
  // Read digital motion value
  boolean tripped = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH;

  Serial.println(tripped);
  send(msg.set(tripped?"1":"0"));  // Send tripped value to gw

  // Sleep until interrupt comes in on motion sensor. Send update every two minute.
  sleep(digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), CHANGE, SLEEP_TIME);
}