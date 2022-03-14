//Libraries               
#include <ESP8266WiFi.h>                      
#include <FirebaseArduino.h>  
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

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
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
bool switcher = false;
  
//Basic project information
const String PROJECT_NAME = "Pet feeder";
const String BOARD_NAME = "ESP8266 ESP-12E";
const String CONNECTED_SENSORS = "Microservo SG90";
const String BOARD_LOCATION = "Indoors"; 
const String PROJECT_INDEX = "3";
const String path = "/Stations/BrowseStations/" + PROJECT_INDEX;   

void setup() {
  myservo.attach(2);
  timeClient.begin();
  Serial.begin(115200);
  delay(500);
  
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
  Firebase.setBool(path + "/Interactive/Servo", switcher); 
}

  //Firebase connection
void firebasereconnect(){
  delay(300);
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(300);
}

void loop() {
  if (Firebase.failed()){
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }
  
  //Servo Motor    
  switcher=Firebase.getBool(path + "/Interactive/Servo"); 
  if(switcher==false){
      delay(1000);
  }
  if(switcher==true){                                      
    for (pos = 90; pos <= 270; pos += 1) { 
      myservo.write(pos); 
      Serial.print(pos);
      delay(1);                      
    }
    for (pos = 270; pos >= 90; pos -= 1) { 
      myservo.write(pos);              
      delay(1);                      
    }
    delay(2000);
    switcher = false;
    Firebase.setBool(path + "/Interactive/Servo", switcher);
    delay(1500);
  }
  delay(2000);

  //Get epoch unix timestamp
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getEpochTime());
  String fireTime = String(timeClient.getEpochTime());
  delay(1000);

  //Send time to firebase
  Firebase.setString(path + "/Time/Timestamp", fireTime);
  delay(1000);
}
