import 'package:fluent_ui/fluent_ui.dart';
import 'package:flutter/gestures.dart';

class ScrollDetector extends StatelessWidget {
  final void Function(PointerScrollEvent event) onPointerScroll;
  final Widget child;

  const ScrollDetector({
    required this.onPointerScroll,
    required this.child,
  });

  @override
  Widget build(BuildContext context) {
    return Listener(
      onPointerSignal: (pointerSignal) {
        if (pointerSignal is PointerScrollEvent) onPointerScroll(pointerSignal);
      },
      child: child,
    );
  }
}
