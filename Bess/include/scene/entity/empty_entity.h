#pragma once

#include "entity.h"

namespace Bess::Scene::Entities {
    class EmptyEntity : public Entity {
      public:
        static std::shared_ptr<EmptyEntity> getInstance();

        EmptyEntity() = default;

        void render() override;
    };
} // namespace Bess::Scene::Entities
