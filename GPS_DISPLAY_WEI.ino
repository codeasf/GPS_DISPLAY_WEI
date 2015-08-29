#include <TinyGPS++.h>
#include "U8glib.h"

#define GPSBaud 38400
byte GPScommand[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x4D, 0x01, 0x01, 0x00, 0x01, 0x00, 0x64, 0x8D};		//3HZ
//byte GPScommand[14] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xF4, 0x01, 0x01, 0x00, 0x01, 0x00, 0x0B, 0x77};		//2HZ

U8GLIB_ST7920_128X64_4X u8g(10);    // SPI Com: SCK = en = 13, MOSI = WR = 11, CS = di = 10
TinyGPSPlus gps;

float topSpeed = 0 , spd = 0;
char buff[15];

void setup() {
	Serial.begin(GPSBaud);
	Serial.write(GPScommand, 14);
}

void loop() {
  //GPS DISPLAY
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

}


void draw() {
    // graphic commands to redraw the complete screen should be placed here

    u8g.setFont(u8g_font_unifont);
    int i, dec;
	i = (int)topSpeed;
	dec = (topSpeed - i) * 100;
	sprintf(buff, "TOP:%d.%02d KM/H", i, dec);
	u8g.drawStr( 0, 10, buff);

    u8g.setFont(u8g_font_fub49n);
	int iSpd = (int)spd;
	sprintf(buff, "%d", iSpd);
    byte x = 0;
    byte w = u8g.getStrWidth(buff) / 2;
    x = 63 - w;
    u8g.drawStr( x, 63, buff);
}