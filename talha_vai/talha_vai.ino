#define CAYENNE_PRINT Serial
#include<CayenneMQTTESP8266.h>
char ssid[] = "ICE_Innovation_Lab";
char wifiPassword[] = "beinnovative#";

char username[] = "84056ba0-9e54-11e9-ace6-4345fcc6b81e";
char password[] = "76e61cdce9846f42dd06daf774403de6c21d1b5c";
char clientID[] = "f9c66e10-af23-11ec-8c44-371df593ba58";

#define gasPin D0
#define soilPin A0
#define dhtPin D2
#define pumpPin D1

#include<DHT.h>
#define DHTTYPE DHT11
DHT dht(dhtPin,DHTTYPE);
int localTemp = 0;
int localHum = 0;
int gasValue;
int soilMoisture;




void setup() {

    Serial.begin(9600);
    Cayenne.begin(username,password,clientID,ssid,wifiPassword);
    pinMode(pumpPin,OUTPUT);
    pinMode(gasPin,INPUT);
    pinMode(soilPin,INPUT);
}

void loop() {
  
  Cayenne.loop();
  getDht();
  getGas();
  getMoisture();
}

void getDht(void){
  float temp = localTemp;
  float hum = localHum;
  localTemp = dht.readTemperature();
  localHum = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(localTemp);
  Serial.print("Humidity: ");
  Serial.println(localHum);
  delay(500);                   
}

void getGas(void){

  gasValue = digitalRead(gasPin);
    if(gasValue == 1)
  {
    Serial.print("Gas Detect: ");
    Serial.println(gasValue);
  }

  else 
  {
    Serial.print("Gas Value: ");
    Serial.println(gasValue);
  }
 
  delay(500);
  
}

void getMoisture(void){

  soilMoisture = analogRead(soilPin);

  if (soilMoisture <700){
    digitalWrite(pumpPin,HIGH);
    
  }

  else{
    digitalWrite(pumpPin,LOW);
  }
  Serial.print("Soil moisture:");
  Serial.println(soilMoisture);
  delay(500);
  
}

CAYENNE_OUT_DEFAULT(){
  Cayenne.virtualWrite(1,soilMoisture);
  Cayenne.virtualWrite(2,gasValue);
  Cayenne.virtualWrite(3,localTemp);
  Cayenne.virtualWrite(4,localHum);
}

CAYENNE_IN(5)
{
   int currentValue=getValue.asInt();
   if(currentValue==1)
   {
     digitalWrite(pumpPin,HIGH);
   }
   else
   {
     digitalWrite(pumpPin,LOW); 
   }
}

void serialPrint(void)
{
  Serial.print("Temperature: ");
  Serial.println(localTemp);
  Serial.print("Humidity: ");
  Serial.println(localHum);
  
  delay(500);
}
