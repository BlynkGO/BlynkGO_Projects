#include <BlynkGO.h>

#define BLYNKGO_KEY   "----------------"

IMG_DECLARE(pic_blue_particle);
FONT_DECLARE(BebasNeueBold_num_50);
FONT_DECLARE(morkhor_40);

GImage image(pic_blue_particle);

GKnob knob(image);
GLabel knob_lb;

GRoller roller_h;
GRoller roller_m;
GRoller roller_s;
GLabel  lb_colon[2];

GLed led(TFT_GREEN);
GLabel label("เปิด",led);

uint32_t bright = 255;
uint32_t total_sec;
GTimer timer;

void roller_onChanged(GWidget*w){
  total_sec = roller_h.selected().toInt()*3600 + roller_m.selected().toInt()*60 + roller_s.selected().toInt();
}

enum {STATE_OFF, STATE_ON, STATE_START, STATE_PAUSE } state;

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

  knob.color(TFT_RED);
  knob.align(ALIGN_CENTER, -knob.width()/2-30, -10);
  knob.range(0,255);
  
  /* หากมีการเปลี่ยนแปลงค่าของ knob */
  knob.onValueChanged([](GWidget* widget){
    GKnob* pKnob = (GKnob*) widget;
    bright = knob.value();
    knob_lb = "ความสว่าง : " + String(knob.value());
  });

  knob_lb = "ความสว่าง : " + String(knob.value());
  knob_lb.font(morkhor_40, TFT_WHITE);
  knob_lb.align(knob, ALIGN_BOTTOM,0, 15);

  roller_h.opa(0); roller_h.font(BebasNeueBold_num_50, TFT_WHITE);  roller_h.visible_rows(1);
  roller_m.opa(0); roller_m.font(BebasNeueBold_num_50, TFT_WHITE);  roller_m.visible_rows(1);
  roller_s.opa(0); roller_s.font(BebasNeueBold_num_50, TFT_WHITE);  roller_s.visible_rows(1);

  roller_h.options(0,23, true); roller_h.onValueChanged(roller_onChanged);
  roller_m.options(0,59, true); roller_m.onValueChanged(roller_onChanged);
  roller_s.options(0,59, true); roller_s.onValueChanged(roller_onChanged);

  lb_colon[0] = ":";  lb_colon[0].font(BebasNeueBold_num_50, TFT_WHITE);
  lb_colon[1] = ":";  lb_colon[1].font(BebasNeueBold_num_50, TFT_WHITE);

  led.size(140,140);
  led.align(knob, ALIGN_RIGHT, 60, -10);
  led.onClicked([](GWidget* w){    
    switch(state){
      case STATE_OFF:
        led.ON();
        led.brightness(bright);
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
        led.brightness(bright);
        state = (total_sec)? STATE_START : STATE_ON;
        break;
    }
    label = led.isON()? "เปิด" : ((total_sec)? "หยุด" : "ปิด");

    if( state == STATE_ON || state == STATE_START ) {
      if( total_sec && led.isON() ) {
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
