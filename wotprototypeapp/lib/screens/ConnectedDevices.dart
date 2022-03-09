import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:wotprototypeapp/screens/ModulesDashboard.dart';
import '../ModulesCallFB.dart';

class ScreenStart extends StatefulWidget {
  @override
  _ScreenStartState createState() => _ScreenStartState();
}

void selectModule(
    BuildContext ctx, AsyncSnapshot snapshot, int index, String moduleName) {
  Navigator.of(ctx).push(
    MaterialPageRoute(
      builder: (_) {
        return Dashboard(
          nameM: moduleName,
          indexM: index,
        );
      },
    ),
  );
}

class _ScreenStartState extends State<ScreenStart> {
  final color = const Color(0xffbfd6ba);
  final databaseReference = FirebaseDatabase.instance.reference();
  final makecall = new ModulesCallFB();

  @override
  Widget build(BuildContext context) {
    var futureBuilder = new FutureBuilder(
      future: makecall.firebaseCalls(databaseReference),
      builder: (BuildContext context, AsyncSnapshot snapshot) {
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
            else
              return ListView.builder(
                itemCount: snapshot.data.length,
                itemBuilder: (BuildContext context, int index) {
                  String moduleName =
                      snapshot.data[index].basicInfo['Name'].toString();
                  return Card(
                    elevation: 8.0,
                    child: InkWell(
                      splashColor: Theme.of(context).primaryColor,
                      onTap: () =>
                          selectModule(context, snapshot, index, moduleName),
                      child: Padding(
                        padding:
                            const EdgeInsets.only(right: 10, top: 10, left: 10),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: <Widget>[
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: <Widget>[
                                if (snapshot.data[index].basicInfo != null)
                                  Text(
                                    snapshot.data[index].basicInfo['Name'] ??
                                        'default',
                                    style: TextStyle(
                                      fontWeight: FontWeight.w500,
                                      fontSize: 20,
                                      fontFamily: 'Roboto-Black',
                                    ),
                                  ),
                                if (int.parse(snapshot.data[index]
                                            .timestamp['Timestamp']) +
                                        120 >
                                    DateTime.now().millisecondsSinceEpoch /
                                        1000)
                                  Text(
                                    'Connected',
                                    style: TextStyle(
                                      fontWeight: FontWeight.bold,
                                      fontSize: 20,
                                      color: Colors.green,
                                    ),
                                  )
                                else
                                  Text(
                                    'Not connected',
                                    style: TextStyle(
                                      fontWeight: FontWeight.bold,
                                      fontSize: 20,
                                      color: Colors.red,
                                    ),
                                  ),
                              ],
                            ),
                            SizedBox(height: 10.0),
                            Column(
                              children: <Widget>[
                                if (snapshot.data[index].basicInfo != null)
                                  Container(
                                    padding: EdgeInsets.symmetric(vertical: 5),
                                    alignment: Alignment.centerLeft,
                                    child: Text(
                                      snapshot.data[index]
                                              .basicInfo['Sensors'] ??
                                          'default',
                                      style: TextStyle(
                                        fontWeight: FontWeight.w700,
                                        color: Colors.black54,
                                      ),
                                    ),
                                  ),
                                if (snapshot.data[index].basicInfo != null)
                                  Container(
                                    padding: EdgeInsets.symmetric(vertical: 5),
                                    alignment: Alignment.centerLeft,
                                    child: Text(
                                      snapshot.data[index]
                                              .basicInfo['Module'] ??
                                          'default',
                                      style: TextStyle(
                                        fontWeight: FontWeight.w700,
                                        color: Colors.black54,
                                      ),
                                    ),
                                  ),
                              ],
                            ),
                          ],
                        ),
                      ),
                    ),
                  );
                },
              );
        }
      },
    );

    _refreshAction() {
      setState(() {});
    }

    return Scaffold(
      resizeToAvoidBottomInset: false,
      appBar: AppBar(
        centerTitle: true,
        title: new Text('Connected devices'),
        elevation: 0.0,
        actions: [
          Padding(
            padding: EdgeInsets.only(right: 20.0),
            child: GestureDetector(
              onTap: () {
                _refreshAction();
              },
              child: Icon(
                Icons.refresh_rounded,
                size: 26.0,
              ),
            ),
          ),
        ],
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
        child: new Column(
          children: <Widget>[
            new Expanded(
              child: Container(
                child: futureBuilder,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
