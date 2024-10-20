#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace Bess::UI {
    void init(GLFWwindow *window);
    void begin();
    void end();

    void drawStats(int fps);

    void shutdown();
    void loadFontAndSetScale(float fontSize, float scale);
    void setCursorPointer();

    class Fonts {
      public:
        static ImFont *largeFont;
        static ImFont *mediumFont;
    };

} // namespace Bess::UI
