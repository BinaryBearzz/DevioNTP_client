#include <Arduino.h>
#include <Stream.h>

class NTP_Devio
{
private:
    String data_input;   
public:
    NTP_Devio();
    void TimeBegin(); //Time Thaizone
    void TimeBeginWithZone(String timeZone); 
    String currentDate;
    String onlyHour;
    String onlyMinute;
    String onlyDay;
    String onlyMouth;
    String onlyYear;  
    void setupNTP_rtc_timezone(String timeZone = "+7");
    void reboot_module();
    void setupModule();
    void setupNTPserver();
    String getTime(unsigned int format = 0); //defaute is 0
    void stopQuaryTime();
    

protected:
    Stream *_serial;
};

