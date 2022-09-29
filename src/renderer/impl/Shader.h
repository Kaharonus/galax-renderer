
#pragma once
#include "../SceneObject.h"
#include "../interfaces/IShader.h"
#include <memory>
#include <string>

namespace Galax::Renderer::SceneObjects {

    class Shader : public IShader {
    public:

        Shader();
        Shader(const std::string& name, const std::string& source, Type type);

        uint getId() override;

        void linkTo(uint programId) override;
        [[nodiscard]] bool getUpdated() const override;

        void setSource(const std::string& source) override;
        void setType(Type type) override;
        std::string getInfoLog() override;
        std::string& getSource() override;
        void recompile() override;
        Type getType() override;
        std::string getTypeString() override ;
        ~Shader() override;

    private:
        bool updated = false;

        uint id = 0;
        std::string infoLog;
        std::string code;

        Type type;
    };
} // namespace MapGenerator::Renderer::SceneObjects