#include <LiquidCrystal_I2C.h>

#include <CircularBuffer.h>
#include <MAX30100.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_PulseOximeter.h>
#include <MAX30100_Registers.h>
#include <MAX30100_SpO2Calculator.h>



#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 20, 4);
 
#define REPORTING_PERIOD_MS     500
 
PulseOximeter pox;
uint32_t tsLastReport = 0;

const int sensor = A1;
float tempc;
float tempf;
float vout;

 int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, Tm, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup()
{
  pinMode(sensor,INPUT);
    lcd.begin(20,4);
 
    // Turn on the blacklight and print a message.
    lcd.backlight();
    
    lcd.setCursor(0,0);
    lcd.print("Initializing...");
    delay(1000);
    lcd.setCursor(0,1);
    if (!pox.begin()) {
        lcd.print("FAILED");
        while(1);
    } else {
        lcd.print("SUCCESS");
    }
 
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    
}
 
void loop()
{
    pox.update();
 
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        lcd.clear();
        lcd.setCursor(0,1);
    
        lcd.print("HR: ");
        lcd.print(pox.getHeartRate());
        lcd.print(" bpm");
 
        lcd.setCursor(0,2);
    
        lcd.print("SpO2: ");
        lcd.print(pox.getSpO2());
        lcd.print("%");
 
        tsLastReport = millis();
    }


     vout=analogRead(sensor);
  vout=(((vout*5.0)/1024)*10);
  tempc=vout;
  tempf=(vout*1.8)+32;
 
  pox.update();

  


   Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  Tm = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = Tm - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0;

  lcd.setCursor(0,3);
  lcd.print("Body Temp. F= ");
  lcd.print(Tf);
  
    lcd.setCursor(0,0);
  lcd.print("Degree C= ");
  lcd.print(tempc);
 // lcd.setCursor(0,2);
  //lcd.print("Farenheit F= ");
  //lcd.print(tempf);
  delay(50);
   

  
  
}
