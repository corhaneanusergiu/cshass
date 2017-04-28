/**
 *
 * DESCRIPTION
 *
 * Vibration Sensor
 *
 * connect the sensor as follows :
 *
 * VCC       >>> 5V
 * S         >>> D3
 * GND       >>> GND
 *
 * Based on: http://www.dfrobot.com/wiki/index.php/DFRobot_Digital_Vibration_Sensor_V2_SKU:DFR0027
 * Contributor: epierre
**/

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>
#include <Wire.h>

#define CHILD_ID_VIBRATION 0
#define VIBRATION_SENSOR_DIGITAL_PIN 3
#define SensorLED     13

unsigned long SLEEP_TIME = 10*1000; // Sleep time between reads (in seconds)

//VARIABLES
int val = 0;           // variable to store the value coming from the sensor
float valVIBRATION =0.0;
float lastVIBRATION =0.0;
unsigned char state = 0;

MyMessage vibrationMsg(CHILD_ID_VIBRATION, V_LEVEL);

void setup()
{
  pinMode(VIBRATION_SENSOR_DIGITAL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(VIBRATION_SENSOR_DIGITAL_PIN), blink, FALLING); // Trigger the blink function when the falling edge is detected
  pinMode(SensorLED, OUTPUT);
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("VIBRATION Sensor", "1.0");

  // Register all sensors to gateway (they will be created as child devices)
  present(CHILD_ID_VIBRATION, S_VIBRATION);
}

void loop()
{

  if(state>=40){ // basically below 40 so ignire basic level
        send(vibrationMsg.set(int(state)));
        state = 0;
        digitalWrite(SensorLED,HIGH);
   }    else {
        state = 0;
        digitalWrite(SensorLED,LOW);
  }


  // Power down the radio.  Note that the radio will get powered back up
  // on the next write() call.
  delay(1000); //delay to allow serial to fully print before sleep

  sleep(SLEEP_TIME); //sleep for: sleepTime
}

void blink()//Interrupts function
{
  state++;
}
