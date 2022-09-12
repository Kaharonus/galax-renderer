//
// Created by tomas on 3.9.22.
//

#include "SolarSystemLoader.h"
#include "PlanetLoader.h"

using namespace Galax::Generators;
using namespace Galax::Assets;
using namespace Galax::Renderer;


std::shared_ptr<Camera> SolarSystemLoader::generateCamera(std::shared_ptr<AssetLoader> loader, float w, float h){
    auto camera = std::make_shared<Camera>("camera");
    camera->setDimensions(w, h);
    camera->acceptInput(true);
    camera->setPosition(glm::vec3(0, 0, -3));
    return camera;
}

std::map<float ,std::shared_ptr<Mesh>> SolarSystemLoader::generateMesh(std::shared_ptr<AssetLoader> loader){
    /*PlanetMeshGenerator planetGen(loader);
    std::map<float ,std::shared_ptr<Mesh>> result;
    auto meshes = planetGen.getPlanetMesh("EarthLike ugliness that will look nice one day", "models/planet", 1);
    for(int i = 0; i < meshes.size(); i++){
        result[(float)i * 10] = meshes[i];
    }
    return result;*/
}


std::shared_ptr<Program> SolarSystemLoader::generateProgram(std::shared_ptr<AssetLoader> loader){
    auto vs = loader->getShader("/shaders/basic.vs.shader", Shader::Type::VERTEX);
    auto fs = loader->getShader("/shaders/basic.fs.shader", Shader::Type::FRAGMENT);
    auto program = std::make_shared<Program>("PlanetProgram",vs, fs);
    return program;
}


std::vector<std::shared_ptr<Animation>> SolarSystemLoader::generateAnimations(std::shared_ptr<AssetLoader> loader){
    auto result = std::vector<std::shared_ptr<Animation>>();

    auto spinner = std::make_shared<Animation>("FidgetSpinner");
    spinner->setLength(10000);
    spinner->setRepeat(Animation::Repeat::LOOP);
    spinner->setEase(Animation::Ease::LINEAR);
    spinner->setTarget(Animation::Target::ROTATION);
    spinner->addKeyFrame(0, glm::vec3(0, 0, 0));
    spinner->addKeyFrame(10000, glm::vec3(0, 360, 0));
    result.push_back(spinner);
    return result;

}


std::shared_ptr<Animation> SolarSystemLoader::generateRotation(float distance){
    auto animation = std::make_shared<Animation>("Orbit (" + std::to_string(distance) + ")");
    auto length = 1000 * distance;
    animation->setLength(length);
    animation->setRepeat(Animation::Repeat::LOOP);
    animation->setEase(Animation::Ease::LINEAR);
    animation->setTarget(Animation::Target::POSITION);
    animation->addKeyFrame(0, glm::vec3(distance));
    animation->addKeyFrame(length, glm::vec3(distance));
    animation->setUpdateFunction([](const Animation& animation, UniformT value, float time){
        constexpr auto pi2 = 2 * glm::pi<float>();
        auto x = std::sin(time * pi2);
        auto y = std::cos(time * pi2);
        return glm::vec3(x, 0, y) * std::get<glm::vec3>(value);
    });

    return animation;
}




std::shared_ptr<SolarSystem> SolarSystemLoader::generateSystem() {

    auto assets = std::make_shared<AssetLoader>();
    bool created = assets->load("assets.data");

    auto system = std::make_shared<SolarSystem>();

    auto camera = std::make_shared<Camera>("freeCam");
    camera->acceptInput(true);
    camera->setPosition(glm::vec3(0, 0, -3));


    //TODO generate sun


    //generate planets

    PlanetLoader::init(assets);
    auto planet = PlanetLoader::fromType("EarthLike", Planet::Type::TEMPERATE);
    planet->setCamera(camera);

    system->setRoot(planet);

    return system;



    /*auto planetMeshes = (assets);

    auto scene = std::make_shared<SolarSystem>();

    auto root = std::make_shared<Node>("empty root node");



    for(int i = 0; i < 1; i++){
        auto planet = generatePlanet(planetMeshes, 1 + (i*0.5), 10 * (i+1));

        planet->setCamera(camera);
        planet->setProgram(program);
        root->addChild(planet);
    }

    scene->setRoot(root);
    scene->build();
    return scene;*/
}
