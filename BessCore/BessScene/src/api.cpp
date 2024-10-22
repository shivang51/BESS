#include "api.h"
#include <stdio.h>
#include "scene_context.h"
#include "GLES3/gl3.h"
#include "GLES2/gl2ext.h"

extern "C" {

int GlVersion() {
    return GL_VERSION;
}

void *Init() {
    std::cout << "[BessSceneApi] Initializing" << std::endl;

    if (!gladLoadGL()) {
        std::cout << "[BessSceneApi] Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    auto ctxPtr = new Bess::Scene::SceneContext();
    ctxPtr->init();
    std::cout << "[BessSceneApi] Initialized" << std::endl;
    return (void*)ctxPtr;
}

void Update(void *ctx) {
    auto sceneCtx = (Bess::Scene::SceneContext *)ctx;
    sceneCtx->update();
}

void Render(void *ctx, int fbo) {
    auto sceneCtx = (Bess::Scene::SceneContext *)ctx;
    sceneCtx->render(fbo);
}

void OnResize(void *ctx, int width, int height) {
    auto sceneCtx = (Bess::Scene::SceneContext *)ctx;

    Bess::Scene::Events::ResizeEventData data;
    data.size = glm::vec2(width, height);
    sceneCtx->onEvent(Bess::Scene::Events::Event::fromEventData(data));
}
}