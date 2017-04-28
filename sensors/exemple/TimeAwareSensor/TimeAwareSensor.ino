/**
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Example sketch showing how to request time from controller.
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>
#include <Time.h>

boolean timeReceived = false;
unsigned long lastUpdate=0, lastRequest=0;

void setup()
{
  // Request time from controller.
  requestTime();
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Clock", "1.0");
}

// This is called when a new time value was received
void receiveTime(unsigned long time) {
  // Ok, set incoming time
  setTime(time);
  timeReceived = true;
}

void loop()
{
  unsigned long now = millis();

  // If no time has been received yet, request it every 10 second from controller
  // When time has been received, request update every hour
  if ((!timeReceived && (now-lastRequest) > (10UL*1000UL))
    || (timeReceived && (now-lastRequest) > (60UL*1000UL*60UL))) {
    // Request time from controller.
    Serial.println("requesting time");
    requestTime();
    lastRequest = now;
  }

  // Print time every second
  if (timeReceived && now-lastUpdate > 1000) {
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(month());
    Serial.print(" ");
    Serial.print(year());
    Serial.println();
    lastUpdate = now;
  }
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
