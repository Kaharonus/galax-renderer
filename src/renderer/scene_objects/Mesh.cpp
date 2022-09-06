#include <glbinding/gl/gl.h>
#include <glm/geometric.hpp>
#include <memory>
#include "Mesh.h"
#include <stdexcept>

using namespace Galax::Renderer::SceneObjects;
using namespace gl;

Mesh::Mesh() : SceneObject() {
    vbo = 0;
    ebo = 0;
    vao = 0;
}

Mesh::Mesh(const std::string &name) : SceneObject(name) {
    vbo = 0;
    ebo = 0;
    vao = 0;
}

void Mesh::addVertex(const glm::vec3 &vertex) {
    addVertex(vertex, zero3, zero2, zero3);
}

void Mesh::addVertex(const glm::vec3 &vertex, const glm::vec3 &normal) {
    addVertex(vertex, normal, zero2, zero3);
}

void Mesh::addVertex(const glm::vec3 &vertex, const glm::vec3 &normal, const glm::vec2 &texCoord) {
    addVertex(vertex, normal, texCoord, zero3);
}

void
Mesh::addVertex(const glm::vec3 &vertex, const glm::vec3 &normal, const glm::vec2 &texCoord, const glm::vec3 &tangent) {
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);
    vertices.push_back(vertex.z);
    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);
    vertices.push_back(texCoord.x);
    vertices.push_back(texCoord.y);
    vertices.push_back(tangent.x);
    vertices.push_back(tangent.y);
    vertices.push_back(tangent.z);
}

void Mesh::addIndex(unsigned int index) {
    count++;
    auto v = glm::vec3(vertices[index * stride], vertices[index * stride + 1], vertices[index * stride + 2]);
    total += v;
    center = total / (float) count;
    indices.push_back(index);
}

void Mesh::addTriangleIndex(unsigned int i1, unsigned int i2, unsigned int i3) {
    addIndex(i1);
    addIndex(i2);
    addIndex(i3);
}

void Mesh::upload() {

    // Create the vertex array
    glGenVertexArrays(1, &vao);

    if (vao == 0) {
        checkError(true);
    }

    glBindVertexArray(vao);

    // Create the vertex buffer
    glGenBuffers(1, &vbo);
    if (vbo == 0) {
        checkError(true);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Create the index buffer
    glGenBuffers(1, &ebo);
    if (ebo == 0) {
        checkError(true);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    // Set up the vertex attributes
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) 0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (3 * sizeof(float)));

    // Texture coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (6 * sizeof(float)));

    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (8 * sizeof(float)));

    // Unbind the vertex array
    glBindVertexArray(0);


};

void Mesh::bind() {
    if (vao != 0) {
        glBindVertexArray(vao);
        return;
    }
    upload();

    glBindVertexArray(vao);
}

uint Mesh::size() {
    return indices.size();
}

void Mesh::unbind() {
    glBindVertexArray(0);
}

void Mesh::calculateTangents() {
    NOT_IMPLEMENTED
}

void Mesh::calculateNormals() {
    NOT_IMPLEMENTED
}

Mesh::~Mesh() {
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
    }
}

uint Mesh::getId() {
    return vao;
}

glm::vec3 Mesh::getCenter() const {
    return center;
}

std::vector<unsigned int> &Mesh::getIndices() {
    return indices;
}

std::vector<float> &Mesh::getVertices() {
    return vertices;
}

int Mesh::getStride() const {
    return stride;
}
