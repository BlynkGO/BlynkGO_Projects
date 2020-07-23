#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------"

// ประกาศรูปแบบ c file ก่อนใช้งาน ในวิตเจ็ต
IMG_DECLARE(pic_matrix_480x320);    
IMG_DECLARE(pic_neo);
IMG_DECLARE(pic_trinity);
IMG_DECLARE(pic_morpheus);
IMG_DECLARE(pic_speech_right);
IMG_DECLARE(pic_matrix_exit);

GImage matrix       (pic_matrix_480x320);     // วิตเจ็ตรูป พื้นหลัง ด้วย รูป pic_matrix_480x320
GImage neo          (pic_neo);                // วิตเจ็ตรูป Neo   ด้วย รูป pic_neo
GImage trinity      (pic_trinity);            // วิตเจ็ตรูป Trinity ด้วย รูป pic_trinity

GImage morpheus     (pic_morpheus);           // วิตเจ็ตรูป Morpheus ด้วย รูป pic_morpheus
GImage speech_right (pic_speech_right, morpheus);  // วิตเจ็ตรูป กล่องข้อความ ด้วยรูป pic_speech_right โดยเป็น layer ลูกของ วิตเจ็ต morpheus
GLabel speech       (speech_right);           // วิตเจ็ตข้อความ speech โดยเป็น layer ลูกของ วิตเจ็ต speech_right
GObject area_red    (morpheus);               // วิตเจ็ตออปเจ็คต์ แทนพื้นที่คลุมมือ morpheus สำหรับกดเลือก เม็ดสีแดง
GObject area_blue   (morpheus);               // วิตเจ็ตออปเจ็คต์ แทนพื้นที่คลุมมือ morpheus สำหรับกดเลือก เม็ดสีน้ำเงิน

GImage matrix_exit  (pic_matrix_exit);        // วิตเจ็ตรูป ประตู   ด้วย รูป pic_matrix_exit

// ตัวสร้าง Animation สำหรับใช้กับวิตเจ็ตต่างๆ
GAnimation anim_neo;
GAnimation anim_trinity;                      
GAnimation anim_morpheus;
GAnimation anim_hide;
GAnimation anim_show;

GGauge temp_gauge( "อุณหภูมิ (°C)", TFT_RED );  // วิตเจ็ตเกจจ์หน้าปัด สำหรับอุณหภูมิ
GGauge humid_gauge( "ความชื้น (%)", TFT_BLUE ); // วิตเจ็ตเกจจ์หน้าปัด สำหรับความชื้น

// ฟังกชั่น ANIM สำหรับ ปรับ OPA
void MATRIX_ANIM_OPA(GWidget*w, int16_t v) {
  morpheus.opa_scale(v);                      // morpheus ถูกปรับ opa แบบ รวมทั้ง layer ลูกของ morpheus ทั้งหมดด้วย
  ((GImage*)morpheus.user_data())->opa(v);    // neo หรือ trinity ที่จะถูกปรับ opa ตาม morpheus
  matrix.opa(v);                              // matrix พื้นหลัง ถูกปรับ opa ตาม morpheus
}

// เมื่อ Animation มีการ แสดงคืนสภาพเสร็จสิ้น
void ANIM_SHOW_Ready_cb(GAnimation* a){
  speech_right.hidden(false);                 // กล่องข้อความให้แสดงขึ้นมา
  speech_right.reset();                       // ให้ล้าง filter ที่เคยกำหนดไป
  speech = "เลือก เม็ด#ff0000 สีแดง# หรือ เม็ด#0000ff สีน้ำเงิน#?      ";   // กำหนดข้อความให้ วิตเจ็ตข้อความ speech
}

// เมื่อมีการกดที่ รูปประตู
void matrix_exit_click(GWidget* widget){
//  Serial.println("exit clicked");
  GImage* img = (GImage*)(morpheus.user_data());
  img->hidden(false);
  matrix_exit.hidden(true); 
  morpheus.hidden(false);
  speech_right.hidden(true);
  
  if( img->sameWith(neo) ) { 
    humid_gauge.hidden(true);
  }else
  if( img->sameWith(trinity) ) { 
    temp_gauge.hidden(true);
  }
  
  anim_show.init(morpheus, MATRIX_ANIM_OPA, 0, 255, 3000);
  anim_show.ready_cb(ANIM_SHOW_Ready_cb);
  anim_show.create();
}

// เมื่อ Animation มีการ ซ่อนสิ่งต่างๆด้วย OPA เสร็จสิ้น
void ANIM_HIDE_Ready_cb(GAnimation* a){
  morpheus.hidden(true);
  GImage* img = (GImage*)(morpheus.user_data());
  img->hidden(true);
  
  if(neo == img){
    Serial.println("Neo Hide");
    humid_gauge.hidden(false);
    humid_gauge.align(ALIGN_LEFT, 40,-10);
    humid_gauge = 60;
    matrix_exit.align(ALIGN_BOTTOM_LEFT, 10,-10);
    matrix_exit.hidden(false); 
    matrix_exit.clickable(true);
    matrix_exit.onClicked( matrix_exit_click );
  }else{
    Serial.println("Trinity Hide");
    temp_gauge.hidden(false);
    temp_gauge.align(ALIGN_RIGHT,-40,-10);
    temp_gauge = 24.3;
    matrix_exit.align(ALIGN_BOTTOM_RIGHT, -10,-10);
    matrix_exit.hidden(false); 
    matrix_exit.clickable(true);
    matrix_exit.onClicked( matrix_exit_click );
  }
}

// เมื่อมีการ click เลือก เม็ดยาสีแดง หรือ สีน้ำเงินเข้ามา
void morpheus_select(GWidget*widget){
  if( widget->sameWith(area_red) ) {   // หาก พื้นที่เม็ดสีแดงที่กด ตรงกับ พื้นที่ widget
    Serial.println("RED Pill");
    BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);
    speech_right.filter(TFT_RED,50);
    speech = "เม็ด#ff0000 สีแดง#!";

    morpheus.user_data(&trinity);
    anim_hide.init(morpheus, MATRIX_ANIM_OPA, 255, 0, 3000);
    anim_hide.delay(1000);
    anim_hide.ready_cb(ANIM_HIDE_Ready_cb);
    anim_hide.create();
  }else
  if( widget->sameWith(area_blue) ){   // หาก พื้นที่เม็ดสีน้ำเงินที่กด ตรงกับ พื้นที่ widget
    speech_right.filter(TFT_BLUE,50);
    Serial.println("BLUE Pill");
    BlynkGO.fillScreen(TFT_BLUE, TFT_CYAN);
    speech = "เม็ด#0000ff สีน้ำเงิน#!";

    morpheus.user_data(&neo);
    anim_hide.init(morpheus, MATRIX_ANIM_OPA, 255, 0,3000);
    anim_hide.delay(1000);
    anim_hide.ready_cb(ANIM_HIDE_Ready_cb);
    anim_hide.create();
  }
}

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);
  BlynkGO.fillScreen(TFT_RED, TFT_ORANGE);

  /* สร้างรูป แต่ละรูปขึ้นมา */
  matrix.create();                      // ทำการสร้าง
  neo.create();                         // ทำการสร้าง
  trinity.align(ALIGN_OUT_LEFT_MID);    // วางไว้นอกจอ ด้านซ้าย ไปก่อน
  morpheus.align(ALIGN_OUT_BOTTOM_MID); // วางไว้นอกจอ ด้านล่าง ไปก่อน
  morpheus.opa_scalable(true);          // ให้วิตเจ็ต morpheus รวมถึง layer ลูกบน morpheus ทำ opa ไปพร้อมๆกัน
  matrix_exit.hidden(true);             // สร้างแล้วซ่อนลงไป

  /* สร้างพื้นที่ click เลือก เม็ดสีแดง */
  area_red.color(TFT_RED);
  area_red.opa(0);                      // ทำให้มองไม่เห็น แต่แสดงอยู่
  area_red.size(80,80);
  area_red.align(ALIGN_BOTTOM_LEFT,10,-10);
  area_red.onClicked( morpheus_select );// เมื่อวิตเจ็ตนี้มีการ clicked ให้ไปเรียก morpheus_select ทำงาน

  /* สร้างพื้นที่ click เลือก เม็ดสีน้ำเงิน */
  area_blue.color(TFT_BLUE);
  area_blue.opa(0);       // ทำให้มองไม่เห็น แต่แสดงอยู่
  area_blue.size(80,80);
  area_blue.align(ALIGN_BOTTOM_RIGHT,-10,-10);
  area_blue.onClicked( morpheus_select );// เมื่อวิตเจ็ตนี้มีการ clicked ให้ไปเรียก morpheus_select ทำงาน

  // Animation สำหรับ Neo
  anim_neo.init(neo, ANIM_X, BlynkGO.width(), 0, 3000);
  anim_neo.create();

  // Animation สำหรับ Trinity
  anim_trinity.init(trinity, ANIM_X, -trinity.width(), BlynkGO.width() - 20 - trinity.width(), 3000);
  anim_trinity.delay(5000); // ให้เริ่มทำงานถัดไปอีก 5 วินาที ค่อยเริ่ม animation นี้
  anim_trinity.create();

  // Animation สำหรับ Morpheus
  anim_morpheus.init(morpheus, ANIM_Y,BlynkGO.height(),  BlynkGO.height() - morpheus.height(), 3000);
  anim_morpheus.delay(10000); // ให้เริ่มทำงานถัดไปอีก 10 วินาที ค่อยเริ่ม animation นี้
  anim_morpheus.ready_cb([](GAnimation*a){
    speech_right.align(ALIGN_IN_TOP, 60, 80);
    speech = "เลือก เม็ด#ff0000 สีแดง# หรือ เม็ด#0000ff สีน้ำเงิน#?      ";
    speech.colorful(true);
    speech.color(TFT_BLACK);
    speech.align_center(0,10);
    speech.long_mode(LABEL_LONG_SROLL_CIRC, speech_right.width()-25);
  });
  anim_morpheus.create();
}

void loop() {
  BlynkGO.update();
}
