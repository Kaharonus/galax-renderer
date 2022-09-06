
#include <glbinding/gl/gl.h>
#include <glbinding/gl/enum.h>


#include "Texture.h"
#include <cassert>
#include <stdexcept>

using namespace Galax::Renderer::SceneObjects;
using namespace gl;

Texture::Texture(Type type, Format format, DataType dataType)
        : SceneObject(), target(type), format(format), dataType(dataType) {
    init();
}

Texture::Texture(const std::string &name, Type type, Format format, DataType dataType)
        : SceneObject(name), target(type), format(format), dataType(dataType) {
    init();
}

void Texture::init() {
    if (target == TYPE_CUBE) {
        dimensions = {0, 0, 0};
        for (int i = 0; i < 6; i++) {
            data.emplace_back();
        }
    } else {
        dimensions = {0, 0, 0};
        data.emplace_back();
    }
}


void Texture::setDimensions(int width, int height) {
    assert(target != TYPE_3D);
    assert(width > 0 && height > 0);
    dimensions = {width, height, 0};
}

void Texture::setDimensions(int width, int height, int depth) {
    assert(target == TYPE_3D);
    assert(width > 0 && height > 0 && depth > 0);
    dimensions = {width, height, depth};
}

void Texture::addArrayLayer() {
    assert(target == TYPE_ARRAY);
    data.emplace_back();
}

void Texture::setData(const std::vector<unsigned char> &data, size_t layer) {
    assert(dataType == UNSIGNED_BYTE);
    assert(!data.empty());
    assert(layer < data.size());
    this->data[layer] = data;
}

void Texture::setData(const std::vector<char> &data, size_t layer) {
    assert(dataType == BYTE);
    assert(!data.empty());
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size());
}

void Texture::setData(const std::vector<unsigned short> &data, size_t layer) {
    assert(dataType == UNSIGNED_SHORT);
    assert(!data.empty());
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * 2);
}

void Texture::setData(const std::vector<short> &data, size_t layer) {
    assert(dataType == SHORT);
    assert(!data.empty());
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(short));
}

void Texture::setData(const std::vector<unsigned int> &data, size_t layer) {
    assert(dataType == UNSIGNED_INT);
    assert(!data.empty());
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(unsigned int));
}

void Texture::setData(const std::vector<int> &data, size_t layer) {
    assert(dataType == INT);
    assert(!data.empty());
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(int));
}

void Texture::setData(const std::vector<float> &data, size_t layer) {
    assert(dataType == FLOAT);
    assert(!data.empty());
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(float));
}


void Texture::setData(const std::vector<unsigned char> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<char> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<unsigned short> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<short> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<unsigned int> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<int> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<float> &data) {
    setData(data, 0);
}


Texture::~Texture() {
}


uint Texture::toInternal() {
    GLenum result;
    switch (format) {
        case Format::RED:
            switch (dataType) {
                case DataType::UNSIGNED_BYTE:
                case DataType::BYTE:
                    result = GL_R8;
                    break;
                case DataType::UNSIGNED_SHORT:
                case DataType::SHORT:
                    result = GL_R16;
                    break;
                case DataType::UNSIGNED_INT:
                case DataType::INT:
                    result = GL_R32I;
                    break;
                case DataType::FLOAT:
                    result = GL_R32F;
                    break;
                default:
                    throw std::runtime_error("Unsupported data type");
            }
            break;
        case Format::RG:
            switch (dataType) {
                case DataType::UNSIGNED_BYTE:
                case DataType::BYTE:
                    result = GL_RG8;
                    break;
                case DataType::UNSIGNED_SHORT:
                case DataType::SHORT:
                    result = GL_RG16;
                    break;
                case DataType::UNSIGNED_INT:
                case DataType::INT:
                    result = GL_RG32I;
                    break;
                case DataType::FLOAT:
                    result = GL_RG32F;
                    break;
                default:
                    throw std::runtime_error("Unsupported data type");
            }
            break;
        case Format::RGB:
        case Format::BGR:
            switch (dataType) {
                case DataType::UNSIGNED_BYTE:
                case DataType::BYTE:
                    result = GL_RGB8;
                    break;
                case DataType::UNSIGNED_SHORT:
                case DataType::SHORT:
                    result = GL_RGB16;
                    break;
                case DataType::UNSIGNED_INT:
                case DataType::INT:
                    result = GL_RGB32I;
                    break;
                case DataType::FLOAT:
                    result = GL_RGB32F;
                    break;
                default:
                    throw std::runtime_error("Unsupported data type");
            }
            break;
        case Format::RGBA:
        case Format::BGRA:
            switch (dataType) {
                case DataType::UNSIGNED_BYTE:
                case DataType::BYTE:
                    result = GL_RGBA8;
                    break;
                case DataType::UNSIGNED_SHORT:
                case DataType::SHORT:
                    result = GL_RGBA16;
                    break;
                case DataType::UNSIGNED_INT:
                case DataType::INT:
                    result = GL_RGBA32I;
                    break;
                case DataType::FLOAT:
                    result = GL_RGBA32F;
                    break;
                default:
                    throw std::runtime_error("Unsupported data type");
            }
            break;
    }
    return (uint) result;
}

uint Texture::getId() {
    return id;
}

void Texture::upload() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
    auto [w, h, d] = dimensions;
    auto glTarget = (GLenum) target;
    auto glInternal = (GLenum) toInternal();
    glCreateTextures(glTarget, 1, &id);
    glBindTexture(glTarget, id);

    switch (target) {
        case TYPE_2D:
            // glTextureStorage2D(id, 1, glInternal, w, h);
            glTexImage2D(glTarget, 0, glInternal, w, h, 0, (GLenum) format, (GLenum) dataType, data[0].data());
            break;
        case TYPE_ARRAY: { // Thanks for the redundant braces, C++
            auto flat = std::vector<unsigned char>(w * h * data.size());
            for (int i = 0; i < data.size(); i++) {
                std::copy(data[i].begin(), data[i].end(), flat.begin() + i * w * h);
            }
            glTexStorage3D(glTarget, 1, (GLenum) toInternal(), w, h, data.size());
            glTexSubImage3D(glTarget, 0, 0, 0, 0, w, h, data.size(), (GLenum) format, (GLenum) dataType, flat.data());
            break;
        }
        case TYPE_CUBE:
            glTextureStorage2D(id, 1, (GLenum) format, w, h);
            for (int i = 0; i < 6; i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (GLenum) toInternal(), w, h, 0, (GLenum) format,
                             (GLenum) dataType, data[i].data());
            }
            break;
        case TYPE_3D:
            glTexImage3D(GL_TEXTURE_3D, 0, (GLenum) toInternal(), w, h, d, 0, (GLenum) format, (GLenum) dataType,
                         data[0].data());
            break;
    }
    checkError(true);

    glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(glTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);

    GLfloat value, max_anisotropy = 8.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
    value = (value > max_anisotropy) ? max_anisotropy : value;
    glTexParameterf(glTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);

    glBindTexture(glTarget, 0);

}

void Texture::bind(uint unit) {
    if (id == 0) {
        upload();
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture((GLenum) target, id);

}

std::vector<unsigned char> Texture::readDataFromGPU() {
    if (this->target != TYPE_2D) {
        throw std::runtime_error("Can only read data from 2D textures");
    }
    auto [w, h, _] = dimensions;
    auto dataVector = std::vector<unsigned char>(w * h * getFormatSize() * getDataSize());
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexImage(GL_TEXTURE_2D, 0, (GLenum) format, (GLenum) dataType, dataVector.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    return dataVector;
}

int Texture::getDataSize() {
    switch (dataType) {
        case BYTE:
        case UNSIGNED_BYTE:
            return 1;
        case SHORT:
        case UNSIGNED_SHORT:
            return 2;
        case INT:
        case UNSIGNED_INT:
        case FLOAT:
            return 4;

    }
    return 0;
}

int Texture::getFormatSize() {
    switch (format) {
        case Format::RED:
            return 1;
        case Format::RG:
            return 2;
        case Format::RGB:
        case Format::BGR:
            return 3;
        case Format::RGBA:
        case Format::BGRA:
            return 4;
    }
    return 0;
}
