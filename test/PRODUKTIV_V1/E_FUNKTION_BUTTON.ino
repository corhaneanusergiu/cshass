
void button(int i)
{
#ifdef BUTTON
	
	if (
		(iomodus[i].sensorType == S_DOOR) ||
		(iomodus[i].sensorType == S_MOTION) ||
		(iomodus[i].sensorType == S_WATER_LEAK)
		)
		{
			if (millis() > next_Time[i])
			{
				next_Time[i] = next_Time[i] + 1000;  //digitaleingï¿½nge nicht hï¿½ufiger als alle 1000ms abfragen
	// ********************************************************************************************
				// After setting up the button, setup debouncer
				debouncer.attach(i);
				debouncer.interval(5);
				debouncer.update();
				// Get the update value
				bool value = debouncer.read();
				
				if (value != loadState(i) || complete_loop)
				{
					Serial.print("Current State of PIN: ");
					Serial.print(i);
					Serial.print(" changes to: ");
					Serial.print(value);
					Serial.println();// Print the Value to serial
					//           send(msg.setSensor(i).set(value, 1));value==HIGH ? 1 : 0
					MyMessage msg(i, iomodus[i].variableType);
					send(msg.set(value == HIGH ? 1 : 0),false);
					saveState(i, value);
				}
			}
		}
#endif // BUTTON
}
