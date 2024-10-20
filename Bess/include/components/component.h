#pragma once
#include "components_manager/component_type.h"
#include "ext/vector_float2.hpp"
#include "ext/vector_float3.hpp"
#include "scene/entity/entity.h"
#include "scene/transform/transform_2d.h"

#include "uuid.h"

#include <any>
#include <functional>
#include <queue>
#include <unordered_map>

namespace Bess::Simulator::Components {

    enum class ComponentEventType {
        none = -1,
        leftClick,
        rightClick,
        mouseEnter,
        mouseLeave,
        mouseHover,
        focus,
        focusLost,
        dragStart,
        drag,
        dragEnd,
    };

    struct ComponentEventData {
        ComponentEventType type;
        glm::vec2 pos;
    };

    typedef std::function<void(const glm::vec2 &pos)> OnLeftClickCB;
    typedef std::function<void(const glm::vec2 &pos)> OnRightClickCB;
    typedef std::function<void(const glm::vec2 &pos)> Vec2CB;
    typedef std::function<void()> VoidCB;

    class Component : public Scene::Entity {
      public:
        Component() = default;
        Component(const uuids::uuid &uid, glm::vec3 position, ComponentType type);
        Component(const uuids::uuid &uid, int renderId, glm::vec3 position, ComponentType type);
        virtual ~Component() = default;

        uuids::uuid getId() const;
        std::string getIdStr() const;

        const glm::vec3 &getPosition();
        void setPosition(const glm::vec3 &pos);

        ComponentType getType() const;

        virtual void render() = 0;

        virtual void update();

        virtual void generate(const glm::vec3 &pos) = 0;

        virtual void deleteComponent() = 0;

        virtual std::string getName() const;
        virtual std::string getRenderName() const;

        virtual void drawProperties();

        virtual void simulate();

      protected:
        ComponentType m_type = ComponentType::none;
        std::string m_name = "Unknown";
        std::unordered_map<ComponentEventType, std::any> m_events = {};
    };
} // namespace Bess::Simulator::Components
