# CN466-Final Project
# Documentation of development
---
## Collecting data for Edge Impulse

>ในส่วนนี้จะทำการเขียนโปรแกรมด้วย Node.js โดยโค้ดสำหรับการทำงานจะอยู่ในไฟล์ **collectingData.js**

#### หน้าที่

ทำการรับค่า temperature, humidity, pressure, acceleration, angular velocity จากบอร์ด แล้วเก็บค่าที่รับได้ลงไฟล์ csv สำหรับการนำไปใช้ train machien learning model

#### การทำงาน

1. ทำการเชื่อมต่อกับ MQTT protocol ที่  Topic : cn466/yodsatorn/cucumber/status
2. รับค่าที่ได้จาก TOPIC นำมาเขียนลง csv file โดยแบ่งเป็นไฟล์สำหรับ train และ test
3. นำไฟล์ csv ที่ได้ upload ขึ้นไปบน [Edge Impulse](https://www.edgeimpulse.com/) เพื่อเทรนโมเดล Machine learning

![4](https://user-images.githubusercontent.com/60430405/145871804-e4176e0a-7c74-46bc-a453-e6dd532b1915.png)
