# CN466-Final Project
# Documentation of development
---
## Line Chatbot

>ในส่วนนี้มีการพัฒนาด้วย Node.js โดยโค้ดสำหรับการทำงานจะอยู่ในไฟล์ **app.js** และ **/static/index.html**

#### หน้าที่
ทำหน้าที่เป็น Chatbot และมี LIFF UI คอยสื่อสารและให้บริการกับผู้ใช้ผ่าน Line

#### การทำงาน
- **Line Chatbot**
    1. ทำการเชื่อมต่อกับ MQTT protocol ด้วย Topic : cn466/yodsatorn/cucumber/status
    2. คอยรับข้อความที่ส่งมาจากผู้ใช้ผ่าน function handle event จากนั้นตอบกลับผู้ใช้นั้นด้วยข้อความที่เรากำหนดเอาไว้
- **LIFF UI**
    1. ทำการเปิดหน้า page ที่ออกแบบไว้ให้ผู้ใช้ได้สามารถเลือกสถานะที่ต้องการจะเช็คจากบอร์ดได้ โดยที่ผู้ใช้ไม่ต้องพิมพ์บอกกับ Line chatbot เอง
    2. เมื่อผู้ใช้ทำการเลือกสถานะจะมี [Vibrate API](https://developer.mozilla.org/en-US/docs/Web/API/Vibration_API) ซึ่งเป็น Web API ที่จะทำให้โทรศัพท์ของผู้ใช้สั่น เพื่อตอบสนองว่าผู้ใช้ได้ทำการเลือก Choice ของสถานะที่ต้องการจะเช็คแล้ว
    **(สำหรับตัว Vibrate API นั้นในปัจจุบัน browser ที่ทำงานบนระบบปฏิบัติการ IOS ยังไม่รองรับ จึงสามารถใช้งานได้เพียงบนระบบปฏิบัติการ Android เท่านั้น)**
    3. หลังจากเลือก Choice แล้ว LIFF จะทำการส่งข้อความตามที่เราออกแบบเอาไว้ไปให้กับ Line Chatbot เองโดยที่ผู้ใช้ไม่ต้องพิมพ์
    
    ![6](https://user-images.githubusercontent.com/60430405/145872060-fffbdde8-0153-4730-96e8-7f2f4f924b56.png)
