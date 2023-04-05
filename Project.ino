#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11);
// BT module TX to pin 10
// BT module RX to pin 11
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <EEPROM.h>
#define trigpin 3
#define echopin 2
#define buzzer 9
#define NUM_LEDS 8
#define BRIGHTNESS 40
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN);
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
uint32_t none = strip.Color(0, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t orange = strip.Color(255, 102, 0);
uint32_t red = strip.Color(255, 0, 0);
String command;
unsigned long start;
unsigned long current;

void setup()
{
  BT.begin(9600);
  Serial.begin(9600);
  strip.setBrightness(BRIGHTNESS);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(buzzer, OUTPUT);
  strip.begin();
  strip.show();
  BT.println("Welcome! Enter your command:");
  start = millis();
}

void serialEvent() {
  if (BT.available()) {
    command = BT.readStringUntil('\n');
    command.trim();  // remove any \r \n whitespace at the end of the String
    if (command.equals("Park")) {
      if (distance >= 12) {
        BT.print("Parking spot available. Distance to nearest obstacle: ");
        BT.print(distance);
        BT.println(" cm");
      }
      else {
        BT.print("Can't park here. Obstacle too close at: ");
        BT.print(distance);
        BT.println(" cm");
      }
    }
    else if (command.equals("Distance")) {
      BT.print(distance);
      BT.println(" cm");
    }
    BT.println("Enter your command:");
  }
}

void loop()
{
  delay(100);
  // Clears the trigPin condition
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
   duration = pulseIn(echopin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  serialEvent();
  if (distance >= 12) {
    strip.setPixelColor(0, green);
    strip.fill(none, 1, NUM_LEDS - 1);
    strip.show();
  }
  else if (distance >= 9) {
    strip.fill(green, 0, 2);
    strip.fill(none, 2, NUM_LEDS - 2);
    strip.show();
    bzzr(2000);   //starts buzzer with specified time interval between beeps
  }
  else if (distance >= 7) {
    strip.fill(green, 0, 3);
    strip.fill(none, 3, NUM_LEDS - 3);
    strip.show();
    bzzr(1000);
  }
  else if (distance >= 6) {
    strip.fill(green, 0, 3);
    strip.setPixelColor(3, orange);
    strip.fill(none, 4, NUM_LEDS - 4);
    strip.show();
    bzzr(700);
  }
  else if (distance >= 5) {
    strip.fill(green, 0, 3);
    strip.fill(orange, 3, 2);
    strip.fill(none, 5, NUM_LEDS - 5);
    strip.show();
    bzzr(500);
  }
  else if (distance >= 4) {
    strip.fill(green, 0, 3);
    strip.fill(orange, 3, 3);
    strip.fill(none, 6, NUM_LEDS - 6);
    strip.show();
    bzzr(400);
  }
  else if (distance >= 3) {
    strip.fill(green, 0, 3);
    strip.fill(orange, 3, 3);
    strip.setPixelColor(6, red);
    strip.setPixelColor(7, none);
    strip.show();
    bzzr(200);
  }
  else if (distance >= 0) {
    strip.fill(green, 0, 3);
    strip.fill(orange, 3, 3);
    strip.fill(red, 6, 2);
    strip.show();
    bzzr(100);
  }
}

void bzzr(int speed) {
  analogWrite(buzzer, 0);
  current = millis();
  if (current - start >= speed) {
    start = current;
    analogWrite(buzzer, 10);
  }
}
