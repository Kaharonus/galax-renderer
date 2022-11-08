//
// Created by tomas on 3.9.22.
//

#include <generators/SolarSystemLoader.h>
#include <generators/PlanetLoader.h>
#include <generators/SunLoader.h>
#include <effects/Bloom.h>
#include <orbital/Gravity.h>

using namespace Galax::Generators;
using namespace Galax::Assets;
using namespace Galax::Renderer;



std::shared_ptr<Animation> SolarSystemLoader::generatePlanetSpin(int spinLength) {
    auto animation = std::make_shared<Animation>();
    animation->setLength(spinLength);
    animation->setRepeat(Animation::Repeat::LOOP);
    animation->setEase(Animation::Ease::LINEAR);
    animation->setTarget(IAnimation::Target::ROTATION);
    animation->addKeyFrame(0, glm::vec3(0, 0, 0));
    animation->addKeyFrame(spinLength, glm::vec3(0, 360, 0));
    return animation;
}


std::shared_ptr<Animation> SolarSystemLoader::generateRotation(float distance, std::shared_ptr<IUniform> sunPosition){
    auto animation = std::make_shared<Animation>("Orbit (" + std::to_string(distance) + ")");
    auto length = 2000 * distance;
    animation->setLength(length);
    animation->setRepeat(Animation::Repeat::LOOP);
    animation->setEase(Animation::Ease::LINEAR);
    animation->setTarget(IAnimation::Target::POSITION);
    animation->addKeyFrame(0, glm::vec3(distance));
    animation->addKeyFrame(length, glm::vec3(distance));
    animation->setUpdateFunction([sunPosition](const IAnimation& animation, IUniform::UniformT value, float time){
        auto center = std::get<glm::vec3>(sunPosition->getValue());
        constexpr auto pi2 = 2 * glm::pi<float>();
        auto x = std::sin(time * pi2) + center.x;
        auto y = std::cos(time * pi2) + center.z;
        return glm::vec3(x, 0, y) * std::get<glm::vec3>(value);
    });

    return animation;
}

std::shared_ptr<PostProcessEffect> SolarSystemLoader::generateHDR(std::shared_ptr<Texture> lightMap, std::shared_ptr<ITexture> bloomMap){
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



std::shared_ptr<Galax::Effects::Bloom> SolarSystemLoader::generateBloom(std::shared_ptr<AssetLoader> assets, std::shared_ptr<Texture> bloomMap) {
    auto bloom = std::make_shared<Galax::Effects::Bloom>("Bloom");
    auto result = std::make_shared<Texture>("bloomResult", Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::MIRRORED_REPEAT, Texture::LINEAR);
    bloom->setPasses(3);
    bloom->setInputTexture(bloomMap);
    bloom->setOutputTexture(result);

    bloom->setShader(assets->getShader("shaders/effects/bloom.fs.shader", Shader::FRAGMENT, "Bloom shader"));
    return bloom;
}

Galax::Generators::SolarSystemLoader::RenderData SolarSystemLoader::generateSystem() {

    assets = std::make_shared<AssetLoader>();
    assets->load("assets.data");
    PlanetLoader::init(assets);

    auto camera = std::make_shared<Camera>("freeCam");
    camera->acceptInput(true);
    camera->setPosition(glm::vec3(0, 0, -15));

    auto system = std::make_shared<SolarSystem>();

    auto sky = generateSkybox(assets);

    sky->setCamera(camera);
    system->setRoot(sky);

    auto sun = SunLoader::load(assets);
    sun->setCamera(camera);
    sky->addChild(sun);

    auto sunLight = std::make_shared<Light>();
    sunLight->setColor(glm::vec3(1, 1, 1));
    sunLight->setIntensity(1);
    sunLight->setPositionUniform(sun->getPositionUniform());


    //generate planets
    for(int i = 0; i < 1; i++){
        std::string name = "Planet " + std::to_string(i);
        auto planet = PlanetLoader::fromType(name, Planet::Type::TEMPERATE);
        planet->addAnimation(generatePlanetSpin(10000));
        //planet->addAnimation(generateRotation(10, sun->getPositionUniform()));
        planet->setScale(glm::vec3(0.25 * (i + 1)));
        planet->setCamera(camera);
        planet->addUniform(std::make_shared<Uniform>("inputSeed", Uniform::FLOAT, 50.0f*((float)i+1)));
        planet->setPosition(glm::vec3(10 * (i + 1), 0, 0));


        /*auto gravity = std::make_shared<Gravity>();
        gravity->setCenter(sun->getPositionUniform());
        gravity->setCenterMass(200000);
        gravity->setMass(100000);
        gravity->setPosition(planet->getPositionUniform());
        planet->addForce(gravity);*/

        sky->addChild(planet);

    }

    //Create the lighting model
    auto lightingModel = std::make_shared<LightingModel>();
    lightingModel->setLightningShader(assets->getShader("shaders/lighting/lighting.fs.shader", Renderer::SceneObjects::Shader::FRAGMENT));

    auto lightTexture = std::make_shared<Texture>("lightMap", Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::MIRRORED_REPEAT, Texture::NEAREST);
    lightingModel->addOutputTexture(lightTexture);

    auto bloomTexture = std::make_shared<Texture>("bloomMap", Texture::TYPE_2D, Texture::RGB, Texture::FLOAT, Texture::MIRRORED_REPEAT, Texture::NEAREST);
    lightingModel->addOutputTexture(bloomTexture);
    lightingModel->addLight(sunLight);


    //generate post processes
    std::vector<std::shared_ptr<IPostProcessEffect>> effects;

    auto bloom = generateBloom(assets, bloomTexture);
    effects.push_back(bloom);
    auto bloomMap = *bloom->getOutputTextures().rbegin();
    auto hdr = generateHDR(lightTexture, bloomMap);
    effects.push_back(hdr);
    return {system, lightingModel, effects};
}

