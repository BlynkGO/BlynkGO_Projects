#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------"

IMG_DECLARE (pic_red_background);       // ประกาศรูปภาพแบบ c file ก่อนนำไปใช้
FONT_DECLARE(sukhumvit_40);             // ประกาสฟอนต์ชุดใหม่ ก่อนนำไปใช้
FONT_DECLARE(sukhumvit_60);             // ประกาสฟอนต์ชุดใหม่ ก่อนนำไปใช้

GImage      image(pic_red_background);  // วิตเจ็ตรูปภาพ จาก c file โดยทำเป็นพื้นหลัง
GLabel      label(image);               // วิตเจ็ตข้อความ เป็น layer ลูกของ image
GAnimation  anim;                       // ตัวสร้างระบบ animation ของ BlynkGO

void text_animation_show(GAnimation*a);
void text_animation_hide(GAnimation*a);

String text[] = { "BlynkGO", 
                  "PRESENT", 
                  "ขอต้อนรับสู่", 
                  "บลิงโก!", 
                  "นี่ไม่ใช่ PowerPoint",
                  "แต่คือ",
                  "BlynkGO Animation",
                  "แสดงข้อความโปร่งใส",
                  "และ การขยาย\nระยะช่องไฟข้อความ",
                  "ด้วยระบบ\nBlynkGO Animation",
                  "Thank you."};

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);

  label.font(sukhumvit_60, TFT_WHITE);    // กำหนด font และ สี  (เนื่องจาก image ถูกกำหนดเป็น layer แม่ จะถูกสร้างอัตโนมัติ)
  label.text_align(TEXT_ALIGN_CENTER);    // กำหนดให้จัดเรียงข้อความแบบชิดกลาง (หากช้อความมีหลายบรรทัดก็ยังชิดกลาง)
  text_animation_show(&anim);             // เรียกฟังกชั่นที่จะสร้าง animation ทำงาน
}

void loop() {
  BlynkGO.update();
}


void text_animation_show(GAnimation*a){
  static uint8_t max_id = sizeof(text)/sizeof(String);
  static int8_t  id = -1;

  id++; if(id >= max_id) id = 0;  // คำนวณเพิ่ม id ปัจจุบัน

  label = text[id];               // ใส่ ข้อความให้ วิตเจ็ตข้อความ
  label.font((id < 6 )? sukhumvit_60 : sukhumvit_40);
  label.text_letter_space(0);     // ระยะระหว่างอักษร เป็น 0

  // กำหนด animation ให้วิตเจ็ต label โดยทำความโปร่งใส (opa) จากค่า 0-255 ในเวลา 5วินาที
  a->init(label, [](GWidget* w, int16_t v){
    label.opa(v);
  }, 0,255, 5000);
  
  // เมื่อ animation ทำงานเสร็จ ให้เรียก ฟังกชั่น text_animation_hide ขึ้นมาทำงาน
  a->ready_cb(text_animation_hide);
  a->create();      // ลงทะเบียนสร้าง animation สู่ระบบภายใน BlynkGO
}

void text_animation_hide(GAnimation*a){
  // กำหนด animation ให้วิตเจ็ต label โดยทำความโปร่งใส (opa) จากค่า 255-0 ในเวลา 3วินาที
  // พร้อมทั้งทำการขยายช่องไฟข้อความ
  a->init(label, [](GWidget* w, int16_t v){
    label.opa(v);
    label.text_letter_space(map_f(v,0,255,20,0));
  }, 255, 0, 3000);
  a->delay(2000);   // หยุด 2 วินาที ก่อนให้ animation เริ่มทำงาน
  // เมื่อ animation ทำงานเสร็จ ให้เรียก ฟังกชั่น text_animation_show ขึ้นมาทำงาน
  a->ready_cb(text_animation_show);
  a->create();      // ลงทะเบียนสร้าง animation สู่ระบบภายใน BlynkGO
}
