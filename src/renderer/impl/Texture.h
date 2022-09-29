
#pragma once
//#include <glbinding/gl/enum.h>
#include "../SceneObject.h"
#include "../interfaces/ITexture.h"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>

namespace Galax::Renderer::SceneObjects {


    class Texture : public ITexture {
    public:
        Texture(Type type = TYPE_2D, Format format = RGBA, DataType dataType = UNSIGNED_BYTE, Wrap wrap = REPEAT, Filtering filtering = ANISOTROPIC);
        Texture(const std::string& name, Type type = TYPE_2D, Format format = RGBA, DataType dataType = UNSIGNED_BYTE, Wrap wrap = REPEAT, Filtering filtering = ANISOTROPIC);

        uint getId() override;

        [[nodiscard]] std::tuple<int, int, int> getDimensions() const override;
        [[nodiscard]] Type getType() const override;
        [[nodiscard]] Format getFormat() const override;
        [[nodiscard]] DataType getDataType() const override;
        [[nodiscard]] Wrap getWrap() const override;
        [[nodiscard]] Filtering getFiltering() const override;
        [[nodiscard]] int getFormatSize() const override;
        [[nodiscard]] int getDataSize() const override;
        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;

        void setDimensions(int width, int height) override;
        void setDimensions(int width, int height, int depth) override;
        void addArrayLayer();
        void setWrap(Wrap wrap) override;
        void setFormat(Format format) override;
        void setDataType(DataType dataType) override;
        void setType(Type type) override;
        void setFiltering(Filtering filtering) override;


        /**
         * @brief Requests the texture to read the texture data from the GPU back to the CPU.
          The callback will be called automatically by the engine.
          Only one callback can be registered at a time.
         * @param callback a method that will be called when the texture data is ready to be read.
         */
        void requestData(const std::function<void(const std::vector<unsigned char>&)>& callback) override;
        void cancelDataRequest() override;
        void setData(const std::vector<unsigned char>& data, size_t layer) override;
        void setData(const std::vector<char>& data, size_t layer) override;
        void setData(const std::vector<unsigned short>& data, size_t layer) override;
        void setData(const std::vector<short>& data, size_t layer) override;
        void setData(const std::vector<unsigned int>& data, size_t layer) override;
        void setData(const std::vector<int>& data, size_t layer) override;
        void setData(const std::vector<float>& data, size_t layer) override;

        void setData(const std::vector<unsigned char>& data) override;
        void setData(const std::vector<char>& data) override;
        void setData(const std::vector<unsigned short>& data) override;
        void setData(const std::vector<short>& data) override;
        void setData(const std::vector<unsigned int>& data) override;
        void setData(const std::vector<int>& data) override;
        void setData(const std::vector<float>& data) override;

        /**
         * @brief Binds the texture to the given texture unit.
           If the texture type is TYPE_ARRAY, the textures will be bound to unit + layer.
         * 
         * @param unit Unit to bind the texture to.*/

        void bind(uint unit) override;
        void unbind() override;

        void upload() override;

        ~Texture();

    private:

        void init();

        uint id = 0;
        std::vector<std::vector<unsigned char>> data;
        std::tuple<int, int, int> dimensions;
        Type target;
        Format format;
        DataType dataType;
        Wrap wrap = REPEAT;
        Filtering filtering = ANISOTROPIC;

        std::function<void(const std::vector<unsigned char>&)> readCallback;
        bool readRequested = false;
        void readDataFromGPU();

        uint toInternal();
    };
} // namespace MapGenerator::Renderer::SceneObjects
