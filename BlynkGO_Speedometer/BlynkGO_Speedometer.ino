#include <BlynkGO.h>

#define BLYNKGO_KEY   "48c89537d6695ad0"

IMG_DECLARE(pic_speedometer);

namespace speedo {
  GImage  speedometer(pic_speedometer); // วิตเจ็ดรูปภาพ โดยมี pic_speedometer เป็นรูปภาพ
  GRect   box_speed(speedometer);       // วิตเจ็ตสี่เหลียม  บอกระดับความเร็ว ให้เป็น layer ลูกของ วิตเจ็ต speedometer
  GLabel  lb_speed(box_speed);          // วิตเจ็ตข้อความ ให้เป็น layer ลูกของ วิตเจ็ต box_speed
  GLabel  lb_unit(speedometer);
  GTriangle triangle(speedometer);      // วิตเจ็ตสามเหลี่ยม
  Point     pivot(160,165);             // จุดหมุน
  GCircle circle(speedometer);          // วิตเจ็ตวงกลมตรงกลม
  int _speed=0;
  
  void create(){
    speedometer.align(ALIGN_LEFT,20);           // วางรูปภาพไว้กลางจอ
  
    box_speed.size(90,40);                // กำหนดขนาด
    box_speed.color(TFT_BLACK);           // กำหนดสี
    box_speed.border(3, TFT_SILVER);
    box_speed.corner_radius(5);           // กำหนดรัศมีมุมโค้ง
    box_speed.align(ALIGN_BOTTOM,0,-70);  // วางไว้ชิดล่าง

  
    lb_speed = "--";
    lb_speed.font(prasanmit_35, TFT_WHITE);
    lb_speed.text_letter_space(7);

    lb_unit = "กม/ชม";
    lb_unit.font(prasanmit_25, TFT_WHITE);
    lb_unit.align(box_speed,ALIGN_BOTTOM,0,-5);
      
    // พิกัดจุดของ สามเหลี่ยม (จุดจะเทียบจากภายใน parent ของสามเหลี่ยม)
    Point p[3];
      p[0] = pivot+ Point( 0, -120 );  /*    /\    */
      p[1] = p[0] + Point( 8, 120 );   /*   /  \   */
      p[2] = p[0] + Point(-8, 120 );   /*  /____\  */
    triangle.setPoints(p);
    triangle.color(TFT_RED);
    
    // สำหรับหมุนสามเหลี่ยม 
    triangle.direction(CLOCKWISE);  // ให้หมุนแบบตามเข็มนาฬิกา
    triangle.angle_offset(-130);    // มุมเริ่มต้น ที่ -130 องศา  (ถอยไป 130องศา เป็นมุมเริ่มต้น
    triangle.angle(0, pivot);
  
    circle.radius(24);
    circle.border(3,TFT_SILVER);
    circle.color(TFT_BLACK);
    circle.align_center(0,5);
  }

  void speed(int val){
    _speed = constrain(val, 0, 160);
    lb_speed = String(_speed);
    triangle.angle(map(_speed,0, 160, 0, 260), pivot);
    box_speed.invalidate();
    BlynkGO.update();
  }
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
