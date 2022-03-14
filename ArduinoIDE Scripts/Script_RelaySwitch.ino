//Libraries               
#include <ESP8266WiFi.h>                      
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>  
#include <MemoryFree.h>

//Time setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Firebase credentials
#define FIREBASE_HOST "fhrsticproject-default-rtdb.europe-west1.firebasedatabase.app"      
#define FIREBASE_AUTH "2wO3ESUMn2Uqm8JC844DzjmA2rgBVwBcrOWeUBKB"

//WiFi name and password 
#define WIFI_SSID "Magic2B3D"                                
#define WIFI_PASSWORD "HWTC5E2B3D95" 
//#define WIFI_SSID "Redmi Note 8 Pro"                                  
//#define WIFI_PASSWORD "12345678"    

//Sensor definitions 
int relay = D1;
int waterValue = 0;
String waterLevel;  
int waterPin = A0;
bool switcher = false;

//Basic project information
const String PROJECT_NAME = "Irrigation system";
const String BOARD_NAME = "ESP8266 ESP-12E";
const String CONNECTED_SENSORS = "Relay Switch + Water Level Sensor";
const String BOARD_LOCATION = "Outdoors"; 
const String PROJECT_INDEX = "2";
const String path = "/Stations/BrowseStations/" + PROJECT_INDEX;   

void setup() {
  timeClient.begin();
  Serial.begin(9600);
  delay(500);

  //Sensor initialization
  pinMode(relay,OUTPUT);

  //WiFi connection               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  //Firebase start                                            
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              

  //Send data to Firebase
  Firebase.setString(path + "/BasicInfo/Name", PROJECT_NAME);
  Firebase.setString(path + "/BasicInfo/Location", BOARD_LOCATION);
  Firebase.setString(path + "/BasicInfo/Module", BOARD_NAME);
  Firebase.setString(path + "/BasicInfo/Sensors", CONNECTED_SENSORS);
  Firebase.setBool(path + "/Interactive/Relay", switcher); 
}

//Firebase connection
void firebasereconnect() {
  delay(300);
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(300);
}

void loop() {
  if (Firebase.failed()) {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }

  //Water Level Sensor
  waterValue = analogRead(waterPin); 
  if (waterValue <= 150) {
    Serial.println("Empty");
    waterLevel = "Empty";
  } else if (waterValue > 150 && waterValue <= 250) {
    Serial.println("Very Low");
    waterLevel = "Very Low";
  } else if (waterValue > 250 && waterValue <= 350) {
    Serial.println("Low");
    waterLevel = "Low";
  } else if (waterValue > 350 && waterValue <= 500) {
    Serial.println("Medium");
    waterLevel = "Medium";
  } else if (waterValue > 500 && waterValue <= 600) {
    Serial.println("High");
    waterLevel = "High";
  } else if (waterValue > 600) {
    Serial.println("Full");
    waterLevel = "Full";
  }
  Serial.println(waterValue);
  delay(1000);

  //Get epoch unix timestamp
  timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
  //Serial.println(timeClient.getEpochTime());
  String fireTime = String(timeClient.getEpochTime());
  delay(1000);
  
  //Send data to Firebase
  Firebase.setString(path + "/NonNumerical/WaterLevel", waterLevel);
  Firebase.setString(path + "/Time/Timestamp", fireTime);
  delay(300);     

  //Relay Switch    
  switcher=Firebase.getBool(path + "/Interactive/Relay"); 
  if(switcher==false) {
      digitalWrite(relay,LOW);
      Serial.println("Relay ON");
  }
  if(switcher==true) {                                      
      digitalWrite(relay,HIGH);
      Serial.println("Relay OFF");
  }
  delay(2000);  
}
