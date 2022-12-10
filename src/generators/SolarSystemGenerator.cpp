//
// Created by tomas on 3.9.22.
//

#include <generators/SolarSystemGenerator.h>
#include <generators/PlanetGenerator.h>
#include <effects/Bloom.h>
#include <orbital/Sun.h>

using namespace Galax::Generators;
using namespace Galax::Assets;
using namespace Galax::Renderer;
using namespace Galax::Renderer::SceneObjects;

void SolarSystemGenerator::setConfig(const std::shared_ptr<Assets::SceneInfo::SceneConfig> &config) {
	this->config = config;
}

void SolarSystemGenerator::setAssets(const std::shared_ptr<Assets::AssetLoader> &assets) {
	this->assets = assets;
}

std::string SolarSystemGenerator::getName() {
	return "SolarSystem";
}

std::shared_ptr<ISceneGenerator::SceneData> SolarSystemGenerator::generate() {
	scene = std::make_shared<Scene>();

	addCamera();

	addSkyBox();

	addSun();

	addPlanets();

	addLighting();

	addPostProcessing();

	auto sceneData = std::make_shared<ISceneGenerator::SceneData>();
	sceneData->scene = scene;
	sceneData->postProcessEffects = postProcessEffects;

	return sceneData;
}


void SolarSystemGenerator::addPostProcessing() {
	postProcessEffects.clear();
	addBloom();
	addOutline();
	addFXAA();
	addHDR();
}


void SolarSystemGenerator::addSun() {
	sun = std::make_shared<Sun>("Sun (Quad based)");
	auto vShader = assets->getShader("shaders/sun/sun.vs.shader", Shader::Type::VERTEX, "Sun VS");
	auto fShader = assets->getShader("shaders/sun/sun.fs.shader", Shader::Type::FRAGMENT, "Sun FS");
	auto program = std::make_shared<Program>("Sun Program", vShader, fShader);
	sun->setProgram(program);
	sun->setPosition(glm::vec3(0, 0, 0));
	sun->setScale(glm::vec3(3));
	sun->setCamera(camera);
	scene->addModel(sun);

}

void SolarSystemGenerator::addSkyBox() {
	auto sky = std::make_shared<Node>("Skybox");
	sky->setMesh(assets->getMesh("models/skybox.obj", "skybox mesh"));

	auto skyTexture = assets->getCubemap("textures/skybox", "skybox");
	skyTexture->setDimensions(4096, 4096);
	sky->addTexture(skyTexture);

	auto skyVertex = assets->getShader("shaders/skybox/skybox.vs.shader", Shader::VERTEX, "Skybox VS");
	auto skyFragment = assets->getShader("shaders/skybox/skybox.fs.shader", Shader::FRAGMENT, "Skybox FS");
	sky->setProgram(std::make_shared<Program>("Skybox program", skyVertex, skyFragment));

	sky->setCamera(camera);
	scene->addModel(sky);
}

void SolarSystemGenerator::addPlanets() {
	auto planetGenerator = std::make_shared<PlanetGenerator>(assets);
	for (int i = 0; i < 1; i++) {
		//TODO change the hardcoded values to dynamic ones

		auto atmosphere = planetGenerator->createAtmosphere();
		atmosphere->setCamera(camera);
		auto orbit = 20 * (i + 1);
		auto planet = planetGenerator->createFromType(Planet::Type::TEMPERATE);
		planet->configure()
				->withPosition(glm::vec3(orbit, 0, 0))
				->withSeed(50.0f * (i + 1))
				->withRadius(0.5f + (0.25f * i))
				->withRotation(10000)
				//->withOrbit(glm::vec3(orbit, 2, orbit), sun->getPositionUniform())
				->withCamera(camera)
				->withAtmosphere(atmosphere);

		auto moon = planetGenerator->createFromType(Planet::Type::MOON);
		planet->addChild(moon);
		moon->configure()
			->withPosition(glm::vec3(0,3,0))
			->withSeed(50.0f * (i + 1))
			->withRadius(0.5f + (0.05 * i))
			->withRotation(10000)
			->withCamera(camera);

		scene->addModel(planet);
	}
}



void SolarSystemGenerator::addHDR() {
	auto hdr = std::make_shared<PostProcessEffect>("HDR");
	auto shader = assets->getShader("shaders/effects/hdr.fs.shader", Shader::FRAGMENT);
	hdr->setShader(shader);
	hdr->addInputTexture(fxaaResult);
	hdr->addInputTexture(bloomResult);
	hdr->addInputTexture(outlineResult);

	postProcessEffects.push_back(hdr);
}


void SolarSystemGenerator::addBloom() {
	bloomResult = std::make_shared<Texture>(
			"bloomResult",
			Texture::TYPE_2D,
			Texture::RGB,
			Texture::FLOAT,
			Texture::MIRRORED_REPEAT,
			Texture::LINEAR
	);
	auto bloom = std::make_shared<Galax::Effects::Bloom>("Bloom");
	bloom->addInputTexture(bloomTexture);
	bloom->addOutputTexture(bloomResult);
	bloom->setShader(assets->getShader("shaders/effects/bloom.fs.shader", Shader::FRAGMENT, "Bloom shader"));
	postProcessEffects.push_back(bloom);
}


void SolarSystemGenerator::addOutline() {
	auto outline = std::make_shared<Galax::Effects::Outline>("Outline");
	auto shader = assets->getShader("shaders/effects/outline.fs.shader", Shader::FRAGMENT, "Outline shader");
	outline->setShader(shader);

	outlineResult = std::make_shared<Texture>(
			"outlineResult",
			Texture::TYPE_2D,
			Texture::RGB,
			Texture::UNSIGNED_BYTE,
			Texture::Wrap::REPEAT,
			Texture::NEAREST
	);
	outline->addOutputTexture(outlineResult);
	outline->requestGBufferTextures(IPostProcessEffect::GBufferTexture::METADATA);
	this->postProcessEffects.push_back(outline);
}


void SolarSystemGenerator::addFXAA() {
	auto fxaa = std::make_shared<PostProcessEffect>("FXAA");
	auto shader = assets->getShader("shaders/effects/fxaa.fs.shader", Shader::FRAGMENT, "FXAA shader");
	fxaa->setShader(shader);
	fxaa->addInputTexture(lightTexture);
	fxaaResult = std::make_shared<Texture>(
			"fxaaResult",
			Texture::TYPE_2D,
			Texture::RGBA,
			Texture::FLOAT,
			Texture::Wrap::MIRRORED_REPEAT,
			Texture::NEAREST
	);
	fxaa->addOutputTexture(fxaaResult);
	this->postProcessEffects.push_back(fxaa);
}

void SolarSystemGenerator::addCamera() {
	camera = std::make_shared<SpaceCamera>("freeCam");
	camera->acceptInput(true);
	camera->setPosition(glm::vec3(20, 0, -8));
}


void SolarSystemGenerator::addLighting() {
	lighting = std::make_shared<LightingModel>();
	lightTexture = std::make_shared<Texture>(
			"lightMap",
			Texture::TYPE_2D,
			Texture::RGBA,
			Texture::FLOAT,
			Texture::MIRRORED_REPEAT,
			Texture::NEAREST
	);

	bloomTexture = std::make_shared<Texture>(
			"bloomMap",
			Texture::TYPE_2D,
			Texture::RGB,
			Texture::FLOAT,
			Texture::MIRRORED_REPEAT,
			Texture::NEAREST
	);

	lighting->addOutputTexture(lightTexture);
	lighting->addOutputTexture(bloomTexture);

	lighting->setLightningShader(assets->getShader("shaders/lighting/lighting.fs.shader", Shader::FRAGMENT));
	scene->setLightingModel(lighting);

	auto sunLight = std::make_shared<Light>();
	sunLight->setColor(glm::vec3(1, 1, 1));
	sunLight->setIntensity(1);
	sunLight->setPositionUniform(sun->getPositionUniform());

	lighting->addLight(sunLight);

}
