void impulse(int i)
{
#ifdef IMPULSE_CNT
if ((iomodus[i].sensorType == S_WATER) && (iomodus[i].sensorVersion == "IMPULSEcount") && (i == 2 || i == 3 || (i >= 18 && i <= 21)))
{   
	if (millis() > next_Time[i])
	{
		byte offset = 23; 
		if (i == 2) 
		{
			offset = 4;
		} 
		if (i == 3) 
		{
			offset = 6;
		}
  
		detachInterrupt(offset-i);
        
      // Because this loop may not complete in exactly 1 second intervals we calculate
      // the number of milliseconds that have passed since the last execution and use
      // that to scale the output. We also apply the calibrationFactor to scale the output
      // based on the number of pulses per second per units of measure (litres/minute in
      // this case) coming from the sensor.
      //450 output pulses/liters, 
      //flowRate = Pulse P/s / 450 P/L*60s =L/min
		double flowRate= 60.00 *(10000.00/(10000.00 + millis()-next_Time[i]))* PULSEcounter[offset - i ] / PULSEdivider[offset - i ]; // L/Min
        //flowLitres = (flowRate/60);
/*        
        Serial.print(" PULSEcounter[offset - i ] ");
        Serial.println(PULSEcounter[offset - i ]);
        Serial.print(" flowRate ");
        Serial.println(flowRate);
*/      
		if (flowRate!=last_double_value[i])
		{
			MyMessage msg(i, iomodus[i].variableType);
			send(msg.setSensor(i).set(flowRate, 2), false);
			last_double_value[i] = flowRate;
		}
		
		PULSEcounter[offset - i ]=0;
      
		next_Time[i] = next_Time[i] + 10000; //onewire updates every 1s
		if ((i == 2))
		{pinMode(2, INPUT_PULLUP); attachInterrupt(0, ISR_0, FALLING);}
		
		if ((i == 3))
		{pinMode(3, INPUT_PULLUP); attachInterrupt(1, ISR_1, FALLING);}
		
		if ((i == 21))
		{pinMode(21, INPUT_PULLUP); attachInterrupt(2, ISR_2, FALLING);}
		
		if ((i == 20))
		{pinMode(20, INPUT_PULLUP); attachInterrupt(3, ISR_3, FALLING);}
		
		if ((i == 19))
		{pinMode(19, INPUT_PULLUP); attachInterrupt(4, ISR_4, FALLING);}
		
		if ((i == 18))
		{pinMode(18, INPUT_PULLUP); attachInterrupt(5, ISR_5, FALLING);}
    }
}
#endif
}

void ISR_0() //Interrupt D2
{
#ifdef IMPULSE_CNT
  PULSEcounter[0]++;
#endif // IMPULSE_CNT
}

void ISR_1() //Interrupt D3
{
#ifdef IMPULSE_CNT
  PULSEcounter[1]++;
#endif // IMPULSE_CNT 
}
void ISR_2() //Interrupt D21
{
#ifdef IMPULSE_CNT
  PULSEcounter[2]++;
#endif // IMPULSE_CNT 
}
void ISR_3() //Interrupt D20
{
#ifdef IMPULSE_CNT
  PULSEcounter[3]++;
#endif // IMPULSE_CNT 
}
void ISR_4() //Interrupt D19
{
#ifdef IMPULSE_CNT
  PULSEcounter[4]++;
#endif // IMPULSE_CNT 
}
void ISR_5() //Interrupt D18
{
#ifdef IMPULSE_CNT
  PULSEcounter[5]++;
#endif // IMPULSE_CNT 
}
