# CN466-Final Project
# Documentation of development
---
## Backend server
>ในส่วนนี้มีการพัฒนาด้วย Node.js หรือภาษา javascript โดยโค้ดสำหรับการทำงานจะอยู่ในไฟล์ **app.js** นอกจากนี้ก็จะมีไฟล์ **edge-impulse-standalone.js** ซึ่งเป็นตัวที่ใช้ในการทำนายผลวิเคราะห์การเคลื่อนไหวของกระถางต้นไม้

#### หน้าที่
ทำหน้าที่เป็น server ที่คอยรับค่าจากบอร์ดเข้ามาเพื่อทำนายผลการเคลื่อนไหวของกระถาง ถ้าหากพบว่ากระถางมีการเคลื่อนไหวก็จะทำการส่งข้อความแจ้งเตือนไปยัง Line ของผู้ใช้

#### การทำงาน
1. ทำการ import module edge-impulse-standalone เพื่อจะนำมาใช้เป็นตัว motion recognition
2. ทำการเชื่อมต่อกับ MQTT protocol ที่ Topic : cn466/yodsatorn/cucumber/status
3. รับค่า acceleration, angular velocity และนำค่าที่ได้รับใส่เป็น input ให้กับ model เพื่อทำการวิเคราะห์และทำนายผลว่ากระถางมีการเคลื่อนไหวหรือไม่
4. หากพบว่ากระถางมีการเคลื่อนย้าย ก็จะทำการ broadcast แจ้งเตือนไปยัง Line ของผู้ใช้ ตาม ACCESS_TOKEN ที่ตั้งเอาไว้

![5](https://user-images.githubusercontent.com/60430405/145871693-1be52c79-bd63-4b70-a8b2-705d0295837c.png)
