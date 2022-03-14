//Libraries
#include <DHT.h>                 
#include <ESP8266WiFi.h>                      
#include <FirebaseArduino.h>
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
#define DHTPIN D2                                            
#define DHTTYPE DHT11                                        
#define rainAnalog A0
#define rainDigital D1
DHT dht(DHTPIN, DHTTYPE);

//Basic project information
const String PROJECT_NAME = "Weather station";
const String BOARD_NAME = "ESP8266 ESP-12E";
const String CONNECTED_SENSORS = "DHT11 + RainSensor";
const String BOARD_LOCATION = "Outdoors"; 
const String PROJECT_INDEX = "1";  
const String path = "/Stations/BrowseStations/" + PROJECT_INDEX;                                                    
 
void setup() {
  timeClient.begin();
  Serial.begin(9600);
  delay(500);
  
  //Sensor initialization
  dht.begin();                                                               
  pinMode(rainDigital,INPUT);
  
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
}

//Firebase connection
void firebasereconnect(){
  delay(300);
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
}

void loop() { 
  //Firebase reconnect
  if (Firebase.failed()){
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }

  //Temperature and humidity reading
  float h = dht.readHumidity();                                              
  float t = dht.readTemperature();                                         
  if (isnan(h) || isnan(t)) {                                                
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h);                                         
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t); 
  delay(500);
  
  //Rain reading
  int rainAnalogVal = analogRead(rainAnalog);
  int rainDigitalVal = digitalRead(rainDigital); 
  String fireRainAnalog;
  Serial.print(rainAnalogVal);
  Serial.print("\t");
  Serial.println(rainDigitalVal);
  if (rainAnalogVal > 1015) {
    fireRainAnalog = "No rain!";
    Serial.println(fireRainAnalog);
  }
  else if (rainAnalogVal > 850 && rainAnalogVal <= 1015) {
    fireRainAnalog = "Light rain!";
    Serial.println(fireRainAnalog);
  }
  else if (rainAnalogVal > 400 && rainAnalogVal <= 850){
    fireRainAnalog = "Medium rain!";
    Serial.println(fireRainAnalog);
  }
  else {
    fireRainAnalog = "Heavy rain!";
    Serial.println(fireRainAnalog);
  }
  delay(500);
  
  //Get epoch unix timestamp
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getEpochTime());
  String fireTime = String(timeClient.getEpochTime());
  delay(1000);

  //Send data to Firebase
  Firebase.setString(path + "/NonNumerical/Rain", fireRainAnalog);
  Firebase.setString(path + "/Numerical/Humidity", fireHumid);   
  Firebase.setString(path + "/Numerical/Temperature", fireTemp);
  Firebase.setString(path + "/Time/Timestamp", fireTime);  
  delay(5000);
}
