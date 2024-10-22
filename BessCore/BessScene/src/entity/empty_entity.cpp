#include "entity/empty_entity.h"
#include <memory>

namespace Bess::Scene::Entities {

    std::shared_ptr<EmptyEntity> EmptyEntity::getInstance() {
        static auto instance = std::make_shared<EmptyEntity>();
        return instance;
    }

    void EmptyEntity::render() {
    }
} // namespace Bess::Scene::Entities
