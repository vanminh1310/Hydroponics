#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(5, 6); 
void setup() {
  s.begin(9600);
  
}
// cai nay phai co 
StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void loop() {
  // put your main code here, to run repeatedly:
  // tao bien json
  root["sensor"] = "gps";
  root["time"] = 1351824120;
  root.printTo(s);
  float x = 1.23;
  float k = 234;
 s.println(x);
  s.println(k);
  s.println("s");
  String a = String(random(1,100));
  s.println("N"+String(a)+"D");
delay(1000);
}
