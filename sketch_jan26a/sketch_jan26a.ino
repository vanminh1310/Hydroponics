
#include "WiFi.h"
 
void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}
 
void loop(){
  Serial.println("Start scan");
  int n = WiFi.scanNetworks();
 
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(") ");
      Serial.print(" [");
      Serial.print(WiFi.channel(i));
      Serial.print("] ");
      String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
      Serial.println(encryptionTypeDescription);
      delay(10);
    }
  }
  Serial.println("Scan done");
  Serial.println("");
  delay(10000);
}
 
String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (0):
      return "Open";
    case (1):
      return "WEP";
    case (2):
      return "WPA_PSK";
    case (3):
      return "WPA2_PSK";
    case (4):
      return "WPA_WPA2_PSK";
    case (5):
      return "WPA2_ENTERPRISE";
    default:
      return "UNKOWN";
    }
  }
