//
// Created by kshitij on 2/13/26.
//

#ifndef CONCEPTFORGE_INSPECTABLE_HPP
#define CONCEPTFORGE_INSPECTABLE_HPP
#include "Camera.hpp"
#include "entt/entt.hpp"
#include "Primitives/Transform.hpp"
#include "Rendering/DirectionalLight.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/PointLight.hpp"


namespace Inspectable
{
    using InspectableComponents = entt::type_list<
        Transform,
        Material,
        DirectionalLight,
        PointLight,
        Camera
    >;
}

#endif //CONCEPTFORGE_INSPECTABLE_HPP