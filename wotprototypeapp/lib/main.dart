import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:wotprototypeapp/screens/ConnectedDevices.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'WoTsApp',
      theme: ThemeData(
        scaffoldBackgroundColor: Color(0xFFE1DFDF),
        textTheme:
            GoogleFonts.cabinCondensedTextTheme(Theme.of(context).textTheme),
        primarySwatch: Colors.blue,
        primaryColor: Colors.blue,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: ScreenStart(),
    );
  }
}
