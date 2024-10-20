#pragma once

#include "../events/entity_event.h"
#include "scene/events/event_type.h"
#include "scene/transform/transform_2d.h"
#include "uuid.h"
#include <queue>

namespace Bess::Scene {
    typedef std::function<void(const Events::EventData &)> EventListener;

    class Entity {
      public:
        Entity();
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
        int m_renderId = -1;
        Transform::Transform2D m_transform;
        bool m_visible = true;
        bool m_selected = false;
        bool m_hovered = false;

        std::queue<Events::EntityEvent> m_events;
        std::unordered_map<Events::EventType, std::vector<EventListener>> m_eventListeners;
    };
} // namespace Bess::Scene
