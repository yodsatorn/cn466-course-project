# CN466-Final Project
# Documentation of development
---
## Cucumber RS board

>ในส่วนของบอร์ด Cucumber RS ได้ใช้ Arduino IDE ในการเขียนไฟล์การทำงานของบอร์ดโดยการทำงานของบอร์ดจะอยู่ในไฟล์ **CucumberRS.ino** 

#### หน้าที่

วัดค่า temperature, humidity, pressure, acceleration, angular velocity จากนั้นทำการ publish ค่าที่วัดได้ผ่าน MQTT protocol

#### การทำงาน

1. ตัวบอร์ดจะทำการเชื่อมต่อกับ MQTT protocol ที่  Topic : cn466/yodsatorn/cucumber/status
2. วัดค่า Temperature, Humidity, Pressure, Acceleration, Angular Velocity ผ่านตัว sersors บนบอร์ด
3. ทำการ publish ค่าทั้งหมดที่วัดได้ผ่าน MQTT protocol โดยใช้ TOPIC ที่ได้กำหนดเอาไว้ ทุก ๆ 0.1 วินาที

