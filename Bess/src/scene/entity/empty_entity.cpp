#include "scene/entity/empty_entity.h"
#include <memory>

namespace Bess::Scene::Entities {

    std::shared_ptr<EmptyEntity> EmptyEntity::getInstance() {
        static std::shared_ptr<EmptyEntity> instance{};
        return instance;
    }

    void EmptyEntity::render() {
    }
} // namespace Bess::Scene::Entities
