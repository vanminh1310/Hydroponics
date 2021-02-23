#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define RX2 34
#define TX2 35 

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial2.begin(9600,SERIAL_8N1,RX2,TX2);



}

void loop() {
// hien thi json gui json 
 StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(Serial2);
  if (root == JsonObject::invalid())
    return;
   Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial2);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
 
    int data3=root["time"];
   Serial.print("Time: ");
    Serial.print(data3);
     Serial.println();
 String  test = Serial2.readString();
 // put your main code here, to run repeatedly:
Serial.print("data:");
Serial.println(test);
if(test.indexOf("s")>=0){
 Serial.println("bat thiet bi 1");
 }
// delay(200);
}
