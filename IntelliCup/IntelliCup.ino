/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define DHTPIN D10  
#define DHTTYPE DHT11   // DHT 11

#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7b368b1fbffb42a4875ee5563cd381ac";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BNK3R";
char pass[] = "handsomejack";

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(D12, D11, D5, D4, D3, D2);

const int sensorPin= A0; //sensor pin connected to analog pin A0
float liquid_level;
int liquid_percentage;
int top_level = 500;//Maximum water level
int bottom_level = 0;//Minimum water level

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

void myTimerEvent()
{
  float humi = dht.readHumidity();
  float temper = dht.readTemperature();

  if (isnan(humi) || isnan(temper)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.println(temper);
    Blynk.virtualWrite(V1, humi); 
    Blynk.virtualWrite(V2, temper);
  }
}



void setup()
{
  // Debug console
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, myTimerEvent);


  pinMode(sensorPin, INPUT);
  Blynk.virtualWrite(V3, liquid_level);
  Blynk.virtualWrite(V4, liquid_percentage);
  
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // set up the LCD’s number of columns and rows:
  lcd.begin(16, 2);
}

void loop()
{
  float  hum = dht.readHumidity();
  float  temp = dht.readTemperature();

  liquid_level = analogRead(sensorPin);
  liquid_percentage = ((liquid_level-bottom_level)/top_level)*100;//Percentage of water in the container 
  Serial.println(liquid_level);//This will print the liquid level in the monitor 
  Serial.println(liquid_percentage);//This will print the percentage of liquid in the monitor
  Blynk.virtualWrite(V3, liquid_level);
  Blynk.virtualWrite(V4, liquid_percentage);
  //delay(100);
  
  
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  // set the cursor to column 0, line 1
  lcd.print("H:");//print name
  lcd.print(hum, 0);//print name
  lcd.print("%; ");
  lcd.print(" T:");//print name
  lcd.print(temp);//print name
  lcd.print("C");
  lcd.setCursor(0, 1); // set the cursor to column 0, line 2
  lcd.print("Water is ");//print name
  lcd.print(liquid_percentage);
  lcd.print("% full.");
  //delay(250);//delay of 0.75sec
  //lcd.scrollDisplayLeft();//shifting data on LCD
  lcd.setCursor(0, 0);// set the cursor to column 0, line1

  
}

