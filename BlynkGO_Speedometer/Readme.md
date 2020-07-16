## [BlynkGO] Speedometer Project

![BlynkGO_Speedometer](https://github.com/BlynkGO/BlynkGO_Projects/blob/master/BlynkGO_Speedometer/BlynkGO_Speedometer.gif?raw=true) 

### หลักการสร้าง
1. รูปหน้าปัด สร้างเป็น วิตเจ็ตรูป ไว้ layer ด้านหลัง

2. วิตเจ็ตสามเหลี่ยม ทำเป็นตัวเข็มหน้าปัด  
สร้างเป็น layer บน วิตเจ็ตรูปหน้าปัด  

3. หมุนวิตเจ็ตสามเหลี่ยมด้วย API `triangle.angle(มุม, จุดหมุน)`  
ทำให้เข็มหน้าปัดเกิดการหมุนตามองศาที่ต้องการ
