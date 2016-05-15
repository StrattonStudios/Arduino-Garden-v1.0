/*
Code Written by Greg Stratton
Thanks to Arduino/Python/Pi/Instructables/BitHub community members who make these awesome libs
Shoutout to Jeremy Blum, Educ8s.com, The Raspberry Pi Guy, go check out their youtube channels!

The Purpose of this code is to monitor a Automated Gardening System
Displaying all information on a 20x4 LCD, Connect Via Rpi, hosting a LAMP server.

THINGS TO DEBUG
1. Add delay to lcd.print on time/date?
3. Do i need LIB rtc_ds3231.h
4. more efficent watering
5. topSensor1 Valve Code 2x
6. accurate temp/hum
*/
///#include <rtc_ds3231.h>
#include <Wire.h>
#include <RTClib.h>
#include "ds3231.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//Time from RTC
RTC_DS1307 rtc;

//Setup of Temp Sensor
#define DHTTYPE DHT11
#define DHTPIN 2

//setup for dergree button
int degreePin = 53;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean celcius = true;

//Setup for Page 2
int pagePin = 52;
boolean lastPage = LOW;
boolean currentPage = LOW;
boolean page = true;
//water timer boolean
int watering[] = {00, 15, 30, 45};

//topSensor1 Valve
int topSensor = A0;
int topSensor_val;
int tankTop;

//topSensor2 Valve
int bottomSensor = A1;
int bottomSensor_val;
int tankBottom;

//temp stuff
float temperatureC, temperatureF, humidity;
float setpoint = 75;
int temp_buffer

//RTC
#define BUFF_MAX 128
uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;
unsigned long prev, interval = 1000;
int hr_val;
int min_val;
int morninglight = (7);
int eveninglight = (19);

//Relay Setup
#define PUMP  6                        
#define FAN  7                        
#define LIGHT  8                        
#define RELAY4  9
int pumpRelay_val;
int fanRelay_val;
int lightRelay_val;

//Lcd ibrary Setup
LiquidCrystal_I2C lcd(0x27,20,4);

//Temp Sensor setup
DHT dht(DHTPIN,DHTTYPE);
//degree symbol
byte degree[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
};
byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
void setup() {

  //Set Relay off
  pinMode(PUMP, LOW);       
  pinMode(FAN, LOW);
  pinMode(LIGHT, LOW);
  pinMode(RELAY4, LOW);
  
  //Serial link
  Serial.begin(9600);
  dht.begin();
  
  
  //create a Wire Object? for rtc
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  memset(recv, 0, BUFF_MAX);
  
  //setting up LCD
  lcd.init();
  lcd.backlight();
   // Print a message to the LCD.
  lcd.print("Initializing Garden");
  
  //Make progress characters
  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);
  {
 lcd.setCursor(0,1);
 lcd.print("                ");   
 
 for (int i = 0; i<20; i++)
 {
   
   for (int j=0; j<5; j++)
   {
      lcd.setCursor(i, 1);   
      lcd.write(j);
      delay(100);
    } 
  }
}

  
  //setting up button
  pinMode(degreePin, INPUT);
  
  //Button temp Bounce
}
boolean debounce(boolean last)
{
  boolean current = digitalRead(degreePin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(degreePin);
  }
  return current;
}
///Page pin debounce

boolean debouncepage(boolean last)
{
  boolean current = digitalRead(pagePin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(pagePin);
  }
  return current;
}

void loop() {
  //Get Time from Real Time Clock
  char in;
  char buff[BUFF_MAX];
  unsigned long now = millis();
  struct ts t;

  // show time once in a while
  if ((now - prev > interval) && (Serial.available() <= 0))
  {
     DS3231_get(&t); //Get time  
  }
     
  // Reading Temp Sensor
  //delay(2500); *fix this or move below?? 
  humidity = dht.readHumidity();
  temperatureC = dht.readTemperature();
  temperatureF = dht.readTemperature(true);
  //topSensor1s to Strings?
  
  char tempF[6]; 
  char humF[6];
  char tempC[6];
  dtostrf(temperatureF, 5, 2, tempF);
  dtostrf(temperatureC, 5, 2, tempC);
  dtostrf(humidity, 2, 2, humF);
  
  //read lvl top
  topSensor_val = analogRead(topSensor);
  Serial.print("topSensor Switch reads");
  Serial.println(topSensor_val);
  if (topSensor_val > 0)
  {
    tankTop = (1);
  }
  else
  {
    tankTop = (0);
  }
  
  //read lvl bottom
  bottomSensor_val = analogRead(bottomSensor);
  Serial.print("bottom Sensor Switch reads");
  Serial.println(bottomSensor_val);
  if (bottomSensor_val > 0)
  {
    tankBottom = (1);
  }
  else
  {
    tankBottom = (0);
  }
  
  //watering timer
  min_val = t.min;
  hr_val = t.hour;
  if (min_val == true)
  {
    pinMode(PUMP, HIGH);
    
  }
  else
  {
    pinMode(PUMP, LOW);
    pumpRelay_val = (0);
  }   
  //Fan Bank
  if (setpoint < temperatureF)
  {
    pinMode(FAN, HIGH);
    fanRelay_val = (1);
  }
  else
  {
    pinMode(FAN, LOW);
    fanRelay_val = (0);
  }
  ///Lights
  hr_val = t.hour;
   if (t.hour > morninglight && t.hour < eveninglight)
  {
    pinMode(LIGHT, HIGH);
    lightRelay_val = (1);
  }
  else 
  {
    pinMode(LIGHT, LOW);
    lightRelay_val = (0);
  }

    
  //Page mode
  currentPage = debouncepage(lastPage);
  if (lastPage == LOW && currentPage == HIGH)
  {
    if (page) page = false;
    else page = true;
  }
  lastPage = currentPage;
  if (page)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Climate");
  lcd.setCursor(0,1);
  lcd.print("Tempature:");
  lcd.setCursor(0,2);
  lcd.print("Humidity:");
  lcd.setCursor(0,3);
  lcd.print("Time:");
  lcd.createChar(0 ,degree);
  
  //Button Mode
   currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH)
  {
    if (celcius) celcius = false;
    else celcius = true;
  }
  lastButton = currentButton;
    
 lcd.setCursor(11,1);
 if (celcius)
 {
   lcd.print(tempC);
   lcd.write((byte)0);
   lcd.print("C");
   //Printing Humidity
   lcd.setCursor(11,2);
   lcd.print(humF);
   lcd.print(" %");
   lcd.setCursor(6,3); //Go to second line of the LCD Screen
   lcd.print("              ");
   lcd.setCursor(6,3);
   
     printHour(t.hour);
     lcd.print(":");
     if(t.min<10)
     {
       lcd.print("0");
     }
     lcd.print(t.min);
     lcd.print(":");
     if(t.sec<10)
     {
       lcd.print("0");
     }
     lcd.print(t.sec);
 }
 else
 {
  // if (tempF == buffer_temp)
   //{
     //return buffer_temp
   }
   
   lcd.print(tempF);
   lcd.write((byte)0);
   lcd.print("F");
 //Printing Humidity
   lcd.setCursor(11,2);
   lcd.print(humF);
   lcd.print(" %");
   lcd.setCursor(0,3);
   lcd.print("Date:");
   lcd.setCursor(6,3);
   lcd.print("              ");
   lcd.setCursor(6,3);
   lcd.print(t.mday);
   printMonth(t.mon);
   lcd.print(t.year+100);
 }

}
  else
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hardware  Tank:");
  lcd.setCursor(15,0);
  if (tankTop == 1)
  {
    lcd.print("     ");
    lcd.setCursor(15,0);
    lcd.print("Low");
  }
   else
   {
     lcd.print("     ");
     lcd.setCursor(15,0);
     lcd.print("Full");
   }
  lcd.setCursor(0,1);
  lcd.print("Pump:");
  if (pumpRelay_val == 1)
  {
    lcd.print("   ");
    lcd.setCursor(5,1);
    lcd.print("ON");
  }
  else
  {
    lcd.print("  ");
    lcd.setCursor(5,1);
    lcd.print("OFF");
  }
  lcd.setCursor(0,2);
  lcd.print("Fans:");
  if (fanRelay_val == 1)
  {
    lcd.print("   ");
    lcd.setCursor(5,2);
    lcd.print("ON");
  }
  else
  {
    lcd.print("  ");
    lcd.setCursor(5,2);
    lcd.print("OFF");
  }
  lcd.setCursor(0,3);
  lcd.print("Lights:");
  if (lightRelay_val == 1)
  {
    lcd.print("   ");
    lcd.setCursor(6,3);
    lcd.print("ON");
  }
  else
  {
    lcd.print("  ");
    lcd.setCursor(6,13);
    lcd.print("OFF");
  }
  }
  {
  delay(1000);
  }
}
///Converision for Months
void printMonth(int month)
{
  switch(month)
  {
    case 1: lcd.print(" January ");break;
    case 2: lcd.print(" February ");break;
    case 3: lcd.print(" March ");break;
    case 4: lcd.print(" April ");break;
    case 5: lcd.print(" May ");break;
    case 6: lcd.print(" June ");break;
    case 7: lcd.print(" July ");break;
    case 8: lcd.print(" August ");break;
    case 9: lcd.print(" September ");break;
    case 10: lcd.print(" October ");break;
    case 11: lcd.print(" November ");break;
    case 12: lcd.print(" December ");break;
    default: lcd.print(" Error ");break;
  } 
}

///Conversion from Military Time
void printHour(int hour)
{
  switch(hour)
  {
    case 0: lcd.print("12");break;
    case 1: lcd.print("1");break;
    case 2: lcd.print("2");break;
    case 3: lcd.print("3");break;
    case 4: lcd.print("4");break;
    case 5: lcd.print("5");break;
    case 6: lcd.print("6");break;
    case 7: lcd.print("7");break;
    case 8: lcd.print("8");break;
    case 9: lcd.print("9");break;
    case 10: lcd.print("10");break;
    case 11: lcd.print("11");break;
    case 12: lcd.print("12");break;
    case 13: lcd.print("1");break;
    case 14: lcd.print("2");break;
    case 15: lcd.print("3");break;
    case 16: lcd.print("4");break;
    case 17: lcd.print("5");break;
    case 18: lcd.print("6");break;
    case 19: lcd.print("7");break;
    case 20: lcd.print("8");break;
    case 21: lcd.print("9");break;
    case 22: lcd.print("10");break;
    case 23: lcd.print("11");break;
    
  } 
}

