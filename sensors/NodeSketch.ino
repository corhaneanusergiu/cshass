/**
 * REVISION HISTORY
 * Version 1.0 - Corhaneanu Sergiu
 *
 * DESCRIPTION
 * The node send radio network (or locally attached sensors) data to your gateway.
 * This is an example that demonstrates how to report the battery level for a sensor
 * Instructions for measuring battery capacity on A0 are available here:
 * http://www.mysensors.org/build/battery
 *
 * Wiring Arduino Pro Mini 3.3V and NRF24L01+ Radio module.
 *
 * Arduino    NRF24L01    Color
 * GND        GND         Black
 * 3.3V       VCC         Red
 * 9          CE          Orange
 * 10         CSN/CS      Yellow
 * 13         SCK         Green
 * 11         MOSI        Blue
 * 12         MISO        Violet
 * 2          IRQ         Gray
 *
 * Note: The IRQ is currently not used by the MySensors library so it can be left un-connected.
 *
 * Arduino    RFM69       Color
 * GND        GND         Black
 * 3.3V       VCC         Red
 * 10         NSS         Yellow
 * 13         SCK         Green
 * 11         MOSI        Blue
 * 12         MISO        Violet
 * 2          DIO0        Gray
 *            ANA         Antenna
 *
 *
 * RFM69 Antenna
 * IMPORTANT: You MUST attach an antenna to the board. Aside from not working without an antenna,
 * transmitters can be damaged if they transmit without an antenna present.
 * By cutting a wire to the proper length you can create a simple antenna for your RFM69 radio.
 * Depending on the frequency of the radio, the antenna has to be cut to different lengths.
 * Cut the antenna slightly longer to give room to solder it to your board. You can always trim it once soldered.
 *
 * Frequency  Length (in)	 Length (mm)
 * 434 MHz    6.47"	       164.7 mm
 * 868 MHz	  3.22"	       82.2 mm
 * 915 MHz	  3.06"	       77.9 mm
 *
 * Above antenna length represent 1/4 wave. There is also commercial antennas to buy in proper length.
 *
 *
 */

/**********************************
*  Serial and debug options
***********************************/

// Enable debug prints to serial monitor. Please set the baud-rate to 115200
//#define MY_DEBUG

/**********************************
*  Radio selection and config
***********************************/

// Enables and select radio type (if attached)
//#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Settings for NRF24L01
//#define MY_RF24_ENABLE_ENCRYPTION
//#define MY_RF24_PA_LEVEL RF24_PA_MAX
//#define MY_RF24_DATARATE RF24_250KBPS

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

//Enable this if you're running the RFM69HW model.
//#define MY_IS_RFM69HW

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


//#include <EEPROM.h>
//#include <SPI.h>
//#include <ArduinoOTA.h>
//#include <MySensors.h>

// Battery report settings pin and sleep time
int BATTERY_SENSE_PIN = A0;  // select the input pin for the battery sense point

unsigned long SLEEP_TIME = 900000;  // sleep time between reads (seconds * 1000 milliseconds)
int oldBatteryPcnt = 0;


void setup() {
  // use the 1.1 V internal reference
  #if defined(__AVR_ATmega2560__)
    analogReference(INTERNAL1V1);
  #else
    analogReference(INTERNAL);
  #endif
}

void presentation()  {
  // Send the sketch version information
  // to the gateway and controller
  sendSketchInfo("Denumire Sketch", "1.0");

  // Register one or more sensors.
  // For available sensor types, see
  // https://home-assistant.io/components/sensor.mysensors/
  present(SENSOR_CHILD_ID, S_CUSTOM);
}

void loop() {
  // Send locally attached sensors data here

  // get the battery Voltage
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  #ifdef MY_DEBUG
  Serial.println(sensorValue);
  #endif

  // 1M, 470K divider across battery and using internal ADC ref of 1.1V
  // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
  // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
  // 3.44/1023 = Volts per bit = 0.003363075

  int batteryPcnt = sensorValue / 10;

  #ifdef MY_DEBUG
  float batteryV  = sensorValue * 0.003363075;
  Serial.print("Battery Voltage: ");
  Serial.print(batteryV);
  Serial.println(" V");

  Serial.print("Battery percent: ");
  Serial.print(batteryPcnt);
  Serial.println(" %");
  #endif

  if (oldBatteryPcnt != batteryPcnt) {
    // Power up radio after sleep
    sendBatteryLevel(batteryPcnt);
    oldBatteryPcnt = batteryPcnt;

}
