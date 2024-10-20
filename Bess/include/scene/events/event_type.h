#pragma once

#include "ext/vector_float2.hpp"
#include "glm.hpp"

namespace Bess::Scene::Events {
    enum class MouseButton {
        left,
        middle,
        right
    };

    enum class EventType {
        mouseMove,
        mouseEnter,
        mouseLeave,
        mouseButton,
        mouseWheel,
        keyPress,
        keyRelease,
        resize
    };

    struct EventData {
        EventType type;

        explicit EventData(const EventType type) {
            this->type = type;
        }
    };

    struct MouseMoveEventData : EventData {
        glm::vec2 position;
        MouseMoveEventData() : EventData(EventType::mouseMove), position(){};
    };

    struct MouseEnterEventData : EventData {
        MouseEnterEventData() : EventData(EventType::mouseEnter), position(){};

        glm::vec2 position;
    };

    struct MouseLeaveEventData : EventData {
        MouseLeaveEventData() : EventData(EventType::mouseLeave), position(){};
        glm::vec2 position;
    };

    struct MouseButtonEventData : EventData {
        MouseButtonEventData() : EventData(EventType::mouseButton), button(), position(), pressed(false){};
        MouseButton button;
        glm::vec2 position;
        bool pressed;
    };

    struct MouseWheelEventData : EventData {
        MouseWheelEventData() : EventData(EventType::mouseWheel), offset(){};
        glm::vec2 offset;
    };

    struct KeyPressEventData : EventData {
        KeyPressEventData() : EventData(EventType::keyPress), key(0){};
        int key;
    };

    struct KeyReleaseEventData : EventData {
        KeyReleaseEventData(): EventData(EventType::keyRelease), key(0){};
        int key;
    };

    struct ResizeEventData : EventData {
        ResizeEventData() : EventData(EventType::resize) {
            size = {};
        };
        glm::vec2 size{};
    };
} // namespace Bess::Scene::Events
