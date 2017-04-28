//#define MY_DEBUG
#define MY_RADIO_NRF24
#define MY_NODE_ID 66
//#define MY_PARENT_NODE_ID 0
//#define MY_PARENT_NODE_IS_STATIC

#include <MySensors.h>
#include "Vcc.h"

#define SKETCH_NAME "MySlim2aaBatteryNode"

#define PRIMARY_CHILD_ID 3
#define PRIMARY_BUTTON_PIN 3

MyMessage msg(PRIMARY_CHILD_ID, V_TRIPPED);

const float VccMin   = 1.7;           // Minimum expected Vcc level, in Volts.
const float VccMax   = 3.3;           // Maximum expected Vcc level, in Volts.
const float VccCorrection = 3.496 / 3.572;  // Measured Vcc by multimeter divided by reported Vcc

const int32_t report_interval = 30000; // 1day -> h * m * s * ms NOTICE: milliseconds, not microseconds!

Vcc vcc(VccCorrection);

#ifdef MY_DEBUG
void before(){
    Serial.begin(9600);
}
#endif

void setup()
{
    pinMode(PRIMARY_BUTTON_PIN, INPUT);
}

void presentation()
{
    sendSketchInfo(SKETCH_NAME, __DATE__);
    present(PRIMARY_CHILD_ID, S_DOOR, "Reed Contact");
}

void loop()
{
    int32_t timestamp = millis();

    uint8_t reedState;
    static uint8_t lastReedState = 2;
    static int32_t lastBatteryReport = -report_interval; // for inital report
    sleep(5); // Short delay to allow buttons to properly settle

    reedState = digitalRead(PRIMARY_BUTTON_PIN);

    if ( (timestamp-lastBatteryReport) >= report_interval ) {
      uint8_t batteryPercent = (uint8_t)vcc.Read_Perc(VccMin, VccMax);
      sendBatteryLevel(batteryPercent);
      lastBatteryReport = timestamp;
    }

    if (reedState != lastReedState) {
        // Value has changed from last transmission, send the updated reedState
        send(msg.set(reedState==HIGH));
        lastReedState = reedState;
    }

    sleep(PRIMARY_BUTTON_PIN-2, CHANGE, 0);
}
