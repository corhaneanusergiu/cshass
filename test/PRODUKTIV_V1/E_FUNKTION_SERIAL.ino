void serial_read()
{
	while (Serial3.available()) 
	{
		// get the new byte:
		char inCharSerial3 = (char)Serial3.read();
		// add it to the inputString:
		
		inputStringSerial3 += inCharSerial3;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

		if (inCharSerial3 == '\n') 
		{
			Serial.print(inputStringSerial3);
			// clear the string:
			inputStringSerial3 = "";
			//Serial.flush();
		}
	}
	
	while (Serial2.available()) 
	{
		// get the new byte:
		char inCharSerial2 = (char)Serial2.read();
		// add it to the inputString:
		
		inputStringSerial2 += inCharSerial2;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

		if (inCharSerial2 == '\n') 
		{
			Serial.print(inputStringSerial2);
			// clear the string:
			inputStringSerial2 = "";
			//Serial.flush();
		}
	}
	while (Serial1.available()) 
	{
		// get the new byte:
		char inCharSerial1 = (char)Serial1.read();
		// add it to the inputString:
		
		inputStringSerial1 += inCharSerial1;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

		if (inCharSerial1 == '\n') 
		{
			Serial.print(inputStringSerial1);
			// clear the string:
			inputStringSerial1 = "";
			//Serial.flush();
		}
	}
	
}
