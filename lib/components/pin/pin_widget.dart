part of components;

class PinWidget extends StatefulWidget {
  const PinWidget({
    super.key,
    required this.id,
    required this.pinObj,
  });

  final String id;
  final Pin pinObj;

  @override
  State<PinWidget> createState() => _PinWidgetState();
}

class _PinWidgetState extends State<PinWidget> {
  bool hovered = false;

  @override
  Widget build(BuildContext context) {
    var drawAreaData = Provider.of<DrawAreaData>(context);
    var appData = Provider.of<AppData>(context);
    var mouseData = Provider.of<MouseData>(context);

    var properties = widget.pinObj.properties as PinProperties;
    var parentProperties =
        drawAreaData.components[properties.parentId]!.properties;

    Offset parentPos =
        drawAreaData.components[properties.parentId]!.properties.pos;

    Offset offset = (properties.behaviour == PinBehaviour.output)
        ? const Offset(20.0, 1.0)
        : const Offset(0.0, 1.0);

    Offset pos = properties.offset + parentPos;

    if ((parentProperties.type == ComponentType.inputButton ||
        parentProperties.type == ComponentType.outputStateProbe)) {
      pos += const Offset(0.0, 1.5);
    }

    properties.pos = pos;

    return GestureDetector(
      onTap: () {
        if (drawAreaData.drawingElement != DrawElement.connection) {
          drawAreaData.setConnStartData(properties.parentId, widget.id);
          drawAreaData.setDrawingElement(DrawElement.connection);
        } else {
          Wire.create(context, widget.id);
          drawAreaData.setDrawingElement(DrawElement.none);
        }
      },
      child: MouseRegion(
        onEnter: (e) {
          setState(() {
            hovered = true;
          });
        },
        onExit: (e) => {
          setState(() {
            hovered = false;
          })
        },
        cursor: hovered ? SystemMouseCursors.click : MouseCursor.defer,
        child: Stack(
          children: [
            Positioned(
              child: Container(
                decoration: BoxDecoration(
                  color: hovered
                      ? MyTheme.pinHoverColor
                      : properties.state == DigitalState.high
                          ? Colors.red
                          : MyTheme.pinBgColor,
                ),
                width: properties.width,
                height: 2.0,
                child: drawAreaData.drawingElement == DrawElement.connection &&
                        drawAreaData.connStartData.startPinId == widget.id
                    ? PseudoWire(
                        startPos: offset,
                        endPos: mouseData.mousePos + offset - pos,
                      )
                    : null,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
