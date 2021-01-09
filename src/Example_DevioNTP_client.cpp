#include <Arduino.h>
#include "NTP_Devio.h"

unsigned long prevMillis = 0;
NTP_Devio NBtime;
void setup()
{
    Serial.begin(9600);
    NBtime.TimeBegin();
}

void loop()
{
    if(millis() - prevMillis > 1000)
    {
      prevMillis = millis();
      Serial.println("Current Time 0 :"+NBtime.getTime()); 
      // Serial.println("Current Time 1 :"+NBtime.getTime(1));
      // Serial.println("Current Time 2 :"+NBtime.getTime(2));
      Serial.println("Current date :"+NBtime.currentDate);
      Serial.println("only Hour :"+NBtime.onlyHour);
      Serial.println("only Minute :"+NBtime.onlyMinute);
      Serial.println("only Day :"+NBtime.onlyDay);
      Serial.println("only Mouth :"+NBtime.onlyMouth);
      Serial.println("only Year :"+NBtime.onlyYear);
    }
}