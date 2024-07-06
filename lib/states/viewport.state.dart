import 'package:fluent_ui/fluent_ui.dart';

class ViewportObject {
  Offset position;

  ViewportObject({required this.position});

  void updatePosition(Offset position) {
    this.position = position;
  }
}

class ViewportState extends ChangeNotifier {
  final List<ViewportObject> _objects = List.generate(2, (index) {
    return ViewportObject(position: Offset(100.0 * index, 100.0 * index));
  });

  List<ViewportObject> get objects => _objects;

  void addObject(ViewportObject object) {
    _objects.add(object);
    notifyListeners();
  }

  void removeObject(ViewportObject object) {
    _objects.remove(object);
    notifyListeners();
  }

  void updateObject(ViewportObject object) {
    notifyListeners();
  }
}
