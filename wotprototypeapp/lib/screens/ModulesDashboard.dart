import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:liquid_progress_indicator/liquid_progress_indicator.dart';
import 'package:wotprototypeapp/ModulesList.dart';
import 'package:flutter_weather_bg/flutter_weather_bg.dart';
import 'package:intl/intl.dart';

class Dashboard extends StatefulWidget {
  final int indexM;
  final String nameM;
  const Dashboard({Key key, this.indexM, this.nameM}) : super(key: key);

  @override
  _DashboardState createState() => _DashboardState(indexM, nameM);
}

class _DashboardState extends State<Dashboard> with TickerProviderStateMixin {
  bool isLoading = false;
  bool isSwitched = false;
  bool isVisible = true;
  int indexModule;
  String nameModule;
  DatabaseReference dbReference =
      FirebaseDatabase.instance.reference().child("Stations");

  _DashboardState(int indexM, String nameM) {
    indexModule = indexM;
    nameModule = nameM;
  }

  var myFocusNode = List<FocusNode>.filled(2, null);

  @override
  void initState() {
    super.initState();
    myFocusNode[0] = FocusNode();
    myFocusNode[1] = FocusNode();
  }

  @override
  void dispose() {
    myFocusNode[0].dispose();
    myFocusNode[1].dispose();

    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(nameModule),
        centerTitle: true,
      ),
      body: Container(
        decoration: BoxDecoration(
          gradient: LinearGradient(
            begin: Alignment.topCenter,
            end: Alignment.bottomCenter,
            colors: [
              Colors.white,
              Colors.blue,
            ],
          ),
        ),
        child: StreamBuilder(
          stream: dbReference.onValue,
          builder: (context, snapshot) {
            switch (snapshot.connectionState) {
              case ConnectionState.none:
                return new Text('Press button to start');
              case ConnectionState.waiting:
                return Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: <Widget>[
                      CircularProgressIndicator(
                        semanticsLabel: 'Linear progress indicator',
                      ),
                    ],
                  ),
                );
              default:
                if (snapshot.hasError)
                  return new Text('Error: ${snapshot.error}');
                else {
                  var _dbData = ModuleDetails.fromJson(
                    snapshot.data.snapshot.value["BrowseStations"][indexModule],
                  );
                  return Column(
                    children: [
                      Expanded(
                        child: Row(
                          children: [
                            if (_dbData.numericalReadings != null)
                              Expanded(
                                child: _futureBuilderNumerical(_dbData),
                              ),
                            if (_dbData.nonNumericalReadings != null)
                              Expanded(
                                child: _futureBuilderNonNumerical(_dbData),
                              ),
                            if (_dbData.interactive != null)
                              Expanded(
                                child: _futureBuilderInteractive(_dbData),
                              ),
                            if (_dbData.counter != null)
                              Expanded(
                                child: _futureBuilderRFIDCounter(_dbData),
                              ),
                          ],
                        ),
                      ),
                      SizedBox(
                        height: _dbData.basicInfo.length.toDouble() * 43,
                        child: _futureBuilderBasicInfo(_dbData),
                      ),
                    ],
                  );
                }
            }
          },
        ),
      ),
    );
  }

  Widget _futureBuilderNumerical(ModuleDetails dbData) {
    if (dbData.numericalReadings != null)
      return ListView.builder(
        itemCount: dbData.numericalReadings.length,
        itemBuilder: (BuildContext context, int index) {
          return Padding(
            padding: const EdgeInsets.all(10.0),
            child: Card(
              elevation: 10,
              child: Column(
                children: [
                  if (dbData.numericalReadings.keys.elementAt(index) ==
                      "Temperature")
                    Icon(Icons.thermostat_rounded),
                  if (dbData.numericalReadings.keys.elementAt(index) ==
                      "Humidity")
                    Icon(Icons.water_damage_rounded),
                  if (dbData.numericalReadings.keys.elementAt(index) ==
                      "SoilMoisture")
                    Icon(Icons.terrain_rounded),
                  SizedBox(
                    height: 120,
                    width: 120,
                    child: dbData.numericalReadings.keys.elementAt(index) ==
                            "Temperature"
                        ? LiquidCircularProgressIndicator(
                            value: double.parse(dbData.numericalReadings.values
                                    .elementAt(index)) /
                                40,
                            valueColor:
                                AlwaysStoppedAnimation(Colors.pink[100]),
                            backgroundColor: Colors.white,
                            borderColor: Colors.red,
                            borderWidth: 5.0,
                            direction: Axis.vertical,
                            center: Text(
                              dbData.numericalReadings.values.elementAt(index) +
                                  " °C",
                              style: TextStyle(
                                fontSize: 25,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          )
                        : LiquidCircularProgressIndicator(
                            value: double.parse(dbData.numericalReadings.values
                                    .elementAt(index)) /
                                100,
                            valueColor:
                                AlwaysStoppedAnimation(Colors.lightBlue[200]),
                            backgroundColor: Colors.white,
                            borderColor: Colors.blue,
                            borderWidth: 5.0,
                            direction: Axis.vertical,
                            center: Text(
                              dbData.numericalReadings.values.elementAt(index) +
                                  " %",
                              style: TextStyle(
                                fontSize: 25,
                                fontWeight: FontWeight.bold,
                              ),
                            ),
                          ),
                  ),
                  Text(
                    dbData.numericalReadings.keys.elementAt(index) ?? 'default',
                    style: TextStyle(fontSize: 20),
                  ),
                ],
              ),
            ),
          );
        },
      );
    else
      return Container();
  }

  Widget _futureBuilderNonNumerical(ModuleDetails dbData) {
    if (dbData.nonNumericalReadings != null)
      return ListView.builder(
        itemCount: dbData.nonNumericalReadings.length,
        itemBuilder: (BuildContext context, int index) {
          return Padding(
            padding: const EdgeInsets.all(10.0),
            child: Card(
              elevation: 10,
              child: Column(
                children: [
                  if (dbData.nonNumericalReadings.keys.elementAt(index) ==
                      "WaterLevel")
                    SizedBox(
                      width: 120,
                      height: 120,
                      child: LiquidCircularProgressIndicator(
                        value: 0.5,
                        valueColor: AlwaysStoppedAnimation(Colors.blue[50]),
                        backgroundColor: Colors.white,
                        borderColor: Colors.blue,
                        borderWidth: 5.0,
                        direction: Axis.vertical,
                        center: Text(
                          dbData.nonNumericalReadings.values.elementAt(index),
                          style: TextStyle(
                            fontSize: 25,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ),
                    ),
                  if (dbData.nonNumericalReadings.keys.elementAt(index) ==
                      "Rain")
                    Padding(
                      padding: const EdgeInsets.all(10.0),
                      child: switchRain(dbData, index),
                    ),
                  Text(
                    dbData.nonNumericalReadings.values.elementAt(index) ??
                        'default',
                    style: TextStyle(fontSize: 20),
                  ),
                ],
              ),
            ),
          );
        },
      );
    else
      return Container();
  }

  Widget _futureBuilderInteractive(ModuleDetails dbData) {
    if (dbData.interactive != null)
      return ListView.builder(
        itemCount: dbData.interactive.length,
        itemBuilder: (BuildContext context, int index) {
          isSwitched = dbData.interactive.values.elementAt(index);
          return Padding(
            padding: const EdgeInsets.all(10.0),
            child: Card(
              elevation: 10,
              child: Column(
                children: [
                  if (dbData.interactive.keys.elementAt(index) == "Servo") ...[
                    Image.asset(
                      "assets/images/guineapig_hungry.png",
                      height: 150,
                      width: 150,
                    ),
                  ] else if (dbData.interactive.keys.elementAt(index) ==
                      "Relay") ...[
                    Padding(
                      padding: const EdgeInsets.all(10.0),
                      child: Image.asset(
                        "assets/images/relay.gif",
                        height: 125,
                        width: 125,
                      ),
                    ),
                  ] else ...[
                    Icon(Icons.power_settings_new_rounded),
                  ],
                  if (dbData.interactive.keys.elementAt(index) == "Servo") ...[
                    Container(
                      margin: EdgeInsets.all(10),
                      height: 80.0,
                      child: SizedBox.fromSize(
                        size: Size(80, 80),
                        child: ClipOval(
                          child: Material(
                            color: Colors.blue,
                            child: InkWell(
                              splashColor: Colors.blue[100],
                              onTap: () {
                                DateTime now = DateTime.now();
                                String formattedDate =
                                    DateFormat('yyyy-MM-dd – kk:mm')
                                        .format(now);

                                updateTimeDB(formattedDate, "Last");
                                updateSwitchStateDB(true, "Servo");
                              },
                              child: Column(
                                mainAxisAlignment: MainAxisAlignment.center,
                                children: <Widget>[
                                  Icon(
                                    Icons.power_settings_new_rounded,
                                    color: Colors.white,
                                  ),
                                  Text(
                                    "Feed",
                                    style: TextStyle(
                                      fontSize: 15,
                                      color: Colors.white,
                                    ),
                                  ),
                                ],
                              ),
                            ),
                          ),
                        ),
                      ),
                    ),
                  ] else ...[
                    Switch(
                      value: isSwitched,
                      onChanged: (value) {
                        setState(() {
                          isSwitched = value;
                          print(isSwitched);
                        });
                        DateTime now = DateTime.now();
                        String formattedDate =
                            DateFormat('yyyy-MM-dd – kk:mm').format(now);

                        updateSwitchStateDB(
                            value, dbData.interactive.keys.elementAt(index));
                        updateTimeDB(formattedDate, "Last");
                      },
                    ),
                    Text(
                      dbData.interactive.keys.elementAt(index) ?? 'default',
                      style: TextStyle(fontSize: 20),
                    ),
                  ],
                ],
              ),
            ),
          );
        },
      );
    else
      return Container();
  }

  Widget _futureBuilderBasicInfo(ModuleDetails dbData) {
    if (dbData.basicInfo != null)
      return ListView.builder(
        itemCount: dbData.basicInfo.length,
        itemBuilder: (BuildContext context, int index) {
          return Card(
            elevation: 5,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                Padding(
                  padding: const EdgeInsets.all(5.0),
                  child: Row(
                    children: [
                      Text(
                        dbData.basicInfo.keys.elementAt(index) + ": " ??
                            'default',
                        style: TextStyle(
                            fontSize: 20, fontWeight: FontWeight.bold),
                      ),
                      Text(
                        dbData.basicInfo.values.elementAt(index) ?? 'default',
                        textAlign: TextAlign.right,
                        style: TextStyle(fontSize: 20),
                      ),
                    ],
                  ),
                )
              ],
            ),
          );
        },
      );
    else
      return Container();
  }

  Widget _futureBuilderRFIDCounter(ModuleDetails dbData) {
    if (dbData.counter != null)
      return ListView.builder(
        itemCount: dbData.counter.length,
        itemBuilder: (BuildContext context, int index) {
          return Padding(
            padding: const EdgeInsets.all(10.0),
            child: Card(
              elevation: 10,
              child: Column(
                children: [
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      IconButton(
                        icon: Icon(
                          Icons.settings_backup_restore_rounded,
                          size: 20,
                        ),
                        onPressed: () => setCounterToZeroDB(
                            index, dbData.counter.keys.elementAt(index)),
                      ),
                      SizedBox(
                        height: 120,
                        width: 220,
                        child: Align(
                          alignment: Alignment.center,
                          child: Text(
                            dbData.counter.values.elementAt(index) ?? 'default',
                            style: TextStyle(
                              fontSize: 40,
                              fontWeight: FontWeight.bold,
                            ),
                          ),
                        ),
                      ),
                      IconButton(
                        icon: const Icon(
                          Icons.edit_rounded,
                          size: 20,
                        ),
                        onPressed: () => rfidEditName(index),
                      ),
                    ],
                  ),
                  Text(
                    dbData.counterNames.values.elementAt(index) ?? 'default',
                    style: TextStyle(fontSize: 20),
                  ),
                  Visibility(
                    visible: isVisible,
                    child: TextField(
                      focusNode: myFocusNode[index],
                      autocorrect: false,
                      autofocus: false,
                      style: TextStyle(fontSize: 20),
                      textAlign: TextAlign.center,
                      onSubmitted: (String value) => updateRFIDNameDB(
                          value, dbData.counterNames.keys.elementAt(index)),
                    ),
                  )
                ],
              ),
            ),
          );
        },
      );
    else
      return Container();
  }

  void rfidEditName(int index) {
    setState(() {
      isVisible = true;
    });
    myFocusNode[index].requestFocus();
  }

  void updateRFIDNameDB(String newName, String key) {
    dbReference
        .child('BrowseStations/' + indexModule.toString() + '/CounterNames')
        .update({key: newName});
    isVisible = false;
  }

  void updateSwitchStateDB(bool state, String switchName) {
    dbReference
        .child('BrowseStations/' + indexModule.toString() + '/Interactive')
        .update({switchName: state});
  }

  void updateTimeDB(String time, String name) {
    dbReference
        .child('BrowseStations/' + indexModule.toString() + '/BasicInfo')
        .update({name: time});
  }

  WeatherBg switchRain(ModuleDetails dbData, int index) {
    switch (dbData.nonNumericalReadings.values.elementAt(index)) {
      case 'No rain!':
        return WeatherBg(
          weatherType: WeatherType.sunny,
          width: 125,
          height: 125,
        );
        break;
      case 'Light rain!':
        return WeatherBg(
          weatherType: WeatherType.lightRainy,
          width: 125,
          height: 125,
        );
        break;
      case 'Medium rain!':
        return WeatherBg(
          weatherType: WeatherType.middleRainy,
          width: 125,
          height: 125,
        );
        break;
      case 'Heavy rain!':
        return WeatherBg(
          weatherType: WeatherType.heavyRainy,
          width: 125,
          height: 125,
        );
        break;
      default:
        return WeatherBg(
          weatherType: WeatherType.overcast,
          width: 125,
          height: 125,
        );
    }
  }

  void setCounterToZeroDB(int index, String name) {
    dbReference
        .child('BrowseStations/' + indexModule.toString() + '/Counter')
        .update({name: '0'});
  }
}
