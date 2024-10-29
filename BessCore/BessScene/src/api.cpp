#include "api.h"
#include "GLES3/gl3.h"
#include "scene_context.h"
#include <iostream>
#include <stdio.h>

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
    return (void *)ctxPtr;
}

void Update(void *ctx) {
    std::cout << "[BessSceneApi] Updating" << std::endl;
    auto sceneCtx = (Bess::Scene::SceneContext *)ctx;
    sceneCtx->update();
}

void Render(void *ctx, int fbo) {
    auto sceneCtx = (Bess::Scene::SceneContext *)ctx;
    std::cout << "[BessSceneApi] Rendering to fbo " << fbo << std::endl;
    sceneCtx->render(fbo);
}

void OnResize(void *ctx, int width, int height) {
    auto sceneCtx = (Bess::Scene::SceneContext *)ctx;
    std::cout << "[BessSceneApi] Resizing to " << width << "x" << height << std::endl;
    Bess::Scene::Events::ResizeEventData data;
    data.size = glm::vec2(width, height);
    sceneCtx->onEvent(Bess::Scene::Events::Event::fromEventData(data));
}
}
