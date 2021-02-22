a#include <ArduinoJson.h>
long nhietdo= 0;
float doam = 0;
long ttb1 = 0;
long ttb2 = 0;
long caidat1 = 100;
long caidat2 =100;
long last = 0;
String chuoisenesp ="";
void cambien();
void datajson(String nhietdo ,String doam ,String ttb1,String ttb2,String caidat1,String caidat2 );
void setup(){
Serial.begin(9600);  
last=millis();
}
void loop(){

  if(millis()-last>=1000){
  cambien();
   datajson(String (nhietdo) ,String (doam) ,String (ttb1),String (ttb2),String (caidat1),String (caidat2 ));
   last = millis();
  }
}
void cambien(){
  nhietdo = nhietdo+ 1;
  doam = doam+2;

}
void datajson(String nhietdo ,String doam ,String ttb1,String ttb2,String caidat1,String caidat2 ){
 chuoisenesp ="";
  chuoisenesp="{\"ND\":\""+String(nhietdo)+"\"}";

   Serial.print("Chuoiesp: ");
   Serial.println(chuoisenesp);
   

}
