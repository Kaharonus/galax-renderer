//
// Created by tomas on 27.9.22.
//

#pragma once

#include <functional>

namespace Galax::Renderer {
    class ITexture : public Galax::Renderer::SceneObject {
    public:
        enum Type {
            TYPE_2D = 0x0DE1,
            TYPE_CUBE = 0x8513,
            TYPE_ARRAY = 0x8C1A,
            TYPE_3D = 0x806F
        };

        enum Format {
            RED = 0x1903,
            RG = 0x8227,
            RGB = 0x1907,
            BGR = 0x80E0,
            RGBA = 0x1908,
            BGRA = 0x80E1
        };

        enum DataType {
            BYTE = 0x1400,
            UNSIGNED_BYTE = 0x1401,
            SHORT = 0x1402,
            UNSIGNED_SHORT = 0x1403,
            INT = 0x1404,
            UNSIGNED_INT = 0x1405,
            FLOAT = 0x1406,
        };

        enum Wrap {
            REPEAT = 0x2901,
            MIRRORED_REPEAT = 0x8370,
            CLAMP_TO_EDGE = 0x812F,
            CLAMP_TO_BORDER = 0x812D
        };

        enum Filtering {
            NEAREST = 0x2600,
            LINEAR = 0x2601,
            ANISOTROPIC = 0
        };

        ITexture() : SceneObject() {};

        ITexture(const std::string &name) : SceneObject(name) {};

        [[nodiscard]] virtual std::tuple<int, int, int> getDimensions() const = 0;

        [[nodiscard]] virtual Type getType() const = 0;

        [[nodiscard]] virtual Format getFormat() const = 0;

        [[nodiscard]] virtual DataType getDataType() const = 0;

        [[nodiscard]] virtual Wrap getWrap() const = 0;

        [[nodiscard]] virtual Filtering getFiltering() const = 0;

        [[nodiscard]] virtual int getFormatSize() const = 0;

        [[nodiscard]] virtual int getDataSize() const = 0;

        [[nodiscard]] virtual int getWidth() const = 0;

        [[nodiscard]] virtual int getHeight() const = 0;

        virtual void setDimensions(int width, int height) = 0;

        virtual void setDimensions(int width, int height, int depth) = 0;

        virtual void setWrap(Wrap wrap) = 0;

        virtual void setFormat(Format format) = 0;

        virtual void setDataType(DataType dataType) = 0;

        virtual void setType(Type type) = 0;

        virtual void setFiltering(Filtering filtering) = 0;

        virtual void requestData(const std::function<void(const std::vector<unsigned char> &)> &callback) = 0;

        virtual void cancelDataRequest() = 0;

        virtual void setData(const std::vector<unsigned char> &data, size_t layer) = 0;

        virtual void setData(const std::vector<char> &data, size_t layer) = 0;

        virtual void setData(const std::vector<unsigned short> &data, size_t layer) = 0;

        virtual void setData(const std::vector<short> &data, size_t layer) = 0;

        virtual void setData(const std::vector<unsigned int> &data, size_t layer) = 0;

        virtual void setData(const std::vector<int> &data, size_t layer) = 0;

        virtual void setData(const std::vector<float> &data, size_t layer) = 0;

        virtual void setData(const std::vector<unsigned char> &data) = 0;

        virtual void setData(const std::vector<char> &data) = 0;

        virtual void setData(const std::vector<unsigned short> &data) = 0;

        virtual void setData(const std::vector<short> &data) = 0;

        virtual void setData(const std::vector<unsigned int> &data) = 0;

        virtual void setData(const std::vector<int> &data) = 0;

        virtual void setData(const std::vector<float> &data) = 0;

        virtual void bind(unsigned int unit) = 0;

        virtual void unbind() = 0;

        virtual void upload() = 0;

    };
}