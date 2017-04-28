void ds18b20_temp(int i)
{
#ifdef DS18B20
	if ((iomodus[i].sensorType == S_TEMP) && (iomodus[i].sensorVersion == "DS18B20"))
    { 	
		if (millis() > next_Time[i])
        {   
			next_Time[i] = next_Time[i] + 10000; //onewire updates every 10s (10s is MINIMUM)
			sensor[i].requestTemperatures();
			// query conversion time and sleep until conversion completed
			//  int16_t conversionTime = sensor[i].millisToWaitForConversion(sensor[i].getResolution());
			// sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
			wait(1000);
    
			for (int j = 1; j <= dsSensor_count; j++)
			{
				if (dsSensor[j].dsonPinNr ==  i)
				{
					unsigned int dsPin=(iomodus[i].Int_to_HLP + j);
					float tempC = sensor[i].getTempC(dsSensor[j].dsAdress);
					if (tempC == -127.00) 
					{
/*            			send(msg.setType(77).set("Error getting temperature"), false);
						Serial.println("Error getting temperature on Pin");
						Serial.print(i);
						Serial.println("MySensors Pin Nr");
						Serial.print(dsPin);
						Serial.println("Probe Nr.");
						Serial.print(j);
*/
					} 
					else
					{
						MyMessage msg(i, iomodus[i].variableType);
						send(msg.setSensor(dsPin).set(tempC, 2), false);
						last_dsTemp[j] = tempC;
    /*        			        Serial.println("C: ");
						Serial.print(tempC);
						Serial.println(" F: ");
						Serial.print(DallasTemperature::toFahrenheit(tempC));
    */        		}
	//					dsPin=0;
              
				}
                    
			}
        }
	}
#endif
}
