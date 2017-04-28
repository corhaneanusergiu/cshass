void MAX31855_temp(int i)
{

#ifdef Temp_MAX31855
	

    if ((iomodus[i].sensorType == S_TEMP) && (iomodus[i].sensorVersion == "MAX31855"))
    { 
		int status;
		MAX31855 tc(clPin, csPin, i);
		if (millis() > next_Time[i])
		{
            next_Time[i] = next_Time[i] + 200;  //5 Readings per second
        
			MAX31855 tc(clPin, csPin, i);
			tc.begin();
			status = tc.read();
//        	Serial.println("status");
//        	Serial.println(status);
			MyMessage msg(i, iomodus[i].variableType);
//			send(msg.setType(77).set("STATUS"), false);
//			send(msg.setType(77).set(status), false);
        
			if (status !=0 ) //Ktype 0 = OK, 2 und 4 Kurzschluss, 8=NICHT VORHANDEN/NICHTS ANGESCHLOSSEN
			{
				if (status == 2) //Short to GND
				{
					//Serial.println("KType Short to GND");
					
					send(msg.setType(77).set("KType_SHORT_TO GND"), false);
				}
				if (status == 4) //Short to VCC
				{
					//Serial.println("KType Short to VCC");
					//MyMessage msg(i, iomodus[i].variableType);
					send(msg.setType(77).set("KType_SHORT_TO VCC"), false);
				}
				if (status == 8) //Thermocouple not connected
				{
				//MyMessage msg(i, iomodus[i].variableType);
				//Serial.println("Thermocouple NOT connected");
				send(msg.setType(77).set("MAX31855 NOT connected"), false);
				}
			}
//        else
//        {
// ********checking if Offset was set***********
			if (Temp_offset[i]==9999)
			{
				send(msg.setType(V_VAR1).set("OFFSET NOT SET"), false);
			}
			// ********if Offset was set/ Set the Offset***********
			else
			{
				tc.setOffset(Temp_offset[i]);
			}
			
// ********Read Temp***********
			tc.read();
			int new_T_ktype=0;
			new_T_ktype = 10* tc.getTemperature();
		
			if (new_T_ktype < 2000 && new_T_ktype!=0) 
			{
				if (countGood[i] < ktype_MAX_COUNT) 
				{
					//write first 10 values to array
					good[i][countGood[i]] = new_T_ktype;
					countGood[i]++;
					Serial.print("T_START:");
					Serial.print(new_T_ktype);
					Serial.print("Nr. Good");
					Serial.println(countGood[i]);
					if (countGood[i] == ktype_MAX_COUNT) 
					{
						// find maximum from the first 10 Values
						new_T_ktype = -3000;
						for (int m = 0; m < ktype_MAX_COUNT; m++) 
						{
							if (good[i][m] > new_T_ktype) 
							{
							new_T_ktype = good[i][m];
							}
						}
// write this max value to all places from Array
						for (int m = 0; m < ktype_MAX_COUNT; m++) 
						{
							good[i][m] = new_T_ktype;
						}
                  
						last_int_value[i] = new_T_ktype;
					}	
            } 
            else 
            {
//wenn Value = +- DIFF dann soll es verarbeitet werden
				if ((last_int_value[i]  > new_T_ktype && last_int_value[i] - new_T_ktype  <= DIFF_Ktype) ||
                (last_int_value[i] <= new_T_ktype && new_T_ktype  - last_int_value[i] <= DIFF_Ktype)) 
                {
					last_int_value[i] = new_T_ktype;
					for (int m = 1; m < ktype_MAX_COUNT; m++) 
					{
						good[i][m - 1] = good[i][m];
						last_int_value[i] += good[i][m];
					}
                  
					good[i][ktype_MAX_COUNT - 1] = new_T_ktype;
					// calculate avg
					last_int_value[i] /= ktype_MAX_COUNT; // todo: long
					Serial.println("AVG Temp");
					Serial.println(last_int_value[i]);
					send(msg.setType(77).set("AVG"), false);
					send(msg.setType(77).set(last_int_value[i],2), false);
					wrong[i] = 0;
                } 
                else 
                {
					wrong[i]++;
					if (wrong[i] > WRONG_COUNT) 
					{
					// reset
					countGood[i] = 0;
					countBad[i]  = 0;
					wrong[i]     = 0;
					}
					Serial.println("Temp wrong x 10:\t");
					Serial.print(new_T_ktype);
					Serial.println("\t");
					Serial.println(wrong[i]);
//              	Serial.println("Status:\t");
//              	Serial.println(status);
                }
              
            }
			}
			else 
			{
				Serial.print("T_WRONG");
				Serial.println(new_T_ktype);
				send(msg.setType(77).set("wrong"), false);
				send(msg.setType(77).set(new_T_ktype,2), false);
			}
            cycleCounter[i]++;
            if (cycleCounter [i]> 50)  
            {
				cycleCounter [i]= 0;
//              if (countGood[i] >= ktype_MAX_COUNT) 
//              {  
					Serial.print("-----------T x 10------------:\t");
					Serial.println(last_int_value[i]);
					float temp_to_send;
					temp_to_send = last_int_value[i]/10.0;
					Serial.println(temp_to_send);
					send(msg.setSensor(i).set(temp_to_send,2),false);
//              	last_int_value[i] = last_int_value[i];
//              	last_int_value[i]=0;
//              }
            } 
      
        }
//      }   
    }
#endif

}
