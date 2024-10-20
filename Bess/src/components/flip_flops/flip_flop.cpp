#include "components/flip_flops/flip_flop.h"
#include "common/helpers.h"
#include "components/component.h"
#include "components/flip_flops/flip_flops.h"
#include "components/flip_flops/jk_flip_flop.h"
#include "components/slot.h"
#include "components_manager/components_manager.h"
#include "pages/main_page/main_page_state.h"
#include "scene/renderer/renderer.h"
#include "settings/viewport_theme.h"
#include "uuid.h"

namespace Bess::Simulator::Components {
    FlipFlop::FlipFlop(const uuids::uuid &uid, int renderId, glm::vec3 position, std::vector<uuids::uuid> inputSlots)
        : Component(uid, renderId, position, ComponentType::flipFlop), m_inputSlots(inputSlots) {

        auto clkRenderId = ComponentsManager::getNextRenderId();
        m_clockSlot = Common::Helpers::UUIDGenerator().getUUID();
        ComponentsManager::components[m_clockSlot] = std::make_shared<Components::Slot>(m_clockSlot, uid, clkRenderId, ComponentType::inputSlot);
        ComponentsManager::addCompIdToRId(clkRenderId, m_clockSlot);
        ComponentsManager::addRenderIdToCId(clkRenderId, m_clockSlot);

        for (int i = 0; i < 2; i++) {
            auto sid = Common::Helpers::uuidGenerator.getUUID();
            auto renderId = ComponentsManager::getNextRenderId();
            ComponentsManager::components[sid] = std::make_shared<Components::Slot>(sid, uid, renderId, ComponentType::outputSlot);
            ComponentsManager::addCompIdToRId(renderId, sid);
            ComponentsManager::addRenderIdToCId(renderId, sid);
            m_outputSlots.push_back(sid);
        }

        m_events[ComponentEventType::leftClick] = (OnLeftClickCB)[this](auto pos) {
            Pages::MainPageState::getInstance()->setBulkId(m_uid);
        };

        m_transform.setScale({140.f, 100.f});
    }

    FlipFlop::FlipFlop(const uuids::uuid &uid, int renderId, const glm::vec3 &position, const std::vector<uuids::uuid> &inputSlots, const std::string &name, const std::vector<uuids::uuid> &outputSlots, const uuids::uuid &clockSlot)
        : Component(uid, renderId, position, ComponentType::flipFlop) {
        m_name = name;
        m_inputSlots = inputSlots;
        m_outputSlots = outputSlots;
        m_clockSlot = clockSlot;
        m_events[ComponentEventType::leftClick] = (OnLeftClickCB)[this](auto pos) {
            Pages::MainPageState::getInstance()->setBulkId(m_uid);
        };
        m_transform.setScale({140.f, 100.f});
    }

    void FlipFlop::drawBackground(const glm::vec4 &borderThicknessPx, float rPx, float headerHeight, const glm::vec2 &gateSize) {
        auto borderColor = m_isSelected ? ViewportTheme::selectedCompColor : ViewportTheme::componentBorderColor;
        auto pos = m_transform.getPosition();
        auto color = ViewportTheme::componentBGColor;

        Renderer2D::Renderer::quad(
            {pos.x, pos.y + headerHeight / 2.f, pos.z},
            {gateSize.x, gateSize.y - headerHeight},
            color,
            m_renderId,
            glm::vec4(0.f, 0.f, rPx, rPx),
            true,
            borderColor,
            glm::vec4(0.f, borderThicknessPx.y, borderThicknessPx.z, borderThicknessPx.w));

        auto headerPos = pos;
        headerPos.y = pos.y - ((gateSize.y / 2) - (headerHeight / 2.f));

        // header
        static glm::vec4 headerColor = ViewportTheme::compHeaderColor;
        Renderer2D::Renderer::quad(
            headerPos,
            {gateSize.x, headerHeight},
            headerColor,
            m_renderId,
            glm::vec4(rPx, rPx, 0.f, 0.f),
            true,
            borderColor,
            glm::vec4(borderThicknessPx.x, borderThicknessPx.y, 0.f, borderThicknessPx.w));
    }

    void FlipFlop::render() {
        float rPx = 16.f;

        glm::vec4 borderThicknessPx({1.f, 1.f, 1.f, 1.f});
        float headerHeight = 20.f;
        glm::vec2 slotRowPadding = {4.0f, 4.f};
        glm::vec2 gatePadding = {4.0f, 4.f};
        float labelGap = 8.f;
        float rowGap = 4.f;
        auto sampleCharSize = Renderer2D::Renderer::getCharRenderSize('Z', 12.f);
        float sCharHeight = sampleCharSize.y;
        float rowHeight = (slotRowPadding.y * 2) + sCharHeight;

        float maxSlotsCount = std::max(m_inputSlots.size() + 1, m_outputSlots.size());
        float maxWidth = sampleCharSize.x * 3;

        maxWidth += labelGap + 8.f + sampleCharSize.x + 16.f + (gatePadding.x * 2.f);

        auto gateSize_ = m_transform.getScale();

        if (maxWidth > gateSize_.x) {
            gateSize_.x += maxWidth - gateSize_.x + 16.f;
        }

        gateSize_.y = headerHeight + (rowHeight + rowGap) * maxSlotsCount + 4.f;

        drawBackground(borderThicknessPx, rPx, headerHeight, gateSize_);

        char startChar = 'A';
        if (m_name == JKFlipFlop::name) {
            startChar = 'J';
        } else if (m_name == DFlipFlop::name) {
            startChar = 'D';
        }

        auto leftCornerPos = Common::Helpers::GetLeftCornerPos(m_transform.getPosition(), gateSize_);

        {
            glm::vec3 inpSlotRowPos = {leftCornerPos.x + 8.f + gatePadding.x, leftCornerPos.y + headerHeight + 4.f, leftCornerPos.z};

            for (int i = 0; i < m_inputSlots.size(); i++) {
                char ch = startChar + i;

                auto height = (slotRowPadding.y * 2) + sCharHeight;

                auto pos = inpSlotRowPos;
                pos.y += height / 2.f;
                pos.z += ComponentsManager::zIncrement / 10;

                Slot *slot = (Slot *)Simulator::ComponentsManager::components[m_inputSlots[i]].get();
                slot->update(pos, {labelGap, 0.f}, std::string(1, ch));
                slot->render();

                inpSlotRowPos.y += height + rowGap;

                if ((i + 1) == (m_inputSlots.size() / 2)) {
                    pos = inpSlotRowPos;
                    pos.y += height / 2.f;
                    pos.z += ComponentsManager::zIncrement / 10;
                    auto slot = ComponentsManager::getComponent<Slot>(m_clockSlot);
                    slot->update(pos, {labelGap, 0.f}, "CLK");
                    slot->render();
                    inpSlotRowPos.y += height + rowGap;
                }
            }
        }

        {
            glm::vec3 outSlotRowPos = {leftCornerPos.x + gateSize_.x - 8.f - gatePadding.x, leftCornerPos.y + headerHeight + 4.f, leftCornerPos.z};

            for (int i = 0; i < m_outputSlots.size(); i++) {
                auto height = rowHeight;

                auto pos = outSlotRowPos;
                pos.y += height / 2.f;
                pos.z += ComponentsManager::zIncrement / 10;

                Slot *slot = (Slot *)Simulator::ComponentsManager::components[m_outputSlots[i]].get();
                slot->update(pos, {-labelGap, 0.f}, (i == 0) ? "Q" : "Q'");
                slot->render();

                outSlotRowPos.y += height + rowGap;
            }
        }

        Renderer2D::Renderer::text(m_name, leftCornerPos + glm::vec3({8.f, 8.f + (sCharHeight / 2.f), ComponentsManager::zIncrement}), 11.f, ViewportTheme::textColor, m_renderId);
    }

    void FlipFlop::update() {
        Component::update();
    }

    void FlipFlop::generate(const glm::vec3 &pos) {}

    void FlipFlop::deleteComponent() {
        for (auto &slot : m_inputSlots) {
            auto comp = ComponentsManager::components[slot];
            comp->deleteComponent();
        }

        for (auto &slot : m_outputSlots) {
            auto comp = ComponentsManager::components[slot];
            comp->deleteComponent();
        }

        auto comp = ComponentsManager::components[m_clockSlot];
        comp->deleteComponent();
    }

    void FlipFlop::fromJson(const nlohmann::json &data) {
        auto type = data["type"].get<int>();
        auto uid = Common::Helpers::strToUUID(data["uid"]);
        auto pos = Common::Helpers::DecodeVec3(data["pos"]);
        auto name = data["name"].get<std::string>();

        std::vector<uuids::uuid> inputSlots;
        for (const auto &slot : data["inputSlots"]) {
            auto id = Slot::fromJson(slot, uid);
            inputSlots.emplace_back(id);
        }

        std::vector<uuids::uuid> outputSlots;
        for (const auto &slot : data["outputSlots"]) {
            auto id = Slot::fromJson(slot, uid);
            outputSlots.emplace_back(id);
        }

        auto sid = Slot::fromJson(data["clockSlot"], uid);

        float renderId = ComponentsManager::getNextRenderId();
        ComponentsManager::renderComponents.emplace_back(uid);
        ComponentsManager::addCompIdToRId(renderId, uid);
        ComponentsManager::addRenderIdToCId(renderId, uid);

        if (name == JKFlipFlop::name)
            ComponentsManager::components[uid] = std::make_shared<JKFlipFlop>(uid, renderId, pos, inputSlots, outputSlots, sid);
        // else if (name == DFlipFlop::name)
        //     ComponentsManager::components[uid] = std::make_shared<DFlipFlop>(uid, ComponentsManager::getNextRenderId(), pos, inputSlots, outputSlots, clkSlot);
    }

    nlohmann::json FlipFlop::toJson() {
        nlohmann::json data;
        data["type"] = (int)m_type;
        data["uid"] = Common::Helpers::uuidToStr(m_uid);
        data["pos"] = Common::Helpers::EncodeVec3(m_transform.getPosition());
        data["name"] = m_name;

        nlohmann::json inputSlots;
        for (const auto &slotId : m_inputSlots) {
            auto slot = ComponentsManager::getComponent<Slot>(slotId);
            inputSlots.push_back(slot->toJson());
        }
        data["inputSlots"] = inputSlots;

        nlohmann::json outputSlots;
        for (const auto &slotId : m_outputSlots) {
            auto slot = ComponentsManager::getComponent<Slot>(slotId);
            outputSlots.push_back(slot->toJson());
        }
        data["outputSlots"] = outputSlots;

        data["clockSlot"] = ComponentsManager::getComponent<Slot>(m_clockSlot)->toJson();

        return data;
    }
} // namespace Bess::Simulator::Components
