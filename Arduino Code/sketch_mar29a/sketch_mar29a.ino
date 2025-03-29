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
	strip.setPixelColor(remap(1,5),100,0,0);
	strip.setPixelColor(remap(2,5),100,0,0);
	strip.setPixelColor(remap(3,5),100,0,0);
	strip.setPixelColor(remap(4,5),100,0,0);
	strip.setPixelColor(remap(5,5),100,0,0);

  strip.show();
  
  delay(200);
}

// check disatance from end of row
// use distance as a measure how down to go

uint16_t remap(uint16_t x, uint16_t y) {
    if (x < 6) {
      if(x%2){
        //Odd rows, They go up in counting
        return (x-1)*40 + (y*2)-1;
      }
      else{
        //Posotive rows, They go down in counting
        return x*40 - (y*2);

      }
    } else {
    }
}


void drawL(int start){
  strip.setPixelColor(start,100,0,0);
  strip.setPixelColor(start+1,100,0,0);
  strip.setPixelColor(start+2,100,0,0);
  strip.setPixelColor(start+3,100,0,0);

}