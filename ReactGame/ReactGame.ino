// React game - a device for training reaction
// Made by Val Kim and D Kim

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Utility.h>
#include "tones.h"

#define GAMETIME 30000 // 30s of lenght time
#define LAMP 9         // num of buttons - max 15
#define RES_KEY 9      // reset button
#define BUZZER_PIN 12  // buzzer
#define BLINK 300      // 300ms blink time

LiquidCrystal_I2C lcd(0x27, 16, 4);
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// default
// char hexaKeys[ROWS][COLS] = {
//  {'<', '8', '4', '0'},
//  {'=', '9', '5', '1'},
//  {'>', ':', '6', '2'},
//  {'?', ';', '7', '3'}
// };

// Keyboard pins
char hexaKeys[ROWS][COLS] = {
    {'0', '4', '8', '<'},
    {'1', '5', '9', '='},
    {'2', '6', ':', '>'},
    {'3', '7', ';', '>'}};
byte colPins[ROWS] = {39, 37, 35, 33};
byte rowPins[COLS] = {41, 43, 45, 47};

// Light pins
boolean lamp_on[15];
boolean steps = true, go_game = true;
int lamp_pin[15] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

long last_pressed = 0;
long nowMillis = 0;
long toSec = 0;
long gameStart = 0;
long toBlink = 0;

int score = 0;
int dlay = 1000;
int butn = 0;
int old_butn = 100; // last pressed button
char customKey;

int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
void (*resetFunc)(void) = 0; // reset function
void setup()
{
  Serial.begin(9600);
  Serial.println("REACT GAME");
  randomSeed(analogRead(0)); // randomizer
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  tone(BUZZER_PIN, 5000, 500);
  foreach (lamp_pin, LAMP, pinMode, OUTPUT)
    ;
  foreach (lamp_pin, LAMP, digitalWrite, HIGH)
    ;
  delay(2000);
  foreach (lamp_pin, LAMP, digitalWrite, LOW)
    ;
  delay(500);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print(" OFTEX");
  delay(500);
  lcd.setCursor(2, 1);
  lcd.print("   REACT GAME");
  delay(500);
  lcd.setCursor(2, 2);
  lcd.print(" START IN 3...");
  tone(BUZZER_PIN, 4000, 200);
  delay(500);
  lcd.setCursor(2, 2);
  lcd.print(" START IN 2...");
  tone(BUZZER_PIN, 4500, 200);
  delay(500);
  lcd.setCursor(2, 2);
  lcd.print(" START IN 1...");
  tone(BUZZER_PIN, 5000, 200);
  delay(500);
  lcd.setCursor(1, 1);
  lcd.print("   GO! GO! GO!    ");
  lcd.setCursor(1, 2);
  lcd.print("   GO! GO! GO!    ");
  for (int i = 0; i < 5; i++)
  {
    int frequency = 4000 + i * 300;
    tone(BUZZER_PIN, frequency, 150);
    delay(130);
  }
  gameStart = millis();
  nowMillis = gameStart;
  score = 0;
  steps = true;
}

void loop()
{
  while ((nowMillis - gameStart < GAMETIME) and (go_game))
  {
    nowMillis = millis();
    if (nowMillis - toSec > 1000)
    {
      lcd_print();
      toSec = nowMillis;
    }
    if (nowMillis - toBlink > BLINK)
    {
      if (lamp_on[old_butn])
      {
        sw_led_off(old_butn);
      }
      else
      {
        sw_led_on(old_butn);
      }
      toBlink = nowMillis;
    }
    if (steps)
    {
      while (old_butn == butn)
      {
        butn = random(LAMP);
      }
      sw_led_off(old_butn);
      old_butn = butn;
      Serial.print("Butn: ");
      Serial.println(butn);
      sw_led_on(butn);
      steps = false;
    }
    customKey = customKeypad.getKey();
    if (customKey)
      Serial.println((int)customKey);
    if (customKey - 48 == butn)
    {
      sw_led_off(customKey - 48);
      score++;
      steps = true;
      lcd_print();
    }
    else if (customKey - 48 == RES_KEY)
    {
      gameStart = millis();
      foreach (lamp_pin, LAMP, digitalWrite, LOW)
        ;
      score = 0;
      Serial.println("RESTART!");
      lcd.clear();
      lcd.setCursor(6, 1);
      lcd.print("RESTART!");
      delay(500);
      resetFunc();
    }
    else
    { // option to set wrong button status
      // lcd.setCursor(8, 1);
      // lcd.print(" WRONG!");
    }
  }
  if (go_game)
    game_over();
  go_game = false;
  customKey = customKeypad.getKey();
  if (customKey - 48 == RES_KEY)
  {
    go_game = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RESTART!");
    delay(500);
    resetFunc();
  }
}

void lcd_print()
{
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("TIME:");
  int time = GAMETIME / 1000 - (nowMillis - gameStart) / 1000;
  lcd.print(time);
  if (time < 11)
  {
    tone(BUZZER_PIN, 3500 + time * 300, 150);
  }
  lcd.setCursor(12, 1);
  lcd.print("BTN:");
  lcd.print(customKey);
  lcd.setCursor(2, 2);
  lcd.print("SCORE:");
  lcd.print(score);
}

void game_over()
{
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print("    GAME OVER");
  lcd.setCursor(4, 2);
  lcd.print(" SCORE: ");
  lcd.print(score);
  foreach (lamp_pin, LAMP, digitalWrite, HIGH)
    ;
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}

void sw_led_on(int num)
{
  digitalWrite(lamp_pin[num], 1);
  lamp_on[num] = 1;
}

void sw_led_off(int num)
{
  digitalWrite(lamp_pin[num], 0);
  lamp_on[num] = 0;
}