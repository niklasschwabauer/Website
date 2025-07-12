#include <Wire.h>
#include <ESP8266WiFi.h>  //Esp8266

#include <SoftwareSerial.h>  //Bluetooth
SoftwareSerial btSerial(D5, D6);

#include <Adafruit_ADS1X15.h>  //Ads1115
Adafruit_ADS1115 ads;
#define ADS1115_I2C_addr0 0x48

uint16_t stateA, stateB, adc2, adc3;
float volts0, volts1, volts2, volts3;


#define thresholdHighA 15000  // Schmitt - Trigger
#define thresholdLowA 5000
#define thresholdHighB 15000
#define thresholdLowB 5000
bool currentStateA = LOW;
bool currentStateB = LOW;


int position = 0;  // Quadratur - Encoder
int lastStateA = 0;

int rotation = 0;

int i;

void setup(void) {
  //Serial.begin(115200);  // Serieller Monitor

  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(9600);  //Bluetooth

  if (!ads.begin(ADS1115_I2C_addr0)) {  //Ads1115
    Serial.println("Finde keinen ADS1115");
    while (true)
      ;
  }
  ads.setGain(GAIN_ONE);
}

void loop(void) {
  stateA = ads.readADC_SingleEnded(0);
  stateB = ads.readADC_SingleEnded(1);

  if (stateA > thresholdHighA && currentStateA == LOW) {  // Schmitt - Trigger
    currentStateA = HIGH;
    //rotation++;
  } else if (stateA < thresholdLowA && currentStateA == HIGH)
    currentStateA = LOW;
  if (stateB > thresholdHighB && currentStateB == LOW) currentStateB = HIGH;
  else if (stateB < thresholdLowB && currentStateB == HIGH) currentStateB = LOW;


  // Wenn A von LOW zu HIGH wechselt:
  if (currentStateA != lastStateA) {
    if (currentStateA == HIGH) {
      // Prüfe B um die Richtung zu bestimmen
      if (currentStateB == LOW) {
        position++;  // Uhrzeigersinn
      } else {
        position--;  // Gegen den Uhrzeigersinn
      }
    } else {
      // Wenn A von HIGH zu LOW wechselt:
      if (currentStateB == HIGH) {
        position++;  // Uhrzeigersinn
      } else {
        position--;  // Gegen den Uhrzeigersinn
      }
    }
    lastStateA = currentStateA;  // Aktualisiere den letzten Zustand von A
  }
  //i++;
  //Serial.print("stateA:");
  //Serial.print(stateA);
  //Serial.print(",");
  //Serial.print("stateB:");
  //Serial.print(stateB);
  //Serial.print(",");
  //Serial.print("rotation:");
  Serial.println(position*25);
  //Serial.println(i);
  //Serial.println(rotation);
  //Serial.println(position);
}