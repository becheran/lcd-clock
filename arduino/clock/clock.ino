/*
  LCD Clock
*/

#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

const int DISPLAY_LED_PIN = 10;
const int ROTARY_CLK_PIN = 7;
const int ROTARY_DT_PIN = 8;
const int ROTARY_BUTTON_PIN = 2;
const int BUZZER_PIN = 9;
const int LED_RED_PIN = A0;
const int LED_GREEN_PIN = A1;
const int LED_BLUE_PIN = A2;

int ROTARY_STEPS[] = {0, 1, 2, 3, 5, 8, 10, 15, 20, 25, 45, 75, 128, 255};
int ROTARY_STEPS_MAX = sizeof(ROTARY_STEPS) / sizeof(int);
LiquidCrystal Lcd(12, 11, 6, 5, 4, 3);

int ClkLast = 0;
int BrightnessStep = 0;
unsigned long LastMinutes = 0;

void setup()
{
  pinMode(ROTARY_CLK_PIN, INPUT);
  pinMode(ROTARY_DT_PIN, INPUT);
  pinMode(ROTARY_BUTTON_PIN, INPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  digitalWrite(ROTARY_CLK_PIN, true);
  digitalWrite(ROTARY_DT_PIN, true);
  digitalWrite(ROTARY_BUTTON_PIN, true);
  digitalWrite(LED_RED_PIN, false);
  digitalWrite(LED_GREEN_PIN, false);
  digitalWrite(LED_BLUE_PIN, false);

  ClkLast = digitalRead(ROTARY_CLK_PIN);
  Lcd.begin(16, 2);
  Lcd.print("Custom text!");
  setSyncProvider(RTC.get);
  analogWrite(DISPLAY_LED_PIN, ROTARY_STEPS[BrightnessStep]);
  attachInterrupt(digitalPinToInterrupt(ROTARY_BUTTON_PIN), buttonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(ROTARY_BUTTON_PIN), buttonRelease, RISING);
  // TODO for debug mode enable serial line
  //Serial.begin(115200);
  Serial.println("Finished Init");
  printTime();
}

void buttonPress()
{
  Serial.println("Button press!");
}

void buttonRelease()
{
  Serial.println("Button release!");
}

void printTime()
{
  noInterrupts();
  char s[25];
  sprintf(s, "%02d:%02d", hour(), minute());
  Lcd.setCursor(0, 1);
  Lcd.print(s);
  interrupts();
}

void loop()
{
  unsigned long min = minute();
  if (minute() != LastMinutes)
  {
    printTime();
    LastMinutes = min;
  }
  int clk = digitalRead(ROTARY_CLK_PIN);
  if (clk != ClkLast)
  {
    noInterrupts();
    if (digitalRead(ROTARY_DT_PIN) != clk)
    {
      BrightnessStep++;
      BrightnessStep = min(BrightnessStep, ROTARY_STEPS_MAX - 1);
    }
    else
    {
      BrightnessStep--;
      BrightnessStep = max(BrightnessStep, 0);
    }
    analogWrite(DISPLAY_LED_PIN, ROTARY_STEPS[BrightnessStep]);
    interrupts();
    Serial.println(BrightnessStep);
  }
  ClkLast = clk;
}
