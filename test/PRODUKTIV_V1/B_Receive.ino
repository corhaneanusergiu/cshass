void receive(const MyMessage &message)
{
	if (message.isAck())
	{
//		Serial.println("This is an ack from gateway for Pin Nr.");
//		Serial.println(message.sensor);
	}
		else
		{
				int i = message.sensor;
/*
				Serial.println("PIN");
				Serial.println(i);
				Serial.println("String");
				Serial.println(message.getString());
				Serial.println("TYPE");
				Serial.println(message.type);
*/				
				MyMessage msg(i, iomodus[i].variableType);

//********************************IF V_STATUS or V_LIGHT existst***************************************

				if (message.type == V_STATUS)
				{
//********************************IF RELAY**************************************
					if ((iomodus[i].sensorType == S_BINARY))
					{
						if (loadState(i) != message.getBool())
						{
#ifdef RELAY							
							if (iomodus[i].sensorVersion == "RELAY")
							{
								digitalWrite(i, message.getBool() ? RELAY_ON : RELAY_OFF);							
								saveState(i, message.getBool());
								send(msg.setSensor(i).set(message.getBool()),false);
							}
#endif //RELAY	
#ifdef BUTTON							
							if (iomodus[i].sensorVersion == "BUTTON")
							{
								saveState(i, message.getBool());
								send(msg.setSensor(i).set(message.getBool()),false);
							}
#endif //BUTTON	
						}
					}
				}

		
//********************************IF V_PERCENTAGE or V_DIMMER existst***************************************
				if ((message.type == V_PERCENTAGE))
				{
//***************PWM Rate 0 to 100 (%)**************
#ifdef PWM
					if ((i <= 13)||((i >= 44) && (i <= 46)))
					{
//						Serial.println("New PWM Value");
//						Serial.println(message.getInt());

						int wilo = message.getInt()+8;
						analogWrite(i, wilo); // Set Value 0-100
						last_int_value[i] = message.getInt();
						send(msg.setSensor(i).set(message.getInt()),false);
					}
#endif
//***************MOTOR Speed Rate 0 to 100 (%)**************
#ifdef MOTOR
					if ((i = 18)||((i = 20))
					{
						Serial.println("Got new Value for MOTOR");
						Serial.println(message.getInt());
						if ((message.getInt()<=100) && (message.getInt()>=0))
						{
							if (i = 18)
							{
								mot_speed_T4 = message.getInt();
								last_int_value[i] = message.getInt();
								send(msg.setSensor(i).set(message.getInt()),false);
								Serial.println("Value for MOTOR at P18 update");
							}
							if (i = 20)
							{
								mot_speed_T3 = message.getInt();
								last_int_value[i] = message.getInt();
								send(msg.setSensor(i).set(message.getInt()),false);
								Serial.println("Value for MOTOR at P20 update");
							}
						
						}
						else
						{
							Serial.println("Wrong Value for MOTOR");
						}
					}
#endif//MOTOR
				}
		}
}
