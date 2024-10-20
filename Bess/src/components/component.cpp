#include "components/component.h"
#include "pages/main_page/main_page_state.h"

namespace Bess::Simulator::Components {

    Component::Component(const uuids::uuid &uid, glm::vec3 position,
                         ComponentType type)
        : Bess::Scene::Entity(uid), m_type(type) {

        m_transform.setPosition(position);
    }

    Component::Component(const uuids::uuid &uid, int renderId, glm::vec3 position,
                         ComponentType type)
        : Bess::Scene::Entity(uid), m_type(type) {

        m_transform.setPosition(position);

        addEventListener<Scene::Events::EventType::mouseButton>([this](const std::shared_ptr<Scene::Events::EventData> data) {
            auto evt = Scene::Events::EventData::asPtr<Scene::Events::MouseButtonEventData>(data);
            if (evt->button == Scene::Events::MouseButton::left) {
                auto cb = std::any_cast<OnLeftClickCB>(m_events[ComponentEventType::leftClick]);
                cb(evt->position);
            }
        });
    }

    uuids::uuid Component::getId() const { return m_uid; }

    const glm::vec3 &Component::getPosition() { return m_transform.getPosition(); }

    void Component::setPosition(const glm::vec3 &pos) {
        m_transform.setPosition(pos);
    }

    std::string Component::getIdStr() const {
        return uuids::to_string(m_uid);
    }

    ComponentType Component::getType() const { return m_type; }

    void Component::simulate() {}

    std::string Component::getName() const {
        return m_name;
    }

    std::string Component::getRenderName() const {
        std::string name = m_name + " " + std::to_string(getRenderId());
        return name;
    }

    void Component::drawProperties() {}

    void Component::update() {
        auto mainPageState = Pages::MainPageState::getInstance();
        m_isSelected = mainPageState->isBulkIdPresent(m_uid);
        m_isHovered = mainPageState->getHoveredId() == getRenderId();

        Entity::update();
    }

} // namespace Bess::Simulator::Components
