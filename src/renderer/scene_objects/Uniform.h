
#pragma once

#include <memory>
#include <variant>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec4.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_mat3x3.hpp>


#include "../SceneObject.h"

namespace Galax::Renderer::SceneObjects {
    typedef std::variant<float, int, bool, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4> UniformT;
    class Uniform : public SceneObject {
    public:

        enum Type{
            FLOAT = 0,
            INT = 1,
            BOOL = 2,
            VEC2 = 3,
            VEC3 = 4,
            VEC4 = 5,
            MAT3 = 6,
            MAT4 = 7
        };
        Uniform(const std::string& name);
        Uniform(const std::string& name,Type type, const UniformT& value = UniformT());
        void setValue(UniformT);
        void setType(Type type);

        uint getId() override;
        UniformT getValue() const;
        template <typename T> T getValue() const{
            return std::get<T>(value);
        };

        Type getType() const;
        ~Uniform();

    private:

        Type type;
        UniformT value;
    };
} // namespace MapGenerator::Renderer::SceneObjects
