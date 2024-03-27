#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include<LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

#define in 2
#define out 3
#define relay 4
#define DHTPIN 7
#define DHTTYPE    DHT11     // DHT 11
const int trigpin = 6;
const int echopin = 5;

DHT_Unified dht(DHTPIN, DHTTYPE);

int l1 = 16;
int l2 = 15;
int l3 = 14;
int count = 0;
long duration;
int distance;

void IN()
{
  count++;
  lcd.clear();
  lcd.print("Person In Room:");
  lcd.setCursor(15, 0);
  lcd.print(count);
  delay(2000);
}

void OUT()
{
  count--;
  lcd.clear();
  lcd.print("Person In Room:");
  lcd.setCursor(15, 0);
  lcd.print(count);
  delay(2000);
}

void setup()
{
  lcd.begin(16, 2); //lcd turn on
  dht.begin(); // dht sensor on

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  lcd.setCursor(5, 1);
  lcd.print("Smart Room");
  delay(2000);

  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  lcd.clear();
  lcd.print("Person In Room:");
  lcd.setCursor(15, 0);
  lcd.print(count);

}

void loop()
{
  { //dht sensor
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));  //arduino monitor output when dht sensor error
    }
    else {
      lcd.setCursor(0, 2);
      lcd.print(F("T:"));
      lcd.setCursor(2, 2);
      lcd.print(event.temperature);
      lcd.setCursor(7, 2);
      lcd.println(F("C  "));
    }
    {
      // Get humidity event and print its value.
      dht.humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Serial.println(F("Error reading humidity!"));  //arduino monitor output when dht sensor error
      }
      else {
        lcd.setCursor(10, 2);
        lcd.print(F("H:"));
        lcd.setCursor(12, 2);
        lcd.print(event.relative_humidity);
        lcd.setCursor(16, 2);
        lcd.println(F("%   "));
      }
      {
        if (event.temperature > 30) //ac controller
        {
          digitalWrite(l2, HIGH);
        }
        else
        {
          digitalWrite(l2, LOW);
        }
      }
      // IR sensor Count
      if (digitalRead(in))
        IN();
      if (digitalRead(out))
        OUT();

      //IR SENSOR
      if (count <= 0)
      {
        lcd.clear();
        digitalWrite(relay, LOW);
        digitalWrite(l1, LOW);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Nobody In Room");
        lcd.setCursor(4, 1);
        lcd.print("Power Is Off");
        //delay(200);
      }

      else
        digitalWrite(relay, HIGH);
        digitalWrite(l1, HIGH);

      //Ultrasonic sensor light controller
      digitalWrite(trigpin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigpin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigpin, LOW);

      duration = pulseIn(echopin, HIGH);
      distance = duration * 0.034 / 4;
      Serial.begin(57600);
      Serial.print("distance");
      //  lcd.setCursor(0,2);
      Serial.println(distance);
      if (distance < 20)
      {
        digitalWrite(l3, HIGH);
      }
      else
      {
        digitalWrite(l3, LOW);
        delay(1000);
      }
    }
  }
}
