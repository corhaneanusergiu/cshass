void presentDS18b20(int i)
{
#ifdef DS18B20
	if (iomodus[i].sensorVersion ==  "DS18B20")
	{
		for (int j = 1; j <= dsSensor_count; j++)
		{
			if (dsSensor[j].dsonPinNr == i)
			{
				unsigned int dsPin = (iomodus[i].Int_to_HLP + j);
				//Serial.println("dsPin");
				//Serial.println(dsPin);
				present(dsPin, iomodus[i].sensorType, dsSensor[j].dsDescription, false);
				dsPin = 0;
			} 
						
		}
	}
#endif
}
