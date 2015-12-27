#include <adk.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
USB Usb;
DHT dht(DHTPIN, DHTTYPE);
ADK adk(&Usb, "developer.mkh", // Manufacturer Name
              "DHT", // Model Name
              "Record temperature and humidity.", // Description (user-visible string)
              "1.0", // Version
              "http://www.dummy.com", // URL (web page to visit if no installed apps support the accessory)
              "000000001"); // Serial Number (optional)
bool connected;

void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);               // start the library
  dht.begin();

  if (Usb.Init() == -1) {
    Serial.print("\r\nOSCOKIRQ failed to assert");
    while (1); // halt
  }

  Serial.println("Start");
}

void loop() {
  Usb.Task();
  lcd.setCursor(0,0);             // move to the begining of the second line

  if (adk.isReady()) {
    if (!connected) {
      connected = true;
      Serial.print(F("\r\nConnected to accessory"));
    }
  } else {
    if (connected) {
      connected = false;
      Serial.print(F("\r\nDisconnected from accessory"));
    }
  }
  if (connected) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    lcd.print("           ");
    lcd.setCursor(0,0); 
    lcd.print(t);
    lcd.print(",");
    lcd.print(h);
    
    sendData(t, h);
    
    delay(60000UL);
  }
}

// t:温度
// h:湿度
void sendData(float t, float h) {
  char buffer[5];
  String sendDataStr;
  sendDataStr = String(dtostrf(t, 5, 2, buffer)) +  "," + String(dtostrf(h, 5, 2, buffer));

  char sendData[11];
  sendDataStr.toCharArray(sendData, sendDataStr.length());

  uint16_t len = sizeof(sendData);
  adk.SndData(len, (uint8_t*)sendData);
}
