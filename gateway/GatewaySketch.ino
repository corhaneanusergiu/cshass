/**
 * REVISION HISTORY
 * Version 1.0 - Corhaneanu Sergiu
 *
 * DESCRIPTION
 * The gateway send radio network (or locally attached sensors) data to your MQTT broker.
 * The node also listens to MY_MQTT_TOPIC_PREFIX and sends out those messages to the radio network.
 *
 * LED purposes:
 * - To use the feature, uncomment WITH_LEDS_BLINKING in MyConfig.h
 * - RX (green) - blink fast on radio message recieved. In inclusion mode will blink fast only on presentation recieved
 * - TX (yellow) - blink fast on radio message transmitted. In inclusion mode will blink slowly
 * - ERR (red) - fast blink on error during transmission error or recieve crc error
 *
 *
 * Wiring Arduino Pro Mini and NRF24L01 Radio with WizNET(W5100) Ethernet module.
 *
 * Arduino    NRF24L01    W5100
 * GND        GND         GND
 * 3.3V       VCC         VCC
 * 13                     SCK
 * 12                     MISO/SO
 * 11                     MOSI/SI
 * 10                     SS/CS
 * A2         MISO
 * A1         MOSI
 * A0         SCK
 * 6          CSN
 * 5          CE
 *
 ******
 *
 * Wiring Arduino Pro Mini and NRF24L01 Radio with ENC28J60 Ethernet module.
 *
 * Arduino    NRF24L01    ENC28J60
 * GND        GND         GND
 * 3.3V       VCC         VCC
 * 13         SCK         SCK
 * 12         MISO        MISO/SO
 * 11         MOSI        MOSI/SI
 * 10                     SS/CS
 * 6          CSN
 * 5          CE
 *
 *
 * Note that the ENC28J60 module uses much more memory than W5100.
 * You'll probably have to remove the MY_DEBUG define to even make it compile.
 *
 *
 *
**/

/**********************************
*  Serial and debug options
***********************************/

// Enable debug prints to serial monitor
//#define MY_DEBUG

/**********************************
*  Radio selection and config
***********************************/

// Enables and select radio type (if attached)
//#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Settings for NRF24L01
//#define MY_RF24_ENABLE_ENCRYPTION
//#define MY_DEBUG_VERBOSE_RF24
//#define MY_RF24_SPI_MAX_SPEED 4000000
//#define MY_RF24_PA_LEVEL RF24_PA_MAX
//#define MY_RF24_DATARATE RF24_250KBPS
//#define MY_RF24_ADDR_WIDTH 5

// When W5100 or ENC28J60 is connected we have to move CE/CSN pins for NRF radio
//#define MY_RF24_CE_PIN 5
//#define MY_RF24_CS_PIN 6

// W5100 Ethernet module SPI enable (optional if using a shield/module that manages SPI_EN signal)
//#define MY_W5100_SPI_EN 4

// Enable Soft SPI for NRF radio (note different radio wiring is required)
// The W5100 ethernet module seems to have a hard time co-operate with
// radio on the same spi bus.
//#define MY_SOFT_SPI_SCK_PIN 14
//#define MY_SOFT_SPI_MISO_PIN 16
//#define MY_SOFT_SPI_MOSI_PIN 15


// RF24 sanity check to verify functional RF module
// This reads back and compares configuration registers.
// Disable if using non-P modules
//#define MY_RF24_SANITY_CHECK

// RF24 radio network identifier.
// This acts as base value for sensor nodeId addresses.
// Change this (or channel) if you have more than one sensor network.
//#define MY_RF24_BASE_RADIO_ID 0x00,0xFC,0xE1,0xA8,0xA8

/**
 * RF channel for the sensor net, 0-125.
 * Frequence: 2400 Mhz - 2525 Mhz Channels: 126
 * http://www.mysensors.org/radio/nRF24L01Plus.pdf
 * 0 => 2400 Mhz (RF24 channel 1)
 * 1 => 2401 Mhz (RF24 channel 2)
 * 76 => 2476 Mhz (RF24 channel 77)
 * 83 => 2483 Mhz (RF24 channel 84)
 * 124 => 2524 Mhz (RF24 channel 125)
 * 125 => 2525 Mhz (RF24 channel 126)
 */
//#define MY_RF24_CHANNEL	76


// Settings for RFM69

// RFM69 frequency to use (RF69_433MHZ for 433MHz, RF69_868MHZ for 868MHz or RF69_915MHZ for 915MHz).
// This must match the hardware version of the RFM69 radio.
//#define MY_RFM69_FREQUENCY RF69_868MHZ

// RFM69 Network ID. Use the same for all nodes that will talk to each other.
//#define MY_RFM69_NETWORKID 13

// RF69 IRQ pin.
//#define MY_RF69_IRQ_PIN RF69_IRQ_PIN

// RF69 SPI chip select pin.
//#define MY_RF69_SPI_CS RF69_SPI_CS

// Enables RFM69 encryption (all nodes and gateway must have this enabled, and all must be personalized with the same AES key)
//#define MY_RFM69_ENABLE_ENCRYPTION

//Enable this if you're running the RFM69HW model.
//#define MY_IS_RFM69HW

/**********************************
*  Gateway Config Settings
***********************************/

// The gateway options available
//#define MY_GATEWAY_W5100
//#define MY_GATEWAY_ENC28J60
//#define MY_GATEWAY_MQTT_CLIENT

// Static ip address of gateway (if this is disabled, DHCP will be used)
//#define MY_IP_ADDRESS 192,168,0,110

// DHCP, default renewal setting in milliseconds.
//#define MY_IP_RENEWAL_INTERVAL 60000

// Ethernet MAC address.
// The MAC address can be anything you want but should be unique on your network.
// Newer boards have a MAC address printed on the underside of the PCB, which you can (optionally) use.
// Note that most of the Ardunio examples use  "DEAD BEEF FEED" for the MAC address.
//#define MY_MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

// Enables UDP mode for Ethernet gateway (W5100)
//#define MY_USE_UDP

// Max buffersize needed for messages coming from controller.
//#define MY_GATEWAY_MAX_RECEIVE_LENGTH 100

// Max buffer size when sending messages.
//#define MY_GATEWAY_MAX_SEND_LENGTH 120

//Max number of parallel clients (sever mode).
//#define MY_GATEWAY_MAX_CLIENTS 1

// Controller ip-address, if this is defined, gateway will act as a client trying to contact controller on MY_PORT.
// If MY_CONTROLLER_IP_ADDRESS is left un-defined, gateway acts as server allowing incoming connections.
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 0, 100

// The Ethernet TCP/UDP port to open on controller or gateway.
//#define MY_PORT 5003

/**********************************
*  MQTT Config Settings
***********************************/

// Set this node's subscribe and publish topic prefix
//#define MY_MQTT_PUBLISH_TOPIC_PREFIX "Pub"
//#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "Sub"

// Set MQTT client id
//#define MY_MQTT_CLIENT_ID "GW-1"

// MQTT broker ip address or url. Define one or the other.
//#define MY_CONTROLLER_URL_ADDRESS "m20.cloudmqtt.com"
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 0, 100

// The MQTT broker port to to open
//#define MY_PORT 1883

/**********************************
*  Over the air firmware updates
***********************************/

// Enable MY_OTA_FIRMWARE_FEATURE in sketch to allow safe over-the-air firmware updates.
// This feature requires external flash and the DualOptiBoot boot-loader.
// Note: You can still have OTA FW updates without external flash but it
// requires the MYSBootloader and disabled MY_OTA_FIRMWARE_FEATURE
//#define MY_OTA_FIRMWARE_FEATURE

// Slave select pin for external flash.
//#define MY_OTA_FLASH_SS 8

// Flash jdecid.
//#define MY_OTA_FLASH_JDECID 0x1F65

/**********************************
*  Message Signing Settings
***********************************/
// Only one signing engine can be activated
// MY_SIGNING_ATSHA204 or MY_SIGNING_SOFT

// Enables HW backed signing functionality in library.
// For any signing related functionality to be included, this define or @ref MY_SIGNING_SOFT has to be enabled.
//#define MY_SIGNING_ATSHA204

// Enables SW backed signing functionality in library.
// For any signing related functionality to be included, this define or @ref MY_SIGNING_ATSHA204 has to be enabled.
//#define MY_SIGNING_SOFT

// Enable this to inform gateway to sign all messages sent to this node.
// If used for a gateway, gateway will only request signatures from nodes that in turn
// request signatures from gateway.
//#define MY_SIGNING_REQUEST_SIGNATURES

// Define a suitable timeout for a signature verification session
// Consider the turnaround from a nonce being generated to a signed message being received
// which might vary, especially in networks with many hops. 5s ought to be enough for anyone.
//#ifndef MY_VERIFICATION_TIMEOUT_MS
//  #define MY_VERIFICATION_TIMEOUT_MS 5000
//#endif

// Enable to turn on whitelisting
// When enabled, a signing node will salt the signature with it's unique signature and nodeId.<br>
// The verifying node will look up the sender in a local table of trusted nodes and
// do the corresponding salting in order to verify the signature.<br>
// For this reason, if whitelisting is enabled on one of the nodes in a sign-verify pair, both
// nodes have to implement whitelisting for this to work.<br>
// Note that a node can still transmit a non-salted message (i.e. have whitelisting disabled)
// to a node that has whitelisting enabled (assuming the receiver does not have a matching entry
// for the sender in it's whitelist). The whitelist to use is defined as the value of the flag.
//#define MY_SIGNING_NODE_WHITELISTING {{.nodeId = GATEWAY_ADDRESS,.serial = {0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01}}}

// Pin where ATSHA204 is attached
//#ifndef MY_SIGNING_ATSHA204_PIN
//  #define MY_SIGNING_ATSHA204_PIN 17
//#endif

// Pin used for random generation in soft signing
// Do not connect anything to this when soft signing is enabled
//#ifndef MY_SIGNING_SOFT_RANDOMSEED_PIN
//  #define MY_SIGNING_SOFT_RANDOMSEED_PIN 7
//#endif

/**********************************************
*  Gateway inclusion button/mode configuration
**********************************************/

// Enabled inclusion mode feature
//#define MY_INCLUSION_MODE_FEATURE

// Enables inclusion-mode button feature on the gateway device
//#define MY_INCLUSION_BUTTON_FEATURE

// The default input pin used for the inclusion mode button.
//#define MY_INCLUSION_MODE_BUTTON_PIN 5

// Number of seconds (default one minute) inclusion mode should be enabled.
//#define MY_INCLUSION_MODE_DURATION 60

// The logical level indicating a pressed inclusion mode button.
//#define MY_INCLUSION_BUTTON_EXTERNAL_PULLUP
//  #if defined(MY_INCLUSION_BUTTON_EXTERNAL_PULLUP)
//    #define MY_INCLUSION_BUTTON_PRESSED HIGH
//  #else
//    #define MY_INCLUSION_BUTTON_PRESSED LOW
//#endif

/**********************************
*  Information LEDs blinking
***********************************/

//#define MY_LEDS_BLINKING_FEATURE

// The following setting allows you to inverse the blinking feature MY_LEDS_BLINKING_FEATURE
// When MY_WITH_LEDS_BLINKING_INVERSE is enabled LEDSs are normally turned on and switches
// off when blinking

//#define MY_WITH_LEDS_BLINKING_INVERSE

// The following defines can be used to set the port pin, that the LED is connected to
// If one of the following is defined here, or in the sketch, MY_LEDS_BLINKING_FEATURE will be
// enabled by default. (Replace x with the pin number you have the LED on)
//#define MY_DEFAULT_ERR_LED_PIN x  // Error led pin
//#define MY_DEFAULT_RX_LED_PIN  x  // Receive led pin
//#define MY_DEFAULT_TX_LED_PIN  x  // Tranmit led pin

// Default LEDs blinking period in milliseconds.
//#define MY_DEFAULT_LED_BLINK_PERIOD 300

//#include <EEPROM.h>
//#include <SPI.h>
//#include <ArduinoOTA.h>
//#include <Ethernet.h>
//#include <UIPEthernet.h>
#include <MySensors.h>

void setup() {
}

void presentation() {
  // Present locally attached sensors here
  //delay(50);
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Denumire Sketch", "1.0");
}

void loop() {
  // Send locally attached sensors data here
}
