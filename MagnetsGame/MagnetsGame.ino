// Magnet game - set of # electromagnets that will deactivate in a random order and will release a magnetized content. This will train reaction time and agility.
// Made by DKim with a help from Michal Valkoun (git/michalvalkoun)

//// Constants: buttons, pins and LED
const int BUTTON_PIN = 11; // Arduino pin connected to button's pin
const int ledPin = 10;     // Arduino pin connected to LED's pin
#define NUM_OF_PINS 8      // number of used pins
#define LED_DELAY 2000     // delay between switching off leds

const uint8_t output_pins[NUM_OF_PINS] = {2, 3, 4, 5, 6, 7, 8, 9}; // Put your pins here

bool button_pressed = false;

//// Array
struct track_of_usage
{
  uint8_t num;
  bool used;
  track_of_usage(uint8_t num) : num(num), used(false) {}
};
//// Buzzer
#include "pitches.h"
// notes in the melody:
int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
// Note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};
//// Setup
void setup()
{                     // initialize digital pin 13 as an output.
  Serial.begin(9600); // initialize serial
  for (int i : output_pins)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP); // INPUT or INPUT_PULLUP depending on your hardware
  pinMode(ledPin, OUTPUT);           // set arduino pin to output mode
  digitalWrite(ledPin, HIGH);
}
//// Randomizer
void random_pin_switch()
{
  track_of_usage data[NUM_OF_PINS] = {0, 1, 2, 3, 4, 5, 6, 7};
  for (int i = 0; i < NUM_OF_PINS; i++)
  {
    randomSeed(millis());
    uint8_t random_pin = random(NUM_OF_PINS);
    while (data[random_pin].used)
      random_pin = random(NUM_OF_PINS);

    data[random_pin].used = true;
    digitalWrite(output_pins[random_pin], LOW);
    Serial.println(output_pins[random_pin]);
    delay(LED_DELAY);
  }
}

void loop()
{
  if (!button_pressed && !digitalRead(BUTTON_PIN))
    button_pressed = true;

  if (button_pressed)
  {
    digitalWrite(ledPin, LOW);
    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {
      // to calculate the note duration, take one second divided by the note type.
      // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(12, melody[thisNote], noteDuration);
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(12);
    };
    delay(1000);

    Serial.println("------------");
    random_pin_switch();

    for (int i : output_pins)
      digitalWrite(i, HIGH);
    digitalWrite(ledPin, HIGH);
    button_pressed = false;
  }
}
