/**

   DESCRIPTION

   Example sketch showing how to send PH readings back to the controller
*/

// Enable debug prints to serial monitor
//#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>

#define COMPARE_PH 1 // Send PH only if changed? 1 = Yes 0 = No

unsigned long SLEEP_TIME = 60000; // Sleep time between reads (in milliseconds)
float lastPH;
boolean receivedConfig = false;
boolean metric = true;
// Initialize PH message
MyMessage msg(0, V_PH);

void setup()
{
  //Setup your PH sensor here (I2C,Serial,Phidget...)
}

float getPH() {
  //query your PH sensor here (I2C,Serial,Phidget...)
  float dummy = 7;
  return dummy;
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("PH Sensor", "1.1");
  present(0, S_WATER_QUALITY);

}

void loop()
{
  float ph = getPH();

#if COMPARE_PH == 1
  if (lastPH != ph) {
#endif

    // Send in the new PH value
    send(msg.set(ph, 1));
    // Save new PH value for next compare
    lastPH = ph;

#if COMPARE_PH == 1
  }
#endif
  sleep(SLEEP_TIME);
}
