/*******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * The W5100 MQTT gateway sends radio network (or locally attached sensors) data to your MQTT broker.
 * The node also listens to MY_MQTT_TOPIC_PREFIX and sends out those messages to the radio network
 *
 * LED purposes:
 * - To use the feature, uncomment WITH_LEDS_BLINKING in MyConfig.h
 * - RX (green) - blink fast on radio message recieved. In inclusion mode will blink fast only on presentation recieved
 * - TX (yellow) - blink fast on radio message transmitted. In inclusion mode will blink slowly
 * - ERR (red) - fast blink on error during transmission error or recieve crc error
 *
 *
 */


// Enable debug prints to serial monitor
#define MY_DEBUG

// Enables and select radio type (if attached)
#define MY_RADIO_RFM69
#define MY_IS_RFM69HW
#define MY_RFM69_FREQUENCY RF69_433MHZ
#define MY_RF69_IRQ_PIN 2
#define MY_RF69_IRQ_NUM  1
#define MY_RF69_SPI_CS 6

#define MY_GATEWAY_MQTT_CLIENT

// Set this node's subscribe and publish topic prefix
#define MY_MQTT_PUBLISH_TOPIC_PREFIX "sensors-out"
#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "sensors-in"

// Set MQTT client id
#define MY_MQTT_CLIENT_ID "sensors"

// W5100 Ethernet module SPI enable (optional if using a shield/module that manages SPI_EN signal)
#define MY_W5100_SPI_EN 4

// Enable these if your MQTT broker requires usenrame/password
//#define MY_MQTT_USER "username"
//#define MY_MQTT_PASSWORD "password"

// Enable MY_IP_ADDRESS here if you want a static ip address (no DHCP)
#define MY_IP_ADDRESS 192,168,1,30

// If using static ip you need to define Gateway and Subnet address as well
#define MY_IP_GATEWAY_ADDRESS 192,168,1,254
#define MY_IP_SUBNET_ADDRESS 255,255,255,0

// MQTT broker ip address
#define MY_CONTROLLER_IP_ADDRESS 192,168,1,10

// The MQTT broker port to to open
#define MY_PORT 1883

/*
// Enable inclusion mode
#define MY_INCLUSION_MODE_FEATURE
// Enable Inclusion mode button on gateway
//#define MY_INCLUSION_BUTTON_FEATURE
// Set inclusion mode duration (in seconds)
#define MY_INCLUSION_MODE_DURATION 60
// Digital pin used for inclusion mode button
//#define MY_INCLUSION_MODE_BUTTON_PIN  3
*/

// Set LED blinking period
#define MY_DEFAULT_LED_BLINK_PERIOD 300

// Flash leds on rx/tx/err
#define MY_DEFAULT_ERR_LED_PIN 3 //3
#define MY_DEFAULT_RX_LED_PIN  5 //5
#define MY_DEFAULT_TX_LED_PIN  7 //7


#include <Ethernet.h>
#include <MySensors.h>

void setup()
{
}

void presentation()
{
	// Present locally attached sensors here
}


void loop()
{
	// Send locally attached sensors data here
}
