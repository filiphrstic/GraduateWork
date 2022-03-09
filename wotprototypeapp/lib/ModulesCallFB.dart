import 'package:wotprototypeapp/ModulesList.dart';
import 'package:firebase_database/firebase_database.dart';
import 'dart:async' show Future;

class ModulesCallFB {
  List<ModuleDetails> listItems = [];

  Future<List<ModuleDetails>> firebaseCalls(
      DatabaseReference databaseReference) async {
    ModulesList moduleList;
    DataSnapshot dataSnapshot = await databaseReference.once();
    Map<dynamic, dynamic> jsonResponse = dataSnapshot.value['Stations'];
    moduleList = new ModulesList.fromJSON(jsonResponse);
    listItems.clear();
    listItems.addAll(moduleList.modulesList);
    return listItems;
  }
}
