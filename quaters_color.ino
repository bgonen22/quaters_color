#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

// the last led of each quarter (the 4th is NUMOFPIXELS-1)
int q1_end = 19;
int q2_end = 39;
int q3_end = 59;

#define NUMOFCOLORS  60  
// the color jump of each iteration
#define COLOR_JUMP  7

// delay between iterations
#define DELAY 5000 // delay between colors change (msec)
#define FADE_DELAY 300 // delay fade (msec)

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.

}

void loop() {
  int c1 = 1;
  int c2 = 10;
  int c3 = 20;
  int c4 = 30;
  while (1) {    
    for (float level = 0.1; level <=1; level = level + 0.1) {
      light_trace(c1,c2,c3,c4,level); 
      pixels.show(); 
      delay (FADE_DELAY/COLOR_JUMP);
    }
    c1 = (c1 + COLOR_JUMP)%NUMOFCOLORS;
    c2 = (c2 + COLOR_JUMP)%NUMOFCOLORS;
    c3 = (c3 + COLOR_JUMP)%NUMOFCOLORS;
    c4 = (c4 + COLOR_JUMP)%NUMOFCOLORS;              
    delay (DELAY);
  }
}

void light_trace(int c1, int c2, int c3, int c4,float level) {  
  for (int i = COLOR_JUMP ; i > 0; --i) {    
//    Serial.print(power);
    //Serial.print(" ");
    //Serial.print(i);
    //Serial.print(" ");
    //Serial.println(fade_in);
    light_quarter (c1-i,0,q1_end,level);
    light_quarter (c2-i,q1_end+1,q2_end,level);
    light_quarter (c3-i,q2_end+1,q3_end,level);
    light_quarter (c4-i,q3_end+1,NUMPIXELS-1,level);    
    
  }
}

void light_quarter (int color, int start_led, int end_led,float level) {        

  for (int i = start_led; i< end_led; ++i) {
     pixels.setPixelColor(i, get_color(color,color - COLOR_JUMP,level));       
   }   
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// the level is how brigt will be tghe light (0 to 255).
uint32_t Wheel(byte color, float level) {
  float power;  
 // Serial.println(color);
  
  if(color < NUMOFCOLORS/3) {
     power=1.0*color/(NUMOFCOLORS/3)*255;
     return pixels.Color(level*(255 - power), 0, level*power); 
  } else if(color < 2*NUMOFCOLORS/3) {
      color -= NUMOFCOLORS/3;
      power=1.0*color/(NUMOFCOLORS/3)*255;
      return pixels.Color(0, level*power, level*(255 - power));
  } else {
     color -= 2*NUMOFCOLORS/3;
     power=1.0*color/(NUMOFCOLORS/3)*255;
     return pixels.Color(level*power, level*(255 - power), 0);
  }
}

uint32_t get_color(byte color_in, byte color_out,float level) {  
    return Wheel(color_in,level) + Wheel(color_out,1-level);
  
}

