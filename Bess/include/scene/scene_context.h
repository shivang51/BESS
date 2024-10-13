#pragma once

#include "renderer/gl/framebuffer.h"
#include "scene/entity/entity.h"
#include "scene/events/event.h"
#include <queue>
#include <vector>

namespace Bess::Scene {
    class SceneContext {
      public:
        SceneContext() = default;
        ~SceneContext();

        void update();
        void render();

        void init();

        unsigned int getTextureId();

        void onEvent(const Events::Event &evt);

        void beginScene();
        void endScene();

        void addEntity(std::shared_ptr<Entity> entity);
        const std::vector<std::shared_ptr<Entity>> &getEntities() const;

      private:
        std::unique_ptr<Gl::FrameBuffer> m_msaaFramebuffer;
        std::unique_ptr<Gl::FrameBuffer> m_framebuffer;
        glm::vec2 m_size;
        std::queue<Events::Event> m_events;
        std::vector<std::shared_ptr<Entity>> m_entities;
        std::unordered_map<int, std::shared_ptr<Entity>> m_renderIdToEntity;

      private: // event handlers
        void handleMouseButton(const Events::MouseButtonEventData &data);
        void handleMouseMove(const Events::MouseMoveEventData &data);
        void handleMouseWheel(const Events::MouseWheelEventData &data);
        void handleKeyPress(const Events::KeyPressEventData &data);
        void handleKeyRelease(const Events::KeyReleaseEventData &data);
        void handleMouseEnter(const Events::MouseEnterEventData &data);
        void handleMouseLeave(const Events::MouseLeaveEventData &data);
        void handleResize(const Events::ResizeEventData &data);

        // mouse button event handlers
        void handleLeftMouseButton(const Events::MouseButtonEventData &data);
        void handleRightMouseButton(const Events::MouseButtonEventData &data);
        void handleMiddleMouseButton(const Events::MouseButtonEventData &data);

      private:
        // gets entity at given pos
        std::shared_ptr<Entity> getEntityAt(const glm::vec2 &pos);
    };
} // namespace Bess::Scene
