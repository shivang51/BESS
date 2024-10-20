#include "scene/scene_context.h"
#include "scene/entity/entity.h"
#include "scene/events/entity_event.h"
#include "scene/events/event_type.h"
#include "settings/viewport_theme.h"

#include "scene/entity/empty_entity.h"

namespace Bess::Scene {

    SceneContext::~SceneContext() {
    }

    void SceneContext::init() {
        std::vector<Gl::FBAttachmentType> attachments = {Gl::FBAttachmentType::RGBA_RGBA, Gl::FBAttachmentType::R32I_REDI, Gl::DEPTH32F_STENCIL8};
        m_msaaFramebuffer = std::make_unique<Gl::FrameBuffer>(800, 600, attachments, true);

        attachments = {Gl::FBAttachmentType::RGB_RGB, Gl::FBAttachmentType::R32I_REDI};
        m_framebuffer = std::make_unique<Gl::FrameBuffer>(800, 600, attachments);

        m_events = {};
        m_entities = {};
        m_renderIdToEntity = {};
    }

    unsigned int SceneContext::getTextureId() {
        return m_framebuffer->getColorBufferTexId(0);
    }

    void SceneContext::beginScene() {
        static int value = -1;
        m_msaaFramebuffer->bind();

        const auto bgColor = ViewportTheme::backgroundColor;
        const float clearColor[] = {bgColor.x, bgColor.y, bgColor.z, bgColor.a};
        m_msaaFramebuffer->clearColorAttachment<GL_FLOAT>(0, clearColor);
        m_msaaFramebuffer->clearColorAttachment<GL_INT>(1, &value);
        Gl::FrameBuffer::clearDepthStencilBuf();
    }

    void SceneContext::endScene() {
        Gl::FrameBuffer::unbindAll();
        for (int i = 0; i < 2; i++) {
            m_msaaFramebuffer->bindColorAttachmentForRead(i);
            m_framebuffer->bindColorAttachmentForDraw(i);
            Gl::FrameBuffer::blitColorBuffer(m_size.x, m_size.y);
        }
        Gl::FrameBuffer::unbindAll();
    }

    void SceneContext::render() {
        // beginScene();
        for (auto &entity : m_entities) {
            entity->render();
        }
        // endScene();
    }

    void SceneContext::update() {
        while (!m_events.empty()) {
            const auto &evt = m_events.front();

            switch (evt.getType()) {
            case Events::EventType::mouseButton: {
                const auto &data = evt.getData<Events::MouseButtonEventData>();
                handleMouseButton(data);
            } break;
            case Events::EventType::mouseMove: {
                const auto &data = evt.getData<Events::MouseMoveEventData>();
                handleMouseMove(data);
            } break;
            case Events::EventType::mouseWheel: {
                const auto &data = evt.getData<Events::MouseWheelEventData>();
                handleMouseWheel(data);
            } break;
            case Events::EventType::keyPress: {
                const auto &data = evt.getData<Events::KeyPressEventData>();
                handleKeyPress(data);
            } break;
            case Events::EventType::keyRelease: {
                const auto &data = evt.getData<Events::KeyReleaseEventData>();
                handleKeyRelease(data);
            } break;
            case Events::EventType::mouseEnter: {
                const auto &data = evt.getData<Events::MouseEnterEventData>();
                handleMouseEnter(data);
            } break;
            case Events::EventType::mouseLeave: {
                const auto &data = evt.getData<Events::MouseLeaveEventData>();
                handleMouseLeave(data);
            } break;
            case Events::EventType::resize: {
                const auto &data = evt.getData<Events::ResizeEventData>();
                handleResize(data);
            } break;
            }

            m_events.pop();
        }

        for (auto &entity : m_entities) {
            entity->update();
        }
    }

    void SceneContext::addEntity(std::shared_ptr<Entity> entity) {
        m_entities.push_back(entity);
        m_renderIdToEntity[entity->getRenderId()] = entity;
    }

    const std::vector<std::shared_ptr<Entity>> &SceneContext::getEntities() const {
        return m_entities;
    }

    const glm::vec2 &SceneContext::getSize() const {
        return m_size;
    }

    void SceneContext::onEvent(const Events::Event &evt) {
        m_events.push(evt);
    }

    void SceneContext::handleMouseButton(const Events::MouseButtonEventData &data) {
        if (data.button == Events::MouseButton::left) {
            handleLeftMouseButton(data);
        } else if (data.button == Events::MouseButton::right) {
            handleRightMouseButton(data);
        } else if (data.button == Events::MouseButton::middle) {
            handleMiddleMouseButton(data);
        }
    }

    void SceneContext::handleLeftMouseButton(const Events::MouseButtonEventData &data) {
        auto entity = getEntityAt(data.position);

        if (!entity->isEmptyEntity()) {
            entity->onEvent(Events::EntityEvent::fromEventData(data));
            return;
        }

        // if event was not on entity
        if (data.pressed) {
        } else {
        }
    }

    void SceneContext::handleRightMouseButton(const Events::MouseButtonEventData &data) {
        auto entity = getEntityAt(data.position);

        if (!entity->isEmptyEntity()) {
            entity->onEvent(Events::EntityEvent::fromEventData(data));
            return;
        }

        // if event was not on entity
        if (data.pressed) {
        } else {
        }
    }

    void SceneContext::handleMiddleMouseButton(const Events::MouseButtonEventData &data) {
        auto entity = getEntityAt(data.position);

        if (!entity->isEmptyEntity()) {
            entity->onEvent(Events::EntityEvent::fromEventData(data));
            return;
        }

        // if event was not on entity
        if (data.pressed) {
        } else {
        }
    }

    void SceneContext::handleMouseMove(const Events::MouseMoveEventData &data) {
        auto entity = getEntityAt(data.position);

        if (!entity->isEmptyEntity()) {
            entity->onEvent(Events::EntityEvent::fromEventData(data));
            return;
        }
    }

    void SceneContext::handleMouseWheel(const Events::MouseWheelEventData &data) {
    }

    void SceneContext::handleKeyPress(const Events::KeyPressEventData &data) {
    }

    void SceneContext::handleKeyRelease(const Events::KeyReleaseEventData &data) {
    }

    void SceneContext::handleMouseEnter(const Events::MouseEnterEventData &data) {
        auto entity = getEntityAt(data.position);

        if (!entity->isEmptyEntity()) {
            entity->onEvent(Events::EntityEvent::fromEventData(data));
            return;
        }
    }

    void SceneContext::handleMouseLeave(const Events::MouseLeaveEventData &data) {
        auto entity = getEntityAt(data.position);

        if (!entity->isEmptyEntity()) {
            entity->onEvent(Events::EntityEvent::fromEventData(data));
            return;
        }
    }

    void SceneContext::handleResize(const Events::ResizeEventData &data) {
        m_size = data.size;
        m_msaaFramebuffer->resize(m_size.x, m_size.y);
        m_framebuffer->resize(m_size.x, m_size.y);
    }

    std::shared_ptr<Entity> SceneContext::getEntityAt(const glm::vec2 &pos) {
        int renderId = m_framebuffer->readIntFromColAttachment(1, pos.x, pos.y);
        if (renderId < 0) {
            return Entities::EmptyEntity::getInstance();
        }
        return m_renderIdToEntity.at(renderId);
    }
} // namespace Bess::Scene
