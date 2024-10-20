#pragma once

#include "../events/entity_event.h"
#include "scene/events/event_type.h"
#include "scene/transform/transform_2d.h"
#include "uuid.h"
#include <queue>

namespace Bess::Scene {
    typedef std::function<void(const std::shared_ptr<Events::EventData>)> EventListener;

    class Entity {
      public:
        Entity() = default;
        Entity(const uuids::uuid &uid);
        virtual ~Entity() = default;

        virtual void render() = 0;

        void update();
        void onEvent(const Events::EntityEvent &evt);

        int getRenderId() const;
        bool isEmptyEntity() const;

        template <Events::EventType T>
        void addEventListener(EventListener listener) {
            m_eventListeners[T].emplace_back(listener);
        }

      protected:
        uuids::uuid m_uid;
        Transform::Transform2D m_transform;
        bool m_isVisible = true;
        bool m_isSelected = false;
        bool m_isHovered = false;

        std::queue<Events::EntityEvent> m_events;
        std::unordered_map<Events::EventType, std::vector<EventListener>> m_eventListeners;

        int m_renderId = -1;
    };
} // namespace Bess::Scene
