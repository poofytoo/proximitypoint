#include "ST7565.h"
#include <math.h>
#include <SoftwareSerial.h>

int ledPin =  13;    // LED connected to digital pin 13

// the LCD backlight is connected up to a pin so you can turn it on & off
#define BACKLIGHT_LED 10

// pin 9 - Serial data out (SID)
// pin 8 - Serial clock out (SCLK)
// pin 7 - Data/Command select (RS or A0)
// pin 6 - LCD reset (RST)
// pin 5 - LCD chip select (CS)
ST7565 glcd(9, 8, 7, 6, 5);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

// a bitmap of a 16x16 fruit icon
static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 
0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };

float angle = 0.0;
float distance = 0.0;
int altitude = 30;
int velocity = 0;


int count = 0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

// The setup() method runs once, when the sketch starts
void setup()   {                
  Serial.begin(9600);
  // Serial.println(freeRam());
  XBee.begin(9600);
  
  // turn on backlight
  // pinMode(BACKLIGHT_LED, OUTPUT);
  // digitalWrite(BACKLIGHT_LED, HIGH);

  // initialize and set the contrast to 0x18
  glcd.begin(0x01);
  glcd.clear();
 
 
  // draw a single pixel
  // glcd.setpixel(10, 10, BLACK);
  // draw the first ~120 characters in the font
  // testdrawchar();
  // glcd.display();
  // delay(2000);
  // glcd.clear();

  // draw a string at location (0,0)
  // glcd.drawstring(0, 0, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation");
  // glcd.display();
  // delay(2000);
  // glcd.clear();

  // draw a bitmap icon and 'animate' movement
  // testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}


void loop()                     
{
   if (XBee.available())
    { // If data comes in from XBee, send it out to serial monitor
    
    // get the new byte:
    char inChar = (char)XBee.read(); 
    // add it to the inputString:
    inputString += inChar;
    while (inChar != ',')
    {
      inChar = (char)XBee.read(); 
      // add it to the inputString:
      inputString += inChar;
    }
    char floatbuf[32]; 
    inputString.toCharArray(floatbuf, sizeof(floatbuf));
    angle = atof(floatbuf);
    
    inputString = "";
    // get the new byte:
    inChar = (char)XBee.read(); 
    // add it to the inputString:
    inputString += inChar;
    while (inChar != '\n')
    {
      inChar = (char)XBee.read(); 
      // add it to the inputString:
      inputString += inChar;
    }

    inputString.toCharArray(floatbuf, sizeof(floatbuf));
    distance = atof(floatbuf);

      Serial.print("angle:");
      Serial.println(angle);
      Serial.print("dist:");
      Serial.println(distance);
//      Serial.write(XBee.read());
    }

      altitude += velocity;
      if (altitude > 128 || altitude < 0) {
        velocity = -velocity;
        altitude += velocity*3;
      }
      // angle += 0.2;
      glcd.clear();
      drawBackground();
      drawMarker(altitude, 30, angle);
}

// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void drawBackground(void) {
  for (uint8_t i=0; i < 6; i++) {
    glcd.drawline(i*24+15, 0, i*24+15, 64, BLACK);
    String num = String(i*10);
  }
  glcd.drawstring(0, 0, "-20");
  glcd.drawstring(30, 0, "-10");
  glcd.drawstring(62, 0, "0");
  glcd.drawstring(85, 0, "10");
  glcd.drawstring(110, 0, "20");
}

void drawMarker(uint8_t altitude, uint8_t offsetX, float angle) {
  const float pi = 3.1415;
  
  float bottomLeft = angle + 5 * pi / 4;
  float bottomRight = angle + 3 * pi / 4;
  
  int topx = (double) sin(angle) * 8 + offsetX;
  int topy = (double) cos(angle) * 8 + altitude;
  int leftx = (double) sin(bottomLeft) * 6 + offsetX;
  int lefty = (double) cos(bottomLeft) * 6 + altitude;
  int rightx = (double) sin(bottomRight) * 6 + offsetX;
  int righty = (double) cos(bottomRight) * 6 + altitude;
  
  glcd.drawcircle(altitude, offsetX, 10, BLACK);
  glcd.drawline(topy, topx, lefty, leftx, BLACK);
  glcd.drawline(topy, topx, righty, rightx, BLACK);
  //glcd.drawline(altitude,32,29,35, BLACK);
  glcd.display();
}

