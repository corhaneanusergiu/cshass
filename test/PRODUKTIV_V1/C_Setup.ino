void setup()
{
	//Serial1.begin(2400);
	//Serial2.begin(2400);
	
	for (int i = 0; i<iomodus_count; i++) //search All Sensor system Ports and set them in the RIGHT way
    {
		while (iomodus[i].sensorVersion == "MY_SystemPin") { i++; }  //do not waste time to check MY_SystemPin's
		MyMessage msg(i, iomodus[i].variableType);
//*************************************************************************************
//*************************************************************************************

//********************************Setup RELAY***************************************
		if (iomodus[i].sensorType == S_BINARY)
        {
#ifdef RELAY
			if (iomodus[i].sensorVersion == "RELAY")
			{
				pinMode(i, OUTPUT);
				// Set RELAY to last known state (using eeprom storage)
				digitalWrite(i, loadState(i) ? RELAY_ON : RELAY_OFF);
				send(msg.setSensor(i).set(loadState(i)==HIGH ? 1 : 0),false);
			}
#endif // RELAY

//********************************Setup BUTTON***************************************
#ifdef BUTTON
			if (iomodus[i].sensorVersion == "BUTTON")
			{
				pinMode(i, INPUT_PULLUP);
				// Set RELAY to last known state (using eeprom storage)
				digitalWrite(i, loadState(i) ? 1 : 0);
				send(msg.setSensor(i).set(loadState(i)==HIGH ? 1 : 0),false);
			}
#endif // BUTTON
		}
//********************************Setup 0-10V Output***************************************
#ifdef PWM
		if (iomodus[i].sensorType == S_DIMMER) 
        {
			pinMode(i, OUTPUT);
			// Set RELAY to last known state (using eeprom storage)
		}
#endif // PWM

//********************************Setup FLOWmeter***************************************
#ifdef IMPULSE_CNT
        //f=4.8* q( l/min) Fehler:& plusmn; 2% Spannung: 3.5-24vdc, Strom nicht mehr als 10ma, 450 Ausgang Impulse/Liter,
        if ((iomodus[i].sensorType == S_WATER) && (iomodus[i].sensorVersion == "IMPULSEcount"))
        {
			if ((PULSEdivider[0] > 0) && (i == 2))
			{pinMode(2, INPUT_PULLUP); attachInterrupt(0, ISR_0, FALLING);}
			if ((PULSEdivider[1] > 0) && (i == 3))
			{pinMode(3, INPUT_PULLUP); attachInterrupt(1, ISR_1, FALLING);}
			if ((PULSEdivider[2] > 0) && (i == 21))
			{pinMode(21, INPUT_PULLUP); attachInterrupt(2, ISR_2, FALLING);}
			if ((PULSEdivider[3] > 0) && (i == 20))
			{pinMode(20, INPUT_PULLUP); attachInterrupt(3, ISR_3, FALLING);}
			if ((PULSEdivider[4] > 0) && (i == 19))
			{pinMode(19, INPUT_PULLUP); attachInterrupt(4, ISR_4, FALLING);}
			if ((PULSEdivider[5] > 0) && (i == 18))
			{pinMode(18, INPUT_PULLUP); attachInterrupt(5, ISR_5, FALLING);}
		}
#endif // IMPULSE_CNT

//********************************Setup MOTOR-Speed_Control***************************************
#ifdef MOTOR
        if ((iomodus[i].sensorType == S_WATER) && (iomodus[i].sensorVersion == "MOTORspeed"))
        {
        if (i == 18)
        {
        pinMode(18, OUTPUT);                          // Set the Triac pin as output
        Timer5.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
        Timer5.attachInterrupt(off_check_T5, freqStep);      //
    }
        if (i == 20)
        {
        pinMode(20, OUTPUT);                          // Set the Triac pin as output
        Timer3.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
        Timer3.attachInterrupt(off_check_T3, freqStep);      //
    }

    }
#endif // MOTOR

//********************************Setup ONEWire***************************************
#ifdef DS18B20
		if ((iomodus[i].sensorType == S_TEMP) && (iomodus[i].sensorVersion == "DS18B20"))
        {
			ds18x20[i].setPin(i);
			sensor[i].setOneWire(&ds18x20[i]);
			// Startup up the OneWire library
			sensor[i].begin();
			
			for (int j = 1; j <= dsSensor_count; j++)
			{
				if (dsSensor[j].dsonPinNr ==  i )
				{
				sensor[i].setResolution(dsSensor[j].dsAdress, 10);
				}
					
			}
    }
#endif // DS18B20

//********************************Setup ANALOG INPUTS for READING VOLTAGE***************************************

		if (((iomodus[i].sensorVersion == "NTC") || (iomodus[i].sensorVersion == "ANALOG"))&&(i >= 54) && (i <= 69))//handling NTC Sensor
        {
			pinMode(55, INPUT_PULLUP);
		}


		}
		

}
