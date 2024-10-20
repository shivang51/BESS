#include "scene/events/event.h"

namespace Bess::Scene::Events {

    Event::Event(const EventType type, const std::any &data) : m_type(type), m_data(data) {}

    Event::~Event() {}

    EventType Event::getType() const { return m_type; }

} // namespace Bess::Scene::Events
