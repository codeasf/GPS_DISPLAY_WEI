#include <TinyGPS++.h>
#include "U8glib.h"

#define GPSBaud 38400
//byte GPScommand[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x4D, 0x01, 0x01, 0x00, 0x01, 0x00, 0x64, 0x8D};		//3HZ
//byte GPScommand[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xF4, 0x01, 0x01, 0x00, 0x01, 0x00, 0x0B, 0x77};		//2HZ
byte GPScommand[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};         //5HZ

U8GLIB_ST7920_128X64_4X u8g(10);    // SPI Com: SCK = en = 13, MOSI = WR = 11, CS = di = 10
TinyGPSPlus gps;

float topSpeed = 0 , spd = 0;
char buff[15];
unsigned long time, lenTime, longTime;

void setup() {
	Serial.begin(GPSBaud);
	Serial.write(GPScommand, 14);

   // 初始化LCD,使两个计时器更准确
       // picture loop
    u8g.firstPage();
    do {
     draw();
    } while ( u8g.nextPage() );
	
	lenTime = 0;
    longTime = 0;
}

void loop() {
  //GPS DISPLAY
	time = millis();
		while (Serial.available() > 0) {
		if ( gps.encode( Serial.read() ) ) {
			if ((gps.speed.kmph() - spd) < 100){   //up to 100KM/H ??
		  		spd = gps.speed.kmph();
		  		if (spd > topSpeed)
		  			topSpeed = spd;

		  	}
		}

	}


    // picture loop
    u8g.firstPage();
    do {
    	draw();
    } while ( u8g.nextPage() );
    lenTime = millis() - time;
    if (lenTime > longTime )
        longTime = lenTime;
    Serial.println(time);
}


void draw() {
    // graphic commands to redraw the complete screen should be placed here

    u8g.setFont(u8g_font_unifont);
    int i, dec;
	i = (int)topSpeed;
	dec = (topSpeed - i) * 100;
	sprintf(buff, "TOP:%d.%02d KM/H", i, dec);
	u8g.drawStr( 0, 10, buff);

    // 显示处理时间
	sprintf(buff, "%d", lenTime);
	u8g.drawStr( 0, 63, buff);
    sprintf(buff, "%d", longTime);
    byte x = u8g.getStrWidth(buff);
    u8g.drawStr(127-x, 63, buff);

    u8g.setFont(u8g_font_fub49n);
	int iSpd = (int)spd;
	sprintf(buff, "%d", iSpd);
    byte w = u8g.getStrWidth(buff) / 2;
    w = 63 - w;
    u8g.drawStr( w, 63, buff);
}
