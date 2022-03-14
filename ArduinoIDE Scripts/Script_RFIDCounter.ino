//Libraries
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>                      
#include <FirebaseArduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#define buzzer D4

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

//RFID reader pins and setup
constexpr uint8_t RST_PIN = 5;     
constexpr uint8_t SS_PIN = 4;     
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

//Basic project information
const String PROJECT_NAME = "RFID Counter";
const String BOARD_NAME = "ESP8266 ESP-12E";
const String CONNECTED_SENSORS = "RFID Reader";
const String BOARD_LOCATION = "Indoors";
const String PROJECT_INDEX = "4";
const String COUNTER_INIT = "0";
const String path = "/Stations/BrowseStations/" + PROJECT_INDEX;   
int count1;
int count2;

void setup() { 
  timeClient.begin();
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  pinMode(buzzer, OUTPUT);

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
  delay(500);

  //Get data from Firebase
  String countString1 = Firebase.getString(path + "/Counter/Counter_1");
  count1 = countString1.toInt();
  String countString2 = Firebase.getString(path + "/Counter/Counter_2");
  count2 = countString2.toInt();                     
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

  //Get epoch unix timestamp
  timeClient.update();
  String fireTime = String(timeClient.getEpochTime());
  if(fireTime.endsWith("0")){
    Firebase.setString(path + "/Time/Timestamp", fireTime);
  }
  
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  String content="";
  for (byte i = 0; i < rfid.uid.size; i++) 
    {
      content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(rfid.uid.uidByte[i], HEX));
    }
  content.toUpperCase();
  if (content.substring(1) == "69 C5 C9 55"){
      Serial.println("This is the card tag");
      String countString = Firebase.getString(path + "/Counter/Counter_1");
      count1 = countString.toInt();
      count1 = count1+1;
      Firebase.setString(path + "/Counter/Counter_1", String(count1));
      tone(buzzer,523);
      delay(200);
      tone(buzzer,587);
      delay(200);
      tone(buzzer,659);
      delay(200);
      tone(buzzer,783);
      delay(250);
      tone(buzzer,659);
      delay(200);
      tone(buzzer,587);
      delay(200);
      noTone(buzzer);
    }
  else {
      Serial.println("This is the keychain tag");
      String countString = Firebase.getString(path + "/Counter/Counter_2");
      count2 = countString.toInt();
      count2 = count2+1;
      Firebase.setString(path + "/Counter/Counter_2", String(count2));
      tone(buzzer,523);
      delay(100);
      noTone(buzzer);
      delay(100);
      tone(buzzer,523);
      delay(200);
      tone(buzzer,880);
      delay(100);
      noTone(buzzer);
      delay(100);
      tone(buzzer,880);
      delay(200);
      tone(buzzer,987);
      delay(100);
      noTone(buzzer);
      delay(100);
      tone(buzzer,987);
      delay(200);
      tone(buzzer,880);
      delay(300);
      noTone(buzzer);
      }
  delay(300);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
