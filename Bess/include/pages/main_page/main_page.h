#pragma once

#include "camera.h"
#include "events/application_event.h"
#include "pages/main_page/main_page_state.h"
#include "pages/page.h"
#include "scene/scene_context.h"
#include "ui/ui_main/ui_main.h"
#include "window.h"

#include <memory>
#include <vector>

namespace Bess::Pages {
    class MainPage : public Page {
      public:
        MainPage(std::shared_ptr<Window> parentWindow);

        static std::shared_ptr<Page> getInstance(const std::shared_ptr<Window> &parentWindow = nullptr);

        static std::shared_ptr<MainPage> getTypedInstance(std::shared_ptr<Window> parentWindow = nullptr);

        void draw() override;

        void update(const std::vector<ApplicationEvent> &events) override;

        void drawScene();

        glm::vec2 getCameraPos();

        std::shared_ptr<Window> getParentWindow();

      private:
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<Window> m_parentWindow;
        std::shared_ptr<Scene::SceneContext> m_sceneContext;

        // event handlers
      private:
        void onMouseWheel(double x, double y);
        void onLeftMouse(bool pressed);
        void onRightMouse(bool pressed);
        void onMiddleMouse(bool pressed);
        void onMouseMove(double x, double y);

        bool isCursorInViewport();
        void finishDragging();

        glm::vec2 getViewportMousePos();
        glm::vec2 getNVPMousePos();

        template <typename T>
        void addSceneEvent(const T &data) {
            if (!UI::UIMain::state.isViewportFocused)
                return;
            m_sceneContext->onEvent(Scene::Events::Event::fromEventData<T>(data));
        }

      private:
        bool m_leftMousePressed = false;
        bool m_rightMousePressed = false;
        bool m_middleMousePressed = false;

        std::chrono::time_point<std::chrono::steady_clock> m_lastUpdateTime;

        std::shared_ptr<MainPageState> m_state;
    };
} // namespace Bess::Pages
