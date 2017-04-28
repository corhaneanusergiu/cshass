/**
 *
 * REVISION HISTORY
 * Version 1.0 - idefix
 *
 * DESCRIPTION
 * Arduino BH1750FVI Light sensor
 * communicate using I2C Protocol
 * this library enable 2 slave device addresses
 * Main address  0x23
 * secondary address 0x5C
 * connect the sensor as follows :
 *
 *   VCC  >>> 5V
 *   Gnd  >>> Gnd
 *   ADDR >>> NC or GND
 *   SCL  >>> A5
 *   SDA  >>> A4
 * http://www.mysensors.org/build/light
 */


// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>
#include <BH1750.h>
#include <Wire.h>

#define CHILD_ID_LIGHT 0
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

BH1750 lightSensor;

// V_LIGHT_LEVEL should only be used for uncalibrated light level 0-100%.
// If your controller supports the new V_LEVEL variable, use this instead for
// transmitting LUX light level.
MyMessage msg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
// MyMessage msg(CHILD_ID_LIGHT, V_LEVEL);
uint16_t lastlux;

void setup()
{
  lightSensor.begin();
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Light Lux Sensor", "1.0");

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
}

void loop()
{
  uint16_t lux = lightSensor.readLightLevel();// Get Lux value
  Serial.println(lux);
  if (lux != lastlux) {
      send(msg.set(lux));
      lastlux = lux;
  }

  sleep(SLEEP_TIME);
}
