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

        template <typename T>
        T as() {
            return static_cast<T>(this);
        }
    };

    struct MouseMoveEventData : EventData {
        glm::vec2 position;
        MouseMoveEventData() {
            type = EventType::mouseMove;
        };
    };

    struct MouseEnterEventData : EventData {
        MouseEnterEventData() {
            type = EventType::mouseEnter;
        };

        glm::vec2 position;
    };

    struct MouseLeaveEventData : EventData {
        MouseLeaveEventData() {
            type = EventType::mouseLeave;
        };
        glm::vec2 position;
    };

    struct MouseButtonEventData : EventData {
        MouseButtonEventData() {
            type = EventType::mouseButton;
        };
        MouseButton button;
        glm::vec2 position;
        bool pressed;
    };

    struct MouseWheelEventData : EventData {
        MouseWheelEventData() {
            type = EventType::mouseWheel;
        };
        glm::vec2 offset;
    };

    struct KeyPressEventData : EventData {
        KeyPressEventData() {
            type = EventType::keyPress;
        };
        int key;
    };

    struct KeyReleaseEventData : EventData {
        KeyReleaseEventData() {
            type = EventType::keyRelease;
        };
        int key;
    };

    struct ResizeEventData : EventData {
        ResizeEventData() {
            type = EventType::resize;
        };
        glm::vec2 size;
    };
} // namespace Bess::Scene::Events
