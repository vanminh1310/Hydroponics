#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DS18B20.h>
SoftwareSerial s(5, 6); 
DS18B20 ds(8);
void setup() {
  Serial.begin(115200);
  s.begin(9600);
  
}
// cai nay phai co 
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void loop() {
  // put your main code here, to run repeatedly:
  // tao bien json
    float t2 = ds.getTempC();
  root["sensor"] = "gps";
   String a = String(random(1,100));
  root["time"] = a;
  root["ND:"]= t2;
  root.printTo(s);
Serial.println(t2);
   float x = 1.23;
   float k = 234;
  s.println(x);
   s.println(k);
   s.println("s");
  
   s.println("N"+String(a)+"D");
 delay(1000);
}
