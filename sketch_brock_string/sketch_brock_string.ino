#include <FastLED.h>

// Const
const int BUTTON_PIN1 = 3; // Arduino pin connected to button's pin
bool button_pressed = false;

// Vars
int buttonState = 0;      

// Define LED
#define NUM_LEDS 6
#define DATA_PIN 2
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 70
#define VOLTS 5
#define MAX_AMPS 500

// Define the array of leds
CRGB leds[NUM_LEDS];

// Function 1
void function1() {
    
    // 1. loop

    leds[5] = CRGB(0,0,255);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[3] = CRGB(0,255,0);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[1] = CRGB(255,0,0);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[3] = CRGB(0,255,0);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[5] = CRGB(0,0,255);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    // 2. loop

    leds[4] = CRGB(0,0,255);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[2] = CRGB(0,255,0);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[0] = CRGB(255,0,0);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[2] = CRGB(0,255,0);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[4] = CRGB(0,0,255);
    FastLED.show();
    delay(5000);
    FastLED.clear();

    leds[4] = CRGB(0,0,0);
    FastLED.show();
    FastLED.clear();
    FastLED.clear();

}

void setup() { 
    FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // GRB ordering is typical
    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS,MAX_AMPS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
    pinMode(BUTTON_PIN1, INPUT_PULLUP); // INPUT or INPUT_PULLUP depending on your hardware
}

void loop() {
    buttonState = digitalRead(BUTTON_PIN1);
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == LOW) {
        function1();
        FastLED.clear();

    } else {
      FastLED.clear();
      };
}
