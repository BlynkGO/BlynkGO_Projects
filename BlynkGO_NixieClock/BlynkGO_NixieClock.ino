#include <BlynkGO.h>
#include "NixieClock.h"
#include <TridentTD_LineNotify.h>

#define BLYNKGO_KEY           "----------------"

#define SSID                  "----------------"
#define PASSWORD              "----------------"
#define LINE_TOKEN            "----------------"


void capture_notify();

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);
  BlynkGO.fillScreen(TFT_WHITE);

  Serial.println("WiFi Connecting...");
  WiFi.begin(SSID, PASSWORD);     // เริ่มเชื่อมต่อสัญญาณ WiFi
  LINE.setToken(LINE_TOKEN);      // กำหนด Line Notify Token

  // สร้าง NixieClock
  GObject *nixie_clock = NixieClock::create();

  // เมื่อมีสัญญาณกดค้างยาว
  nixie_clock->onLongPressed([](GWidget*w){
    Serial.println("Capture Notify!!");
    capture_notify();     // จับภาพหน้าจอ และส่ง LINE Notify
  });
  
}

void loop() {
  BlynkGO.update();

}

/* เมื่อ WiFi มีการเชื่อมต่อสำเร็จ*/
WIFI_CONNECTED(){
  Serial.print("WiFi Connected. IP : ");
  Serial.println(WiFi.localIP());
}

/* เมื่อ BlynkGO ซิงค์เวลาจาก NTP สำเร็จ*/
NTP_SYNCED(){
  static GTimer clock_timer;
  clock_timer.setInterval(1000L,[](){
    static time_t now;
    static struct tm tm;
    time(&now); localtime_r(&now, &tm);
    NixieClock::setTime(tm);
  });
}

void capture_notify(){
  static uint8_t capture_counter =1;

  BlynkGO.capture(        StringX::printf("SD://BlynkGO/captures/nixieclock_%02d.png" , capture_counter).c_str());
  LINE.notifyPicture( SD, StringX::printf("/BlynkGO/captures/nixieclock_%02d.png"     , capture_counter).c_str());

  capture_counter++;
}


