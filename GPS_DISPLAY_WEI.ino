#include <TinyGPS++.h>
#include "U8glib.h"

U8GLIB_ST7920_128X64_4X u8g(7);    // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 7
TinyGPSPlus gps;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 10, "TOP:123.45KM/H");
    
  u8g.setFont(u8g_font_fub49n);
  u8g.drawStr( 6, 63, "012");
}

void setup(void) {

}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}

