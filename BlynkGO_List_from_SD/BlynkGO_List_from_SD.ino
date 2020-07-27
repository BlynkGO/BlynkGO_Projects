#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------" 


#define SD_MP3_FOLDER    "/BlynkGO/mp3"      // folder บน SD ที่เก็บ ไฟล์ mp3

// ประกาศก่อนใช้ รูปภาพ c file
IMG_DECLARE(ico_mp3);

// GRect สำหรับ แสดง หัวข้อและ list
GRect   rect_music_list;                  // วิตเจ็ตสี่เหลี่ยม สำหรับทั้งหน้าจอ
GRect   rect_titlebar(rect_music_list);   // วิตเจ็ตสี่เหลี่ยม สำหรับเป็น titlebar โดยเป็น layer ลูกของ rect_music_list
GLabel  lb_title(rect_titlebar);          // วิตเจ็ตข้อความ โดยเป็น layer ลูก่ของ rect_titlebar
GList   list(rect_music_list);            // วิตเจ็ต list โดยเป็น ayer ลูกของ rect_music_list

// GRect สำหรับ บอกข้อมูลที่ได้กดเลือก
GRect   rect_info;
GLabel  lb_info(rect_info);
GLabel  lb_song(rect_info);

// ฟังกชั่น สำหรับ อ่านจาก sd/ spiffs เพื่อ เพิ่มรายชื่อ ไฟล์ นามสกุลที่ต้องการเข้าไปใน list
void AddFileList(GList &blynkgo_list, fs::FS &fs, const char * dirname, String file_surname, img_t &img);

void setup() {
  Serial.begin(115200); Serial.println();
  BlynkGO.begin(BLYNKGO_KEY);

  rect_music_list.size(GScreen);        // ให้มีขนาดเต็มหน้าจอ
  rect_music_list.color(TFT_BLACK);     // กำหนดสีดำ
  rect_titlebar.width(rect_music_list); // กว้างเท่ากับความกว้างของวิตเจ็ต rect_music_list
  rect_titlebar.color(TFT_ORANGE);      // สีเหลี่ยม titlebar นี้สีส้ม
  rect_titlebar.font_color(TFT_BLACK);  // หากมีวิตเจ็ตข้อความบน titlebar ให้มีสีดำ
  lb_title = "รายชื่อเพลงจาก SD Card";
  lb_title.align(ALIGN_TOP_LEFT,10);    // วางชิดบนซ้าย ของ rect_titlebar ขยับไป 10 pixel ด้านขวา
  rect_titlebar.height(lb_title);       // ความสูงเท่ากับ วิตเจ็ต ข้อความ
  rect_titlebar.align(ALIGN_TOP);       // วางไว้ชิดด้านบนภายในของ rect_music_list

  list.size(rect_music_list.width(), rect_music_list.height() - rect_titlebar.height());
  list.align(rect_titlebar, ALIGN_BOTTOM);//วางไว้ใต้ถัดจาก rect_titlebar
  list.font(prasanmit_40, TFT_WHITE);
  list.padding_top(7);                   // ระยะช่องไฟด้านบนของ item
  list.padding_bottom(7);                // ระยะช่องไฟด้านล่างของ item
  list.padding_left(20);                 // ระยะช่องไฟด้านซ้ายของ item
  list.padding_right(0);                 // ระยะช่องไฟด้านขวาของ item

  list.style[RELEASED].color(TFT_BLACK);    // กำหนดสีขณะปล่อย
  list.style[PRESSED].color(TFT_DARKGRAY);  // กำหนดสีขณะกดเลือก

  // เมื่อมีการกด click เข้ามา
  list.onClicked([](GWidget * widget) {
    uint8_t id = list.item_selected_id();
    if(id >= 0) {
      Serial.printf("List Clicked : [%d] %s\n", id, list.item_selected().c_str());
      rect_info.hidden(false);
      lb_song = list.item_selected();       // สิ่งที่กดเลือกเข้ามา ให้ใส่ที่ lb_song

      // ปรับลูกเล่นการแสดงผลหากข้อความยาวเกินระยะ
      lb_song.long_mode(LABEL_LONG_EXPAND); 
      int lb_song_max_width = rect_info.width()-100;
      if(lb_song.width() > lb_song_max_width) {
        lb_song.long_mode(LABEL_LONG_SROLL_CIRC, lb_song_max_width);
      }
    }
  });

  rect_info.size(GScreen.width()-30, 200);    // กำหนดขนาด
  rect_info.corner_radius(20);                // กำหนดรัศมีความโค้งของมุม
  rect_info.border(5, TFT_SILVER);            // กำหนดขอบ
  rect_info.font(prasanmit_40, TFT_BLACK);    // กำหนด font ภายใน layer นี้
  rect_info.onClicked([](GWidget*w){          // เมื่อมีการ click
    rect_info.hidden(true);
  });
  rect_info.hidden(true);                     // สถานะเริ่มแรกให้ซ่อนลงไปก่อน
  lb_info = "เพลงที่คุณเลือก : ";
  lb_info.font(prasanmit_25, TFT_BLACK);
  lb_song = "--";
  lb_info.align(ALIGN_TOP_LEFT,50, lb_song.topY()-lb_info.height());  // วางไว้เหนือ lb_info แต่ห่างจากขอบซ้าย 50 pixel

  // เพิ่มรายชื่อ ไฟล์ นามสกุลที่ต้องการเข้าไปใน list
  AddFileList(list, SD, SD_MP3_FOLDER,  "mp3", (img_t&) ico_mp3);
}

void loop() {
  BlynkGO.update();
}

// ฟังกชั่น สำหรับ อ่านจาก sd/ spiffs เพื่อ เพิ่มรายชื่อ ไฟล์ นามสกุลที่ต้องการเข้าไปใน list
void AddFileList(GList &blynkgo_list, fs::FS &fs, const char * dirname, String file_surname, img_t &img ) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      String file_name = file.name();
      if( file_surname == "*" || file_name.endsWith( file_surname )) {
        file_name = file_name.substring(file_name.lastIndexOf("/")+1);
        blynkgo_list.addItem(img,  file_name);
      }
    }
    file = root.openNextFile();
  }
}
