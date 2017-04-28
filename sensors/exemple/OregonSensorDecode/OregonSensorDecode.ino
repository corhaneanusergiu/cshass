/**
 *
 * REVISION HISTORY
 * WARNING: I use MySensors V1.6.0 (dev branch) (https://github.com/mysensors/Arduino/tree/development/libraries)
 *
 * Version 1.0 - Hubert Mickael <mickael@winlux.fr> (https://github.com/Mickaelh51)
 *  - Clean ino code
 *  - Add MY_DEBUG mode in library
 * Version 0.2 (Beta 2) - Hubert Mickael <mickael@winlux.fr> (https://github.com/Mickaelh51)
 *  - Auto detect Oregon 433Mhz
 *  - Add battery level
 *  - etc ...
 * Version 0.1 (Beta 1) - Hubert Mickael <mickael@winlux.fr> (https://github.com/Mickaelh51)
 *
 *******************************
 * DESCRIPTION
 * This sketch provides an example how to implement a humidity/temperature from Oregon sensor.
 * - Oregon sensor's battery level
 * - Oregon sensor's id
 * - Oregon sensor's type
 * - Oregon sensor's channel
 * - Oregon sensor's temperature
 * - Oregon sensor's humidity
 *
 * MySensors gateway <=======> Arduino UNO <-- (PIN 2) --> 433Mhz receiver <=============> Oregon sensors
 */

// Enable debug prints
#define MY_DEBUG

#define MY_NODE_ID 10

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensor.h>
#include <Oregon.h>

//Define pin where is 433Mhz receiver (here, pin 2)
#define MHZ_RECEIVER_PIN 2
//Define maximum Oregon sensors (here, 3 differents sensors)
#define COUNT_OREGON_SENSORS 3

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_BAT 2

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgBat(CHILD_ID_BAT, V_VAR1);

void setup ()
{

  Serial.println("Setup started");

  //Setup received data
  attachInterrupt(digitalPinToInterrupt(MHZ_RECEIVER_PIN), ext_int_1, CHANGE);

  Serial.println("Setup completed");
}

void presentation()
{
  // Send the Sketch Version Information to the Gateway
  sendSketchInfo("Oregon Sensor", "1.0");

  // Present all sensors to controller
  for (int i=0; i<COUNT_OREGON_SENSORS; i++) {
     present(i, S_TEMP);
     present(i, S_HUM);
     present(i, S_CUSTOM); //battery level
  }
}



void loop () {
    //------------------------------------------
    //Start process new data from Oregon sensors
    //------------------------------------------
    cli();
    word p = pulse;
    pulse = 0;
    sei();
    if (p != 0)
    {
        if (orscV2.nextPulse(p))
        {
            //Decode Hex Data once
            const byte* DataDecoded = DataToDecoder(orscV2);
            //Find or save Oregon sensors's ID
            int SensorID = FindSensor(id(DataDecoded),COUNT_OREGON_SENSORS);

            // just for DEBUG
            OregonType(DataDecoded);
            channel(DataDecoded);

            //Send messages to MySenors Gateway
            send(msgTemp.setSensor(SensorID).set(temperature(DataDecoded), 1));
            send(msgHum.setSensor(SensorID).set(humidity(DataDecoded), 1));
            send(msgBat.setSensor(SensorID).set(battery(DataDecoded), 1));
        }

    }
}
