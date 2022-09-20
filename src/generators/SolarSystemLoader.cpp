//
// Created by tomas on 3.9.22.
//

#include "SolarSystemLoader.h"
#include "PlanetLoader.h"

using namespace Galax::Generators;
using namespace Galax::Assets;
using namespace Galax::Renderer;



std::shared_ptr<Animation> SolarSystemLoader::generatePlanetSpin(int spinLength) {
    auto animation = std::make_shared<Animation>("Planet spin");
    animation->setLength(spinLength);
    animation->setRepeat(Animation::Repeat::LOOP);
    animation->setEase(Animation::Ease::LINEAR);
    animation->setTarget(Animation::Target::ROTATION);
    animation->addKeyFrame(0, glm::vec3(0, 0, 0));
    animation->addKeyFrame(spinLength, glm::vec3(0, 360, 0));
    return animation;
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

std::shared_ptr<PostProcessEffect> SolarSystemLoader::generateHDR(std::shared_ptr<Texture> lightMap){
    auto hdr = std::make_shared<PostProcessEffect>("HDR");
    auto shader = assets->getShader("shaders/effects/hdr.fs.shader", Shader::FRAGMENT);
    hdr->setShader(shader);
    hdr->addInputTexture(lightMap);
    return hdr;
}


Galax::Generators::SolarSystemLoader::RenderData SolarSystemLoader::generateSystem() {

    assets = std::make_shared<AssetLoader>();
    assets->load("assets.data");
    PlanetLoader::init(assets);


    //Create the lighting model
    auto lightingModel = std::make_shared<LightingModel>();
    lightingModel->setLightningShader(assets->getShader("shaders/lighting/lighting.fs.shader", Renderer::SceneObjects::Shader::FRAGMENT));

    auto lightTexture = std::make_shared<Texture>("lightMap", Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::REPEAT, Texture::NEAREST);
    lightingModel->addOutputTexture(lightTexture);

    auto system = std::make_shared<SolarSystem>();


    auto camera = std::make_shared<Camera>("freeCam");



    camera->acceptInput(true);
    camera->setPosition(glm::vec3(0, 0, -3));

    //TODO generate sun


    //generate planets

    auto planet = PlanetLoader::fromType("EarthLike", Planet::Type::TEMPERATE);
    planet->addAnimation(generatePlanetSpin(5000));
    planet->setCamera(camera);

    system->setRoot(planet);


    //generate post processes
    std::vector<std::shared_ptr<PostProcessEffect>> effects;
    effects.push_back(generateHDR(lightTexture));

    return {system, lightingModel, effects};
}
