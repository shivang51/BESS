#include "scene/events/entity_event.h"

namespace Bess::Scene::Events {
    EntityEvent::EntityEvent(const EventType type, const EventData &data) : Event(type, data) {}

    EntityEvent EntityEvent::fromEventData(const EventData &data) {
        EntityEvent evt(data.type, data);
        return evt;
    }
} // namespace Bess::Scene::Events
