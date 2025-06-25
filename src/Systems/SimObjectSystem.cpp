#include "SimObjectSystem.hpp"

#include "Components/Primitives/Transform.hpp"
#include "Components/Rendering/Material.hpp"

namespace SimObject {

    entt::entity Create(entt::registry& registry, std::string name){
        auto entity = registry.create();

        // Add a transform
        registry.emplace<Transform>(entity, Transform{
            .name = name
        });
        registry.emplace<Material>(entity);

        return entity;
    }

}
