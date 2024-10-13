#pragma once

#include "scene/events/event.h"
#include "scene/events/event_type.h"

namespace Bess::Scene::Events {

    class EntityEvent : public Event {
      public:
        EntityEvent(const EventType type, const EventData &data);
        static EntityEvent fromEventData(const EventData &data);
    };
} // namespace Bess::Scene::Events
