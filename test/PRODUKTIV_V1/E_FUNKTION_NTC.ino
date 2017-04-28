void NTC_temp(int i)
{
#ifdef NTC

if ((iomodus[i].sensorType == S_TEMP) && (iomodus[i].sensorVersion == "NTC") && (i >= 54) && (i <= 69))//handling NTC Sensor
    {
		if (millis() > next_Time[i])
		{
			next_Time[i] = next_Time[i] + 10000;  //Update Value every 10s
			Rt = Rv / ((1024.0 / analogRead(i)) - 1.0);
			float tempNTC = 0;
			tempNTC = (B_wert * Tn / (B_wert + (Tn * log(Rt / Rn)))) - Tn + 25.0 + Temp_offset[i];// Here Offset if needed
			
			if ((tempNTC > (last_float_value[i] + delta_ntc)) || (tempNTC < (last_float_value[i] - delta_ntc))
				|| complete_loop)
			{
				MyMessage msg(i, iomodus[i].variableType);
				//          Serial.print("tempNTC of Pin ");
				//          Serial.print(i);
				//          Serial.print(" is ");
				//          Serial.println(tempNTC);
				send(msg.setSensor(i).set(tempNTC, 2), false);
			
				last_float_value[i] = tempNTC;
		
			}
		}
    }
#endif
}
