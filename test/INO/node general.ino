
#define MY_RADIO_NRF24 

#pragma region Includes 
// #include <Adafruit_SleepyDog.h> // Werkt niet wegens interferentie van __vector_6 
#include <SPI.h> 
#include <MySensors.h> 
#include <SparkFunTSL2561.h> 
#include <Wire. h> 
#include <HIH6130.h> 
#include <Adafruit_INA219.h> 
#pragma endregion 

#pragma region Child id definitions 
#define CHILD_ID_LIGHT 1 
#define CHILD_ID_MULTIMETER 2 
#define CHILD_ID_TEMPERATURE 3 
#define CHILD_ID_HUMIDITY 4 
#define CHILD_ID_DOOR 5 
#define CHILD_ID_MOTORCURRENT 7 
# define CHILD_ID_RELAIS1 8 
#define CHILD_ID_RELAIS2 9 
// #define CHILD_ID_POWER 10 
#pragma endregion 

#pragma region engine timing parameter settings 
#define MAX_LOOPTIJD 6000    
#define EXTRA_TIME_CLOSE 100    
#define RETURN_AFTER_FAULT_TIME 2000 // which 2000 
#define CAMERA_ON_TIME 90 // in seconds what 60 
#define DOOR_AFTER_CAMERA_TIME 60 // in seconds what 30 
#define SLEEP_CYCLES 25 
#define SLEEP_PERIOD 200 
#define LUX_CLOSE 10.0F 
#define LUX_OPEN 100.0f 
#pragma endregion 

#pragma region Pin definitions 
// pin definitions 
#define PushButtonOpenPin 3 // gels cable (right) sluiten op wit (midden) 
#define PushbuttonClosePin A3 // Blauwe cable (left) sluiten op wit (midden) 
#define RelayMotorOpenPin 5 
#define RelayMotorClosePin 4 
#define Relay2Pin 6 
#define Relay1Pin 7 
#define PirPin 8 
#define EndDetectionOpenPin A1 // Zwarte cable (right) sluiten op wit (midden) 
#define EndDetectionClosePin A0 // Groene cable (left) sluiten op wit (midden) 
#define BatteryVoltagePin A2 
#define MotorStatus_Stop 0 
#define MotorStatus_Open 1 
#define MotorStatus_Close 2 
#define MotorStatus_Retour 3 
#define DoorStatus_InBetween 0 
#define DoorStatus_Open 1 
#define DoorStatus_Closed 2 
#pragma endregion 

#pragma region Library object declarations 
byte HIH6130_address = 0x27; 
HIH6130 rht (HIH6130_address); 
Adafruit_INA219 ina; 
SFE_TSL2561 light; 
#pragma endregion 

#pragma region MySensors message declarations 
MyMessage lightMsg (CHILD_ID_LIGHT, V_LIGHT_LEVEL ); 
MyMessage currentMsg (CHILD_ID_MULTIMETER, V_CURRENT); 
MyMessage voltageMsg (CHILD_ID_MULTIMETER, V_VOLTAGE); 
MyMessage temperatureMsg (CHILD_ID_MULTIMETER, V_TEMP); 
MyMessage humidityMsg (CHILD_ID_MULTIMETER, V_HUM); 
MyMessage doorStatusMsg (CHILD_ID_DOOR, V_LOCK_STATUS); 
MyMessage motorCurrentMsg (CHILD_ID_MOTORCURRENT, V_CURRENT); 
MyMessage relais1Msg (CHILD_ID_RELAIS1, V_LIGHT); 
// MyMessage powerWattMsg (CHILD_ID_POWER, V_WATT); 
// MyMessage powerkWhMsg (CHILD_ID_POWER, V_KWH); 
#pragma endregion 

#pragma region Global status variable 
byte motor status; 
byte door status; 
byte load door status = DoorStatus_InBetween; 
bool button open load status = false; 
bool button close load status = false; 
bool doorOpenRequested = false; 
bool doorCloseRequested = false; 
bool cameraOn = false; 
bool cameraActivatedByLuxSensor = false; 
uint32_t camera startTime = 0; 
bool button open pressed = false; 
bool button close pressed = false; 
unsigned long motor stop time; 
boolean gain; // Gain setting, 0 = X1, X16 = 1; 
unsigned char time = 0; 
unsigned int ms; // Integration ( "shutter") time in milliseconds 
#pragma endregion 

#pragma region Global variables for sensor measurements 
double lux; 
float temp; 
float humidity; 
float current; 
float motor current; 
float currentSum; 
int counter current; 
float voltage; 
#pragma endregion 


void setup ()   
{ 
   Serial.begin (115200); 
   Serial.println ( "Kippenhok_4"); 

   #pragma region Pin setup and initialization 
   pinMode (PushButtonOpenPin, INPUT_PULLUP); 
   pinMode (PushbuttonClosePin, INPUT_PULLUP); 
   pinMode (RelayMotorOpenPin, OUTPUT); 
   pinMode ( RelayMotorClosePin, OUTPUT); 
   pinMode (Relay1Pin, OUTPUT); 
   pinMode (Relay2Pin, OUTPUT); 
   digitalWrite (RelayMotorOpenPin, LOW); 
   digitalWrite (RelayMotorClosePin, LOW); 
   digitalWrite (Relay1Pin, LOW); 
   digitalWrite (Relay2Pin, LOW); 
   pinMode (PirPin , INPUT_PULLUP); 
   pinMode (EndDetectionOpenPin, INPUT_PULLUP); 
   pinMode (EndDetectionClosePin, INPUT_PULLUP); 
   #pragma endregion 

   ActivateMotor (MotorStatus_Stop); 

   #pragma region Library object initialization 
   rht.begin (); 
   ina.begin (); 
   light.begin (); 
   unsigned char id; 
   gain = 0; 
   time = 1; 
   light.setTiming (gain, time, ms); 
   light.setPowerUp (); 
   delay (500); 
   Serial.println ( "application started"); 
   request time (); 
} 

uint16_t sleep counter = 0; 
int luxState = 0; // = Unknown 
uint32_t controller time; 
uint32_t second after midnight = 0; 
byte hours = 0; 
byte minutes = 0; 
byte seconds = 0; 
bool TimeReceived = false; 

void loop () 
{ 
// currentSum = 0.0; 
// current counter = 0; 
   Serial .println ( "... loop ..."); 
   TimeReceived = false; 
   request time (); 
   wait (50); 
   door status = GetDoorStatus (); 
   CheckHardware button (); // Affects -> button pressed open, close button pressed 
                                 // button open load status, button close load status 
                                 // doorOpenRequested, doorCloseRequested 
   MotorLoop (); 

   String door string; 
   switch (door status) 
   { 
      case DoorStatus_Open: 
         door string = "Open"; break; 
      case DoorStatus_InBetween: 
         door string = "inbetween"; break; 
      case DoorStatus_Closed: 
         door string = "Closed"; break; 
   } 

   // sensor processing 
   // Collect sensorData and transmitted through mySensors 
   rht.readRHT (); 
   temp = rht.temperature; 
   humidity = rht.humidity; 
   current = ina.getCurrent_mA () + 0.7; 
   voltage = ina.getBusVoltage_V ( ); 

   unsigned int data0, data1; 
   boolean good; // True if neither sensor is saturated 
   if (light.getData (data0, data1)) 
   { 
      // Geen saturatie time = 0, gain = 0 
      // Saturatie bij time = 1, gain = 0 -> Lux: 788.37 (data0 = 5047, data1 = 5047) 
      // Saturatie bij time = 2, gain = 0 -> 
      // Saturatie bij time = 0, gain = 1 -> Lux: 49.27 (data0 = 5047, data1 = 5047) 

      good = light .getLux (gain, ms, data0, data1, lux); 
      if (lux <805.0F && lux> 804.0F) 
         good = false; 
   } 

   if (luxState == 0) 
   { 
    // + 1 (wasOver) overdag, -1 (wasUnder) 's night 
         luxState = (lux> LUX_CLOSE 1: -1); 
   } 

   if (good) 
   { 
      if (luxState <0 && lux> LUX_OPEN) { 
         doorOpenRequested = true; 
         luxState = 1; 
      } 

      if (luxState> 0 && lux <LUX_CLOSE) { 
         // doorCloseRequested = true; 
         Serial.println ( "switching on the camera Because of sunset - door will close shortly"); 
         cameraActivatedByLuxSensor = true; 
         digitalWrite (Relay1Pin, HIGH); 
         cameraOn = true; 
         camera startTime = controller time ;      
         luxState = -1; 
      } 
   } 

   if (cameraActivatedByLuxSensor && controller time> camera startTime + DOOR_AFTER_CAMERA_TIME) 
   { 
      doorCloseRequested = true; 
      cameraActivatedByLuxSensor = false; 
      Serial.println ( "Door closing Because of sunset"); 
   } 

   Serial.print ( "Door"); 
   Serial.print (door string); 
   Serial.print ( ""); 
   if (good) { 
      Serial.print ( "Lux"); 
      Serial.print (lux); 
   } 
   else 
   { 
      Serial.print ( "Lux Bad"); 
   } 
   Serial.print ( "luxState"); 
   Serial.print (luxState); 
   Serial.print (cameraOn? "Camera on": "camera off"); 
   Serial.println (? CameraActivatedByLuxSensor "(CLUX)": "(-)"); 

   Serial.print ( "Battery"); 
   Serial.print (voltage); 
   Serial .print ( "V"); 
   Serial.print (current); 
   Serial.println ( "mA"); 

   // Send sensorData to the MySensors gateway 
   if (good) { 
      lightMsg.set (lux, 2); 
      send ( lightMsg); 
   } 

   currentMsg.set (current, 2); 
   send (currentMsg); 

   voltageMsg.set (voltage, 2); 
   send (voltageMsg); 

   temperatureMsg.set (temp, 2); 
   send (temperatureMsg); 

   humidityMsg.set ( humidity, 2); 
   send (humidityMsg); 

   int = door_status_mysensors door status == DoorStatus_Closed? 0: 1; 
   doorStatusMsg.set (door_status_mysensors); 
   send (doorStatusMsg); 

   byte batLvl = 0; 
   if (voltage> 12.0) batLvl = 50; 
   if (voltage> 12.5) batLvl = 75; 
   if (voltage> 12.6) = 80 batLvl ; 
   if (voltage> 12.7) batLvl = 90; 
   if (voltage> = 12.9) batLvl = 100; 
   send battery level (batLvl); 

   motorCurrentMsg.set (motor current, 2); 
   send (motorCurrentMsg); 

   relais1Msg.set (cameraOn); 
   send ( relais1Msg); 

   if (TimeReceived)! 
   { 
      Serial.print ( "- NO--"); 
      controller time + = (* SLEEP_PERIOD SLEEP_CYCLES) / 1000; 
   } 
   Serial.print ( "Time ="); 
   Serial.print (controller time) ; 
   second after midnight = controller time% 86400; 
   hours = second after midnight / 3600; 
   minutes = (second after midnight% 3600) / 60; 
   seconds = second after midnight% 60; 
   Serial.print ( ""); 
   Serial.print (hours); 
   Serial.print ( " : "); 
   Serial.print (minutes); 
   Serial.print (": "); 
   Serial.print (seconds); 
   Serial.println (); 
  
   // End of loop activities 
   load door status = door status; 

   // Sleep for 10 seconds unless one of the hardware buttons which pressed 
   sleep counter = 0; 
   while (button close pressed && button open pressed && (sleep counter <SLEEP_CYCLES)!) 
   { 
      // sleep (SLEEP_PERIOD); // Activate sleep (instead of wait) to decrease power usage - to be tested 
      wait (SLEEP_PERIOD); // Allows instructions from Domoticz to be sent to the node 
      sleep counter ++; 
      CheckHardware Buttons (); 
   } 

   if (cameraOn && controller time> camera startTime + CAMERA_ON_TIME) { 
      Serial.println ( "Switching OFF the camera (timer)"); 
      digitalWrite (Relay1Pin, LOW ); 
      cameraOn = false; 
      camera startTime = 0; 
   } 
  
} 

void receiveTime (unsigned long ts) { 
   //Serial.print("Time received "); 
   //Serial.println(ts); 
   controller time = ts; 
   TimeReceived = true; 
} 

void receive (const MyMessage & message) { 
   if (message.sensor == CHILD_ID_DOOR) { 
      uint8_t value = message.getByte (); 
      if (value == 0) doorCloseRequested = true; 
      if (value == 1) doorOpenRequested = true; 
   } 
   if (message.sensor == CHILD_ID_RELAIS1) { 
      uint8_t value = message.getByte (); 
      if (value == 0) { 
        digitalWrite (Relay1Pin, LOW); 
        Serial.println ( "Camera switched off remotely"); 
        cameraOn = false; 
        camera startTime = 0; 
      } 
      if (value == 1) { 
        digitalWrite (Relay1Pin, HIGH); 
        Serial.println ( "Camera switched ON remotely"); 
        cameraOn = true; 
        camera startTime = controller time; 
      } 
   } 
   if (message.sensor == CHILD_ID_RELAIS2 ) { 
      uint8_t value = message.getByte (); 
      if (value == 0) digitalWrite (Relay2Pin, LOW); 
      if (value == 1) digitalWrite (Relay2Pin, HIGH); 
   } 
} 


byte GetDoorStatus () 
{ 
   if (digitalRead ( EndDetectionClosePin) == LOW) 
      return DoorStatus_Closed; 
   if (digitalRead (EndDetectionOpenPin) == LOW) 
      return DoorStatus_Open; 
   return DoorStatus_InBetween; 
} 

void CheckHardware buttons () 
{ 
   button open pressed = digitalRead (PushButtonOpenPin) == LOW; 
   button close pressed = digitalRead (PushbuttonClosePin) == LOW ; 
   //Serial.print(buttonOpenPressed? "A": "a"); 
   ? //Serial.print(buttonClosePressed "B", "b"); 
   if (! Door status = DoorStatus_Closed && ((button close pressed && button close load status!))) 
      DoorCloseRequested = true; 
   if (door status! = DoorStatus_Open && ((button open pressed && button open load status)!)) 
      doorOpenRequested = true; 

   button close load status = button close pressed, 
   button open load status = button open pressed; 
} 

void MotorLoop () 
{ 
   unsigned long engine starting time, 

   door status = GetDoorStatus (); 
   if (door status = DoorStatus_Closed && doorCloseRequested)! 
   { 
      ActivateMotor (MotorStatus_Close); 
      doorCloseRequested = false; 
   } 
   if (door status = DoorStatus_Open && doorOpenRequested!) 
   { 
      ActivateMotor (MotorStatus_Open); 
      doorOpenRequested = false; 
   } 

   if (engine status = MotorStatus_Stop!) 
   { 
      currentSum = 0.0; 

      while (true) 
      { 
         current = ina .getCurrent_mA () + 0.7; 
         currentSum + = current; 
         current counter ++; 
         motor current = currentSum / current counter; 
         door status = GetDoorStatus (); 
         if (motor status == MotorStatus_Open && door status == DoorStatus_Open) break; 
         if (motor status == MotorStatus_Retour && door status == DoorStatus_Open) break; 
         if (motor status == MotorStatus_Close && door status == DoorStatus_Closed) { 
            wait (EXTRA_TIME_CLOSE); 
            break; 
         } 
         wait (10); 
         if (engine status = MotorStatus_Stop && millis (!)> motor stop time) 
         { 
            Serial.println ( "= - = - = - = - = - = Emergency door reopening = - = - = - = - = - = "); 
            wait (200); 
            ActivateMotor (MotorStatus_Retour); 
         } 
      } 
      ActivateMotor (MotorStatus_Stop); 
      // TODO: activatie van retour india de open tijd overschreden expectant. 
   } 
} 

unsigned long engine starting time; 

void ActivateMotor (int newStatus) 
{ 
   switch (newStatus) 
   { 
      case MotorStatus_Stop: 
         digitalWrite (RelayMotorOpenPin, LOW); 
         digitalWrite (RelayMotorClosePin, LOW); 
         doorOpenRequested = false; 
         doorCloseRequested = false ; 
         Serial.print (F ( "motor stopped at")); 
         Serial.println (millis ()); 
         break; 
      case MotorStatus_Open: 
      case MotorStatus_Retour: 
         digitalWrite (RelayMotorOpenPin, HIGH); 
         digitalWrite (RelayMotorClosePin, LOW); 
         engine starting time = millis ( ); 
         motor stop time = millis () + ((newStatus == MotorStatus_Open)? MAX_LOOPTIJD: RETURN_AFTER_FAULT_TIME); 
         Serial.print (F ( "engine started")); 
         if (newStatus == MotorStatus_Open) 
            Serial.print (F ( "open at")); 
         else 
            Serial.print (F ( "retour at") ); 
         Serial.println (millis ()); 
         break; 
      case MotorStatus_Close: 
         digitalWrite (RelayMotorOpenPin, LOW); 
         digitalWrite (RelayMotorClosePin, HIGH); 
         engine starting time = millis (); 
         motor stop time = millis () + MAX_LOOPTIJD; 
         Serial.print (F ( "engine started closing at")); 
         Serial.println (millis ()); 
         break; 
   } 
   engine status = newStatus; 
} 

void presentation () 
{ 
   #pragma region MySensors setup and initialization 
   send sketch info ( "kippenhok", "1.0"); 
   / / register all sensor to gateway (theywill be created as child devices) 
   present (CHILD_ID_LIGHT, S_LIGHT_LEVEL); 
   present (CHILD_ID_MULTIMETER, S_MULTIMETER); 
// present (CHILD_ID_POWER, S_POWER); 
   present (CHILD_ID_TEMPERATURE, S_TEMP); 
   present (CHILD_ID_HUMIDITY, S_HUM ); 
   present (CHILD_ID_DOOR, S_LOCK); 
   present (CHILD_ID_MOTORCURRENT, S_MULTIMETER); 
   present (10, S_ARDUINO_NODE); 
   present (CHILD_ID_RELAIS1, S_LIGHT); 
   present (CHILD_ID_RELAIS2, S_LIGHT); 
   #pragma endregion 
}
