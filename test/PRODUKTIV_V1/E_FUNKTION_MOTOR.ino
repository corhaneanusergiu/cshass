void off_check_T3() 
{
#ifdef MOTOR
  if (T3 >= mot_speed_T3)
  {
    digitalWrite(20, LOW);  // turn off SSR
    //      Serial.println("OFF");
    //      Serial.println(T3);
  }
  else
  {
    digitalWrite(20, HIGH);
    //      Serial.println("ON");
    //      Serial.println(T3);
  }
  T3 += 5;
  if (T3 >= 100) {
    T3 = 0;
  }
#endif
}
//SSR für den Motor am PIN 18*********************************************************************************************
void off_check_T5() 
{
#ifdef MOTOR
  if (T5 >= mot_speed_T5)
  {
    digitalWrite(18, LOW);  // turn off SSR
    //      Serial.println("OFF");
    //      Serial.println(T5);
  }
  else
  {
    digitalWrite(18, HIGH);
    //      Serial.println("ON");
    //      Serial.println(T5);
  }
  T5 += 5;
  if (T5 >= 100) {
    T5 = 0;
  }
#endif
}
// *********************************************************************************************
