#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------"

IMG_DECLARE(pic_blue_particle);       // ประกาศรูปภาพ c file ก่อนใช้เสมอ
FONT_DECLARE(BebasNeueBold_num_50);   // ประกาศเพิ่มฟอนต์ ก่อนใช้
FONT_DECLARE(morkhor_40);             // ประกาศเพิ่มฟอนต์ ก่อนใช้

GImage image(pic_blue_particle);  // วิตเจตรูปภาพ ทำเป็นพื้นหลัง โดยจากรูปจาก c file
GKnob knob(image);                // วิตเจตปุ่มหมุน โดยมี layer แม่ คือ image 
GLabel knob_lb;                   // วิตเจตข้อความ สำหรับบอกข้อมูล ว่า วิตเจตปุ่มหมุน ที่หมุนไป มีค่าเท่าไหร่

GRoller roller_h;         // วิตเจตลูกกลิ้ง ทำเป็นตัวตั้งเวลา ของ ชั่วโมง
GRoller roller_m;         // วิตเจตลูกกลิ้ง ทำเป็นตัวตั้งเวลา ของ นาที
GRoller roller_s;         // วิตเจตลูกกลิ้ง ทำเป็นตัวตั้งเวลา ของ วินาที
GLabel  lb_colon[2];

GLed led(TFT_GREEN);      // วิตเจ็ต led ทำเป็นปุ่มกด โดยมีสีเขียว
GLabel label("เปิด",led);  // วิตเจ็ตข้อความ โดยมี layer แม่คือ led

uint32_t total_sec;       // ค่าวินาทีทั้งหมด จาก ชม : นาที : วินาที  ที่เกิดจาก วิตเจ็ตลูกกลิ้ง GRoller
GTimer timer;             // ตัวตั้งเวลาทำงาน

// เมื่อ วิตเจ็ตลูกกลิ้ง GRoller มีการกลิ้งเปลี่ยนค่า ให้คำนวณ ค่าวินาทีรวมทั้งหมดออกมา
void roller_onChanged(GWidget*w){
  total_sec = roller_h.selected().toInt()*3600 + roller_m.selected().toInt()*60 + roller_s.selected().toInt();
}

// สถานะของ วิตเจ็ต led
enum {STATE_OFF, STATE_ON, STATE_START, STATE_PAUSE } state;

// ฟังก์ชั่น สำหรับ reset ค่า
void reset_timer(){
  label = "ปิด";
  total_sec = 0;
  roller_h.selected(0);
  roller_m.selected(0);
  roller_s.selected(0);
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);

  
  knob.color(TFT_RED);      // กำหนดสี knob
  knob.align(ALIGN_CENTER, -knob.width()/2-30, -10);  // กำหนดตำแหน่งโดยใช้การจัดเรียง (align)
  knob.range(0,255);        // กำหนดระยะของปุ่มหมุน knob มีค่าตั้งแต่ 0-255
  
  // หากมีการเปลี่ยนแปลงค่าของปุ่มหมุน  knob
  knob.onValueChanged([](GWidget* widget){
    knob_lb = "ความสว่าง : " + String(knob.value());
  });

  // กำหนดข้อความ สำหรับบอกระดับของปุ่มหมุน knob
  knob_lb = "ความสว่าง : " + String(knob.value());
  knob_lb.font(morkhor_40, TFT_WHITE);      // กำหนด font และสีข้อความ
  knob_lb.align(knob, ALIGN_BOTTOM,0, 15);  // จัดเรียงชิดล่าง ปุ่มหมุน ลงไปอีก 15 pixels

  // กำหนดพื้นหลังของลูกลิ้งแบบโปร่งใส (opa=0), กำหนด font และ จำนวนแถวที่เห็นได้ คือ 1 บรรทัด
  roller_h.opa(0); roller_h.font(BebasNeueBold_num_50, TFT_WHITE);  roller_h.visible_rows(1);
  roller_m.opa(0); roller_m.font(BebasNeueBold_num_50, TFT_WHITE);  roller_m.visible_rows(1);
  roller_s.opa(0); roller_s.font(BebasNeueBold_num_50, TFT_WHITE);  roller_s.visible_rows(1);

  // เพิ่ม 0-23 โดยเติม 0 นำหน้าด้วย ให้ ลูกกลิ้งชั่วโมง ...
  // พร้อม กำหนด callback function เมื่อผู้ใช้หมุนเปลี่ยนค่ามีการเปลี่ยนแปลงเกิดขึ้น
  roller_h.options(0,23, true); roller_h.onValueChanged(roller_onChanged);
  roller_m.options(0,59, true); roller_m.onValueChanged(roller_onChanged);
  roller_s.options(0,59, true); roller_s.onValueChanged(roller_onChanged);

  // ใส่เครื่องหมาย : ระหว่าง ชั่วโมง นาที และ วินาที
  lb_colon[0] = ":";  lb_colon[0].font(BebasNeueBold_num_50, TFT_WHITE);
  lb_colon[1] = ":";  lb_colon[1].font(BebasNeueBold_num_50, TFT_WHITE);

  led.size(140,140);    // กำหนดขนาดวิตเจ็ต led
  led.align(knob, ALIGN_RIGHT, 60, -10);  // จัดเรียงชิดขวาของ knob ถัดออกไป 60 pixel และขึ้นบน 10 pixel

  // เมื่อมีการกด clicked เกิดขึ้น
  led.onClicked([](GWidget* w){
    switch(state){
      case STATE_OFF:
        led.ON();
        state = (total_sec)? STATE_START : STATE_ON;
        break;
      case STATE_ON:
        led.OFF();
        state = (total_sec)? STATE_PAUSE : STATE_OFF;
        break;
      case STATE_START:
        led.OFF();
        state = (total_sec)? STATE_PAUSE : STATE_OFF;
        break;
      case STATE_PAUSE:
        led.ON();
        state = (total_sec)? STATE_START : STATE_ON;
        break;
    }
    label = led.isON()? "เปิด" : ((total_sec)? "หยุด" : "ปิด");

    if( state == STATE_ON || state == STATE_START ) {
      if( total_sec && led.isON() ) {
        // ตั้งเวลาทำงานทุกๆ 1 วินาที
        timer.setInterval(1000L,[](){
          static uint8_t h_prev, m_prev;
          total_sec--;
          uint8_t h = int(total_sec/3600);
          uint8_t m = int((total_sec - h*3600)/60);
          uint8_t s = total_sec - h*3600 - m*60;
    
          if( h_prev != h ) {
            roller_h.selected(h, true);
            h_prev = h;
          }
          if( m_prev != m) {
            roller_m.selected(m, true);
            m_prev = m;
          }
          roller_s.selected(s, true);
          if(total_sec==0) { timer.del(); led.OFF();  label = "ปิด"; }
        });
      }
    }else{
      timer.del();
    }
  });

  led.OFF();
  // หากมีการกดค้างยาวเกิดขึ้นให้ทำงาน reset ค่า
  led.onLongPressed([](GWidget*w){
    reset_timer();
  });

  label.font(prasanmit_40, TFT_WHITE);

  roller_m.align(led, ALIGN_BOTTOM,0,30);
  roller_h.align(roller_m, ALIGN_LEFT, -10);
  roller_s.align(roller_m, ALIGN_RIGHT, 10);

  lb_colon[0].align(roller_h,ALIGN_RIGHT, 1, -5);
  lb_colon[1].align(roller_m,ALIGN_RIGHT, 1, -5);
}

void loop() {
  BlynkGO.update();
}
