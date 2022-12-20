//
// Created by tomas on 27.9.22.
//

#pragma once

#include <string>
#include <renderer/SceneObject.h>

namespace Galax::Renderer {
    class IShader : public Galax::Renderer::SceneObject {
    public:
        enum Type {
            None = 0,
            VERTEX = 0x8B31,
            FRAGMENT = 0x8B30,
            GEOMETRY = 0x8DD9,
            TESSALATION_CTRL = 0x8E88,
            TESSALATION_EVAL = 0x8E87,
            COMPUTE = 0x91B9
        };

        IShader() : SceneObject() {};

        IShader(const std::string &name) : SceneObject(name, true) {};

        virtual ~IShader() = default;

        virtual void linkTo(unsigned int program) = 0;

        virtual bool getUpdated() const = 0;

        virtual void setSource(const std::string &source) = 0;

        virtual void setType(Type type) = 0;

        virtual std::string getInfoLog() = 0;

        virtual std::string &getSource() = 0;

        virtual void recompile() = 0;

        virtual Type getType() = 0;

        virtual std::string getTypeString() = 0;

    };
}