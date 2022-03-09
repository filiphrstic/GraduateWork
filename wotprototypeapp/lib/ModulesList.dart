class ModulesList {
  List<ModuleDetails> modulesList;

  ModulesList({this.modulesList});

  factory ModulesList.fromJSON(Map<dynamic, dynamic> json) {
    return ModulesList(modulesList: parsemodules(json));
  }

  static List<ModuleDetails> parsemodules(modulesjson) {
    var mList = modulesjson['BrowseStations'] as List;
    List<ModuleDetails> modulesList =
        mList.map((data) => ModuleDetails.fromJson(data)).toList();
    return modulesList;
  }
}

class ModuleDetails {
  Map<dynamic, dynamic> numericalReadings;
  Map<dynamic, dynamic> nonNumericalReadings;
  Map<dynamic, dynamic> interactive;
  Map<dynamic, dynamic> basicInfo;
  Map<dynamic, dynamic> counter;
  Map<dynamic, dynamic> counterNames;
  Map<dynamic, dynamic> timestamp;

  ModuleDetails(
      {this.numericalReadings,
      this.nonNumericalReadings,
      this.interactive,
      this.basicInfo,
      this.counter,
      this.counterNames,
      this.timestamp});

  factory ModuleDetails.fromJson(Map<dynamic, dynamic> parsedJson) {
    return ModuleDetails(
        numericalReadings: parsedJson['Numerical'],
        nonNumericalReadings: parsedJson['NonNumerical'],
        interactive: parsedJson['Interactive'],
        basicInfo: parsedJson['BasicInfo'],
        counter: parsedJson['Counter'],
        counterNames: parsedJson['CounterNames'],
        timestamp: parsedJson['Time']);
  }
}
