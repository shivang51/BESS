#include "components/slot.h"
#include "components/nand_gate.h"
#include "renderer/renderer.h"

#include "application_state.h"

#include "common/theme.h"
#include "ui.h"
#include <common/bind_helpers.h>

namespace Bess::Simulator::Components {

glm::vec3 connectedBg = {0.42f, 0.82f, 0.42f};

Slot::Slot(const UUIDv4::UUID &uid, const UUIDv4::UUID& parentUid, int id, ComponentType type)
    : Component(uid, id, { 0.f, 0.f, 0.f }, type), m_parentUid{ parentUid } {
    m_events[ComponentEventType::leftClick] =
        (OnLeftClickCB)BIND_FN_1(Slot::onLeftClick);
    m_events[ComponentEventType::mouseHover] =
        (VoidCB)BIND_FN(Slot::onMouseHover);
    m_state = DigitalState::low;
}

void Slot::update(const glm::vec3 &pos) { m_position = pos; }

void Slot::render() {
    Renderer2D::Renderer::circle(m_position, 8.f,
                                 m_highlightBorder
                                     ? Theme::selectedWireColor
                                     : Theme::componentBorderColor,
                                 m_renderId);
    Renderer2D::Renderer::circle(
        m_position, m_highlightBorder ? 6.f : 7.f,
        (connections.size() == 0) ? Theme::backgroundColor : connectedBg,
        m_renderId);
}

void Slot::onLeftClick(const glm::vec2 &pos) {
    if (ApplicationState::drawMode == DrawMode::none) {
        ApplicationState::connStartId = m_uid;
        ApplicationState::points.emplace_back(m_position);
        ApplicationState::drawMode = DrawMode::connection;
        return;
    }

    auto slot = ComponentsManager::components[ApplicationState::connStartId];

    if (ApplicationState::connStartId == m_uid || slot->getType() == m_type)
        return;

    ComponentsManager::addConnection(ApplicationState::connStartId, m_uid);

    ApplicationState::drawMode = DrawMode::none;
    ApplicationState::connStartId = ComponentsManager::emptyId;
    ApplicationState::points.pop_back();
}

void Slot::onMouseHover() { UI::setCursorPointer(); }

void Slot::onChange()
{
    if (m_type == ComponentType::outputSlot) {
        for (auto& slot : connections) {
            auto slotComp = (Slot*)ComponentsManager::components[slot].get();
            slotComp->setState(m_uid, m_state);
        }
    }
    else {
        auto &parent = ComponentsManager::components[m_parentUid];
        switch (parent->getType())
        {
        case ComponentType::gate: {
            auto parentPtr = (Components::NandGate*)parent.get();
            parentPtr->simulate();
        }break;
        default:
            break;
        }
    }
}

void Slot::addConnection(const UUIDv4::UUID &uid) {
    if (isConnectedTo(uid)) return;
    connections.emplace_back(uid);
    auto slotComp = (Slot*)ComponentsManager::components[uid].get();
    slotComp->setState(m_uid, m_state);
}

bool Slot::isConnectedTo(const UUIDv4::UUID& uId) {
    for (auto& conn : connections) {
        if (conn == uId) return true;
    }
    return false;
}

void Slot::highlightBorder(bool highlight) { m_highlightBorder = highlight; }

Simulator::DigitalState Slot::getState() const
{
    return m_state;
}

void Slot::setState(const UUIDv4::UUID& uid, Simulator::DigitalState state)
{
    if (m_type == ComponentType::inputSlot) {
        stateChangeHistory[uid] = state == DigitalState::high; 
        if (state == DigitalState::low) {
            for (auto& ent : stateChangeHistory) {
                if (!ent.second) continue;
                state = DigitalState::high;
                break;
            }
        }
    }

    if (m_state == state) return;
    m_state = state;
    onChange();
}

DigitalState Slot::flipState() {
    if (DigitalState::high == m_state) {
        m_state = DigitalState::low;
    }
    else {
        m_state = DigitalState::high;
    }
    onChange();
    return m_state;
}

const UUIDv4::UUID& Slot::getParentId()
{
    return m_parentUid;
}
} // namespace Bess::Simulator::Components
