#pragma once

#include "event_type.h"
#include <any>
#include <memory>

namespace Bess::Scene::Events {
    class Event {
      public:
        Event() = default;
        Event(EventType type, const std::any &data);
        ~Event();

        template <typename T>
        T getData() const {
            return std::any_cast<T>(m_data);
        }

        template <typename T>
        std::shared_ptr<T> getDataPtr() const {
            auto data = std::any_cast<T>(m_data);
            return std::make_shared<T>(data);
        }

        EventType getType() const;

        template <typename T>
        static Event fromEventData(const T &data) {
            const EventData d = data;
            Event evt(d.type, std::any(data));
            return evt;
        }

      protected:
        EventType m_type;
        std::any m_data;
    };
} // namespace Bess::Scene::Events
