// SPDX-License-Identifier: BSD-2-Clause
// 2026 Copyright RMT0
#include "ArduinoGraphics.h"
#include <Arduino_LED_Matrix.h>
#include <LiquidCrystal.h>
#include <DHT.h>
void displayCharMatrix(ArduinoLEDMatrix &matrix, const char* text)
{
    matrix.beginDraw();
    matrix.stroke(0xFFFFFFFF);
    matrix.textFont(Font_4x6);
    matrix.beginText(0, 0, 0xFFFFFF); 
    matrix.print(text);               
    matrix.endText();
    matrix.endDraw();
}

void clearMatrix(ArduinoLEDMatrix &matrix)
{
  matrix.beginDraw();
  matrix.clear(); 
  matrix.endDraw();
}
void scrollTextMatrix(ArduinoLEDMatrix &matrix, const char* text, int totalDuration) {
    int textLen = strlen(text) * 5; 
    int displayWidth = 12;           
    int steps = textLen + displayWidth;
    if (steps <= 0) return;

    int delayMs = totalDuration / steps; 

    for (int x = 0; x < steps; x++) {
        matrix.beginDraw();
        
        matrix.stroke(0xFFFFFFFF);
        matrix.textFont(Font_5x7);
        matrix.beginText(displayWidth - x, 0, 0xFFFFFF);
        matrix.print(text);
        matrix.endText();
        matrix.endDraw();
        delay(delayMs);
    }
}

ArduinoLEDMatrix MAT;
DHT dht(12, DHT11);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
void setup() {
  MAT.begin();
  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("2026 (C) RMT0");
  lcd.setCursor(0, 1);
  lcd.print("Weath LCD 0.0.1");
  lcd.write(0xE2);
  displayCharMatrix(MAT, "ON");
  delay(5000);
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  bool iserror = false;
  if (isnan(h) || isnan(t))
  {
      iserror = true;
  }
  lcd.clear();
  clearMatrix(MAT);
  lcd.setCursor(0, 0);
  if (!iserror)
  {
    lcd.print("Wilg. ");
    lcd.print((int)h);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temp. ");
    lcd.print((int)t);
    lcd.write(0xA1);
    lcd.print("C");
    char scrollBuf[16];
    snprintf(scrollBuf, sizeof(scrollBuf), "H:%d%% T:%d°C", (int)h, (int)t);
    scrollTextMatrix(MAT, scrollBuf, 6000); 

  }
  else
  {
      lcd.print("Wilg. blad");
      lcd.setCursor(0, 1);
      lcd.print("Temp. blad");
      displayCharMatrix(MAT,"ERR");
      delay(6000);
  }
}
