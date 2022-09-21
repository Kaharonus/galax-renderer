//
// Created by tomas on 3.9.22.
//

#include "SolarSystemLoader.h"
#include "PlanetLoader.h"
#include "SunLoader.h"

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

std::shared_ptr<PostProcessEffect> SolarSystemLoader::generateHDR(std::shared_ptr<Texture> lightMap, std::shared_ptr<Texture> bloomMap){
    auto hdr = std::make_shared<PostProcessEffect>("HDR");
    auto shader = assets->getShader("shaders/effects/hdr.fs.shader", Shader::FRAGMENT);
    hdr->setShader(shader);
    hdr->addInputTexture(lightMap);
    hdr->addInputTexture(bloomMap);

    return hdr;
}


std::shared_ptr<Node> SolarSystemLoader::generateSkybox(std::shared_ptr<AssetLoader> assets){
    auto sky = std::make_shared<Node>("Skybox");
    sky->setMesh(assets->getMesh("models/skybox.obj", "skybox mesh"));
    auto skyTexture = assets->getCubemap("textures/skybox", "skybox");
    skyTexture->setDimensions(4096, 4096);
    sky->addTexture(skyTexture);
    auto skyVertex = assets->getShader("shaders/skybox/skybox.vs.shader", Shader::VERTEX, "Skybox vertex shader");
    auto skyFragment = assets->getShader("shaders/skybox/skybox.fs.shader", Shader::FRAGMENT, "Skybox fragment shader");
    sky->setProgram(std::make_shared<Program>("Skybox program", skyVertex, skyFragment));
    return sky;
}



std::shared_ptr<PostProcessEffect> SolarSystemLoader::generateBloom(std::shared_ptr<Texture> bloomMap) {
    auto bloom = std::make_shared<PostProcessEffect>("Bloom");
    auto shader = assets->getShader("shaders/effects/bloom.fs.shader", Shader::FRAGMENT);
    bloom->addOutputTexture(bloomMap);
    bloom->addInputTexture(bloomMap);
    bloom->setShader(shader);
    //Should be an even number - every other iteration is a switch between horizontal and vertical blur
    bloom->setCallCount(10);
    return bloom;
}

Galax::Generators::SolarSystemLoader::RenderData SolarSystemLoader::generateSystem() {

    assets = std::make_shared<AssetLoader>();
    assets->load("assets.data");
    PlanetLoader::init(assets);

    auto camera = std::make_shared<Camera>("freeCam");
    camera->acceptInput(true);
    camera->setPosition(glm::vec3(-4.5, 0, -6));

    auto system = std::make_shared<SolarSystem>();

    auto sky = generateSkybox(assets);

    sky->setCamera(camera);
    system->setRoot(sky);

    //TODO generate sun

    auto sun = SunLoader::load(assets);
    sun->setCamera(camera);
    sky->addChild(sun);

    //generate planets
    auto planet = PlanetLoader::fromType("EarthLike", Planet::Type::TEMPERATE);
    planet->addAnimation(generatePlanetSpin(5000));
    planet->setCamera(camera);
    sky->addChild(planet);



    //Create the lighting model
    auto lightingModel = std::make_shared<LightingModel>();
    lightingModel->setLightningShader(assets->getShader("shaders/lighting/lighting.fs.shader", Renderer::SceneObjects::Shader::FRAGMENT));

    auto lightTexture = std::make_shared<Texture>("lightMap", Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::REPEAT, Texture::NEAREST);
    lightingModel->addOutputTexture(lightTexture);

    auto bloomTexture = std::make_shared<Texture>("bloomMap", Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::REPEAT, Texture::NEAREST);
    lightingModel->addOutputTexture(bloomTexture);

    //generate post processes
    std::vector<std::shared_ptr<PostProcessEffect>> effects;

    auto bloom = generateBloom(bloomTexture);
    effects.push_back(bloom);
    auto bloomMap = *bloom->getOutputTextures().rbegin();
    auto hdr = generateHDR(lightTexture, bloomMap);
    effects.push_back(hdr);
    return {system, lightingModel, effects};
}

