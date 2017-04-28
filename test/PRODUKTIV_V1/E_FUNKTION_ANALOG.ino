void ANALOGinput(int i)
{
#ifdef ANALOG
if ((iomodus[i].sensorType == S_TEMP) && (iomodus[i].sensorVersion == "ANALOGinput") && (i >= 54) && (i <= 69))//handling NTC Sensor
{ 
	if (millis() > next_Time[i])
    { 
		next_Time[i] = next_Time[i] + 1000; //analogeingänge nicht häufiger als alle 1000ms abfragen
		int ANALOGinput = analogRead(i);
		if ((ANALOGinput > (last_int_value[i] + delta_analog))
        || (ANALOGinput < (last_int_value[i] - delta_analog)) || complete_loop)
		{
			Serial.print("ANALOGinput ");
			Serial.print(i);
			Serial.println(ANALOGinput);
			send(msg.setSensor(i).set(ANALOGinput), false);
			last_int_value[i] = ANALOGinput;
		}
    }
}
#endif
}
