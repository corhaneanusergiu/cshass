void presentation()
{
	//delay(4500);
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("ETHduino by JR", "7.0");
	Serial1.begin(2400);
	Serial2.begin(2400);
	
	//all Ports
	for (int i = 0; i < iomodus_count; i++)
	{
		MyMessage msg(i, iomodus[i].variableType);
//		Serial.println(iomodus[i].sensorType);
//		Serial.println(i);
		if (iomodus[i].sensorVersion != "tbd" && iomodus[i].sensorVersion != "MY_SystemPin")
		{
			if (iomodus[i].sensorVersion == "DS18B20")
			{	
				#ifdef DS18B20
					presentDS18b20(i);
				#endif
			} else
			{
				present(i, iomodus[i].sensorType, iomodus[i].sensorDescription, false);
				//Send a et message which shows in controller where Warnings will come
//				send(msg.setType(77).set("for Warnings"), false);
			}
		}
		
	}
		Serial2.println("0;0;3;0;19;force presentation");

		if (Serial2.available()) 
		{
			Serial.print(Serial2.readString());
			Serial.flush();
		}
	
	
	
}

