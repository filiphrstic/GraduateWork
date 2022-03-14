//Libraries
#include <DHT.h>
#include <ESP8266WiFi.h>                      
#include <FirebaseArduino.h>
#include <LiquidCrystal_I2C.h>
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
#define DHTPIN D3                                            
#define DHTTYPE DHT11                                        
DHT dht(DHTPIN, DHTTYPE);                                                    
const int sensor_pin = A0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Basic project information
const String PROJECT_NAME = "Plant station";
const String BOARD_NAME = "ESP8266 ESP-12E";
const String CONNECTED_SENSORS = "DHT11 + SoilMoisture";
const String BOARD_LOCATION = "Indoors";
const String PROJECT_INDEX = "0"; 
const String path = "/Stations/BrowseStations/" + PROJECT_INDEX; 

void setup() {
  timeClient.begin();
  Serial.begin(9600);
  delay(500);

  //Sensor initialization
  dht.begin();
  delay(300); 
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();  

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
  String lcdHumid = String((int)round(h)) + String("%");
  String lcdTemp = String((int)round(t));
  delay(500);

  //Soil moisture reading
  float moisture_percentage;                                                            
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
  Serial.print("Soil Moisture: ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  String fireMoist = String(moisture_percentage);
  String lcdMoist = String((int)round(moisture_percentage)) + String("%"); 
  delay(500);

  //Get epoch unix timestamp
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getEpochTime());
  String fireTime = String(timeClient.getEpochTime());
  delay(1000);
  
  //Send data to Firebase
  Firebase.setString(path + "/Numerical/SoilMoisture", fireMoist);
  Firebase.setString(path + "/Numerical/Humidity", fireHumid);   
  Firebase.setString(path + "/Numerical/Temperature", fireTemp);
  Firebase.setString(path + "/Time/Timestamp", fireTime);
  delay(500);

  //Show data on LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Temp");
  lcd.setCursor(5, 0);
  lcd.print("Humid");
  lcd.setCursor(11, 0);
  lcd.print("Moist");
  lcd.setCursor(0, 1);
  lcd.print(lcdTemp);
  lcd.setCursor(2, 1);
  lcd.print((char)223);
  lcd.setCursor(6, 1);
  lcd.print(lcdHumid);
  lcd.setCursor(12, 1);
  lcd.print(lcdMoist);                        
  delay(5000);   
}
