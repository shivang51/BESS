#include "pages/main_page/main_page.h"
#include "GLFW/glfw3.h"
#include "common/types.h"
#include "components/clock.h"
#include "components/connection.h"
#include "components_manager/components_manager.h"
#include "events/application_event.h"
#include "ext/matrix_transform.hpp"
#include "ext/vector_float2.hpp"
#include "ext/vector_float3.hpp"
#include "ext/vector_float4.hpp"
#include "pages/page_identifier.h"
#include "scene/events/event_type.h"
#include "scene/renderer/renderer.h"
#include "settings/viewport_theme.h"
#include "simulator/simulator_engine.h"
#include "ui/ui_main/ui_main.h"
#include <set>

using namespace Bess::Renderer2D;

namespace Bess::Pages {
    class DummyEntity : public Scene::Entity {
      public:
        DummyEntity() : Scene::Entity() {
            m_renderId = 0;
        }

        ~DummyEntity() override = default;

        void render() override {
            Renderer2D::Renderer::quad({0.f, 0.f, 0.f}, {10.f, 10.f}, {1.f, 0.f, 0.f, 1.f}, -1.f);
        }
    };

    std::shared_ptr<Page>
    MainPage::getInstance(const std::shared_ptr<Window> &parentWindow) {
        static auto instance = std::make_shared<MainPage>(parentWindow);
        return instance;
    }

    std::shared_ptr<MainPage> MainPage::getTypedInstance(std::shared_ptr<Window> parentWindow) {
        const auto instance = getInstance(parentWindow);
        return std::dynamic_pointer_cast<MainPage>(instance);
    }

    MainPage::MainPage(std::shared_ptr<Window> parentWindow) : Page(PageIdentifier::MainPage) {
        if (m_parentWindow == nullptr && parentWindow == nullptr) {
            throw std::runtime_error("MainPage: parentWindow is nullptr. Need to pass a parent window.");
        }
        m_camera = std::make_shared<Camera>(800, 600);
        m_parentWindow = parentWindow;

        UI::UIMain::state.cameraZoom = Camera::defaultZoom;
        m_sceneContext = std::make_shared<Scene::SceneContext>();
        m_sceneContext->init();
        auto ent = std::make_shared<DummyEntity>();
        m_sceneContext->addEntity(ent);

        UI::UIMain::state.viewportTexture = m_sceneContext->getTextureId();
        m_state = MainPageState::getInstance();
    }

    void MainPage::draw() {
        m_sceneContext->beginScene();
        Renderer2D::Renderer::begin(m_camera);
        m_sceneContext->render();
        Renderer2D::Renderer::end();
        m_sceneContext->endScene();
        UI::UIMain::draw();
    }

    void MainPage::update(const std::vector<ApplicationEvent> &events) {
        if (m_sceneContext->getSize() != UI::UIMain::state.viewportSize) {
            Scene::Events::ResizeEventData data;
            data.size = UI::UIMain::state.viewportSize;
            auto evt = Scene::Events::Event::fromEventData<Scene::Events::ResizeEventData>(data);
            m_sceneContext->onEvent(evt);
            m_camera->resize(UI::UIMain::state.viewportSize.x, UI::UIMain::state.viewportSize.y);
        }

        if (UI::UIMain::state.cameraZoom != m_camera->getZoom()) {
            // auto mp = getViewportMousePos();
            // mp = m_mousePos;
            // m_camera->zoomToPoint({ mp.x, mp.y},
            // UI::UIMain::state.cameraZoom);
            m_camera->setZoom(UI::UIMain::state.cameraZoom);
        }

        auto &dragData = m_state->getDragData();

        // if (!dragData.isDragging) {
        //     auto viewportMousePos = getViewportMousePos();
        //     viewportMousePos.y = UI::UIMain::state.viewportSize.y - viewportMousePos.y;
        //     int hoverId = m_normalFramebuffer->readIntFromColAttachment(1, static_cast<int>(viewportMousePos.x), static_cast<int>(viewportMousePos.y));
        //     if (Simulator::ComponentsManager::renderComponents.size() == 0 && hoverId != -1) {
        //         std::cout << "No render components found but hover id was " << hoverId << std::endl;
        //         hoverId = -1;
        //     }
        //     m_state->setHoveredId(hoverId);
        // }
        //
        // if (m_state->shouldReadBulkIds()) {
        //     m_state->setReadBulkIds(false);
        //     m_state->clearBulkIds();
        //     auto end = getViewportMousePos();
        //     auto start = dragData.vpMousePos;
        //     auto size = end - start;
        //     glm::vec2 pos = {std::min(start.x, end.x), std::max(start.y, end.y)};
        //     size = glm::abs(size);
        //
        //     int w = (int)size.x;
        //     int h = (int)size.y;
        //     int x = pos.x, y = UI::UIMain::state.viewportSize.y - pos.y;
        //
        //     auto ids = m_normalFramebuffer->readIntsFromColAttachment(1, x, y, w, h);
        //
        //     if (ids.size() == 0)
        //         return;
        //
        //     std::set<int> uniqueIds(ids.begin(), ids.end());
        //
        //     for (auto &id : uniqueIds) {
        //         if (!Simulator::ComponentsManager::isRenderComponent(id))
        //             continue;
        //         m_state->addBulkId(Simulator::ComponentsManager::renderIdToCid(id));
        //     }
        //     m_state->clearDragData();
        // }
        //
        for (auto &event : events) {
            switch (event.getType()) {
            case ApplicationEventType::MouseWheel: {
                const auto data = event.getData<ApplicationEvent::MouseWheelData>();
                onMouseWheel(data.x, data.y);
                Scene::Events::MouseWheelEventData e;
                e.offset = {data.x, data.y};
                addSceneEvent(e);
            } break;
            case ApplicationEventType::MouseButton: {
                const auto data = event.getData<ApplicationEvent::MouseButtonData>();
                Scene::Events::MouseButtonEventData e;
                e.position = getViewportMousePos();
                e.pressed = data.pressed;
                if (data.button == MouseButton::left) {
                    onLeftMouse(data.pressed);
                    e.button = Scene::Events::MouseButton::left;
                } else if (data.button == MouseButton::right) {
                    onRightMouse(data.pressed);
                    e.button = Scene::Events::MouseButton::right;
                } else if (data.button == MouseButton::middle) {
                    onMiddleMouse(data.pressed);
                    e.button = Scene::Events::MouseButton::middle;
                }
                addSceneEvent(e);
            } break;
            case ApplicationEventType::MouseMove: {
                const auto data = event.getData<ApplicationEvent::MouseMoveData>();
                onMouseMove(data.x, data.y);
                Scene::Events::MouseMoveEventData e;
                e.position = {data.x, data.y};
                addSceneEvent(e);
            } break;
            case ApplicationEventType::KeyPress: {
                const auto data = event.getData<ApplicationEvent::KeyPressData>();
                m_state->setKeyPressed(data.key, true);
                Scene::Events::KeyPressEventData e;
                e.key = data.key;
                addSceneEvent(e);
            } break;
            case ApplicationEventType::KeyRelease: {
                const auto data = event.getData<ApplicationEvent::KeyReleaseData>();
                m_state->setKeyPressed(data.key, false);
                Scene::Events::KeyReleaseEventData e;
                e.key = data.key;
                addSceneEvent(e);
            } break;
            default:
                break;
            }
        }

        m_sceneContext->update();

        // key board bindings
        // {
        //     if (m_state->isKeyPressed(GLFW_KEY_DELETE)) {
        //         for (auto &compId : m_state->getBulkIds()) {
        //             if (compId == Simulator::ComponentsManager::emptyId)
        //                 continue;
        //             Simulator::ComponentsManager::deleteComponent(compId);
        //         }
        //     }
        //
        //     if (m_state->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        //         if (m_state->isKeyPressed(GLFW_KEY_A)) {
        //             m_state->clearBulkIds();
        //             for (auto &compId : Simulator::ComponentsManager::renderComponents)
        //                 m_state->addBulkId(compId);
        //         }
        //     }
        // }
        //
        // if (isCursorInViewport() && m_state->getHoveredId() != -1) {
        //     auto &cid = Simulator::ComponentsManager::renderIdToCid(m_state->getHoveredId());
        //     Simulator::Components::ComponentEventData e;
        //     e.type = Simulator::Components::ComponentEventType::mouseHover;
        //     Simulator::ComponentsManager::components[cid]->onEvent(e);
        // }

        // for (auto &comp : Simulator::ComponentsManager::components) {
        //     if (comp.second->getType() == Simulator::ComponentType::clock) {
        //         const auto clockCmp = std::dynamic_pointer_cast<Simulator::Components::Clock>(comp.second);
        //         clockCmp->update();
        //     } else if (comp.second->getType() == Simulator::ComponentType::connection) {
        //         const auto connCmp = std::dynamic_pointer_cast<Simulator::Components::Connection>(comp.second);
        //         connCmp->update();
        //     } else if (comp.second->getType() != Simulator::ComponentType::connectionPoint) {
        //         comp.second->update();
        //     }
        // }

        if (!m_state->isSimulationPaused()) {
            Simulator::Engine::Simulate();
        }
    }

    glm::vec2 MainPage::getCameraPos() {
        return m_camera->getPos();
    }

    std::shared_ptr<Window> MainPage::getParentWindow() {
        return m_parentWindow;
    }

    void MainPage::onMouseWheel(double x, double y) {
        if (!isCursorInViewport())
            return;

        if (m_state->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
            const float delta = static_cast<float>(y) * 0.1f;
            UI::UIMain::state.cameraZoom += delta;
            if (UI::UIMain::state.cameraZoom < Camera::zoomMin) {
                UI::UIMain::state.cameraZoom = Camera::zoomMin;
            } else if (UI::UIMain::state.cameraZoom > Camera::zoomMax) {
                UI::UIMain::state.cameraZoom = Camera::zoomMax;
            }
        } else {
            glm::vec2 dPos = {x, y};
            dPos *= 10 / m_camera->getZoom() * -1;
            m_camera->incrementPos(dPos);
        }
    }

    void MainPage::onLeftMouse(bool pressed) {
        if (!isCursorInViewport())
            return;

        if (pressed != m_leftMousePressed) {
            m_lastUpdateTime = std::chrono::steady_clock::now();
        }
        m_leftMousePressed = pressed;

        if (!pressed) {
            auto dragData = m_state->getDragData();
            if (m_state->getDrawMode() == UI::Types::DrawMode::selectionBox) {
                m_state->setReadBulkIds(true);
                m_state->setDrawMode(UI::Types::DrawMode::none);
                dragData.isDragging = false;
                m_state->setDragData(dragData);
            } else if (dragData.isDragging && m_state->getDrawMode() == UI::Types::DrawMode::none) {
                auto cid = m_state->getBulkIdAt(0);
                auto comp = Simulator::ComponentsManager::getComponent(cid);
                comp->setPosition({glm::vec2(comp->getPosition()), dragData.orinalEntPos.z});
                m_state->clearDragData();
            }
            return;
        }
        auto &cid = Simulator::ComponentsManager::renderIdToCid(m_state->getHoveredId());

        if (Simulator::ComponentsManager::emptyId == cid) {
            if (m_state->getDrawMode() == UI::Types::DrawMode::connection) {
                if (m_state->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                    m_state->getPointsRef().emplace_back(glm::vec3(getNVPMousePos(), 0.f));
                } else {
                    m_state->setConnStartId(Simulator::ComponentsManager::emptyId);
                    m_state->getPointsRef().clear();
                    m_state->setDrawMode(UI::Types::DrawMode::none);
                }
            } else {
                m_state->clearBulkIds();
                m_state->setDrawMode(UI::Types::DrawMode::none);
            }
            return;
        }

        Simulator::Components::ComponentEventData e;
        e.type = Simulator::Components::ComponentEventType::leftClick;
        e.pos = getNVPMousePos();

        Simulator::ComponentsManager::components[cid]->onEvent(e);
    }

    void MainPage::onRightMouse(bool pressed) {

        if (!isCursorInViewport())
            return;

        m_rightMousePressed = pressed;

        auto hoveredId = m_state->getHoveredId();

        if (!pressed && hoveredId == -1) {
            auto pos = glm::vec3(getNVPMousePos(), 0.f);
            const auto prevGen = m_state->getPrevGenBankElement();
            if (prevGen == nullptr)
                return;
            Simulator::ComponentsManager::generateComponent(*prevGen, glm::vec3({getNVPMousePos(), 0.f}));
            return;
        }

        auto &cid = Simulator::ComponentsManager::renderIdToCid(hoveredId);
        if (cid == Simulator::ComponentsManager::emptyId)
            return;

        Simulator::Components::ComponentEventData e;
        e.type = Simulator::Components::ComponentEventType::rightClick;
        e.pos = getNVPMousePos();

        Simulator::ComponentsManager::components[cid]->onEvent(e);
    }

    void MainPage::onMiddleMouse(bool pressed) {
        if (!isCursorInViewport())
            return;

        m_middleMousePressed = pressed;
    }

    void MainPage::onMouseMove(double x, double y) {
        const auto prevMousePos = m_state->getMousePos();
        const float dx = static_cast<float>(x) - prevMousePos.x;
        const float dy = static_cast<float>(y) - prevMousePos.y;
        m_state->setMousePos({x, y});

        if (!isCursorInViewport()) {
            return;
        }

        auto &dragData = m_state->getDragData();

        if (m_state->isHoveredIdChanged() && !dragData.isDragging) {
            auto prevHoveredId = m_state->getPrevHoveredId();
            const auto hoveredId = m_state->getHoveredId();

            if (prevHoveredId != -1 && Simulator::ComponentsManager::isRenderIdPresent(prevHoveredId)) {
                auto &cid = Simulator::ComponentsManager::renderIdToCid(
                    prevHoveredId);
                Simulator::Components::ComponentEventData e;
                e.type = Simulator::Components::ComponentEventType::mouseLeave;
                Simulator::ComponentsManager::components[cid]->onEvent(e);
            }

            prevHoveredId = hoveredId;

            if (hoveredId > 0 && Simulator::ComponentsManager::isRenderIdPresent(hoveredId)) {
                auto &cid = Simulator::ComponentsManager::renderIdToCid(hoveredId);
                Simulator::Components::ComponentEventData e;
                e.type = Simulator::Components::ComponentEventType::mouseEnter;
                Simulator::ComponentsManager::components[cid]->onEvent(e);
            }
        }

        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_lastUpdateTime).count();

        if (m_middleMousePressed) {
            glm::vec2 dPos = {dx / UI::UIMain::state.cameraZoom, dy / UI::UIMain::state.cameraZoom};
            dPos *= -1;
            m_camera->incrementPos(dPos);
        } else if (m_leftMousePressed && (diff > 50 || dragData.isDragging)) {
            // dragging an entity
            if (m_state->getHoveredId() > -1 || (dragData.isDragging && m_state->getDrawMode() != UI::Types::DrawMode::selectionBox)) {
                for (auto &id : m_state->getBulkIds()) {
                    const auto &entity = Simulator::ComponentsManager::components[id];

                    // dragable components start from 101
                    if (static_cast<int>(entity->getType()) <= 100)
                        return;

                    auto &pos = entity->getPosition();

                    if (!dragData.isDragging) {
                        Types::DragData dragData{};
                        dragData.isDragging = true;
                        dragData.orinalEntPos = pos;
                        dragData.dragOffset = getNVPMousePos() - glm::vec2(pos);
                        m_state->setDragData(dragData);
                    }

                    auto dPos = getNVPMousePos() - dragData.dragOffset;
                    float snap = 4.f;
                    dPos = glm::round(dPos / snap) * snap;

                    entity->setPosition({dPos, 9.f});
                }
            } else if (m_state->getHoveredId() == -1) { // box selection when dragging in empty space
                if (!dragData.isDragging) {
                    Types::DragData dragData{};
                    dragData.isDragging = true;
                    dragData.dragOffset = getNVPMousePos();
                    dragData.vpMousePos = getViewportMousePos();
                    m_state->setDrawMode(UI::Types::DrawMode::selectionBox);
                    m_state->setDragData(dragData);
                    m_state->clearBulkIds();
                }
            }
        }
    }

    bool MainPage::isCursorInViewport() {
        const auto &viewportPos = UI::UIMain::state.viewportPos;
        const auto &viewportSize = UI::UIMain::state.viewportSize;
        const auto mousePos = getViewportMousePos();
        return mousePos.x >= 5.f &&
               mousePos.x < viewportSize.x &&
               mousePos.y >= 5.f &&
               mousePos.y < viewportSize.y;
    }

    glm::vec2 MainPage::getViewportMousePos() {
        const auto &viewportPos = UI::UIMain::state.viewportPos;
        const auto mousePos = m_parentWindow->getMousePos();
        auto x = mousePos.x - viewportPos.x;
        auto y = mousePos.y - viewportPos.y;
        return {x, y};
    }

    glm::vec2 MainPage::getNVPMousePos() {
        const auto &viewportPos = getViewportMousePos();

        glm::vec2 pos = viewportPos;

        const auto cameraPos = m_camera->getPos();
        glm::mat4 tansform = glm::translate(glm::mat4(1.f), glm::vec3(cameraPos.x, cameraPos.y, 0.f));
        tansform = glm::scale(tansform, glm::vec3(1.f / UI::UIMain::state.cameraZoom, 1.f / UI::UIMain::state.cameraZoom, 1.f));

        pos = glm::vec2(tansform * glm::vec4(pos.x, pos.y, 0.f, 1.f));
        auto span = m_camera->getSpan() / 2.f;
        pos -= glm::vec2({span.x, span.y});
        return pos;
    }

} // namespace Bess::Pages
