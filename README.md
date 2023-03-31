# ตู้ขยะอัจฉริยะกว่าคนทำ

**ชื่อโครงงาน** : ตู้ขยะอัจฉริยะกว่าคนทำ

**วิชา** : ระบบฝังตัว (Embedded System) 

**รหัสวิชา** : 01204322

**กลุ่ม** : สาวสวย 5 หน่อ

**คณะผู้จัดทำ** :
1. 6310503561 วิมลมณี วิศิษฎ์สกุลเจริญ
2. 6310503588 สุพิชชา วสุประสาท
3. 6310505696 จิดาภา ณัฐภานนท์
4. 6310505700 ชนนันท์ วัฒนชานนท์
5. 6310505718 ณัฐรดา วัชราสินธร

คณะวิศวกรรมศาสตร์ สาขาคอมพิวเตอร์ มหาวิทยาลัยเกษตรศาสตร์

## รายละเอียดของไฟล์ :
**Pictures** (โฟลเดอร์รวบรวมไฟล์รูปภาพชิ้นงาน)

**Schematic** :
- Schematic_1_DetectMetal.png (Schematic ของบอร์ดที่ใช้แยกขยะ และนับจำนวนขยะที่ได้)
- Schematic_2_Display.png (Schematic ของบอร์ดที่ใช้แสดงผลกับผู้ใช้ เช่น ปริมาณขยะ จำนวนขยะที่ทิ้ง)
- Schematic_3_ChangeCoin.png (Schematic ของบอร์ดที่ใช้ในการจ่ายเหรียญ)
- Schematic_4_DeptDB.png (Schematic ของบอร์ดที่ใช้ในการจำเครดิตของผู้ใช้งาน หนี้ที่ติดผู้ใช้ รวมถึงการแสดงผลหมายเลขโทรศัพท์ที่กดผ่าน keypad)
- Schematics_5_TrashStatus_LineNotify.png (Schematic ของบอร์ดที่ใช้ในการวัดปริมาณถังขยะ และทำไลน์ notification)

**SourceCode** :
- B1_DetectMetal.ino
- B2_Display.ino
- B3_Change_Coin
  - change_coin
    - change_coin.cpp
- B4_DeptDB
  - deptDB
    - deptDB.cpp
- B5_TrashStatus
  - trash_status
    - trash_status.cpp
- B6_LineNotify
  - line_notify
    - line_notify.cpp

**License.txt**

**README.md**		

## Library ที่ใช้
- ESP32Servo.h
- esp_now.h
- WiFi.h
- Bounce2.h
- Foo.h
- Bar.h
- SPI.h
- Wire.h
- Adafruit_GFX.h
- Adafruit_SSD1306.h
- Arduino.h
- iostream
- map
- HardwareSerial.h
- Ultrasonic.h
- WiFiClientSecure.h
- ESP_Line_Notify.h


## Hardware
1. NodeMCU 6 บอร์ด
2. Switch 3 ตัว
3. Inductive proximity sensor 1 ตัว
4. Ultrasonic sensors 3 ตัว
5. Servo motor 3 ตัว
6. Breadboard 5 ชิ้น
7. Battery 12 V
8. Keypad 3x4 1 ชิ้น
9. OLED 2 จอ
10. สายไฟ
11. กระดาษลัง
12. ฟิวเจอร์บอร์ด
13. กาวร้อน
