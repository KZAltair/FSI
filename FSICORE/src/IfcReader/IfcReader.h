#pragma once

#include "FSIScene.h"

namespace fsi {
    class IfcReader {
    public:
        IfcReader() = delete;
        static FSIScenePtr read(const std::string &filename);
    };
}
