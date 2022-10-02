//
// Created by tomas on 30.9.22.
//

#pragma once

#include <vector>
#include "../SceneObject.h"

namespace Galax::Renderer{
    class ISSBO : public SceneObject {
    public:
        ISSBO() : SceneObject() {};
        ISSBO(const std::string &name) : SceneObject(name) {};
        virtual ~ISSBO() = default;
        virtual void setData(const std::vector<char>& data) = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
    };

}
