#include <DHT.h>  // Including library for dht11

#define DHTPIN 0         //pin where the dht11 is connected

DHT dht(DHTPIN, DHT11);

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   delay(10);
   dht.begin();
       

}

void loop() {
  // put your main code here, to run repeatedly:
   float h = dht.readHumidity();
   float t = dht.readTemperature();

   Serial.print(h);
   Serial.print(t);
   delay(500);
      

}
