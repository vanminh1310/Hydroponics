#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DS18B20.h>
SoftwareSerial s(5, 6);
DS18B20 ds(8);
long last = 0;
 int led = 12; 
 StaticJsonBuffer<1000> jsonBuffer;
JsonObject &root = jsonBuffer.createObject();
 void senduart()
{
  float t2 = ds.getTempC();
  root["sensor"] = "gps";
  String a = String(random(1, 100));
  root["time"] = a;
  root["ND:"] = t2;
  root.printTo(s);
  Serial.println(t2);
  float x = 1.23;
  float k = 234;
  s.println(x);
  s.println(k);
  s.println("s");

  s.println("N" + String(a) + "D");
}
void setup()
{
  Serial.begin(115200);
  s.begin(9600);
  last = millis();
  pinMode(led,OUTPUT);
}
// cai nay phai co



void loop()
{
  // put your main code here, to run repeatedly:
  // tao bien json
if(millis()-last>=10000){
  senduart();
  last=millis();
}
  String test = s.readString();
    Serial.println(test);
   if(test.indexOf("s")>=0){
 Serial.println("bat thiet bi 1");
 digitalWrite(led,HIGH);
 }
 if(test.indexOf("a")>=0){
 digitalWrite(led,LOW);
 }


}
