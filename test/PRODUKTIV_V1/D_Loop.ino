


// ######################################################################		
void loop()
{
// *********************************************************************************************
// *********************************************************************************************
	serial_read();

	
/*	
	if (Serial2.available()) 
	{
		//Serial.write(Serial2.read());
		Serial.print(Serial2.readString());
		//Serial.flush();

	}
	if (Serial1.available()) 
	{
		Serial.write(Serial1.read());
		//Serial.flush();
	}
*/
	complete_loop = 0;
	if (millis() > next_full_loop) //mindestens jede Stunde eine komplette Aktualisierung
	{
		complete_loop = 1; next_full_loop = millis() + delta_time;
		if (next_full_loop < millis()) 
		{ 
			complete_loop = 0; 
		}   //wichtig wegen Zahlensprung
       //von millis() alle 50 Tage

	
	}

// *********************************************************************************************
// *********************************************************************************************

    for (int i = 0; i < iomodus_count; i++) //behandlung aller Ports D2 bis D69
    {

		while (iomodus[i].sensorType == S_UNUSED) { i++; }  // unbenutzte pins überspringen

//		MyMessage msg(i, iomodus[i].variableType);

// ********************************************************************************************
// DIGITAL TASTER ON THE BOARD
#ifdef BUTTON
		button(i);
#endif // BUTTON

// *********************************************************************************************
// ONEWire with DALLAS
#ifdef DS18B20
		ds18b20_temp(i);
#endif // DS18B20

// *********************************************************************************************
// ONEWire with MAX31855
#ifdef Temp_MAX31855
		MAX31855_temp(i);
#endif // MAX31855

// *********************************************************************************************
// NTC (Analog ONLY) INPUTS ON THE BOARD
#ifdef NTC
		NTC_temp(i);
#endif // NTC    
    
     // *********************************************************************************************
  // IMPULSEcounter (D2, D3, D18, D19, D20,D21  ONLY) INPUTS ON THE BOARD
#ifdef IMPULSE_CNT
		impulse(i);
#endif
    
// *********************************************************************************************
// ANALOG INPUTS
#ifdef ANALAOG
		ANALOGinput(i);
#endif


  // **************************   ende loop  *****************************************************
}
}



// #############################################################################################
// #############################################################################################
// #############################  Unterprogramme   #############################################


