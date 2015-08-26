#include <TinyGPS++.h>
#include "U8glib.h"

#define GPSBaud 38400
byte command3hz[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x4D, 0x01, 0x01, 0x00, 0x01, 0x00, 0x64, 0x8D};
byte command2hz[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xF4, 0x01, 0x01, 0x00, 0x01, 0x00, 0x0B, 0x77};

U8GLIB_ST7920_128X64_4X u8g(10);    // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
TinyGPSPlus gps;

float topSpeed =0 , spd =0;

void setup() {
  Serial.begin(GPSBaud);
}

void loop() {
  //GPS DISPLAY
  while (Serial.available() > 0){
    if (gps.encode(Serial.read())) {
      //if ((gps.speed.kmph() - spd) < 100){   //up to 100KM/H ??
      if (gps.course.isValid()){
        spd = gps.speed.kmph();
        if(spd > topSpeed)
        topSpeed = spd;

      // picture loop
      u8g.firstPage();
      do {
        draw();
        } while ( u8g.nextPage() );
      }
    }
  }
}

void draw() {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 10, F2String(topSpeed));

  u8g.setFont(u8g_font_fub49n);
  u8g.drawStr( 6, 63, I2String(spd));
}

char* F2String(float sp) {
  int i, dec;
  char _buff[15];
  i = sp;
  dec = (sp-(int)sp) *100;
  sprintf(_buff, "TOP:%s.%sKM/H", i, dec);
  return _buff;
}

char* I2String(float sp) {
  char _buff [15];
  sprintf(_buff, "%d", sp);
  return _buff;
}
