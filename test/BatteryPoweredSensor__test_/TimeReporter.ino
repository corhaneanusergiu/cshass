#include <SPI.h>
#include <MySensor.h>  
#include <Time.h>  
#include <avr\wdt.h> 

#define NODE_ID AUTO
#define CHILD_ID_GENERAL 0
#define CHILD_ID_POWER 1
#define CHILD_ID_TEMPERATURE 2
#define SketchName "TimeReporter"
#define SketchVersion "20150826"

#define UPDATE_TIME 100000L
#define REPORT_TIME 5000L

MySensor gw;
boolean timeReceived = false;
unsigned long ms_now;
unsigned long lastUpdate=0, lastRequest=0;

MyMessage msgGeneral(CHILD_ID_GENERAL, V_VAR1);
MyMessage msgPower(CHILD_ID_POWER, V_VOLTAGE);
MyMessage msgTemperature(CHILD_ID_TEMPERATURE, V_TEMP);

void setup()  
{  
  // watchdog 8s
  wdt_enable(WDTO_8S);
  // repeater mode on
  gw.begin(NULL,NODE_ID,true);
  // Send sketch version and information
  gw.sendSketchInfo(SketchName, SketchVersion);
  // Register sensors to gateway
  gw.present(CHILD_ID_GENERAL, S_ARDUINO_NODE,"Main node"); 
  gw.present(CHILD_ID_POWER, S_POWER,"ADC power",true); 
  gw.present(CHILD_ID_TEMPERATURE, S_TEMP,"Chip temp"); 
  // request time
  gw.requestTime(receiveTime);
    
}

// time callback
void receiveTime(unsigned long time) {
  // Ok, set incoming time 
  setTime(time);
  timeReceived = true;
}

long readMUX(uint8_t aControl) {
  long result;
  
  ADMUX = aControl;
  delay(20); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  return result;
}

long readVcc() {
 // Read 1.1V reference against AVcc
  return 1126400L / readMUX(_BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1));  
}

float readTemp() {
 // Read 1.1V reference against MUX3  
  return (readMUX(_BV(REFS1) | _BV(REFS0) | _BV(MUX3)) - 125) * 0.1075; 
}
 
void loop()     
{     
  // watchdog reset
  wdt_reset();
  // process incoming messages and repeater function
  gw.process();
  // get millis since start
  ms_now = millis();
  // time update needed?
  if (timeReceived && ms_now-lastRequest > UPDATE_TIME) {
    // Request time from controller. 
    gw.requestTime(receiveTime);  
    lastRequest = ms_now;
  }
  
  // report time
  if (timeReceived && ms_now-lastUpdate > REPORT_TIME) {
    
    gw.send(msgGeneral.set(ms_now));  
    char Output[10];
    String OutStr = String(hour()) + ":" + String(minute()) + ":" + String(second());
    OutStr.toCharArray(Output,10);
    
    gw.send(msgGeneral.set(Output));
    gw.send(msgTemperature.set(readTemp(),1));
    gw.send(msgPower.set(readVcc()),true);
    lastUpdate = ms_now;
  }

}