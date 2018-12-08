
#include "DHT.h"///dht11
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <Wire.h>;
#include "RTClib.h"///thoigianthuc
#include <LiquidCrystal_I2C.h>;///i2c

#include "HX711.h"///adc24bit hx711 hotro loadcell

#define Pump1                   5             
#define Pump2                   8
#define Coolingfan              9
#define Heatinglamp             10
#define Ledlight                11
#define Feeder                  12
#define Gasdetect               13
#define LOADCELL_DOUT           3
#define LOADCELL_CLK            2
#define DHTTYPE               DHT11
#define DHTPIN                  4
#define GAS_MQ7                 A0


RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String buff[9];
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
String tempSet,timerSetHour, timerSetMin;
uint8_t checkHour, checkMin;
uint8_t firstWeight, LastWeight, remainWeight;
boolean startSending = false;
boolean updateStateFlag = false;
boolean setTempThreholdFlag = false;
boolean setArlamTimemerFlag = false;
boolean feederFlag = false;
DHT dht(DHTPIN,DHTTYPE);
HX711 scale(LOADCELL_DOUT, LOADCELL_CLK);
uint8_t gasValue;
float grams;
SoftwareSerial ser(6,7);
LiquidCrystal_I2C lcd(0x3F,16,2);
String inputString; 
//boolean stringComplete = false; 
  // Temperature and Humidity
  
void setup() {
  Serial.begin(115200);              //Starting serial communication
  dht.begin();
  lcd.init();      
  lcd.backlight();
  ser.begin(115200);
  lcd.createChar(5, check);
  // Fire Sensor Config

  pinMode(13, OUTPUT);
   // Init Relay Pin 
  pinMode(Pump1, OUTPUT);
  pinMode(Pump2, OUTPUT);
  pinMode(Coolingfan, OUTPUT);
  pinMode(Heatinglamp, OUTPUT);
  pinMode(Ledlight, OUTPUT);
  pinMode(Gasdetect, OUTPUT);
  // Init state of RELAY
  digitalWrite(Pump1, HIGH);
  digitalWrite(Pump2, HIGH);
  digitalWrite(Coolingfan, HIGH);
  digitalWrite(Heatinglamp, HIGH);
  digitalWrite(Ledlight, HIGH);
  
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
    
   scale.set_scale();
   scale.tare();  //Reset the scale to 0 
   lcd.setCursor(0,0);
   lcd.print("-LE THANH TRONG-");
   lcd.setCursor(0,1);
   lcd.print("DATN-SMART FARM-");
   delay(3000);
    lcd.clear(); 
}
  
void loop() {
     
  //Adjust to this calibration factor
  scale.set_scale(-376.0);
  // Read an average of X readings
  grams = scale.get_units(5);
  // Temperature and Humidity
  float Temperature = dht.readTemperature();
  float Humidity = dht.readHumidity();
  gasValue = analogRead(GAS_MQ7);
  Serial.println(gasValue);
  String detectGas = "NO GAS FOUND";
  if(gasValue > 170) 
    {
       detectGas = "PHAT HIEN KHI C0";
       digitalWrite(Coolingfan, LOW); 
       digitalWrite(Gasdetect, HIGH);
       Serial.println(detectGas);    
     }
      else if(setTempThreholdFlag == false && gasValue < 170)
      {       
        detectGas = "KHONG PHAT HIEN KHI C0";
        digitalWrite(Coolingfan, HIGH); 
        digitalWrite(Gasdetect, LOW);
        Serial.println(detectGas); 
        }
     // Sensor sending value
    buff[0] = "temp=" + String(Temperature);
    buff[1] = "humi=" + String(Humidity);
//    buff[2] = "gas =" + detectGas;
    buff[3] = "food=" + String(firstWeight)+ "/" + String(LastWeight) + ">>" + String(remainWeight);

  lcd.setCursor(7,1);
  lcd.print("FW:");
  lcd.print(grams);
  lcd.print("g");
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
    lcd.print('|');
    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print((uint8_t)Temperature);
    lcd.print((char)223);
    lcd.print("C");
    lcd.print("|");
    
  while(ser.available()) 
  { 
    dataReceiveFromRaspberry();  
  }
  sendStringDataToRaspberry();
  setThreholdTemperature();
  setArlamTimer();
  delay(2000);
}

void sendStringDataToRaspberry()
{
    ser.println(buff[0]);
    Serial.println(buff[0]);
//    delay(50);
    
    ser.println(buff[1]);
    Serial.println(buff[1]);
//    delay(50);
  
    ser.println(buff[2]);
    Serial.println(buff[2]);
//    delay(50);
  
    if(feederFlag == true)
    {
      ser.println(buff[3]);
      Serial.println(buff[3]);
      delay(50);
      }

//    ser.println(buff[4]);
//    Serial.println(buff[4]);
////    delay(50);
}

void dataReceiveFromRaspberry()
{
  lcd.setCursor(14,0);
  lcd.print("->");
  inputString = ser.readStringUntil('\r'); 
 
   // Temp threhold detect value
  if(inputString.substring(0,8) == "threhold")
  {
    tempSet = inputString.substring(8,10);
    Serial.println("detect value" + tempSet);  
    setTempThreholdFlag = true;
  }
  
   // Timer detect value
  if(inputString.substring(0,8) == "timerSet")
  {
    timerSetHour = inputString.substring(8,10);
    timerSetMin = inputString.substring(11,13); 
    Serial.println("detect value" + timerSetHour);
    Serial.println("detect value" + timerSetMin); 
    setArlamTimemerFlag = true;
  }
    // Relay 1 -
//    if(inputString.substring(0,2) == "AA")
      if(inputString.substring(0,2) == "A1")
       { 
          digitalWrite(Pump1, LOW);   
          lcd.setCursor(8,1);   
          lcd.print("1N");  
       } 
      if(inputString.substring(0,2) == "A0")
        {
          digitalWrite(Pump1, HIGH); 
          lcd.setCursor(8,1);   
          lcd.print("1F"); 
//          ser.println("Turned Off"); 
        }
    // Relay 2 -
//    if(inputString.substring(0,7) == "Relay 2")
      if(inputString.substring(0,2) == "B1")
       { 
          digitalWrite(Coolingfan, LOW); 
          lcd.setCursor(10,1);   
          lcd.print("2N");    
//          ser.println("Turned On"); 
       } 
      if(inputString.substring(0,2) == "B0")
        {
          digitalWrite(Coolingfan, HIGH);  
          lcd.setCursor(10,1);   
          lcd.print("2F"); 
//          ser.println("Turned Off Led");
        }


     // Relay 3 -
//    if(inputString.substring(0,7) == "Relay 3")
//    {
      if(inputString.substring(0,2) == "C1")
       { 
          digitalWrite(Heatinglamp, LOW);  
          lcd.setCursor(12,1);   
          lcd.print("3N");     
//          ser.println("Turned On");  
       } 
      if(inputString.substring(0,2) == "C0")
        {
          digitalWrite(Heatinglamp, HIGH); 
          lcd.setCursor(12,1);   
          lcd.print("3F"); 
//          ser.println("Turned Off Led"); 
        }

     // Relay 4 -
//    if(inputString.substring(0,7) == "Relay 4")
//    {
      if(inputString.substring(0,2) == "D1")
       { 
          digitalWrite(Ledlight, LOW);  
          lcd.setCursor(14,1);   
          lcd.print("4N");      
//          ser.println("Turned On");  
       } 
      if(inputString.substring(0,2) == "D0")
        {
          digitalWrite(Ledlight, HIGH);
          lcd.setCursor(14,1);   
          lcd.print("4F");   
//          ser.println("Turned Off Led"); 
        }  

       if(inputString.substring(0,2) == "E1")
       { 
          digitalWrite(Pump2, LOW);  
//          lcd.setCursor(14,1);   
//          lcd.print("5N");      
//          ser.println("Turned On");  
       } 
      if(inputString.substring(0,2) == "E0")
        {
          digitalWrite(Pump2, HIGH);
//          lcd.setCursor(14,1);   
//          lcd.print("5F");   
//          ser.println("Turned Off Led"); 
        }  
}

void setThreholdTemperature()
{
  if(setTempThreholdFlag == true)
  {
    float temperature = dht.readTemperature();
    float tempThrehold = 30;
    tempThrehold = tempSet.toFloat();
    Serial.println(tempThrehold);
    if(temperature > tempThrehold)
    {
      lcd.setCursor(0,1);
      lcd.print("DOWN--");
      
      Serial.println("Turned on Relay 2 - Fan");
      digitalWrite(Coolingfan, LOW);
      digitalWrite(Heatinglamp, HIGH);
      }

     if(temperature < tempThrehold)
     {
      Serial.println("Turned on Relay 4 - Heater Light ");
      digitalWrite(Heatinglamp, LOW);
      digitalWrite(Coolingfan, HIGH);
      lcd.setCursor(0,1);
      lcd.print("Up++++");
      }

     if(temperature == tempThrehold)
     {
      lcd.setCursor(0,1);
      lcd.print("DONE.");
      Serial.println("Setting on Threhold complete!");
      setTempThreholdFlag = false;
      digitalWrite(Heatinglamp, HIGH);
      digitalWrite(Coolingfan, HIGH);
      Serial.println(setTempThreholdFlag);
//      lcd.setCursor(10,1);
//      lcd.print(String(setTempThreholdFlag));
      }
    }
  }

void setArlamTimer()
{
   DateTime Now = rtc.now();
   uint8_t compareHour = Now.hour();
   uint8_t compareMin = Now.minute();
   uint8_t compareSec = Now.second();
   checkHour = timerSetHour.toInt();
   checkMin = timerSetMin.toInt();
   uint8_t turnOffFlagMin = checkMin + 5;
   if(setArlamTimemerFlag == false && feederFlag == true && compareMin == turnOffFlagMin)
  {   
    feederFlag = false;
  }
  if(setArlamTimemerFlag == true)
  {
    lcd.setCursor(9, 0);
    lcd.print(checkHour);
    lcd.print(":");
    lcd.setCursor(12, 0);
    lcd.print(checkMin);
    Serial.println(compareHour);
    Serial.println(compareMin);    
     if(compareHour == checkHour && compareMin == checkMin)
      {
        Serial.println("Turned on Feeder ");
        digitalWrite(Feeder, HIGH);
//        lcd.setCursor(9, 1);
//        lcd.print("OK");
        setArlamTimemerFlag = false;
        if(compareSec < 15)
        {
          firstWeight = scale.get_units(5);
          Serial.println(firstWeight);
        }
       }
  }
  uint8_t increaseMin = checkMin +1;
  if(setArlamTimemerFlag == false && compareMin == increaseMin)
  {
    digitalWrite(Feeder, LOW);
    lcd.setCursor(9, 0);
    lcd.print("DONE--");
    if(compareSec < 15)
        {
          LastWeight = scale.get_units(5);
          Serial.println(LastWeight);
          }
    remainWeight = firstWeight - LastWeight;
    feederFlag = true;
  }
}
