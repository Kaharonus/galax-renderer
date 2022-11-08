//
// Created by tomas on 20.9.22.
//

#include <generators/SunLoader.h>

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Generators;
using namespace Galax::Assets;

std::shared_ptr<Node> SunLoader::load(std::shared_ptr<AssetLoader> &loader) {
    auto sun = std::make_shared<Node>("Sun");
    sun->setMesh(loader->getMesh("models/sun.obj", "sun"));
    auto vShader = loader->getShader("shaders/sun/sun.vs.shader", Shader::Type::VERTEX, "sun vs");
    auto fShader = loader->getShader("shaders/sun/sun.fs.shader", Shader::Type::FRAGMENT, "sun fs");
    auto program = std::make_shared<Program>("sun program", vShader, fShader);
    sun->setProgram(program);
    sun->setPosition(glm::vec3(0, 0, 0));
    sun->setScale(glm::vec3(1, 1, 1));

    return sun;
}
