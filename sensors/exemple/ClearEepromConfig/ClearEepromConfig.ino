/*
 *
 * DESCRIPTION
 *
 * This sketch clears radioId, relayId and other routing information in EEPROM back to factory default
 *
 */
// load core modules only
#define MY_CORE_ONLY

#include <EEPROM.h>
#include <MySensor.h>

void setup()
{
  Serial.begin(MY_BAUD_RATE);
  Serial.println("Started clearing. Please wait...");
  for (int i=0;i<EEPROM_LOCAL_CONFIG_ADDRESS;i++) {
    hwWriteConfig(i,0xFF);
  }
  Serial.println("Clearing done. You're ready to go!");
}

void loop()
{
  // Nothing to do here...
}
