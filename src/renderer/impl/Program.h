
#pragma once

#include "Shader.h"
#include "Texture.h"
#include <memory>
#include "Uniform.h"
#include "../SceneObject.h"
#include "../interfaces/IProgram.h"
#include "../interfaces/ITexture.h"
#include "../interfaces/IUniform.h"
#include "../interfaces/ISSBO.h"
#include <vector>
#include <map>


namespace Galax::Renderer::SceneObjects {

    class Program : public IProgram {
    public:
        Program();
        Program(const std::string& name);
        Program(const std::string& name, const std::vector<std::shared_ptr<IShader>>& shaders);
        Program(const std::string& name, const std::shared_ptr<IShader>& vertex, const std::shared_ptr<IShader>& fragment);
        ~Program();

        uint getId() override;

        void addShader(std::shared_ptr<IShader> shader) override;

        void addTexture(std::shared_ptr<ITexture> texture);

        bool hasTesslation() override;

        void setTexture(const std::shared_ptr<ITexture>& texture, int unit) override;
        void setSSBO(const std::shared_ptr<ISSBO>& ssbo) override;
        uint getTexturePosition(const std::shared_ptr<ITexture>& texture) override;
        uint getTextureCount() override;
        std::shared_ptr<IShader> getShader(IShader::Type type) override;

        std::vector<std::shared_ptr<IShader>> getShaders() override;


        void setUniform(const std::shared_ptr<IUniform>& uniform);

        bool compile() override;
        void bind() override;
        void unbind() override;
    protected:

        struct IntDefaultedToMinusOne{
            int i = -1;

            operator int() const {
                return i;
            }

        };

        bool compiled;
        uint id = 0;

        std::unordered_map<unsigned int ,std::shared_ptr<ITexture>> additionalTextures;
        std::shared_ptr<IShader> vertexShader;
        std::shared_ptr<IShader> fragmentShader;
        std::shared_ptr<IShader> geometryShader;
        std::shared_ptr<IShader> tessalationControlShader;
        std::shared_ptr<IShader> tessalationEvaluationShader;
        std::shared_ptr<IShader> computeShader;
        std::unordered_map<uint, IntDefaultedToMinusOne> uniformLocations;
        std::unordered_map<uint, IntDefaultedToMinusOne> ssboLocations;

        bool shadersUpdated();

        void compileAndLinkShaders();

        bool checkStatus();

        void readUniforms();

        void createProgram();
    };
} // namespace MapGenerator::Renderer::SceneObjects