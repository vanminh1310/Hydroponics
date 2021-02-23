#include <SoftwareSerial.h>
SoftwareSerial s(5, 6);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
  s.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 float k = 234;
  s.println(k);
  String  test2 = s.readString();
  Serial.print("data:");
Serial.println(test2);
  
}
