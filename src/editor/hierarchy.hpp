#pragma once

#include "primitives/entity.hpp"

#include <imgui.h>
#include <unordered_map>
#include <memory>
#include <string>

using EntityID = uint32_t;

namespace Editor {

    class Hierarchy {
    public:
        Hierarchy();

        void Show();// Renders the hierarchy window
        void AddEntity(std::shared_ptr<SimObject::Entity> entity); // Adds a new entity
        SimObject::Entity* GetSelected();// Returns the selected entity (if any)
        std::unordered_map<EntityID, std::shared_ptr<SimObject::Entity>> entities;

    private:
        uint32_t entityCount = 0;
        EntityID selectedEntityID = 0;
        bool scrollToSelection = false;
    };

} // namespace Editor
