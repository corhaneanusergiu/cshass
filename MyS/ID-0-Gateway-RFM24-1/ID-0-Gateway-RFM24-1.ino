// This MQTT gateway works on Arduino UNO
//
// Ethernet W5100 use the following PINs, do not use them:
// UNO:  4, 10, 11, 12, 13
//

#define SI "GatewayW5100MQTTClient"
#define SN "1.0"

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enables and select radio type (if attached)
#define MY_RADIO_NRF24
//#define MY_RF24_ENABLE_ENCRYPTION
//#define MY_RF24_PA_LEVEL RF24_PA_MAX
//#define MY_RF24_DATARATE RF24_250KBPS

#define MY_GATEWAY_MQTT_CLIENT

#define MY_OTA_FIRMWARE_FEATURE

// Set this nodes subscripe and publish topic prefix, keep it as short as possible
#define MY_MQTT_PUBLISH_TOPIC_PREFIX "P"
#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "S"

// Set MQTT client id
#define MY_MQTT_CLIENT_ID "GW1"

#define MY_GATEWAY_W5100
#define MY_MAC_ADDRESS 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x01

// W5100 Ethernet module SPI enable (optional if using a shield/module that manages SPI_EN signal)
//#define MY_W5100_SPI_EN 4

// Enable Soft SPI for NRF radio (note different radio wiring is required)
// The W5100 ethernet module seems to have a hard time co-operate with
// radio on the same spi bus.
// The PINs mentioned here are for Arduino Mega2560 ! Color code is from MySensor site
#if !defined(MY_W5100_SPI_EN)
  #define MY_SOFTSPI
  #define MY_SOFT_SPI_SCK_PIN 14 // A0, Green
  #define MY_SOFT_SPI_MISO_PIN 16  // A2, Violet
  #define MY_SOFT_SPI_MOSI_PIN 15  // A1, Blue
#endif

// When W5100 is connected we have to move CE/CSN pins for NRF radio
#define MY_RF24_CE_PIN 5  // Orange
#define MY_RF24_CS_PIN 6  // Yellow

// Enable these if your MQTT broker requires usenrame/password
#define MY_MQTT_USER "PI"
#define MY_MQTT_PASSWORD "368545CSG"

// If you use static IP, uncomment the following lines
// If you define the Gateway IP in the router and use DHCP, no modification is needed
// Enable MY_IP_ADDRESS here if you want a static ip address (no DHCP)
#define MY_IP_ADDRESS 192,168,1,101

// If using static ip you need to define Gateway and Subnet address as well
#define MY_IP_GATEWAY_ADDRESS 192,168,1,1
#define MY_IP_SUBNET_ADDRESS 255,255,255,0


// MQTT broker ip address, usually the IP of the server where the controller is running
#define MY_CONTROLLER_IP_ADDRESS 192,168,1,100

// The MQTT broker port to to open
#define MY_PORT 1883

#include <Ethernet.h>
#include <SPI.h>
#include <MySensors.h>


/*
Ethernet use the following PINs, do not use them:
UNO:  4, 10, 11, 12, 13
*/
void setup()
{
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SI, SN);
}

void loop()
{
  // Send locally attached sensors data here
}
