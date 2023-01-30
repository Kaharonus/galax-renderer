
#pragma once
#include "../SceneObject.h"
#include "../interfaces/IMesh.h"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Galax::Renderer::SceneObjects {
    class Mesh : public IMesh {
    public:
        Mesh();
        Mesh(const std::string& name);
        ~Mesh();

        uint getId() override;

        void addVertex(const glm::vec3& vertex);
        void addVertex(const glm::vec3& vertex, const glm::vec3& normal);
        void addVertex(const glm::vec3& vertex, const glm::vec3& normal, const glm::vec2& texCoord);
        void addVertex(const glm::vec3& vertex, const glm::vec3& normal, const glm::vec2& texCoord, const glm::vec3& tangent);
        void addIndex(unsigned int index);
        void addTriangleIndex(unsigned int i1, unsigned int i2, unsigned int i3);
        [[nodiscard]] glm::vec3 getCenter() const override;
        std::vector<float>& getVertices() override;
        std::vector<unsigned int>& getIndices() override;
        int getStride() const;

        void bind() override;
        void unbind() override;

        void upload();

        uint size() override;

        void calculateTangents();
        void calculateNormals();

    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        glm::vec3 center = glm::vec3(0.0f);
        uint count = 0;
        glm::vec3 total = glm::vec3(0.0f);

        glm::vec3 zero3 = glm::vec3(0.0f);
        glm::vec2 zero2 = glm::vec2(0.0f);

        uint vbo;
        uint ebo;
        uint vao;



        int stride = 11;

    };
} // namespace MapGenerator::Renderer::SceneObjects
