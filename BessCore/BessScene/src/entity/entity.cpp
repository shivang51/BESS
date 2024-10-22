
#include "entity/entity.h"
#include "events/event_type.h"

namespace Bess::Scene {
    Entity::Entity(const uuids::uuid &uid) : m_uid(uid) {
        m_renderId++;
        m_events = std::queue<Events::EntityEvent>();
        m_eventListeners = std::unordered_map<Events::EventType, std::vector<EventListener>>();
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

            auto type = evt.getType();
            switch (type) {
            case Events::EventType::mouseButton: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::MouseButtonEventData>());
                }
            } break;
            case Events::EventType::mouseMove: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::MouseMoveEventData>());
                }
            } break;
            case Events::EventType::mouseWheel: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::MouseWheelEventData>());
                }
            } break;
            case Events::EventType::keyPress: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::KeyPressEventData>());
                }
            } break;
            case Events::EventType::keyRelease: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::KeyReleaseEventData>());
                }
            } break;
            case Events::EventType::mouseEnter: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::MouseEnterEventData>());
                }
            } break;
            case Events::EventType::mouseLeave: {
                for (auto &cb : m_eventListeners[type]) {
                    cb(evt.getDataPtr<Events::MouseLeaveEventData>());
                }
            } break;
            default:
                break;
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
