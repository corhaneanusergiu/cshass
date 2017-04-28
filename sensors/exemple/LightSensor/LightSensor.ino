/**
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik EKblad
 *
 * DESCRIPTION
 * Example sketch showing how to measue light level using a LM393 photo-resistor
 * http://www.mysensors.org/build/light
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>

#define CHILD_ID_LIGHT 0
#define LIGHT_SENSOR_ANALOG_PIN 0

unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

MyMessage msg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
int lastLightLevel;


void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Light Sensor", "1.0");

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
}

void loop()
{
  int lightLevel = (1023-analogRead(LIGHT_SENSOR_ANALOG_PIN))/10.23;
  Serial.println(lightLevel);
  if (lightLevel != lastLightLevel) {
      send(msg.set(lightLevel));
      lastLightLevel = lightLevel;
  }
  sleep(SLEEP_TIME);
}
