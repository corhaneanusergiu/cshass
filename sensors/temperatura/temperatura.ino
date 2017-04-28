#include <SPI.h>
#include <MySensor.h>  
#include <Wire.h>

#define NODE_ID 44
#define TEMP_CHILD 1

MySensor gw;
float temperature;
MyMessage tempMsg(TEMP_CHILD, V_ID);

void setup() 
{
   gw.begin(NULL,NODE_ID);
   gw.sendSketchInfo("Minir", "1.0");
   gw.present(TEMP_CHILD, S_TEMP);
}

void loop() 
{
   Serial.println("start");
   temperature = 100.0;
   Serial.print("temperature:");
   Serial.print(temperature);
   Serial.println("°");
   gw.send(tempMsg.set(temperature, 1));
   gw.sleep(5000);
}

-------------------

  // Startup and initialize MySensors library. Set callback for incoming messages.
  gw.begin();

  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("Temperature Sensor", "1.1");

  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();

  // Present all sensors to controller
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {   
     gw.present(i, S_TEMP);
  }


----------------

