#include <Adafruit_NeoPixel.h>

#define PIN 5
#define NUMPIXELS 4 // NeoPixel strip size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.clear();
  
  pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // 빨간색
  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); // 초록색
  pixels.setPixelColor(2, pixels.Color(0, 0, 255)); // 파란색
  pixels.setPixelColor(3, pixels.Color(255, 255, 255)); // 흰색

  // pixels.setPixelColor(3, pixels.Color(123, 2, 89)); // CUSTOM

  pixels.show();
}

void loop() {
}