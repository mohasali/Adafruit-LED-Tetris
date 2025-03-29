#include <Adafruit_NeoPixel.h>

#define PIN            12  // Define the pin to which the NeoPixel data line is connected
#define NUMPIXELS      320 // Define the number of pixels in the NeoPixel chain

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int r = 235;
int g = 52;
int b = 52;

// 0 - 239 is first 6 strips
// 240 - 319 is last 4

void setup() {
  strip.begin();  // Initialize the NeoPixel strip
  strip.show();   // Initialize all pixels to 'off'
}

void loop(){
    strip.setPixelColor(39,100,0,0);
    strip.setPixelColor(38,100,0,0);
    strip.setPixelColor(37,100,0,0);
    strip.setPixelColor(36,100,0,0);
    strip.setPixelColor(43,100,0,0);


  strip.show();
  delay(200);
}
/*
void loop() {
  for(int i = 0; i < NUMPIXELS ;i++){
      strip.setPixelColor(i,g,r,b);
  }
  setColors();
  strip.show();
  delay(20);
}*/

void setColors(){
  if(r==235 && g < 235 && b==52 ){
    //initial phase increase green
    g+=3;
  }
  else if(g==235 && r > 52 && b==52 ){
    // 2nd phase decrease red
    r-=3;
  }
  else if(g==235 && b < 235 && r==52){
    // 3rd phase increase blue
    b+=3;
  }
  else if(b==235 && g > 52 && r==52){
    // 4th phase decrease green
    g-=3;
  }
  else if(b==235 && r < 235 && g==52){
    // 5th phase increase red
    r+=3;
  }
  else{
    // decrease blue
    b-=3;
  }
}