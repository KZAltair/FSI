#pragma once

#include <glm/glm.hpp>

#include "FSIObjectTypes.h"

namespace fsi {
    class IfcReader;
    class FSIScene {
        friend class IfcReader;
    public:
        FSIScene() = default;
        ~FSIScene() = default;

        const ObjectPtr getRoot() const;
        const std::vector<ObjectPtr>& getObjects() const;

    private:
        ObjectPtr m_root;
        std::vector<ObjectPtr> m_objects;
    };
    typedef std::shared_ptr<FSIScene> FSIScenePtr;
}

