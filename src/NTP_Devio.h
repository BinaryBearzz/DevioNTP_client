#include <Arduino.h>
#include <Stream.h>

class NTP_Devio
{
private:
    String data_input;   
public:
    NTP_Devio();
    void Begin();
    String currentDate;
    String onlyHour;
    String onlyMinute;
    String onlyDay;
    String onlyMouth;
    String onlyYear;   
    void reboot_module();
    void setupModule();
    void setupNTPserver();
    String getTime(unsigned int format = 0); //defaute is 0
    void stopQuaryTime();

protected:
    Stream *_serial;
};

