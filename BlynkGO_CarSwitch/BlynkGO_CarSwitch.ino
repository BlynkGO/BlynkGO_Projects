#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------" 

// ประกาศ รูปแบบ c file ก่อนใช้ใน วิตเจต GImage
IMG_DECLARE(pic_car_switch);
IMG_DECLARE(pic_car_airflow_switch);
IMG_DECLARE(pic_car_temp_switch);
IMG_DECLARE(pic_car_switch_knob);
IMG_DECLARE(pic_car_light_switch);
IMG_DECLARE(pic_car_light_knob);

GMultiScreen Screens;

// สวิตซ์รถยนต์ สำหรับปรับความแรงลม ด้วยวิตเจตรูป
GImage car_switch(pic_car_switch);
GImage car_switch_knob(pic_car_switch_knob, car_switch);
GLabel lb_switch("ความแรงลม");

// สวิตซ์รถยนต์ สำหรับปรับทิศทางลม ด้วยวิตเจตรูป
GImage car_airflow_switch(pic_car_airflow_switch);
GImage car_airflow_knob(pic_car_switch_knob, car_airflow_switch);
GLabel lb_airflow_switch("ทิศทางลม");

// สวิตซ์รถยนต์ สำหรับปรับความเย็นของลม ด้วยวิตเจตรูป
GImage car_temp_switch(pic_car_temp_switch);
GImage car_temp_knob(pic_car_switch_knob, car_temp_switch);
GLabel lb_temp_switch("ความเย็น-อุ่น");

// สวิตซ์รถยนต์ สำหรับปรับไฟหน้า ด้วยวิตเจตรูป
GImage car_light_switch(pic_car_light_switch);
GImage car_light_knob(pic_car_light_knob, car_light_switch);
GLabel lb_light_switch("ไฟหน้า");


void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);
  BlynkGO.fillScreen(TFT_COLOR(28,28,28)); //TFT_RED, TFT_ORANGE);

  Screens.addScreen(2);

  // Screens[0]
  car_switch          .parent(Screens[0]);
  car_airflow_switch  .parent(Screens[0]);
  lb_switch           .parent(Screens[0]);
  lb_airflow_switch   .parent(Screens[0]);
    car_switch        .align( ALIGN_CENTER, - car_switch.width()/2-20);
    car_airflow_switch.align(car_switch, ALIGN_RIGHT, 40);
    lb_switch         .align(car_switch,ALIGN_TOP);
    lb_airflow_switch .align(car_airflow_switch,ALIGN_TOP);
    lb_switch         .font(prasanmit_40, TFT_WHITE);
    lb_airflow_switch .linkStyle(lb_switch);

  // Screens[1]
  car_temp_switch     .parent(Screens[1]);
  car_light_switch    .parent(Screens[1]);
  lb_temp_switch      .parent(Screens[1]);
  lb_light_switch     .parent(Screens[1]);
    car_temp_switch   .align( ALIGN_CENTER, - car_temp_switch.width()/2-20);
    car_light_switch  .align(car_temp_switch, ALIGN_RIGHT, 40);
    lb_temp_switch    .align(car_temp_switch, ALIGN_TOP);
    lb_light_switch   .align(car_light_switch, ALIGN_TOP);
    lb_temp_switch    .linkStyle(lb_switch);
    lb_light_switch   .linkStyle(lb_switch);

  car_switch_level(4);
  car_airflow_switch_level(3);  
  car_temp_switch_level(-2);
  car_light_switch_level(3);
}

uint32_t timer;
int8_t switch_level = 0;          // 0..6
int8_t airflow_switch_level = 0;  // 0..4
int8_t temp_switch_level = -5;     // -5..5
int8_t light_switch_level = 0;    // 0..5

void loop() {
  BlynkGO.update();
  
  if(millis() >= timer){
    timer = millis() + 1000;
    car_switch_level(switch_level);
    car_airflow_switch_level(airflow_switch_level);  
    car_temp_switch_level(temp_switch_level);
    car_light_switch_level(light_switch_level);
    
    switch_level++; airflow_switch_level++; temp_switch_level++; light_switch_level++;
    if(switch_level > 6) switch_level = 0;
    if(airflow_switch_level > 4) airflow_switch_level = 0;
    if(temp_switch_level > 5) temp_switch_level = -5;
    if(light_switch_level > 5) light_switch_level = 0;
  }
}

// ปรับระดับ สำหรับ สวิทซ์ปรับความแรงลม : 0..6
void car_switch_level(uint8_t level){
  int angle_offset= -96;
  int angle = (constrain(level, 0,6))* abs(angle_offset)/3 + angle_offset;
  car_switch_knob.angle(angle);
}

// ปรับระดับ สำหรับ สวิทซ์ปรับทิศทางลม : 0..4
void car_airflow_switch_level(uint8_t level){
  int angle_offset= -116;
  int angle = (constrain(level, 0,4))* abs(angle_offset)/2 + angle_offset;
  car_airflow_knob.angle(angle);
}
// ปรับระดับ สำหรับ สวิทซ์ปรับความเย็น-อ่น : -5..5
void car_temp_switch_level(int8_t level){
  int angle_offset= -110;
  int angle = (constrain(level, -5, 5))* abs(angle_offset)/5 ;
  car_temp_knob.angle(angle);
}

// ปรับระดับ สำหรับ สวิทซ์ปรับไฟหน้า : 0..5
void car_light_switch_level(uint8_t level){
  int angle_offset= -60;
  int angle = (constrain(level, 0,5))* abs(angle_offset)/2 + angle_offset;
  car_light_knob.angle(angle);
}
