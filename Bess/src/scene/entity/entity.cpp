
#include "scene/entity/entity.h"
#include "scene/events/event_type.h"

namespace Bess::Scene {
    Entity::Entity() {
        m_renderId = -1;
        m_events = std::queue<Events::EntityEvent>();
        m_eventListeners = std::unordered_map<Events::EventType, std::vector<std::function<void(const Events::EventData &)>>>();
        m_transform = Transform::Transform2D();
    }

    void Entity::onEvent(const Events::EntityEvent &evt) {
        m_events.push(evt);
    }

    void Entity::update() {
        while (!m_events.empty()) {
            const auto &evt = m_events.front();
            // Skiped events
            if (evt.getType() == Events::EventType::resize) {
                m_events.pop();
                continue;
            }

            for (auto &cb : m_eventListeners[evt.getType()]) {
                cb(evt.getData<Events::MouseButtonEventData>());
            }

            m_events.pop();
        }
    }

    int Entity::getRenderId() const {
        return m_renderId;
    }

    bool Entity::isEmptyEntity() const {
        return m_renderId < 0;
    }
} // namespace Bess::Scene
