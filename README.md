# GraduateWork
Graduate work for Faculty of Organisation and Informatics (Diplomski rad)

Final version can be downloaded from this link: https://repozitorij.foi.unizg.hr/islandora/object/foi%3A7037

This work consists out of five smaller IoT projects and a central mobile application. Primary goal was to upgrade "Internet of Things" projects to "Web of Things". That's why every IoT project is connected to Firebase via Web protocols. Firebase is used as a central database and a link between projects and mobile application.

Mobile application was created using Dart language and Flutter framework. Application is retrieving data from Firebase Realtime Database and displays it on screen, in realtime. Each project dashboard is different, but it is made out of single codebase. That's what makes this app modular. It is not important how many data does user have in a database, they will all be properly displayed. Each data type will get it's own equally wide column. For example, if there is four types of data stored in a database (numerical, non-numerical, interactive,...) there will be four columns with it's data. Also, each column will be independent and scrollable. At the bottom of the screen is basic info about that project.

NodeMCU Lolin v3 with ESP-01 Wifi Module and ESP8266 SoC was used for IoT projects. Development boards were programmed with ArduinoIDE and each script can be found in "ArduinoIDE Scripts" folder. 

<img src="media/cd.png" width=200 align=center>
List of all IoT projects


<img src="media/ws.png" width=200 align=center>
Weather station dashboard with temperature/humidity readings from DHT11 sensor (numerical) and rain readings from Rain sensor (non-numerical)


<img src="media/plant_pet.png" width=400 align=center>
Plant station and Pet feeder dashboards


<img src="media/switch_rfid.png" width=400 align=center>
Relay switch and RFID counter dashboards


![Alt Text](media/testws.gif)
Weather station with DHT11 and Rain sensor

![Alt Text](media/testps.gif)
Plant station with DHT11, Moisture sensor and LCD display

![Alt Text](media/testrs.gif)
Relay switch embedded in an extension cord with Water level sensor (so it can be used in automatic irrigation projects)

![Alt Text](media/testrfid.gif)
RFID Counter with RFID card and RFID tag

![Alt Text](media/testpf.gif)
Pet feeder with servo motor
