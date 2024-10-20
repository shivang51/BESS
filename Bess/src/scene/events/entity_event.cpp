#include "scene/events/entity_event.h"

namespace Bess::Scene::Events {
    EntityEvent::EntityEvent(const EventType type, const std::any &data) : Event(type, data) {}
} // namespace Bess::Scene::Events
