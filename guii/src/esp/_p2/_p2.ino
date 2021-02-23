#include <SoftwareSerial.h>
//#define RX2 34
//#define TX2 35 
SoftwareSerial s(34, 35);
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
//.begin(9600,SERIAL_8N1,RX2,TX2);
  s.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
String test =s.readString();
Serial.print("data:");
Serial.println(test);
s.println("test");
}
