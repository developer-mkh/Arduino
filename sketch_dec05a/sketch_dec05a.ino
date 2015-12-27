#include <Arduino.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
DHT dht(DHTPIN, DHTTYPE);

 
 
void setup(){
   lcd.begin(16, 2);               // start the library
   dht.begin();
}
  
void loop(){
   lcd.setCursor(0,0);             // move to the begining of the second line

  delay(3000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    lcd.print("Failed to read from DHT sensor!");
    return;
  }

  lcd.print("H: "); 
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0,1);
  lcd.print("T: "); 
  lcd.print(t);
  lcd.print(" *C ");
}
