# CN466-Final Project
# Documentation of development
---
## Table of contents
- [Motivation](#Motivation)
- [Requirement](#Requirement)
- [Design](#Design)
- [Test](#Test)
- [DemoVideo](#DemoVideo)
---
## Motivation 

![CN466 Course project](https://user-images.githubusercontent.com/60430405/145685621-99f662b3-848e-4220-a37e-0a5df6146c20.png)

## Requirement
ในการทำงานนี้จะมีการใช้งาน Software, Device หรือ Service อยู่หลัก ๆ ทั้งหมด 6 ส่วนด้วยกัน ได้แก่
- Cucumber RS Board
- HiveMQ
- Edge Impulse
- LIFF UI
- Web API
- Cloud service

โดยแต่ละส่วนจะมีหน้าที่ต่าง ๆ ดังนี้
1. Cucumber RS board: ใช้ Sensor วัดค่า Temperature, Pressure, Humidity, Acceleration, Angular velocity
2. HiveMQ: เป็นโปรโตคอลที่ทำหน้าที่ส่งข้อมูลจากบอร์ดไปให้เซิฟเวอร์ มี Topic เป็น **cn466/yodsatorn/cucumber/status** ข้อมูล payload จะเป็นค่า temperature, humidity, pressure, acceleration, angular velocity และอยู่ในรูปแบบ JSON format
3. Edge Impulse: ใช้ [Edge Impulse](https://www.edgeimpulse.com/) เพื่อ train Machine learning model ที่สามารถวิเคราะห์การเคลื่อนไหวได้ว่ากระถางอยู่นิ่งหรือไม่ หากมีการเคลื่อนไหวหรือถูกเคลื่อนย้ายก็ให้มีการแจ้งเตือนผ่านไลน์
4. LIFF UI: มี List ของคำสั่งหรือฟีเจอร์ต่าง ๆ ที่สามารถเรียกใช้งานกับ Chatbot ได้ โดยผู้ใช้สามารถมาเรียกผ่านทาง LIFF UI นี้ได้
5. Web API: มี [Vibrate API](https://developer.mozilla.org/en-US/docs/Web/API/Vibration_API) ซึ่งจะทำการสั่งให้โทรศัพท์มือถือของผู้ใช้สั่นเมื่อผู้ใช้ทำการกดเลือก ตัวเลือกคำสั่งต่าง ๆ ในหน้า LIFF UI สำหรับเช็คสถานะ
6. Cloud service: มีการใช้ [Heroku](https://www.heroku.com/) ซึ่งเป็น Cloud service แบบ Platform as a Service (PaaS) ในการ Deploy code ส่วนของ Chatbot, LIFF UI และ Backend server

## Design
![CN466 Course project (1)](https://user-images.githubusercontent.com/60430405/145689362-a4821a73-33d6-4a83-af50-33bdae94a9e6.png)

## Test

## DemoVideo
