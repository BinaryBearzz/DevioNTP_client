# DevioNTP_client Ver.1.0.0
ใช้สำหรับ Devio AIS NBIoT โมดูล(SIM7020E) Device 
this Project Make for use Devio sync Time from NTP Server
โปรเจ็คนี้ใช้สำหรับทำการ Sync เวลาจาก NTP Server มาไว้บน RTC ของโมดูล SIM7020E (เท่าที่ลองอ่านจาก Manual นะครับผิดพลาดอย่างอะไรขออภัยด้วย) โดย Sync ใหม่ทุกครั้งหลังจากมีการ Power on ตัวอุปกรณ์ขึ้น

โดยฟังชั่นการใช้งานจะเรียบง่ายดังนี้

#include "NTP_Devio.h"

NTP_Devio NBtime;  #note* "NBtime" เป็นแค่การยกตัวอย่างสืบทอดคลาสจาก Library NTP_Devio เท่านั้นนะครับสามารถแก้ไขได้
void TimeBegin();   \_ใช้ในช่วงแรกเพื่อทำการ initial module และ setup ntp server ใส่ไว้ใน void setup() #เวลาไทย

หรือ

void TimeBeginWithZone(String timeZone);  \_เช่น อยากได้ Time zone +12 จะต้อง Set Parameter ดังนี้ 
-   NBtime.TimeBeginWithZone("+12");
============================================================================================================

*   NBtime.getTime(<int value 0 - 2>);   \_ฟังชั่น String ที่จะ return ค่าวันเวลาออกมาแสดง โดยจะสามารถเซ็ต parameter ได้ 3 แบบ ดังนี้
-   NBtime.getTime(); \_หรือ NBtime.getTime(0) ค่า default จะ return ค่าเวลาออกมาในรูปแบบ  dd/mm/yy  hour:minute:second 18/12/2020 17:02:33 เป็น String  
-   NBtime.getTime(1); \_จะ return ค่าเวลาออกมาในรูปแบบ hour:minute:second  dd/mm/yy ---> 17:02:33 18/12/2020  เป็น String  
-   NBtime.getTime(2); \_จะ return ค่าเวลาออกมาในรูปแบบ Timestamp Format ดังนี้ yy-mm-ddT"Hour":minute:second.0000000+07:00 --->  2020-12-18T17:02:33.1000001+07:00 เป็น String 

อีกทั้งยังมีตัวแปร String ให้เรียกใช้จาก Library ที่ผมสร้างไว้ดังนี้ด้วยครับ

 *สมมุติเวลา 17:02:33 18/12/2020 
  
 - NBtime.currentDate;  \_ จะได้ 18/12/2020\_
 - NBtime.onlyHour;     \_ จะได้ 17  เวลาแค่ชั่วโมงนั้นๆออกมา\_
 - NBtime.onlyMinute;   \_ จะได้ 02  เวลาแค่นาทีนั้น\_
 - NBtime.onlyDay;        \_ จะได้ 18 แค่วันที่เท่านั้น\_
 - NBtime.onlyMouth;     \_ จะได้ 12 แค่เดือนเท่านั้น\_
 - NBtime.onlyYear;     \_ จะได้ 2020 แค่ปีเท่านั้น\_

void stopQuaryTime();   ฟังก์ชั่นนี้ใช้สำหรับหยุด Query Time จาก network ซึ่งจากที่ทดลองแล้วผมยังไม่ทราบจุดประสงค์ที่แน่ชัดแต่ใน Manual AT command ของเขา จะใช้หลังจาก Get time เสร็จเสมอ 
แต่จะใส่หรือไม่ใส่ผมก็ยังไม่เห็นผลต่างจากเดิมครับ
ปล. ถ้าใช้ stopQuaryTime(); ในโหมด TimeBeginWithZone("xx"); จะทำให้ gettimeอยู่ทำงาน #จะไม่ใช้ก็ได้ครับฟังชั่น stopQuaryTime();
