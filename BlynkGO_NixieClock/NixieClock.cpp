#include "NixieClock.h"

#define NIXIE_SOURCE    "SD://BlynkGO/images/nixie/nixie_%d.png"
//#define NIXIE_SOURCE    "SD://BlynkGO/images/nixie/nixie_%d.jpg"
//#define NIXIE_SOURCE    "SD://BlynkGO/images/nixie/nixie_%d.bmp"

namespace NixieClock {
  GObject obj_nixie_clock;
  GImage nixie_h[2], nixie_m[2], nixie_s[2];
  GCircle cir_dot[4];
  GLabel  date;
  char* weekday[7] = {"อา", "จ", "อัง", "พ.", "พฤ", "ศ", "ส"};

  void set_src(GImage &img, uint8_t num)  { 
    img = StringX::printf( NIXIE_SOURCE, num);
    img.invalidate();
  }
  
  void set_num(GImage *img, uint8_t num, bool both=true) {
    if(both) set_src(img[0], (uint8_t) (num / 10));
    set_src(img[1], (uint8_t) (num % 10));
//    BlynkGO.update();
  }
  
  void setTime(struct tm &tm){
    static struct tm prev_tm;
    
    if( prev_tm.tm_hour != tm.tm_hour)  set_num( nixie_h, tm.tm_hour, (uint8_t)(prev_tm.tm_hour / 10 )  != (uint8_t)(tm.tm_hour / 10 ));
    if( prev_tm.tm_min  != tm.tm_min)   set_num( nixie_m, tm.tm_min , (uint8_t)(prev_tm.tm_min / 10 )   != (uint8_t)(tm.tm_min / 10 ));
    if( prev_tm.tm_sec  != tm.tm_sec)   set_num( nixie_s, tm.tm_sec , (uint8_t)(prev_tm.tm_sec / 10 )   != (uint8_t)(tm.tm_sec / 10 ));
    if( prev_tm.tm_mday != tm.tm_mday)  date = StringX::printf("%s %02d/%02d/%04d", weekday[tm.tm_wday], tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900+543);

    memcpy(&prev_tm, &tm, sizeof(struct tm));
//    obj_nixie_clock.invalidate();
  }
  
  GObject* create(){
    obj_nixie_clock.create();
    
    set_num( nixie_h, 0);
    set_num( nixie_m, 0);
    set_num( nixie_s, 0);
  
    int padding       = 30;
    int padding_inner = 20;
    int nixie_w = nixie_h[0].width()*6 + 2*padding_inner;
  //  int nixie_h = nixie_h[0].height();
  
    obj_nixie_clock.size(nixie_w  + 2 * padding, nixie_h[0].height() + 2 * padding);//60);
    obj_nixie_clock.color(TFT_BLACK);
    obj_nixie_clock.corner_radius(30);
    obj_nixie_clock.font(prasanmit_25, TFT_WHITE);
    
    nixie_h[0].parent(obj_nixie_clock);
    nixie_h[1].parent(obj_nixie_clock);
    nixie_m[0].parent(obj_nixie_clock);
    nixie_m[1].parent(obj_nixie_clock);
    nixie_s[0].parent(obj_nixie_clock);
    nixie_s[1].parent(obj_nixie_clock);
  
    nixie_h[0].clickable(false);
    nixie_h[1].clickable(false);
    nixie_m[0].clickable(false);
    nixie_m[1].clickable(false);
    nixie_s[0].clickable(false);
    nixie_s[1].clickable(false);
    
  //  nixie_h[0].align(ALIGN_CENTER, -nixie_w/2);
    nixie_h[0].align(ALIGN_LEFT, padding, 10);
    nixie_h[1].align(nixie_h[0],ALIGN_RIGHT);
  
    nixie_m[0].align(nixie_h[1],ALIGN_RIGHT, padding_inner);
    nixie_m[1].align(nixie_m[0],ALIGN_RIGHT);
  
    nixie_s[0].align(nixie_m[1],ALIGN_RIGHT, padding_inner);
    nixie_s[1].align(nixie_s[0],ALIGN_RIGHT);

    date.parent(obj_nixie_clock);
    date = "อา. --/--/----";
    date.align(nixie_s[1], ALIGN_TOP_RIGHT, -10);
  
    for(int i = 0; i < 4; i++) {
      cir_dot[i].parent(obj_nixie_clock);
      cir_dot[i].radius(3);
      cir_dot[i].color(TFT_WHITE);
    }
    
    cir_dot[0].c_align(nixie_h[1], ALIGN_RIGHT,  9,  -5);
    cir_dot[1].  align(cir_dot[0], ALIGN_BOTTOM, 0,  10);
    cir_dot[2].c_align(nixie_m[1], ALIGN_RIGHT,  9,  -5);
    cir_dot[3].  align(cir_dot[2], ALIGN_BOTTOM, 0,  10);

    return &obj_nixie_clock;
  }
} // namespace NixieClock
