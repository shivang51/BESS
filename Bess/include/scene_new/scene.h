#pragma once

#include "camera.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include "events/application_event.h"
#include "glm.hpp"
#include "scene/renderer/gl/framebuffer.h"

namespace Bess::Canvas {
    class Scene {
      public:
        Scene();
        ~Scene();

      public:
        void reset();
        void render();
        void update(const std::vector<ApplicationEvent> &events);

        unsigned int getTextureId();

        void beginScene();
        void endScene();

      public:
        glm::vec2 getCameraPos();
        float getCameraZoom();
        void setZoom(float value);

        void resize(const glm::vec2 &size);
        entt::registry &getEnttRegistry();
        const glm::vec2 &getSize();

      private:
        void onMouseMove(const glm::vec2 &pos);
        void onLeftMouse(bool isPressed);
        glm::vec2 getNVPMousePos(const glm::vec2 &mousePos);
        glm::vec2 getViewportMousePos(const glm::vec2 &mousePos);
        bool isCursorInViewport(const glm::vec2 &pos);

      private:
        std::unique_ptr<Gl::FrameBuffer> m_msaaFramebuffer;
        std::unique_ptr<Gl::FrameBuffer> m_normalFramebuffer;
        glm::vec2 m_size, m_mousePos;
        std::shared_ptr<Camera> m_camera;

        bool m_isLeftMousePressed = false;

      private:
        entt::registry m_registry;
        entt::entity m_hoveredEntiy;
    };
} // namespace Bess::Canvas
