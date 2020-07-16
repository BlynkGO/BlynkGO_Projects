#include <BlynkGO.h>

//#define BLYNKGO_KEY   "----------------"
//#define SSID          "----------------"
//#define PASSWORD      "----------------"
//
//#define BLYNK_AUTH    "----------------"
//#define BLYNK_HOST    "blynk.iot-cm.com"   // ตย. Blynk Server แบบกำหนดเอง
//#define BLYNK_PORT    8080                 // ตย. Blynk Port   แบบกำหนดเอง

#define BLYNKGO_KEY   "48c89537d6695ad0"
#define SSID          "MiHome"
#define PASSWORD      "12345678"

#define BLYNK_AUTH    "fql-0VbqsoIKKgxdKC6e1mUUbl3n7lqH"
#define BLYNK_HOST    "blynk.iot-cm.com"   // ตย. Blynk Server แบบกำหนดเอง
#define BLYNK_PORT    8080                 // ตย. Blynk Port   แบบกำหนดเอง


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
//uint8_t pin[]= {22, 21, 5, 15, 25, 13};   
uint8_t pin[]= {22, 21, 5, 25, 13, 15};  

void setup() {
  Serial.begin(115200); Serial.println();
//  BlynkGO.begin(BLYNKGO_KEY); 
  BlynkGO.begin(BLYNKGO_KEY, BLYNK_AUTH, SSID, PASSWORD, BLYNK_HOST, BLYNK_PORT); // แบบเชื่อมต่อ Blynk Server กำหนดเอง

  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  /* กำหนดขา gpio , สถานะ ON หรือ OFF เริ่มต้น, และ ตำแหน่งของ ปุ่มกดต่างๆ */
  /* หาก relay เป็นแบบ ACTIVE_LOW (ติดเมื่อจ่ายไฟ 0) ให้ ใช้คำสั่ง เช่น bedroom.gpio(pin, ACTIVE_LOW); แทน */
//  closet      .gpio(pin[0]);  closet      .ON();  closet      .position(35,10);
//  bedroom     .gpio(pin[1]);  bedroom     .ON();  bedroom     .align(closet,      ALIGN_RIGHT_TOP);
//  bathroom    .gpio(pin[2]);  bathroom    .ON();  bathroom    .align(bedroom,     ALIGN_RIGHT_TOP);
//  kitchen     .gpio(pin[3]);  kitchen     .ON();  kitchen     .align(bathroom,    ALIGN_BOTTOM);
//  sittingroom .gpio(pin[4]);  sittingroom .ON();  sittingroom .align(closet,      ALIGN_BOTTOM_LEFT);
//  dinningroom .gpio(pin[5]);  dinningroom .ON();  dinningroom .align(sittingroom, ALIGN_RIGHT_TOP);

  closet      .gpio(pin[0], ACTIVE_LOW);  closet      .ON();  closet      .position(35,10);
  bedroom     .gpio(pin[1], ACTIVE_LOW);  bedroom     .ON();  bedroom     .align(closet,      ALIGN_RIGHT_TOP);
  bathroom    .gpio(pin[2], ACTIVE_LOW);  bathroom    .ON();  bathroom    .align(bedroom,     ALIGN_RIGHT_TOP);
  kitchen     .gpio(pin[3], ACTIVE_LOW);  kitchen     .ON();  kitchen     .align(bathroom,    ALIGN_BOTTOM);
  sittingroom .gpio(pin[4], ACTIVE_LOW);  sittingroom .ON();  sittingroom .align(closet,      ALIGN_BOTTOM_LEFT);
  dinningroom .gpio(pin[5], ACTIVE_LOW);  dinningroom .ON();  dinningroom .align(sittingroom, ALIGN_RIGHT_TOP);

  /* เพิ่ม virtual pin ให้ทำงานกับแอพ Blynk ได้ */
  closet      .virtualPin(V1);
  bedroom     .virtualPin(V2);
  bathroom    .virtualPin(V3);
  kitchen     .virtualPin(V4);
  sittingroom .virtualPin(V5);
  dinningroom .virtualPin(V6);
}

void loop() {
  BlynkGO.update();
}

WIFI_CONNECTED(){
  Serial.print("[WiFi] Connected. IP : ");
  Serial.println(WiFi.localIP());
}

WIFI_DISCONNECTED(){
  Serial.print("[WiFi] Disconnected");
}

/* เพิ่มเติมให้มีการ sync จากสถานะปัจจุบันจากฝั่ง ESP32 ส่งไปให้ Blynk Server */
BLYNK_CONNECTED(){
  Serial.println("[Blynk] Connected");
  BlynkGO.virtualWrite(V1, closet.isON());
  BlynkGO.virtualWrite(V2, bedroom.isON());
  BlynkGO.virtualWrite(V3, bathroom.isON());
  BlynkGO.virtualWrite(V4, kitchen.isON());
  BlynkGO.virtualWrite(V5, sittingroom.isON());
  BlynkGO.virtualWrite(V6, dinningroom.isON());
}

BLYNK_DISCONNECTED(){
  Serial.println("[Blynk] DisConnected");
}

/* เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V1 */
BLYNK_WRITE(V1){    
  closet.state(param.asInt());  // ให้ทำการ update สถานะ ของ closet ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัวอัตโนมัติ)
}

/* เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V2 */
BLYNK_WRITE(V2){    
  bedroom.state(param.asInt());  // ให้ทำการ update สถานะ ของ bedroom ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัวอัตโนมัติ)
}

/* เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V3 */
BLYNK_WRITE(V3){    
  bathroom.state(param.asInt());  // ให้ทำการ update สถานะ ของ bathroom ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัวอัตโนมัติ)
}

/* เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V4 */
BLYNK_WRITE(V4){    
  kitchen.state(param.asInt());  // ให้ทำการ update สถานะ ของ kitchen ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัวอัตโนมัติ)
}

/* เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V5 */
BLYNK_WRITE(V5){    
  sittingroom.state(param.asInt());  // ให้ทำการ update สถานะ ของ sittingroom ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัวอัตโนมัติ)
}

/* เมื่อ แอพ blynk มีการกดปุ่มเข้ามาทาง virtual pin V6 */
BLYNK_WRITE(V6){    
  dinningroom.state(param.asInt());  // ให้ทำการ update สถานะ ของ dinningroom ด้วย (ซึ่งจะอัพเดตขา gpio ด้วยในตัวอัตโนมัติ)
}
