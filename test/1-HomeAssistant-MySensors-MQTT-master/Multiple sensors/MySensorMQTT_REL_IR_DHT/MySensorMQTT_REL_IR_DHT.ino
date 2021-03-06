// Relay(4) + IR receiver (HX1838 kit)+ TempDHT 11 or 22
#include <MyTransportNRF24.h>
#include <MySensor.h>
#include <SPI.h>
#include <DHT.h>  
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
MyHwATMega328 hw;
MySensor gw(radio, hw);
#include <IRremote.h>

#define NODE_ID 10

#define CHILD_ID_REL1 1
#define CHILD_ID_REL2 2
#define CHILD_ID_REL3 3
#define CHILD_ID_REL4 4
#define CHILD_ID_HUM 5
#define CHILD_ID_TEMP 6

#define REL1_PIN 8
#define REL2_PIN 7
#define REL3_PIN 6
#define REL4_PIN 5
#define IR_PIN 4

#define HUMIDITY_SENSOR_DIGITAL_PIN 3

MyMessage msgREL1(CHILD_ID_REL1, V_LIGHT);
MyMessage msgREL2(CHILD_ID_REL2, V_LIGHT);
MyMessage msgREL3(CHILD_ID_REL3, V_LIGHT);
MyMessage msgREL4(CHILD_ID_REL4, V_LIGHT);
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

DHT dht;

bool StateREL=0, StateREL1=0, StateREL2=0, StateREL3=0, StateREL4=0;
const unsigned long tUpdate=60000; // update interval
unsigned long t0;
byte IrAct;
IRrecv irrecv(IR_PIN);
decode_results results;

float Temp, Hum;

void setup() {
  gw.begin(incomingMessage, NODE_ID, true);
  gw.sendSketchInfo("RelIRTemp", "1.0");
  gw.present(CHILD_ID_REL1, S_LIGHT);
  gw.present(CHILD_ID_REL2, S_LIGHT);
  gw.present(CHILD_ID_REL3, S_LIGHT);
  gw.present(CHILD_ID_REL4, S_LIGHT);
  gw.present(CHILD_ID_HUM, S_HUM);
  gw.present(CHILD_ID_TEMP, S_TEMP);
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN);

  pinMode(REL1_PIN, OUTPUT);
  digitalWrite(REL1_PIN, StateREL1);
  pinMode(REL2_PIN, OUTPUT);
  digitalWrite(REL2_PIN, StateREL2);
  pinMode(REL3_PIN, OUTPUT);
  digitalWrite(REL3_PIN, StateREL3);
  pinMode(REL4_PIN, OUTPUT);
  digitalWrite(REL4_PIN, StateREL4);
  pinMode(IR_PIN, INPUT);
  irrecv.enableIRIn();
  t0=millis();
}

void loop() {
  gw.process();
  IRr();
  if ((millis()-t0) > tUpdate) ServerUpdate();
}

void ServerUpdate() {
  Hum = dht.getHumidity();
  Temp = dht.getTemperature();
  gw.send(msgTemp.set(Temp, 1));
  gw.send(msgHum.set(Hum, 1));
  t0=millis();
}

void incomingMessage(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_LIGHT) {
    StateREL=strcmp(message.getString(),"ON")==0;
    switch(message.sensor) {
      case 1: {
        StateREL1=StateREL;
        if (StateREL) {
          digitalWrite(REL1_PIN, HIGH);
        }
        else {
          digitalWrite(REL1_PIN, LOW);
        }
        gw.send(msgREL1.set(StateREL1 ? "ON":"OFF"));
      }
      break;
      case 2: {
        StateREL2=StateREL;
        if (StateREL) {
          digitalWrite(REL2_PIN, HIGH);
        }
        else {
          digitalWrite(REL2_PIN, LOW);
        }
        gw.send(msgREL2.set(StateREL2 ? "ON":"OFF"));
      }
      break;
      case 3: {
        StateREL3=StateREL;
        if (StateREL) {
          digitalWrite(REL3_PIN, HIGH);
        }
        else {
          digitalWrite(REL3_PIN, LOW);
        }
        gw.send(msgREL3.set(StateREL3 ? "ON":"OFF"));
      }
      break;
      case 4: {
        StateREL4=StateREL;
        if (StateREL) {
          digitalWrite(REL4_PIN, HIGH);
        }
        else {
          digitalWrite(REL4_PIN, LOW);
        }
        gw.send(msgREL4.set(StateREL4 ? "ON":"OFF"));
      }
      break;

    }

   } 
}

void IRr(){
  unsigned long IC1=0;
  if (irrecv.decode(&results)) {
    IC1 = results.value;
    switch (IC1) {
      case 16724175: IrAct=1; break; // 1
      case 16718055: IrAct=2; break; // 2
      case 16743045: IrAct=3; break; // 3
      case 16716015: IrAct=4; break; // 4
      case 16769055: IrAct=10; break; // -
      case 16754775: IrAct=20; break; // +
      default:IrAct=0; break;
      }
  irrecv.resume();

  switch(IrAct) {
      case 1: {
        StateREL1=!StateREL1;
        if (StateREL1) {
          digitalWrite(REL1_PIN, HIGH);
        }
        else {
          digitalWrite(REL1_PIN, LOW);
        }
        gw.send(msgREL1.set(StateREL1 ? "ON":"OFF"));
      }
      break;
      case 2: {
        StateREL2=!StateREL2;
        if (StateREL2) {
          digitalWrite(REL2_PIN, HIGH);
        }
        else {
          digitalWrite(REL2_PIN, LOW);
        }
        gw.send(msgREL2.set(StateREL2 ? "ON":"OFF"));
      }
      break;
      case 3: {
        StateREL3=!StateREL3;
        if (StateREL3) {
          digitalWrite(REL3_PIN, HIGH);
        }
        else {
          digitalWrite(REL3_PIN, LOW);
        }
        gw.send(msgREL3.set(StateREL3 ? "ON":"OFF"));
      }
      break;
      case 4: {
        StateREL4=!StateREL4;
        if (StateREL4) {
          digitalWrite(REL4_PIN, HIGH);
        }
        else {
          digitalWrite(REL4_PIN, LOW);
        }
        gw.send(msgREL4.set(StateREL4 ? "ON":"OFF"));
      }
      break;
      case 10: {
        StateREL1=0;
        StateREL2=0;
        StateREL3=0;
        StateREL4=0;
        digitalWrite(REL1_PIN, LOW);
        digitalWrite(REL2_PIN, LOW);
        digitalWrite(REL3_PIN, LOW);
        digitalWrite(REL4_PIN, LOW);
        gw.send(msgREL1.set("OFF"));
        gw.send(msgREL2.set("OFF"));
        gw.send(msgREL3.set("OFF"));
        gw.send(msgREL4.set("OFF"));
      }
      break;
      case 20: {
        StateREL1=1;
        StateREL2=1;
        StateREL3=1;
        StateREL4=1;
        digitalWrite(REL1_PIN, HIGH);
        digitalWrite(REL2_PIN, HIGH);
        digitalWrite(REL3_PIN, HIGH);
        digitalWrite(REL4_PIN, HIGH);
        gw.send(msgREL1.set("ON"));
        gw.send(msgREL2.set("ON"));
        gw.send(msgREL3.set("ON"));
        gw.send(msgREL4.set("ON"));
      }
      break;
    }
  }
}



/* IR Remote codes HEX:
 *  16753245  FFA25D  CH-
 *  16736925  FF629D  CH
 *  16769565  FFE21D  CH+
 *  16720605  FF22DD  <<
 *  16712445  FF02FD  >>
 *  16761405  FFC23D  >|
 *  16769055  FFE01F  -
 *  16754775  FFA857  +
 *  16748655  FF906F  EQ
 *  16738455  FF6897  0
 *  16750695  FF9867  100+
 *  16756815  FFB04F  200+
 *  16724175  FF30CF  1
 *  16718055  FF18E7  2
 *  16743045  FF7A85  3
 *  16716015  FF10EF  4
 *  16726215  FF38C7  5
 *  16734885  FF5AA5  6
 *  16728765  FF42BD  7
 *  16730805  FF4AB5  8
 *  16732845  FF52AD  9
 */
 
