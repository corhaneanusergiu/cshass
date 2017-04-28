/**
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Example sketch showing how to create a node thay repeates messages
 * from nodes far from gateway back to gateway.
 * It is important that nodes that has enabled repeater mode calls
 * process() frequently. Repeaters should never sleep.
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

//#define MY_NODE_ID 254
// Here write details about node.

// Enabled repeater feature for this node
#define MY_REPEATER_FEATURE

#include <SPI.h>
#include <MySensor.h>

void setup() {

}

void presentation()
{
  //Send the sensor node sketch version information to the gateway
  sendSketchInfo("Repeater Node", "1.0");
}

void loop()
{
}
