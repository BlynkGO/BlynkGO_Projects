#include <BlynkGO.h>

#define BLYNKGO_KEY   "48c89537d6695ad0"

/* ประกาศก่อนใช้รูปภาพจาก c file */
IMG_DECLARE(closet_off);      IMG_DECLARE(closet_on);
IMG_DECLARE(bedroom_off);     IMG_DECLARE(bedroom_on);
IMG_DECLARE(bathroom_off);    IMG_DECLARE(bathroom_on);
IMG_DECLARE(kitchen_off);     IMG_DECLARE(kitchen_on);
IMG_DECLARE(sittingroom_off); IMG_DECLARE(sittingroom_on);
IMG_DECLARE(dinningroom_off); IMG_DECLARE(dinningroom_on);

/* ประกาศตัวแปร GImageButton สร้างปุ่มกดจากรูปภาพ */
GImageButton closet     (BUTTON_SWITCH, closet_off,       closet_on);
GImageButton bedroom    (BUTTON_SWITCH, bedroom_off,      bedroom_on);
GImageButton bathroom   (BUTTON_SWITCH, bathroom_off,     bathroom_on);
GImageButton kitchen    (BUTTON_SWITCH, kitchen_off,      kitchen_on);
GImageButton sittingroom(BUTTON_SWITCH, sittingroom_off,  sittingroom_on);
GImageButton dinningroom(BUTTON_SWITCH, dinningroom_off,  dinningroom_on);

/* ขา gpio ที่ต่อกับ RELAY */
uint8_t pin[]= {22, 21, 5, 25, 13, 15};  

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY); 
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  /* กำหนดขา gpio , สถานะ ON หรือ OFF เริ่มต้น, และ ตำแหน่งของ ปุ่มกดต่างๆ */
  /* หาก relay เป็นแบบ ACTIVE_LOW (ติดเมื่อจ่ายไฟ 0) ให้ ใช้คำสั่ง เช่น bedroom.gpio(pin, ACTIVE_LOW); แทน */
  closet      .gpio(pin[0], ACTIVE_LOW);  closet      .ON();  closet      .position(35,10);
  bedroom     .gpio(pin[1], ACTIVE_LOW);  bedroom     .ON();  bedroom     .align(closet,      ALIGN_RIGHT_TOP);
  bathroom    .gpio(pin[2], ACTIVE_LOW);  bathroom    .ON();  bathroom    .align(bedroom,     ALIGN_RIGHT_TOP);
  kitchen     .gpio(pin[3], ACTIVE_LOW);  kitchen     .ON();  kitchen     .align(bathroom,    ALIGN_BOTTOM);
  sittingroom .gpio(pin[4], ACTIVE_LOW);  sittingroom .ON();  sittingroom .align(closet,      ALIGN_BOTTOM_LEFT);
  dinningroom .gpio(pin[5], ACTIVE_LOW);  dinningroom .ON();  dinningroom .align(sittingroom, ALIGN_RIGHT_TOP);
}

void loop() {
  BlynkGO.update();
  if(Serial.available()){ 
    String command = Serial.readStringUntil('\n'); command.trim();
    if(command == "mem") MEM_CHECK;
    else
    if(command == "capture") BlynkGO.capture();
  }
}
