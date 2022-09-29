
#pragma once

#include <memory>
#include <variant>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec4.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_mat3x3.hpp>


#include "../SceneObject.h"
#include "../interfaces/IUniform.h"

namespace Galax::Renderer::SceneObjects {
    class Uniform : public IUniform {
    public:

        Uniform(const std::string& name);
        Uniform(const std::string& name,Type type, const UniformT& value = UniformT());
        void setValue(UniformT) override;
        void setType(Type type) override;

        uint getId() override;
        UniformT getValue() const override;
        template <typename T> T getValue() const{
            return std::get<T>(value);
        };

        Type getType() const override;
        ~Uniform() override;

    private:

        Type type;
        UniformT value;
    };
} // namespace MapGenerator::Renderer::SceneObjects
