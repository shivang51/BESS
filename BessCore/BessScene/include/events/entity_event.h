#pragma once

#include "events/event.h"
#include "events/event_type.h"

namespace Bess::Scene::Events {

    class EntityEvent : public Event {
      public:
        EntityEvent() = default;
        EntityEvent(const EventType type, const std::any &data);

        template <typename T>
        static EntityEvent fromEntEventData(const T &data) {
            const EventData d = data;
            EntityEvent evt(d.type, std::any(data));
            return evt;
        }
    };
} // namespace Bess::Scene::Events
