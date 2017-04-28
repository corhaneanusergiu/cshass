//

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensors.h>  
#include <BH1750.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 3 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
int BATTERY_SENSE_PIN = A0;  // select the input pin for the battery sense point
int SOLAR_SENSE_PIN = A1;  // select the input pin for the solar sense point
#define CHILD_ID_BATTERY 4
#define CHILD_ID_SOLAR 6
#define CHILD_ID_LIGHT 1
#define CHILD_ID_TEMP1 2

BH1750 lightSensor;
float TempC = 50.5;
int oldBatteryPcnt = 0;
MyMessage msgTempC(CHILD_ID_TEMP1,V_TEMP);
MyMessage msgbatt(CHILD_ID_BATTERY, V_VOLTAGE);
MyMessage msgsolar(CHILD_ID_SOLAR, V_VOLTAGE);
MyMessage msg(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
uint16_t lastlux;  //lux

void setup(void)
{
// use the 1.1 V internal reference
  #if defined(__AVR_ATmega2560__)
  analogReference(INTERNAL1V1);
  #else
  analogReference(INTERNAL);
  #endif
  
// Send the sketch version information to the gateway and Controller
  sendSketchInfo("Battery Meter", "2.0");
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
   
  sensors.begin(); // start reading sensor
  
  // search for devices on the bus and assign based on an index.  ideally,
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  
  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  //sensors.setResolution(insideThermometer, 9);
}

void loop(void)
{
  
 // get the battery Voltage
  int sensorValue2 = analogRead(BATTERY_SENSE_PIN);
  delay(1000);   
  
  int sensorValue1 = analogRead(BATTERY_SENSE_PIN);
  delay(100);
  
  int sensorValue = analogRead(BATTERY_SENSE_PIN);
  delay(1000);
    
  int sensorValueS = analogRead(SOLAR_SENSE_PIN);
  delay(1000);
 
#ifdef MY_DEBUG
  Serial.print("Battery Voltage2: ");
  Serial.println(sensorValue2);
  Serial.print("Battery Voltage1: ");
  Serial.println(sensorValue1);
  Serial.print("Battery Voltage: ");
  Serial.println(sensorValue);
  Serial.print("Solar Voltage: ");
  Serial.println(sensorValueS);
#endif

  // 1M, 470K divider across battery and using internal ADC ref of 1.1V
  // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
  // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
  // 3.44/1023 = Volts per bit = 0.003363075
  const float vRef = 4.200/ 1.05/ 1023 ;  
  int batteryPcnt = sensorValue / 10;
  float batteryV  = sensorValue * 0.0042598 ; // 0.0038952294568380753114792412093 max 4,2 volt
  float batteryS  = sensorValueS * 0.0102459 ; // 0.0038952294568380753114792412093 max 10 volt
  float TempC = sensors.getTempC(insideThermometer);

#ifdef MY_DEBUG
  Serial.print("DS1820 Temperature: ");
  Serial.print(TempC);
  Serial.println(" C");
send(msgTempC.set(TempC ,2));
  
  Serial.print("Battery Voltage: ");
  Serial.print(batteryV);
  Serial.println(" V");
send(msgbatt.set(batteryV ,2));

  Serial.print("Solar Voltage: ");
  Serial.print(batteryS);
  Serial.println(" V");
send(msgsolar.set(batteryS ,2));

  Serial.print("Battery percent: ");
  Serial.print(batteryPcnt);
  Serial.println(" %");
#endif

   if (oldBatteryPcnt != batteryPcnt) {
    // Power up radio after sleep
    sendBatteryLevel(batteryPcnt);
    oldBatteryPcnt = batteryPcnt;
  }
      
  uint16_t lux = lightSensor.readLightLevel();// Get Lux value
  Serial.println(lux);
  if (lux != lastlux) {
      send(msg.set(lux));
      lastlux = lux;
  }
  sleep(SLEEP_TIME);
}
