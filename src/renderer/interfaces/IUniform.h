//
// Created by tomas on 27.9.22.
//
#pragma once
#include <variant>
#include <glm/glm.hpp>
namespace Galax::Renderer {
    class IUniform : public Galax::Renderer::SceneObject {
    public:
        typedef std::variant<float, int, bool, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4> UniformT;
        enum Type {
            FLOAT = 0,
            INT = 1,
            BOOL = 2,
            VEC2 = 3,
            VEC3 = 4,
            VEC4 = 5,
            MAT3 = 6,
            MAT4 = 7
        };

        IUniform() : SceneObject() {};

        IUniform(const std::string &name) : SceneObject(name) {};

        virtual ~IUniform() = default;

        virtual void setValue(UniformT) = 0;

        virtual void setType(Type type) = 0;

        virtual UniformT getValue() const = 0;

        virtual Type getType() const = 0;
    };
}