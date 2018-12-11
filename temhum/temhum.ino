#include <DHT.h>
#include <SoftwareSerial.h>
#include "LowPower.h"
//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
const int wakepin = 2;

void setup()
{
  Serial.begin(9600);
  dht.begin();
}


void loop()
{
  uint8_t sleeper=0;
  //sleep XBee
  pinMode(wakepin, INPUT);
  digitalWrite(wakepin, HIGH);
/*
  while(sleeper < 1) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    sleeper++;
  }*/
  delay(8000);
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature();

  pinMode(wakepin, OUTPUT);
  digitalWrite(wakepin, LOW);

  
  put_xbee(hum,temp);
}

void put_xbee(float hum, float temp)
{
  int i=0;
  uint8_t checksum=0;
  unsigned char device=0;
  
  uint8_t frame[16]={1,1,1,1,1,1,1,0,0,0,0,0,0,checksum};
  uint8_t humInts,humDecs,tempInts,tempDecs;
  humInts=(int)hum;
  tempInts=(int)temp;
  humDecs=((int)(hum*100)%100);
  tempDecs=((int)(temp*100)%100);

  frame[6]=0x01; //Device ID
  frame[5]=0x01; //Device group ID
  frame[7]=tempInts;
  frame[8]=tempDecs;
  frame[9]=humInts;
  frame[10]=humDecs;
  
    for(i = 5; i<11; i++)
  {
    Serial.print(frame[i]);
    Serial.print(" ");
  }
  Serial.println();
}
