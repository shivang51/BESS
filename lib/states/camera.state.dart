import 'package:fluent_ui/fluent_ui.dart';

class CameraState extends ChangeNotifier {
  Offset _position = Offset.zero;
  Offset get position => _position;

  double _zoom = 1.0;
  double get zoom => _zoom;

  void setPosition(Offset position) {
    _position = position;
    notifyListeners();
  }

  void movePosition(Offset delta) {
    _position += delta;
    notifyListeners();
  }

  void setZoom(double zoom) {
    _zoom = zoom;
    notifyListeners();
  }

  void zoomIn() {
    _zoom *= 1.1;

    if (_zoom > 3) {
      _zoom = 3;
    }
    notifyListeners();
  }

  void zoomOut() {
    _zoom /= 1.1;

    if (_zoom < 0.1) {
      _zoom = 0.1;
    }

    notifyListeners();
  }

  Matrix4 get transform {
    return Matrix4.identity()
      ..translate(_position.dx, _position.dy)
      ..scale(_zoom);
  }
}
