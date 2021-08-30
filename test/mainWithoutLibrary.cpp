//Littles Noom(BinaryBear) 19/12/2020 Prototype 
#include <Arduino.h>
#include <HardwareSerial.h>
#define hwResetPin 26

#define Rxpin 16
#define Txpin 17
HardwareSerial NB(1);
String data_input;
String hourNTP;
String dateNTP;
String minuteNTP;
String onlyDayNTP,onlyMonthNTP,onlyYearNTP;
unsigned long prevMillis = 0;

void reboot_module() 
{
  pinMode(hwResetPin, OUTPUT); 
  digitalWrite(hwResetPin, LOW);
  delay(1000);
  digitalWrite(hwResetPin, HIGH);
  delay(2000); 
}

void SetupModule()
{
  reboot_module();
  NB.println(F("AT"));
  while(1)
  {  
    if(NB.available())
    {
     data_input = NB.readStringUntil('\n');
     if(data_input.indexOf(F("OK")) != -1)
      {
        Serial.println("Module Ready");
        break;
      }
      else if (data_input.indexOf(F("ERROR")) != -1) 
      {
        delay(500);
        NB.println(F("AT"));
        delay(100);
        Serial.print(F("."));
      }    
    }
  }
}

void SetUpNTPServer()
{
  Serial.println("Set NTP Server");
  //NB.println(F("AT+CSNTPSTOP"));
  NB.println(F("AT+CURTC=1"));
  delay(500);
  NB.println(F("AT+CRESET"));
  delay(500);
  // NB.println(F("AT+CSNTPSTART=\"1.th.pool.ntp.org\",+28"));
  NB.println(F("AT+CSNTPSTART=\"1.th.pool.ntp.org\",+07"));
  while (1) {
    if (NB.available()) 
    {
      data_input = NB.readStringUntil('\n');
      // Serial.println(data_input);
      if (data_input.indexOf(F("OK")) != -1) 
      {
        Serial.println("NTP Server Start...");
        break;
      }
      else if (data_input.indexOf(F("ERROR")) != -1) {
        Serial.println("NTP Server Register yet...");
        break;
      }
    }
  }
  Serial.print(".");
}

String calltimeNTP(unsigned int format)
{
  String time_buffer;
  String thai_time;
  NB.println(F("AT+CCLK?"));

  delay(10);
  while (1)
  {
    if (NB.available())
    {
      data_input = NB.readStringUntil('\n');
      if (data_input.indexOf(F("+CCLK:")) != -1) 
      {
        // Serial.println(data_input);
        time_buffer = data_input;
        // +CSNTP: 20/02/07,07:33:48:28+07  Raw Data
        ///////////////RESET and Substring Formate TIME /////////////////////
        String time_sub = time_buffer.substring(7, time_buffer.length() - 4);
        String day_sub = time_buffer.substring(7, time_buffer.length() - 13);

        String year_sub = day_sub.substring(0, 2);
        String mount_sub = day_sub.substring(3, 5);
        String date_sub = day_sub.substring(6, 8);

        String hour_sub = time_sub.substring(9, 11);
        String minuteAsec_sub = time_sub.substring(11, time_sub.length());
        ////For Time Thai Zone

        dateNTP = date_sub + "/" + mount_sub + "/20" + year_sub;
        minuteNTP = minuteAsec_sub.substring(1,3);
        hourNTP = hour_sub;

        onlyDayNTP = date_sub;
        onlyMonthNTP = mount_sub;
        onlyYearNTP = "20"+year_sub;

        if (format == 0) //Time : dd/mm/yy >>>17:02:33 18/12/2020
        {
          thai_time = hour_sub + minuteAsec_sub + " " + date_sub + "/" + mount_sub + "/20" + year_sub;
        }
        else if (format == 1) // dd/mm/yy Time >>>18/12/2020 17:02:33
        {
          thai_time = date_sub + "/" + mount_sub + "/20" + year_sub + " " + hour_sub + minuteAsec_sub;
        }
        else if (format == 2) //Time Stamp  yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"  >>2020-12-18T17:02:33.1000001+07:00
        {
          thai_time =  "20" + year_sub + "-" + mount_sub + "-" + date_sub + "T" + hour_sub + minuteAsec_sub + ".1000001+07:00";
        }

        time_buffer = thai_time;
        return time_buffer;
        break;
      }
      else if (data_input.indexOf(F("ERROR")) != -1)
      {
        Serial.println("ERROR Can't Call Time From NTP Server");
        break;
      }
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("HELLO");
  NB.begin(9600,SERIAL_8N1,Rxpin,Txpin);
  SetupModule();
  SetUpNTPServer();
}

void loop() 
{
    if(millis() - prevMillis > 5000)
    {
      prevMillis = millis();
      Serial.println("Time : dd/mm/yy ->" + calltimeNTP(0));
      Serial.println("dd/mm/yy : Time ->" + calltimeNTP(1));
      Serial.println("Time Stamp ->" + calltimeNTP(2));
      Serial.println("Hour : ->" + hourNTP);
      Serial.println("Minute : ->" + minuteNTP);
      Serial.println("Date : ->"+ dateNTP);
      Serial.println("only day : ->"+ onlyDayNTP);
      Serial.println("only month : ->"+ onlyMonthNTP);
      Serial.println("only year : ->"+ onlyYearNTP);
      Serial.println("*--------------------------------------*");
    }
}