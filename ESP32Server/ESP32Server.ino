#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

const int IRLedPin = 4;
const int serverPort = 10203;
const char* networkName = "";
const char* networkPswd = "";
const String secretCeritificateKey = "!@#$%^&*()_+";

WiFiServer server(serverPort);
WiFiClient client;

String verifyData(String data){
   int splitIndex = data.indexOf(":");
   String key = "";
   String value = "";
   if(splitIndex != -1){
      key = data.substring(0, splitIndex);
      value = data.substring(splitIndex + 1);
      if(value.indexOf(":") != -1){
         value = "";
      }     
   }
   if(!secretCeritificateKey.equals(key)){
     value = "";
   }
   return value;

}
void setIRLightMode(String value){
  if(value.equals("ON")){
    digitalWrite(IRLedPin,HIGH);
    Serial.println("IR Lights On!");
  }else{
    digitalWrite(IRLedPin,LOW);
    Serial.println("IR Lights off!");
  }
}
void setup(){

  pinMode(IRLedPin,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(networkName, networkPswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Trying to Connect to WiFi...");
  }

  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
    client = server.available();
    if(client){
      Serial.print("New Client Connected: ");
      Serial.println(client.remoteIP());
      while (client.connected()){
        if (client.available()) {
              String data = String(client.readStringUntil('\r').c_str());
              String value = verifyData(data);
              Serial.print("Client Data: ");     
              Serial.println(value); 
              if(value.equals("")){
                client.println("Unauthorized Request, Request Rejected!");
              }else{
             
                setIRLightMode(value); 
              }
          }
      }
      Serial.println("Client Disconnected!");
    }
    delay(1000);
}

