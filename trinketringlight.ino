#include <Adafruit_NeoPixel.h>

#define N_PIXELS  24  // Number of pixels you are using
#define LED_PIN    0  // NeoPixel LED strand is connected to GPIO #0 / D0
#define BUTTON_PIN 2  // Mode change button is connected to GPIO #1

Adafruit_NeoPixel  strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

int stripMode = 0;
const int MAXSTRIPMODE = 8;
uint32_t colormodes[MAXSTRIPMODE] = {
  Adafruit_NeoPixel::Color(255, 255, 255), // white
  Adafruit_NeoPixel::Color(255, 200, 128), //warm white
  Adafruit_NeoPixel::Color(255, 0,   0),   // red
  Adafruit_NeoPixel::Color(0,   255, 0),   // green
  Adafruit_NeoPixel::Color(0,   0,   255), // blue
  Adafruit_NeoPixel::Color(255, 255, 0),   // yellow
  Adafruit_NeoPixel::Color(0,   255, 255), // cyan
  Adafruit_NeoPixel::Color(255, 0,   255)}; // magenta
              
bool changeColor = false;    // Whether we want to change the color
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin (pulled up by default)

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers              


void setup() {
  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Save money, use the resistors we have!
  strip.begin();

  // We only have so much amperage, limit brightness to 100
  strip.setBrightness(100);                // Set LED brightness 0-255
  colorWipe(strip.Color(255, 255, 255),0); // fill the strip with all white
  strip.show();                            // Update strip
}

void loop() {
  // read the state of the button into a local variable:
  int buttonVal = digitalRead(BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (buttonVal != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the buttonVal is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (buttonVal != buttonState) {
      buttonState = buttonVal;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        changeColor = true;
      }
    }
  }
  
  if (changeColor) // Button pressed
  {
    // increment display color
    stripMode++;
    if (stripMode == MAXSTRIPMODE)
    {
      stripMode=0;
    }
    colorWipe(colormodes[stripMode],0);
    changeColor = false;
  }

  lastButtonState = buttonVal;
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}
