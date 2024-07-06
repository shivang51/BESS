import 'package:bess/pages/main_page/widgets/scroll_detector.dart';
import 'package:bess/states/camera.state.dart';
import 'package:bess/states/viewport.state.dart';
import 'package:fluent_ui/fluent_ui.dart';
import 'package:flutter/gestures.dart';
import 'package:provider/provider.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key});

  static const String route = 'main/';

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => CameraState()),
        ChangeNotifierProvider(create: (_) => ViewportState()),
      ],
      child: Column(
        children: [
          Row(
            children: [
              Button(
                onPressed: () {},
                child: const Text("File"),
              ),
            ],
          ),
          const Expanded(
            child: Row(
              children: [
                Expanded(
                  flex: 2,
                  child: Card(
                    child: Column(
                      children: [
                        Text("Left"),
                      ],
                    ),
                  ),
                ),
                SizedBox(
                  width: 2.0,
                  height: double.infinity,
                ),
                ViewPort(),
              ],
            ),
          ),
          BottomBar(),
        ],
      ),
    );
  }
}

class BottomBar extends StatelessWidget {
  const BottomBar({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    final cameraState = Provider.of<CameraState>(context);
    return Container(
      color: Colors.grey,
      padding: const EdgeInsets.all(4.0),
      child: Row(
        children: [
          Text(
            "Pos ${cameraState.position * -1}",
          ),
        ],
      ),
    );
  }
}

class ViewPort extends StatelessWidget {
  const ViewPort({
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    final cameraState = Provider.of<CameraState>(context);
    final viewportState = Provider.of<ViewportState>(context);

    return Expanded(
      flex: 8,
      child: GestureDetector(
        onHorizontalDragUpdate: (details) {
          cameraState.movePosition(Offset(details.primaryDelta!, 0));
        },
        onVerticalDragUpdate: (details) {
          cameraState.movePosition(Offset(0, details.primaryDelta!));
        },
        child: ScrollDetector(
          onPointerScroll: (event) {
            if (event.scrollDelta.dy > 0)
              cameraState.zoomOut();
            else
              cameraState.zoomIn();
          },
          child: ClipRect(
            child: Card(
              padding: const EdgeInsets.all(0.0),
              child: Transform(
                transform: Matrix4.identity()
                  ..translate(
                    cameraState.position.dx,
                    cameraState.position.dy,
                  )
                  ..scale(cameraState.zoom),
                child: Stack(
                  children: [
                    const SizedBox(
                      height: double.infinity,
                    ),
                    ...(List.generate(viewportState.objects.length, (ind) {
                      var object = viewportState.objects[ind];
                      return Transform.translate(
                        offset: object.position,
                        child: DraggableWidget(
                          position: object.position,
                          onDragEnd: (offset) {
                            object.updatePosition(
                                offset * (1 / cameraState.zoom));
                            viewportState.updateObject(object);
                          },
                        ),
                      );
                    })),
                  ],
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}

class DraggableWidget extends StatefulWidget {
  const DraggableWidget({
    super.key,
    required this.onDragEnd,
    required this.position,
  });

  final Function(Offset) onDragEnd;
  final Offset position;

  @override
  State<DraggableWidget> createState() => _DraggableWidgetState();
}

class _DraggableWidgetState extends State<DraggableWidget> {
  Offset position = Offset.zero;

  @override
  void initState() {
    position = widget.position;
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Draggable(
      feedback: const Text("Center 1"),
      onDragUpdate: (details) {
        setState(() {
          position += details.delta;
        });
      },
      onDragEnd: (details) {
        widget.onDragEnd(position);
        debugPrint("Dragged to " + position.toString());
      },
      child: Text("Center 1"),
    );
  }
}
