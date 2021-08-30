//BinaryBear NBIoT_NTP_client ver. 1.0
#include <Arduino.h>
#include "NTP_Devio.h"
#include "HardwareSerial.h"

HardwareSerial serialPort(1);
#define hwResetPin 26
#define Rxpin 16
#define Txpin 17
#define buadrate 9600


NTP_Devio::NTP_Devio() {}

void NTP_Devio::reboot_module()
{
  pinMode(hwResetPin, OUTPUT); //Reset HW
  // pinMode(2,OUTPUT); //LED Build in
  digitalWrite(hwResetPin, LOW);
  delay(1000);
  digitalWrite(hwResetPin, HIGH);
  delay(2000); 
}
void NTP_Devio::setupModule()
{
    Serial.print(F(">>Rebooting ."));
    reboot_module();
    Serial.println(F("..OK"));
    serialPort.begin(buadrate,SERIAL_8N1,Rxpin,Txpin);
    _serial = &serialPort;

    Serial.print(F(">>Check module status "));
    
    _serial->println(F("AT"));
    while(1)
    {  
        if(_serial->available())
        {
        data_input = _serial->readStringUntil('\n');
        if(data_input.indexOf(F("OK")) != -1)
        {
            Serial.println("Module Ready");
            break;
        }
        else if (data_input.indexOf(F("ERROR")) != -1) 
          {
              delay(500);
              _serial->println(F("AT"));
              delay(100);
              Serial.print(F("."));
          }    
        }
    }
}

void NTP_Devio::setupNTP_rtc_timezone(String timeZone)
{
  Serial.println("------------------------------------------");
  Serial.println("Setup NTP Server RTC and Time Zone on \""+timeZone+"\"");
  Serial.println("For Change Time Zone Please Reset or power off board 1 time after Upload...");
  Serial.println("------------------------------------------");
  delay(1000);
  _serial->println("AT+CSNTPSTART=\"1.th.pool.ntp.org\","+timeZone);
  while (1) {
    if (_serial->available()) 
    {
      data_input = _serial->readStringUntil('\n');
      // Serial.println(data_input);
      if (data_input.indexOf(F("OK")) != -1) 
      {
        Serial.println("NTP Server Start..");
        break;
      }
      else if (data_input.indexOf(F("ERROR")) != -1) {
        Serial.println("NTP Server is Register yet...");
        break;
      }
    }
  }
  Serial.println(".");
}

void NTP_Devio::setupNTPserver()
{
  Serial.println("Setup NTP Server");
  _serial->println(F("AT+CURTC=1"));
  delay(500);
  _serial->println(F("AT+CRESET"));
  delay(500);
  // _serial->println(F("AT+CSNTPSTART=\"1.th.pool.ntp.org\",+00"));
  _serial->println(F("AT+CSNTPSTART=\"1.th.pool.ntp.org\""));
  while (1) {
    if (_serial->available()) 
    {
      data_input = _serial->readStringUntil('\n');
      // Serial.println(data_input);
      if (data_input.indexOf(F("OK")) != -1) 
      {
        Serial.println("NTP Server Start..");
        break;
      }
      else if (data_input.indexOf(F("ERROR")) != -1) {
        Serial.println("NTP Server is Register yet...");
        break;
      }
    }
  }
  Serial.println(".");
}

String NTP_Devio::getTime(unsigned int format)
{
  String time_buffer;
  String thai_time;
  _serial->println(F("AT+CCLK?"));
  delay(10);
  while (1)
  {
    if (_serial->available())
    {
      data_input = _serial->readStringUntil('\n');
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
        if(year_sub == "00") //Module Not Ready -->01:01:2000
        {
          currentDate = "null";
          onlyMinute = "null";
          onlyHour = "null";

          onlyDay = "null";
          onlyMouth = "null";
          onlyYear = "null"; 
          return "NTP Not Ready Please Wait...";
        }

        currentDate = date_sub + "/" + mount_sub + "/20" + year_sub;
        onlyMinute = minuteAsec_sub.substring(1,3);
        onlyHour = hour_sub;

        onlyDay = date_sub;
        onlyMouth = mount_sub;
        onlyYear = "20"+ year_sub;  

        switch (format)
        {
        case 0:
          thai_time = date_sub + "/" + mount_sub + "/20" + year_sub + " " + hour_sub + minuteAsec_sub;
          return thai_time;
          break;
        case 1:
          thai_time = hour_sub + minuteAsec_sub + " " + date_sub + "/" + mount_sub + "/20" + year_sub;
          return thai_time;
          break;
        case 2:
          thai_time =  "20" + year_sub + "-" + mount_sub + "-" + date_sub + "T" + hour_sub + minuteAsec_sub + ".1000001+07:00";
          return thai_time;
          break;        
        default:
          return "Wrong Parameter in Request Please Set Value GetTime(\"value\") only --> (0 - 2)";
          break;
        }

        //# old return format
        // if (format == 0) // dd/mm/yy Time >>>18/12/2020 17:02:33
        // {
        //   thai_time = date_sub + "/" + mount_sub + "/20" + year_sub + " " + hour_sub + minuteAsec_sub;
        //   return thai_time;
        // }  
        // else if (format == 1) //Time : dd/mm/yy >>>17:02:33 18/12/2020
        // {
        //   thai_time = hour_sub + minuteAsec_sub + " " + date_sub + "/" + mount_sub + "/20" + year_sub;
        //   return thai_time;
        // }

        // else if (format == 2) //Time Stamp  yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"  >>2020-12-18T17:02:33.1000001+07:00
        // {
        //   thai_time =  "20" + year_sub + "-" + mount_sub + "-" + date_sub + "T" + hour_sub + minuteAsec_sub + ".1000001+07:00";
        //   return thai_time;
        // }

        // time_buffer = thai_time;
        // return time_buffer;
        
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

void NTP_Devio::TimeBeginWithZone(String timeZone)
{
  String buff = timeZone;
  setupModule();
  setupNTP_rtc_timezone(buff);
}

void NTP_Devio::TimeBegin()
{
  setupModule();
  setupNTPserver();
}

void NTP_Devio::stopQuaryTime()
{
    _serial->println(F("AT+CSNTPSTOP"));
    delay(500);
}