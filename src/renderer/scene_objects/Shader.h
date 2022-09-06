
#pragma once
#include "../SceneObject.h"
#include <memory>
#include <string>

namespace Galax::Renderer::SceneObjects {

    class Shader : public SceneObject {
    public:
        enum Type {
            None,
            VERTEX,
            FRAGMENT,
            GEOMETRY,
            TESSALATION_CTRL,
            TESSALATION_EVAL,
            COMPUTE
        };

        Shader();
        Shader(const std::string& source, Type type);
        Shader(const std::string& name, const std::string& source, Type type);

        uint getId() override;

        void linkTo(uint programId);
        bool getUpdated() const;

        void setSource(const std::string& source);
        void setType(Type type);
        std::string getInfoLog();
        std::string& getSource();
        void recompile();
        Type getType();
        int getGLType();
        std::string getTypeString();
        ~Shader();

    private:
        bool updated = false;

        uint id = 0;
        std::string infoLog;
        std::string code;

        Type type;
    };
} // namespace MapGenerator::Renderer::SceneObjects