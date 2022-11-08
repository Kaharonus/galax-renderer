//
// Created by tomas on 30.9.22.
//

#pragma once

#include <vector>
#include <renderer/SceneObject.h>

namespace Galax::Renderer{
    class ISSBO : public SceneObject {
    public:
        ISSBO() : SceneObject() {};
        ISSBO(const std::string &name) : SceneObject(name) {};
        virtual ~ISSBO() = default;
        virtual void setData(const std::vector<char>& data) = 0;
        virtual void bind(unsigned int location) = 0;
        virtual void unbind() = 0;
    };

}
