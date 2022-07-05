#include "fsipch.h"
#include "FSIScene.h"


namespace fsi {
    const ObjectPtr FSIScene::getRoot() const {
        return m_root;
    }
    const std::vector<ObjectPtr>& FSIScene::getObjects() const {
        return m_objects;
    }
}

