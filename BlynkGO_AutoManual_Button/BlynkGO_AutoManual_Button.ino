/************************************************************************
 * 
 * BlynkGO Project : โปรเจคตัวอย่างสำหรับ ทำปุ่มอัตโนมัติ / Manual
 *                   พร้อมการตั้งค่าช่วงอุณหภูมิ ที่ต้องการให้ทำงานเมื่ออยู่ในโหมดอัตโนมัติ
 *                   โดยมีการเก็บค่าช่วงอุณหภูมิ ไว้บน flahsMem
 * 
 * TFT : 3.5 inch / 4.0 inch
 * 
 ************************************************************************/

#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------"

FONT_DECLARE(prasanmit_60);       // ประกาศเพิ่มฟอนต์ ก่อนนำไปใช้
IMG_DECLARE(pic_knob_temp);       // ประกาศเพิ่มรูปภาพแบบ c file ก่อนนำไปใช้

GButton     btn_auto_manual (BUTTON_SWITCH, "อัตโนมัติ");     // วิตเจ็ตปุ่ม สำหรับ สลับ Auto / Manual
GButton     btn_on_off      (BUTTON_SWITCH, "เปิด");         // วิตเจ็ตปุ่ม สำหรับ สลับ เปิด / ปิด

GContainer  cont_temp;                                      // วิตเจ็ต container สำหรับจัดกลุ่ม
GLabel      lb_info = "ช่วงอุณหภูมิทำงาน (โหมดอัตโนมัติ)";      // วิตเจ็ตข้อความ
GLabel      lb_range( cont_temp);                           // วิตเจ็ตข้อความ             โดยมี เลเยอร์ cont_temp เป็น เลเยอร์แม่
GSlider     slider(20,35, SLIDER_TYPE_RANGE, cont_temp);    // วิตเจ็ต slider มีช่วง 20-35 โดยมี เลเยอร์ cont_temp เป็น เลเยอร์แม่

// เมื่อปุ่มมีการกดเข้ามา
void btn_onclicked(GWidget*w){
  if(w->sameWith(btn_on_off)){      // คำสั่ง sameWith(..) ใช้สำหรับ เฃ็คว่าเป็น วิตเจ็ต เดียวกันหรือไม่
    btn_auto_manual.OFF();
    btn_auto_manual = "Manual";
    btn_auto_manual.color(TFT_ORANGE);

    if(btn_on_off.isON()){
      btn_on_off = "เปิด";
      btn_on_off.color(TFT_GREEN);
    }else{
      btn_on_off = "ปิด";
      btn_on_off.color(TFT_RED);
    }
  }else
  if(w->sameWith(btn_auto_manual)){
    if(btn_auto_manual.isON()){
      btn_auto_manual = "อัตโนมัติ";
      btn_auto_manual.color(TFT_BLUE);
    }else{
      btn_auto_manual = "Manual";
      btn_auto_manual.color(TFT_ORANGE);
    }    
  }
}

void setup() {
  BlynkGO.begin(BLYNKGO_KEY);

  btn_on_off      .width(170);  btn_on_off      .corner_radius(15);   btn_on_off      .onClicked(btn_onclicked);
  btn_auto_manual .width(170);  btn_auto_manual .corner_radius(15);   btn_auto_manual .onClicked(btn_onclicked);

  btn_auto_manual .align(ALIGN_TOP, -95, 30);
  btn_on_off      .align(ALIGN_TOP,  95, 30);

  btn_on_off      .ON();
  btn_on_off      .event_send(EVENT_CLICKED);     // ส่งสัญญาณ EVENT_CLICKED เพื่อจำลองกำหนดสถานะเริ่มต้น

  btn_auto_manual .ON();
  btn_auto_manual .event_send(EVENT_CLICKED);     // ส่งสัญญาณ EVENT_CLICKED เพื่อจำลองกำหนดสถานะเริ่มต้น


  cont_temp.fit(FIT_TIGHT);                       // มีขนาดแบบพอดี (fit) ทั้งด้านกว้างและด้านสูง กับ เลเยอร์ลูกที่เป็นสมาชิก
  cont_temp.layout(LAYOUT_COL_M);                 // ให้มีการจัด layout แบบ เรียงตาม column ลงมาเป็นลำดับ และไว้กึ่งกลาง
  cont_temp.padding_top(40);                      // ให้มีระยะขอบด้านบน 40 pixel
  cont_temp.padding_bottom(30);                   // ให้มีระยะขอบด้านล่าง 30 pixel
  cont_temp.padding_left(30);                     // ให้มีระยะขอบด้านซ้าย 30 pixel
  cont_temp.padding_right(30);                    // ให้มีระยะขอบด้านขวา 30 pixel
  cont_temp.corner_radius(15);                    // กำหนดรัศมีมุมโค้ง 15 pixel
  cont_temp.border(3, TFT_GRAY);                  // มีขอบหนา 3 ขอบสีเทา
  cont_temp.align(ALIGN_BOTTOM, 0, -30);          // ให้ วางไว้ชิดด้านล่างขึ้นมา 30 pixel

  lb_range  .font(prasanmit_60, TFT_BLACK);       // กำหนด font และ สี ให้ วิตเจ็ตข้อความ lb_range
  
  slider    .width(300);                          // กำหนดความกว้างให้ วิตเจ็ต slider
  slider    .color(TFT_BLUE, TFT_RED);            // กำหนดสีแบบเฉดสี 2 สี
  slider    .knob_image(pic_knob_temp);           // กำหนดรูปให้ปุ่มเลื่อน ของ slider

  if( BlynkGO.flashMem_exists("TEMP_MIN")){       // เช็คว่า flashMem มี key "TEMP_MIN" อยู่หรือไม่ ?
    slider    .left_value (BlynkGO.flashMem_Int("TEMP_MIN"));    // กำหนดค่าปุ่มเลื่อน ค่าฝั่งซ้าย  
  }
  if( BlynkGO.flashMem_exists("TEMP_MAX")){       // เช็คว่า flashMem มี key "TEMP_MAX" อยู่หรือไม่ ?
    slider    .right_value(BlynkGO.flashMem_Int("TEMP_MAX"));    // กำหนดค่าปุ่มเลื่อน ค่าฝั่งซ้าย  
  }

  // หาก User มีการเลื่อนปรับค่า slider
  slider    .onValueChanged([](GWidget*w){
    lb_range = StringX::printf("%d"SYMBOL_DEGREE" - %d"SYMBOL_DEGREE, slider.left_value(), slider.right_value());
    BlynkGO.flashMem("TEMP_MIN", slider.left_value());          // จัดเก็บค่าด้านซ้าย ลงบน flashMem
    BlynkGO.flashMem("TEMP_MAX", slider.right_value());         // จัดเก็บค่าด้านขวา ลงบน flashMem
  });
  slider.event_send(EVENT_VALUE_CHANGED);         // ส่งสัญญาณ EVENT_VALUE_CHANGED เพื่อจำลองกำหนดสถานะเริ่มต้น

  lb_info.font(prasanmit_30, TFT_BLACK);          // กำหนด font และ สี ให้ วิตเจ็ตข้อความ lb_info
  lb_info.align(cont_temp, ALIGN_IN_TOP, 0, 10);  // ให้ วางชิดไว้ ด้านใน ณ บนกลาง ของ cont_temp ลงไปอีก 10 pixel
}

void loop() {
  BlynkGO.update();
}
