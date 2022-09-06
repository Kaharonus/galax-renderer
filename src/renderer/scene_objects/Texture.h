
#pragma once
//#include <glbinding/gl/enum.h>
#include "../SceneObject.h"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>

namespace Galax::Renderer::SceneObjects {


    class Texture : public SceneObject {
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
            RGBA =0x1908,
            BGRA = 0x80E1
        };

        enum DataType{
            BYTE = 0x1400,
            UNSIGNED_BYTE = 0x1401,
            SHORT = 0x1402,
            UNSIGNED_SHORT = 0x1403,
            INT = 0x1404,
            UNSIGNED_INT = 0x1405,
            FLOAT = 0x1406,
        };

        Texture(Type type = TYPE_2D, Format format = RGBA, DataType dataType = UNSIGNED_BYTE);
        Texture(const std::string& name, Type type = TYPE_2D, Format format = RGBA, DataType dataType = UNSIGNED_BYTE);

        uint getId() override;

        std::tuple<int, int, int> getDimensions() const;
        Type getType() const;
        Format getFormat() const;
        DataType getDataType() const;

        void setDimensions(int width, int height);
        void setDimensions(int width, int height, int depth);
        void addArrayLayer();

        /**
         * @brief Requests the texture to read the texture data from the GPU back to the CPU.
          The callback will be called automatically by the engine.
          Only one callback can be registered at a time.
         * @param callback a method that will be called when the texture data is ready to be read.
         */
        void requestData(const std::function<void(const std::vector<unsigned char> &)> &callback);

        void setData(const std::vector<unsigned char>& data, size_t layer);
        void setData(const std::vector<char>& data, size_t layer);
        void setData(const std::vector<unsigned short>& data, size_t layer);
        void setData(const std::vector<short>& data, size_t layer);
        void setData(const std::vector<unsigned int>& data, size_t layer);
        void setData(const std::vector<int>& data, size_t layer);
        void setData(const std::vector<float>& data, size_t layer);

        void setData(const std::vector<unsigned char>& data);
        void setData(const std::vector<char>& data);
        void setData(const std::vector<unsigned short>& data);
        void setData(const std::vector<short>& data);
        void setData(const std::vector<unsigned int>& data);
        void setData(const std::vector<int>& data);
        void setData(const std::vector<float>& data);

        /**
         * @brief Binds the texture to the given texture unit.
           If the texture type is TYPE_ARRAY, the textures will be bound to unit + layer.
         * 
         * @param unit Unit to bind the texture to.*/

        void bind(uint unit);

        void upload();

        ~Texture();

    private:
        int getDataSize();

        void init();

        uint id = 0;
        std::vector<std::vector<unsigned char>> data;
        std::tuple<int, int, int> dimensions;
        Type target;
        Format format;
        DataType dataType;

        std::function<void(const std::vector<unsigned char>&)> readCallback;
        bool readRequested = false;
        void readDataFromGPU();

        uint toInternal();

        int getFormatSize();

    };
} // namespace MapGenerator::Renderer::SceneObjects
