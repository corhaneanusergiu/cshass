#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <OneWire.h>    //für Temperatursensoren DS18B20 see here
//http://technobabble.prithvitech.com/simplifying-multiple-onewire-buses-on-arduino/
//http://www.hacktronics.com/code/OneWire.zip
#include <DallasTemperature.h>

// Enable debug prints to serial monitor
#define MY_DEBUG

#define MY_GATEWAY_SERIAL

// Enable to UDP
// #define MY_USE_UDP

// #define MY_GATEWAY_W5100
#ifdef MY_GATEWAY_W5100
// Enable MY_IP_ADDRESS here if you want a static ip address (no DHCP)
#define MY_IP_ADDRESS 192,168,111,99

// If using static ip you need to define Gateway and Subnet address as well
#define MY_IP_GATEWAY_ADDRESS 192,168,111,1
#define MY_IP_SUBNET_ADDRESS 255,255,255,0



// The MQTT broker port to to open
#define MY_PORT 5003

// Controller ip address. Enables client mode (default is "server" mode).
// Also enable this if MY_USE_UDP is used and you want sensor data sent somewhere.
#define MY_CONTROLLER_IP_ADDRESS 192, 168, 111, 97

// The MAC address can be anything you want but should be unique on your network.
// Newer boards have a MAC address printed on the underside of the PCB, which you can (optionally) use.
// Note that most of the Ardunio examples use  "DEAD BEEF FEED" for the MAC address.
#define MY_MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED

// When W5100 is connected we have to move CE/CSN pins for NRF radio
#define MY_RF24_CE_PIN 5
#define MY_RF24_CS_PIN 6

#endif


// W5100 Ethernet module SPI enable (optional if using a shield/module that manages SPI_EN signal)
// #define MY_W5100_SPI_EN 4

// Enable Soft SPI for NRF radio (note different radio wiring is required)
// The W5100 ethernet module seems to have a hard time co-operate with
// radio on the same spi bus.
#if !defined(MY_W5100_SPI_EN) && !defined(ARDUINO_ARCH_SAMD)
#define MY_SOFTSPI
#define MY_SOFT_SPI_SCK_PIN 14
#define MY_SOFT_SPI_MISO_PIN 16
#define MY_SOFT_SPI_MOSI_PIN 15
#endif

#ifdef MY_USE_UDP
#include <EthernetUdp.h>
#endif

#include <Ethernet.h>
#include <MySensor.h>
#include <SoftwareSerial.h>

#include <stddef.h>
#include <stdint.h>

#include <Bounce2.h>



// #############################################################################################
// #############################################################################################
//I/O-Kennung: hier wird die Funktion aller verwendbaren IOös mit einer Kennziffer festgelegt
//dabei haben alle IOös die Standardfunktionen plus spez. Sonderfunktionen
//     Standardfunktionen sind  STANDARD TYPE:
//      '1' =MY_SystemPin; '2' =DS18B20; '3'=MAX31855; '4' =RELAY '5' =DHTxx; '6' =PWM, '7' =IMPULSEcount, '8'=NTC, '9'=BUTTON,

typedef struct s_iomodus 
  {
  mysensor_sensor sensorType;
  mysensor_data variableType;
  const char* sensorDescription;
  const char* sensorVersion;
  const unsigned char Int_to_HLP;
  } iomodus_t;

iomodus_t iomodus[] = 
{
/*Please fill in the RIGH Sensor Type and Variable Type from the SERIAL API
    http://www.mysensors.org/download/
    {presentation, set/req, description}
*/
//	SENSOR			VARIABLE		DESCRIPTION			    		STANDARD TYPE		OFFSET
//	sensorType		variableType	sensorDescription				sensorVersion		Int_to_HLP
//__________________________________Relay An / Relay Aus____________________________________________________________
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"MY_SystemPin", 	0}, 	//D0 DO NOT TOUCH
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"MY_SystemPin", 	0}, 	//D1 DO NOT TOUCH
  { S_UNUSED, 		V_UNKNOWN, 		"Dach / Garage FREI",  	 		"tbd", 				0}, 	//D2 : PWM?      = IR_Rx??  '=IMPULSEcount??; =tft?????;
  { S_UNUSED, 		V_UNKNOWN, 		"Dach / Garage FREI",  	 		"tbd", 				0}, 	//D3 : PWM?        = 433_Rx??   =IMPULSEcount;  =tft??;
  { S_TEMP, 		V_TEMP, 		"Temperaturen Puffer",   		"DS18B20", 			100}, 	//D4 : PWM?        = 433_Tx??   =lcd;      =tft??;
  { S_TEMP, 		V_TEMP, 		"Temperaturen WW",  	 		"DS18B20", 			200}, 	//D5 : PWM?                        =lcd;      =tft??;
  { S_DIMMER,		V_PERCENTAGE,	"Umschicht Pumpe",  	 		"PWM", 				0}, 	//D6 : PWM?        = buzzer     =lcd;      =tft??;
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D7 : PWM?                        =lcd;      =tft??;
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D8 : PWM?                        =lcd;      =tft??;
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D9 : PWM?        = IR_Tx??   =lcd;      =tft??;
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D10 :       = W5100 SS-Pin;
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D11 : PWM?
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D12 : PWM?
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D13 : PWM?
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D14 : TX3
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS", 				"tbd", 				0}, 	//D15 :	RX3
  { S_UNUSED, 		V_UNKNOWN, 		"Serial Dach TX2",  		 	"MY_SystemPin",		0}, 	//D16 : SCK KType CHECK for "DESCRIPTIONS",  "MAX31855 NOT connected"
  { S_UNUSED, 		V_UNKNOWN, 		"Serial Dach RX2",  		 	"MY_SystemPin",		0}, 	//D17 : CS KType CHECK for "DESCRIPTIONS",  "MAX31855 NOT connected"
  { S_UNUSED, 		V_UNKNOWN,		"Serial FBH TX1",  	    	 	"MY_SystemPin", 	0}, 	//D18 :/TX1 :   =IMPULSEcount;	mot_speed_T4
  { S_UNUSED, 		V_UNKNOWN, 		"Serial FBH RX1",  	    	 	"MY_SystemPin", 	0}, 	//D19 :/RX1 :   =IMPULSEcount;
  { S_UNUSED, 		V_UNKNOWN, 		"Dach / Garage FREI",  		 	"tbd", 				0}, 	//D20 :/SDA :   =IMPULSEcount;   mot_speed_T3
  { S_WATER, 		V_FLOW, 		"Durchfluss Solar",  		 	"IMPULSEcount", 	0}, 	//D21 :/SCL :   =IMPULSEcount;   
  { S_BINARY, 		V_STATUS, 		"Umschicht / WP-PUF",  		 	"RELAY",            0}, 	//D22 : 2er Relay Oben
  { S_BINARY, 		V_STATUS, 		"Schichten / >Solar",  		 	"RELAY", 			0}, 	//D23 : 2er Relay Unten
  { S_BINARY, 		V_STATUS, 		"WP zum WW / 4Wege",  		 	"RELAY", 	        0}, 	//D24 : 2er Relay Oben
  { S_BINARY, 		V_STATUS, 		"Solar>Schichtrohr / 4W",	 	"RELAY", 	    	0}, 	//D25 : 2er Relay Unten
  { S_BINARY, 		V_STATUS, 		"FREI",  	    			 	"RELAY",			0}, 	//D26 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"WP Temp 64C / Orig",  		 	"RELAY",			0}, 	//D27 : 8er Relay Vertikal http://www.bapihvac.com/wp-content/uploads/2010/11/Thermistor_1.8K.pdf
  { S_BINARY, 		V_STATUS, 		"FREI",  	    			 	"RELAY",			0}, 	//D28 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"HV aus / HV an",  	    	 	"RELAY",			0}, 	//D29 : 8er Relay Vertikal
  { S_BINARY, 		V_STATUS, 		"FREI",  	    			 	"RELAY", 			0},		//D30 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"Solar>PUF / Solar>WW",			"RELAY", 			0}, 	//D31 : 8er Relay Vertikal
  { S_BINARY, 		V_STATUS, 		"FREI",  	    				"RELAY", 			0}, 	//D32 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"RL PUF zu / >Solar",			"RELAY", 			0}, 	//D33 : 8er Relay Vertikal
  { S_BINARY, 		V_STATUS, 		"Zirkulation AN",  	    		"RELAY", 			0}, 	//D34 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"Schichten mitte / >oben",		"RELAY", 			0}, 	//D35 : 8er Relay Vertikal
  { S_BINARY, 		V_STATUS, 		"FREI",  	    		 		"RELAY", 			0}, 	//D36 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"WP zu / WP>PUF o. WW", 		"RELAY", 			0}, 	//D37 : 8er Relay Vertikal
  { S_BINARY, 		V_STATUS,		"WP AUS",  	            		"RELAY", 			0}, 	//D38 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS,		"FBH Mischer AN",  	    		"RELAY", 			0}, 	//D39 : 8er Relay Vertikal
  { S_UNUSED, 		V_UNKNOWN, 		"DESCRIPTIONS",  	    		"tbd", 				0}, 	//D40 : 8er Relay Horizontal
  { S_BINARY, 		V_STATUS, 		"FBH Kälter / FBH Heißer",		"RELAY", 			0}, 	//D41 : 8er Relay Vertikal
  { S_UNUSED, 		V_UNKNOWN, 		"FBH 1",  	    				"tbd", 				0}, 	//D42 :
  { S_UNUSED, 		V_UNKNOWN, 		"FBH 1",  	    				"tbd", 				0}, 	//D43 :
  {	S_DIMMER, 		V_PERCENTAGE, 	"Pumpe 1 10V",  				"PWM",				0}, 	//D44 : PWM?
  {	S_UNUSED, 		V_UNKNOWN, 		"FBH 2",		  	   			"tbd", 				0}, 	//D45 : PWM?
  {	S_DIMMER, 		V_PERCENTAGE,	"Pumpe 2 10V",	 				"PWM", 				0}, 	//D46 : PWM?
  { S_UNUSED, 		V_UNKNOWN, 		"FBH 2",			 			"tbd", 				0}, 	//D47 :
  { S_UNUSED, 		V_UNKNOWN, 		"FBH 2",			 			"tbd", 				0}, 	//D48 :
  { S_UNUSED, 		V_UNKNOWN, 		"FBH 2",			 			"tbd", 				0}, 	//D49 :
  { S_UNUSED, 		V_UNKNOWN,	 	"FBH 2",			 			"tbd", 				0}, 	//D50 : MISO       =W5100;   =CC3000;   ICSP-Stecker
  { S_UNUSED, 		V_UNKNOWN,	 	"FBH 2",			 			"tbd", 				0}, 	//D51 : MOSI       =W5100;   =CC3000;   ICSP-Stecker
  { S_UNUSED, 		V_UNKNOWN,	 	"FBH 2",			 			"tbd", 				0}, 	//D52 : SCK        =W5100;   =CC3000;   ICSP-Stecker
  { S_UNUSED, 		V_UNKNOWN,	 	"FBH 2",			 			"tbd", 				0}, 	//D53 :SS                       =CC3000;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D54 : A0 : =analog;  =NTC;  =tft??;  =lcd;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D55 : A1 : =analog;  =NTC;  =tft??;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D56 : A2 : =analog;  =NTC;  =tft??;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D57 : A3 : =analog;  =NTC;  =tft??;
  { S_BINARY, 		V_STATUS,	 	"Steckdose 1",  				"RELAY", 			0}, 	//D58 : A4 : =analog;  =NTC;  =tft??;
  { S_BINARY, 		V_STATUS,	 	"Steckdose 2",  				"RELAY", 			0}, 	//D59 : A5 : =analog;  =NTC;
  { S_BINARY, 		V_STATUS,	 	"Steckdose 3",  				"RELAY", 			0}, 	//D60 : A6 : PWM_5V
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D61 : A7 : PWM_5V
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D62 : A8 : =analog;  =NTC;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D63 : A9 : =analog;  =NTC;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D64 : A10 : =analog;  =NTC;
  { S_UNUSED, 		V_UNKNOWN,	 	"DESCRIPTIONS", 				"tbd", 				0}, 	//D65 : A11 : =analog;  =NTC;
};
const size_t iomodus_count = sizeof(iomodus) / sizeof(*iomodus);
// ###############################################################################


#define DS18B20
//#define MY_SystemPin
//#define Temp_MAX31855
#define RELAY
//#define BUTTON
//#define DHTxx
#define PWM
#define IMPULSE_CNT
//#define NTC
//#define MOTOR
//#define ANALOG

// ################################################################################
// +++++++++++++++++++++++Variables for Onewire DS18XXX++++++++++++++++++++++++++++
#ifdef DS18B20

//DS18B20-Adressen: hier werden die Adresen der Angeschlossenen Sensoren festgehalten
//Adresse, Beschreibung, An welchem Pin 

typedef struct s_dsSensor 
  {
  DeviceAddress dsAdress;
  const char* dsDescription;
  int dsonPinNr;
  } dsSensor_t;

dsSensor_t dsSensor[] = {

//DeviceAddress dsSensor[] = //Adress-Array definieren
//|Address											|DESCRIPTION			|Connected |
//|_________________________________________________|_______________________|to PIN____|_____________
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	"NC", 					0},			//00 leer
  {0x28, 0xFF, 0x58, 0x1E, 0x71, 0x15, 0x02, 0x25, 	"SolarVL", 				0},			//01 Solar
  {0x28, 0xFF, 0x33, 0x1F, 0x71, 0x15, 0x02, 0xCB, 	"NC", 					0},			//02
  {0x28, 0xFF, 0x1F, 0x27, 0x71, 0x15, 0x02, 0x46, 	"NC", 					0},			//03
  {0x28, 0xFF, 0x8A, 0x1D, 0x71, 0x15, 0x02, 0x70, 	"NC", 					0},			//04
  {0x28, 0xFF, 0xDF, 0x8D, 0x71, 0x15, 0x02, 0xD6, 	"FBH VL", 				4},			//05
  {0x28, 0xFF, 0x85, 0x1D, 0x71, 0x15, 0x02, 0x1F, 	"BUF VL WP&HV",			4},			//06
  {0x28, 0xFF, 0x9E, 0x6C, 0x71, 0x15, 0x02, 0x34, 	"BUF top", 				4},			//07
  {0x28, 0xFF, 0x5F, 0x27, 0x71, 0x15, 0x02, 0xAF, 	"BUF mid", 				4},			//08
  {0x28, 0xFF, 0x4B, 0x27, 0x71, 0x15, 0x02, 0xCC, 	"BUF down",				4},			//09
  {0x28, 0xFF, 0xD1, 0x1D, 0x71, 0x15, 0x02, 0x95, 	"BUF to Solar", 		4},			//10
  {0x28, 0xFF, 0x6D, 0x1E, 0x71, 0x15, 0x02, 0x73, 	"FBH RL gesamt", 		4},			//11
  {0x28, 0xFF, 0xD1, 0x4D, 0x01, 0x15, 0x04, 0x7F, 	"WW down", 				5},			//12
  {0x28, 0xFF, 0x63, 0x3E, 0x70, 0x14, 0x04, 0x13, 	"SOLAR after WW", 		5},			//13
  {0x28, 0xFF, 0x81, 0x31, 0x70, 0x14, 0x04, 0xD0, 	"SOLAR VL", 			5},			//14
  {0x28, 0xFF, 0xCA, 0x57, 0x70, 0x14, 0x04, 0xD0, 	"HV RL / Umschicht RL",	5},			//15
  {0x28, 0xFF, 0x72, 0x59, 0x70, 0x14, 0x04, 0xB4, 	"WW mid", 				5},			//16
  {0x28, 0xFF, 0x72, 0xC4, 0x01, 0x15, 0x03, 0xCB, 	"WP RL", 				5},			//17
  {0x28, 0xFF, 0x80, 0x57, 0x70, 0x14, 0x04, 0x84, 	"FREI", 				5},			//18
  {0x28, 0xFF, 0xCF, 0x57, 0x70, 0x14, 0x04, 0x02, 	"NC", 					0},			//19
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	"NC", 					0},			//20
  {0x28, 0xFF, 0x62, 0x9B, 0x70, 0x14, 0x04, 0xF6, 	"NC", 					0},			//21
  {0x28, 0xFF, 0x13, 0xC7, 0x01, 0x15, 0x03, 0x9F, 	"NC", 					0} 			//22
};
const size_t dsSensor_count = sizeof(dsSensor) / sizeof(*dsSensor);

//const int oneWirePinsCount = iomodus_count;
OneWire ds18x20[iomodus_count];
DallasTemperature sensor[iomodus_count];



const float delta_onewire = 0.2; //Deltas für Sendeauslösung
const float delta_ntc = 0.5; //in C
float last_dsTemp[dsSensor_count];

#endif

// ######################################################################
// +++++++++++++++++++++++Variables for RELAY+++++++++++++++++++++++
#ifdef RELAY
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
#define RELAY_ON 0  // GPIO value to write to turn on attached RELAY (SSR RELAY)
#define RELAY_OFF 1 // GPIO value to write to turn off attached RELAY (SSR RELAY)
#define RELAY_STD 0 // Value for NORMAL (no power) Position of attached RELAY (Blue Relay)
#define RELAY_ACT 1 // Value for ACTIVE (powered) Position of attached RELAY (Blue Relay)
Bounce debouncer = Bounce();
#endif

// ***********************************************************************
// +++++++++++++++++++++++Variables for Temp sensors+++++++++++++++++++++++

// ######################################################################
// +++++++++++++++++++++++Variables for MAX31855+++++++++++++++++++++++++

#ifdef Temp_MAX31855
#include <MAX31855.h>
#define MAX_COUNT 10
#define DIFF_Ktype 20 // max differenz x10
#define WRONG_COUNT 10

int good[iomodus_count][MAX_COUNT];
int countGood[iomodus_count]={0};
int countBad[iomodus_count]={0};
int wrong[iomodus_count]={0};
int cycleCounter[iomodus_count]={0};
int ktype_MAX_COUNT= 10;
int clPin, csPin;

#endif

// ######################################################################
// +++++++++++++++++++++++Variables for NTC++++++++++++++++++++++++++++++
#ifdef NTC
// +++++++++++++++++++++++Variables for NTC
const float B_wert = 3950; //aus dem Datenblatt des NTC //<<user-eingabe<<
const float Tn = 298.15; //25Celsius in Kelvin
const float Rv = 10000; //Vorwiderstand
const float Rn = 10000; //NTC-Widerstand bei 25öC
float Rt;
const int delta_analog = 2;

#endif


// ######################################################################
// +++++++++++++++++++++++Variables for MOTOR++++++++++++++++++++++++++++
#ifdef MOTOR

#include <TimerFive.h>
#include <TimerThree.h>

volatile int T3;               // Variable to use as a counter of dimming steps. It is volatile since it is passed between interrupts
volatile int T5;

//int AC_pin_T5 = 18;              // Output to Opto Triac HARDCODED
//int AC_pin_T3 = 20;              // Output to Opto Triac HARDCODED
int mot_speed_T3 = 100;           // in % Speed level (0-100)  0 = off, 100 = full speed
int mot_speed_T5 = 100;           // in % Speed level (0-100)  0 = off, 100 = full speed
//1 Second = 1.000.000
int freqStep = 20000;             // Means 20ms ONE FULL wave. Every 20ms we check if we should switch SSR off to get the right speed
//if speed is 50 (50%) we set Pin HIGH every 200ms, but after 5x calling Timer (i=50)Time = 100ms we set PIN low for next 5x calling of timer)
//if speed is 10 (10%) we set Pin HIGH every 200ms, but after 1x calling Timer (i=10)Time = 20ms we set PIN low for next 9x calling of timer)

#endif

// ######################################################################
// +++++++++++++++++++++++Variables for IMPULSE_CNT+++++++++++++++++++++
#ifdef IMPULSE_CNT
// **********************************************************************
//Variables for Flow Sensor /IMPULSE COUNTER
//PULSEvalue = PULSEcounter[offset - i ] / DEVIDEfactor[offset - i ];
// f.e. f=4.8* q(l/min)-> q(l/min)=f/4.8->DEVIDEfactor=4.8
const float delta_counter = 5; //in counter inkrement
volatile unsigned long PULSEcounter[6] =
{
  0, 	//Count status for D2 -Impulse Input after RESET
  0, 	//Count status for D3 -Impulse Input after RESET
  0, 	//Count status for D21-Impulse Input after RESET
  0, 	//Count status for D20-Impulse Input after RESET
  0, 	//Count status for D19-Impulse Input after RESET
  0 	//Count status for D18-Impulse Input after RESET
};
//hier wird der DEVIDEfactor für die Impulszaehler festgelegt
const int PULSEdivider[6] =
{ 6, 	//DEVIDEfactor D2 :   		<<user INPUT
  1, 	//DEVIDEfactor D3 :   		<<user INPUT
  1, 	//DEVIDEfactor D21 :  		<<user INPUT
  1, 	//DEVIDEfactor D20 :  		<<user INPUT
  1, 	//DEVIDEfactor D19 :  		<<user INPUT
  450, 	//DEVIDEfactor D18 :		<<user INPUT
};		
#endif

// ######################################################################
// +++++++++++++++++++++++GLOBAL Variables ++++++++++++++++++++++++++++++
// **********************************************************************
float Temp_offset[iomodus_count] = {9999};
//Serial Variables
//SoftwareSerial SerialTwo(62, 63);

//LAST_Variables
bool last_bool_value[iomodus_count];
int last_int_value[iomodus_count];
float last_float_value[iomodus_count];
double last_double_value[iomodus_count];
unsigned long last_ulong_value[6];

//Variables for Loop
unsigned long next_full_loop = 0;
unsigned long delta_time = 3600000; // jede Stunde werden alle Inputs aktualisiert
boolean complete_loop = 1; // wenn 1, dann einmal komplett durchlaufen
unsigned long next_Time[iomodus_count];
unsigned long next_uTime[iomodus_count];
String inputStringSerial1 = "";
String inputStringSerial2 = "";
String inputStringSerial3 = "";
// **********************************************************************
// **********************************************************************		
