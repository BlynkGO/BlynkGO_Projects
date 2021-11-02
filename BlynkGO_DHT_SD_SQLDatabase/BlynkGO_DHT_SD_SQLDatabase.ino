#include <BlynkGOv2.h>
#include <DHTesp.h>  // ให้เข้าไป download ไลบรารี่ DHTesp ได้ที่  https://github.com/beegee-tokyo/DHTesp

#define DATABASE_FILE         "SD://DHT22_DATABASE.db"
#define DATABASE_TABLE_NAME   "my_dht22_sensor"

#define DHT_PIN               27              // กำหนดขา pin ที่ต่อ DHT
#define DHT_TYPE              DHTesp::DHT22   // กำหนดประเภท DHT

DHTesp dht;                   // dht เซนเซอร์

GWiFiManager  wifi_manager;   // วิตเจ็ต ตั้งไวไฟจากหน้าจอ
GGaugeRainbow gauge_temp;     // วิตเจ็ต GGauge
GGaugeRainbow gauge_humid;    // วิตเจ็ต GGauge

GTimer timer;                 // ตัวตั้งเวลาทำงานเป็นระยะๆ

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin();

  //-------------------------------------
  // sensor initialize
  dht.setup(DHT_PIN, DHT_TYPE);         // เริ่มต้นใช้งาน dht ด้วยการกำหนด ขา และประเภท ของ dht

  //-------------------------------------
  // ออกแบบ กราฟิก
  wifi_manager.align(ALIGN_TOP_RIGHT, -5);

  gauge_temp.size(200, 200);
  gauge_temp.color(TFT_BLUE, TFT_RED);
  gauge_temp.decimal(1);
  gauge_temp.align(ALIGN_CENTER, -gauge_temp.width() / 2  - 10);
  gauge_temp = 0;

  gauge_humid.size(200, 200);
  gauge_humid.color(TFT_BLUE, TFT_COLOR_HSV(250, 100, 40) );
  gauge_humid.decimal(1);
  gauge_humid.align(ALIGN_CENTER, gauge_humid.width() / 2 + 10);
  gauge_humid = 0;
  
  //-------------------------------------
  // เตรียม database

  // SD.remove("/DHT22_DATABASE.db");     // คำสั่งลบ file ฐานข้อมูล บน SD
  GDatabase  db(DATABASE_FILE);
  //db.exec( "DROP TABLE IF EXISTS " DATABASE_TABLE_NAME ";"); // หากจะลบ ตาราง ออกจาก database
  if (!db.tableExists(DATABASE_TABLE_NAME)) {
    db.exec( "CREATE TABLE " DATABASE_TABLE_NAME " (id INTEGER PRIMARY KEY AUTOINCREMENT, date DATE, time TIME, temperature FLOAT(4), humid FLOAT(4));" );
  }

  //-------------------------------------
  // ตั้งเวลาอ่านค่า sensor พร้อมแสดงออกไปที่ graphic ที่เตรียมไว้
  timer.setInterval(10000L, []() {       // ตั้งเวลาทำงานทุกๆ 10 วินาที
       float temp  = dht.getTemperature();
       float humid = dht.getHumidity();
//    float temp = random(2000, 2500) / 100.0;
//    float humid = random(6000, 8000) / 100.0;

    // เช็คว่าเป็น NAN ( Not a Number ) ไหม ถ้าไม่ก็จับใส่วิตเจ็ตที่ต้องการ
    if (!isnan(temp))   gauge_temp  = temp;
    if (!isnan(humid))  gauge_humid = humid;

    // ค่าอ่านได้ไม่ NAN และ เวลาได้รับการซิงค์เวลาจาก internet แล้ว
    if (!isnan(temp) && !isnan(humid) && BlynkGO.isNTPSynced() ) {
      // หลัง sync เวลาจะใช้ API เกี่ยวกับเวลาต่างๆได้
      try {
        GDatabase  db(DATABASE_FILE);
        String SQL_INSERT = StringX::printf("INSERT INTO " DATABASE_TABLE_NAME " (date, time, temperature, humid) VALUES ('%04d-%02d-%02d', '%02d:%02d:%02d', %.2f, %.2f );",
                                            year(), month(), day(), hour(), minute(), second(), temp, humid);
        Serial.println(SQL_INSERT);
        db.exec(SQL_INSERT);
        
        // ตัวอย่าง คำสั่งหาจำนวนแถว ของ ตาราง
        int row_count = db.execAndGet("SELECT COUNT(*) FROM " DATABASE_TABLE_NAME ).toInt();
        Serial.println();
        Serial.printf("Total row in table '%s' : %d\n", DATABASE_TABLE_NAME, row_count);
    
        // อ๊อบเจ็ค GQuery จะใช้สำหรับ ยิงคำสั่ง sql แบบ ต้องการรอรับผลข้อมูลที่ดึงคืนกลับมาจาก table บน GDatabase
        Serial.println("Query GDatabase ...");
        GQuery query(db, "SELECT * FROM " DATABASE_TABLE_NAME "");
        int r=0;
        while ( query.available()) {
          if(r==0) {
            Serial.println("------------------------------------------------------------------");
            for(int c=0; c < query.getColumnCount(); c++){
              Serial.printf("%-15s\t", query.getColumnName(c));
            }
            Serial.println();
            Serial.println("------------------------------------------------------------------");
          }
          
          r++;
          for(int c=0; c <query.getColumnCount(); c++){
            Serial.printf("%-15s\t", query.getColumn(c).c_str());
          }
          Serial.println();
        }
      
      }
      catch (String exception) {    // ส่วนของ catch(...) ใส่เพื่อ ดักป้องกันเมื่อการเรียกใช้ GDatabase หากมีความผิดพลาดจะมาแสดงตรงส่วนนี้
        Serial.printf("[Exception] %s\n", exception.c_str());
//        while (1) delay(1);         // สั่งให้ ESP32 หยุดการทำงานลง หากไม่ต้องการให้เอาบรรทัดนี้ออก
      }
      catch (std::exception& e) {
        Serial.printf("[Exception] %s\n", e.what());
        //        while(1) delay(1);          // สั่งให้ ESP32 หยุดการทำงานลง หากไม่ต้องการให้เอาบรรทัดนี้ออก
      }
    }
  });

}

void loop() {
  BlynkGO.update();
}
