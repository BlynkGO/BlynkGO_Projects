#include <BlynkGO.h>

#define BLYNKGO_KEY   "---------------"

IMG_DECLARE(pic_speedometer);

namespace speedo {
  GImage  speedometer(pic_speedometer); // วิตเจ็ดรูปภาพ โดยมี pic_speedometer เป็นรูปภาพ
  GRect   box_speed(speedometer);       // วิตเจ็ตสี่เหลียม บอกระดับความเร็ว ให้เป็น layer ลูกของ วิตเจ็ต speedometer
  GLabel  lb_speed(box_speed);          // วิตเจ็ตข้อความ ไว้บอกระดับความเร็ว ให้เป็น layer ลูกของ วิตเจ็ต box_speed
  GLabel  lb_unit(speedometer);         // วิตเจ็ตขอ้ความ สำหรับบอกหน่วย ความเร็ว กม/ชม ให้เป็น layer ลูกของ วิตเจ็ต speedometer
  GTriangle triangle(speedometer);      // วิตเจ็ตสามเหลี่ยม ให้เป็นเข็มหน้าปัด ให้เป็น layer ลูกของ วิตเจ็ต speedometer
  Point     pivot(160,165);             // จุดหมุน
  GCircle circle(speedometer);          // วิตเจ็ตวงกลมตรงกลาง สำหรับบัง สามเหลี่ยมเฉพาะตรงกลางให้ดูสวยงาม ให้เป็น layer ลูกของ วิตเจ็ต speedometer
  int _speed=0;
    
  /* API สำหรับสร้าง speedometer ขึ้นมาสำหรับแสดงบนจอ TFT */
  void create(){
    speedometer.align(ALIGN_CENTER);      // วางรูปภาพไว้กลางจอ
  
    box_speed.size(90,40);                // กำหนดขนาด
    box_speed.color(TFT_BLACK);           // กำหนดสี
    box_speed.border(3, TFT_SILVER);
    box_speed.corner_radius(5);           // กำหนดรัศมีมุมโค้ง
    box_speed.align(ALIGN_BOTTOM,0,-70);  // วางไว้ชิดล่าง ปรับเลื่อนขึ้น 70 pixels

    lb_speed = "--";
    lb_speed.font(prasanmit_35, TFT_WHITE);
    lb_speed.text_letter_space(7);        // กำหนดช่องไฟภายในข้อความ ให้อักขระห่างกัน 7 pixels

    lb_unit = "กม/ชม";
    lb_unit.font(prasanmit_25, TFT_WHITE);
    lb_unit.align(box_speed,ALIGN_BOTTOM,0,-5); // วางไว้ชิดล่างของ box_speed โดยปรับเลื่อนขึ้น 5 pixels
      
    // พิกัดจุดของ สามเหลี่ยม (จุดจะเทียบจากภายใน parent ของสามเหลี่ยม)
    Point p[3];
      p[0] = pivot+ Point( 0, -120 );  /*    /\    */
      p[1] = p[0] + Point( 8, 120 );   /*   /  \   */
      p[2] = p[0] + Point(-8, 120 );   /*  /____\  */
    triangle.setPoints(p);                // วาดสามเหลี่ยมชี้ที่ 12 นาฬิกา (ชี้ขึ้น)
    triangle.color(TFT_RED);
    
    // สำหรับหมุนสามเหลี่ยม 
    triangle.direction(CLOCKWISE);  // ให้หมุนแบบตามเข็มนาฬิกา
    triangle.angle_offset(-130);    // กำหนดให้มุมเริ่มต้น อยู่ที่ -130 องศา  (ย้อนจาก 12 นาฬิกา ถอยไป 130องศา เป็นมุมเริ่มต้น)
    triangle.angle(0, pivot);       // ให้ปรับมุม ที่ 0 องศา นับจากมุมเริ่มต้น (angle offset) รอบจุดหมุน pivot
  
    circle.radius(24);
    circle.border(3,TFT_SILVER);
    circle.color(TFT_BLACK);
    circle.align_center(0,5);
  }
  /* API สำหรับ รับค่า ความเร็ว ให้ไปอัพเดตวิตเจตบนจอ TFT */
  void speed(int val){
    _speed = constrain(val, 0, 160);    // ความเร็วบังคับให้อยู่ระหว่าง 0-160 เท่านั้น
    lb_speed = String(_speed);          // แสดงความเร็วไว้ที วิจเจ็ตข้อความ สำหรับแสดงความเร็ว
    triangle.angle(map(_speed,0, 160, 0, 260), pivot);  // เปลี่ยน ความเร็วไปเป็นองศา เพื่อให้ วิตเจ็ตสามเหลี่ยมหมุนไปเป็นกี่องศา รอบจุดหมุน pivot
    box_speed.invalidate();             // ให้ refresh วิตเจต box_speed ด้วย 
    BlynkGO.update();                   // สั่งให้ระบบทำการวาด ทุกๆ layer ที่มีการเปลีี่ยนแปลงแสดงออกไปที่จอ TFT
  }
  /* API สำหรับ คืนค่าความเร็วปัจจุบันของ speedometer */
  int speed(){
    return _speed;
  }
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);
  
  speedo::create();
  speedo::speed(0);

  static GTimer timer;
  timer.setInterval(50L,[](){
    static int step = 2;
    static int spd = 0;
    speedo::speed(spd);  
    spd += step;
    if(spd > 160 )   { spd = 160; step = -step; }
    else if(spd < 0) { spd = 0; step = -step; }
  });
}

void loop() {
  BlynkGO.update();
}


