//
// Created by tomas on 7.9.22.
//

#include <generators/PlanetGenerator.h>
#include <Extensions.h>
#include <memory>
#include <map>
#include <unordered_map>
#include <glm/gtx/hash.hpp>

using namespace Galax::Orbital;

using namespace Galax::Generators;

PlanetGenerator::PlanetGenerator(const std::shared_ptr<AssetLoader> &loader) {
	srand(static_cast <unsigned> (time(0)));
	this->loader = loader;
	std::vector<std::string> types = {"temperate", "rocky", "ocean", "hot"};

	for (auto &typeName: types) {
		//Load the programs
		auto type = fromStr(typeName);
		auto fs = loader->getShader("shaders/planets/" + typeName + ".fs.shader", Shader::Type::FRAGMENT);
		auto vs = loader->getShader("shaders/planets/planet.vs.shader", Shader::Type::VERTEX);
		auto gs = loader->getShader("shaders/planets/planet.gs.shader", Shader::Type::GEOMETRY);
		auto program = std::make_shared<Program>(typeName + " prog");
		program->addShader(vs);
		program->addShader(fs);
		program->addShader(gs);
		programs[type] = program;

		//Load planet configs
		auto config = loader->getJson<Assets::PlanetConfig>("planets/" + typeName + ".json");
		configs[type] = config;

		//Generate color palettes
		if(!config->colorPalette.empty()){
			generatePalette(type);

		}
	}

	prepareAtmosphere();

	prepareMoon();

	//Init the planet mesh generator
	planetMesh = loader->getMesh("models/planet.obj");
	atmosphereMesh = loader->getMesh("models/skybox.obj", "Atmosphere - cube");
	std::vector<std::string> paths = {
			"shaders/planets/generation/vertex.shader",
			"shaders/planets/generation/fragment.shader",
			"shaders/planets/generation/geometry.shader",
			"shaders/planets/generation/tess_control.shader",
			"shaders/planets/generation/tess_eval.shader",
	};
	std::vector<Shader::Type> pathTypes = { Shader::Type::VERTEX, Shader::Type::FRAGMENT, Shader::Type::GEOMETRY,
											Shader::Type::TESSALATION_CTRL, Shader::Type::TESSALATION_EVAL};

	for(int i = 0; i < paths.size(); i++) {
		planetGeneratorShaders[i] = loader->getShader(paths[i], pathTypes[i]);
	}


}

//This function is terrible. It was okay ish but i was doing a lot of last minute changes and didnt have time to do a proper refactor
//TODO: actually do this correctly
std::shared_ptr<Planet> PlanetGenerator::createFromType(Galax::Orbital::Planet::Type type, std::shared_ptr<SpaceCamera> camera) {
	planetCount++;
	bool isMoon = type == Planet::Type::MOON;
	auto config = configs[type];
	auto name =(isMoon ? "Moon " : "Planet ") + std::to_string(planetCount);
    auto planet = std::make_shared<Planet>(name, type);
    planet->setProgram(programs[type]);
    planet->setGeneratorProgram(createPlanetGenerator(isMoon));
    planet->setMesh(planetMesh);
    planet->setBodyMass(5);
	planet->setClampUnder(config->clampUnder);

	for(auto noiseLevel: config->noiseSettings){
		planet->addNoiseLevel(noiseLevel.roughness, noiseLevel.strength);
	}
	planet->configure()
			->withSeed(rndFloat(0,500))
			->withCamera(camera);

	if(isMoon){ // This shit doesnt need more configuration.
		return planet;
	}
	glm::vec3 position = glm::vec3(0);
	while(glm::length(position) < 10){
		auto xy = rndFloat(-250,250);
		position = glm::vec3(xy,rndFloat(-2,2), xy);
	}
	planet->configure()
			->withRadius(rndFloat(config->minSize, config->maxSize))
			->withPosition(position)
			->withRotation(10000);


	planet->addTexture(colorPalette[type]);
	auto moonCount = rndInt(config->minMoons, config->maxMoons);
	auto moonConfig = configs[Orbital::Planet::Type::MOON];
	for(int i = 0; i < moonCount; i++){
		auto moon = createFromType(Planet::Type::MOON, camera);
		moon->setParent(planet);
		planet->configure()->withMoon(moon);
		auto position = glm::vec3(rndFloat(-8,8),rndFloat(-8,8), rndFloat(-8,8));
		moon->configure()
				->withPosition(position)
				->withRadius(rndFloat(moonConfig->minSize, moonConfig->maxSize))
				->withRotation(5000);
	}

	if(!config->atmosphere.color.empty()){
		auto atmo = createAtmosphere("Atmosphere (" + name + ")");
		atmo->setDensity(config->atmosphere.density);
		atmo->setColor(glm::vec3(config->atmosphere.color[0], config->atmosphere.color[1], config->atmosphere.color[2]));
		atmo->setRadius(config->atmosphere.radius);
		atmo->setCamera(camera);
		planet->configure()->withAtmosphere(atmo);
	}
    return planet;
}





std::shared_ptr<Atmosphere> PlanetGenerator::createAtmosphere(const std::string& name){
	auto atmosphere = std::make_shared<Atmosphere>(name);
	atmosphere->setMesh(atmosphereMesh);
	atmosphere->setScale(glm::vec3(5.f));
	atmosphere->setProgram(atmosphereProgram);
	atmosphere->setTransparent(true);
	return atmosphere;
}

void PlanetGenerator::prepareAtmosphere(){
	auto fs = loader->getShader("shaders/planets/atmosphere/atmosphere.fs.shader", Shader::Type::FRAGMENT);
	auto vs = loader->getShader("shaders/planets/atmosphere/atmosphere.vs.shader", Shader::Type::VERTEX);
	auto program = std::make_shared<Program>("Atmosphere program",vs, fs);
	atmosphereProgram = program;
}

Planet::Type PlanetGenerator::fromStr(const std::string &str) {
    if (str == "rocky") return Planet::Type::ROCKY;
    if (str == "hot") return Planet::Type::HOT;
    if (str == "temperate") return Planet::Type::TEMPERATE;
    if (str == "ocean") return Planet::Type::OCEAN;
    return Planet::Type::TEMPERATE;
}

void PlanetGenerator::prepareMoon() {
	auto fs = loader->getShader("shaders/moons/moon.fs.shader", Shader::Type::FRAGMENT, "Moon - FS");
	auto vs = loader->getShader("shaders/moons/moon.vs.shader", Shader::Type::VERTEX, "Moon - VS");
	auto gs = loader->getShader("shaders/moons/moon.gs.shader", Shader::Type::GEOMETRY, "Moon - GS");
	auto program = std::make_shared<Program>("Moon program");
	program->addShader(vs);
	program->addShader(fs);
	program->addShader(gs);
	programs[Planet::Type::MOON] = program;

	vs = loader->getShader("shaders/moons/generation/vertex.shader", Shader::Type::VERTEX, "MoonGenerator - VS");
	fs = loader->getShader("shaders/moons/generation/fragment.shader", Shader::Type::FRAGMENT, "MoonGenerator - FS");
	gs = loader->getShader("shaders/moons/generation/geometry.shader", Shader::Type::GEOMETRY, "MoonGenerator - GS");
	auto tcs = loader->getShader("shaders/moons/generation/tess_control.shader", Shader::Type::TESSALATION_CTRL, "MoonGenerator - TCS");
	auto tes = loader->getShader("shaders/moons/generation/tess_eval.shader", Shader::Type::TESSALATION_EVAL, "MoonGenerator - TES");
	moonGeneratorShaders[0] = vs;
	moonGeneratorShaders[1] = fs;
	moonGeneratorShaders[2] = gs;
	moonGeneratorShaders[3] = tcs;
	moonGeneratorShaders[4] = tes;

	auto config = loader->getJson<Assets::PlanetConfig>("planets/moon.json");
	configs[Orbital::Planet::Type::MOON] = config;
}


std::shared_ptr<FeedbackProgram> PlanetGenerator::createPlanetGenerator(bool moon){
    auto planetGeneratorProgram = std::make_shared<FeedbackProgram>();
	auto generatorShaders = moon ? moonGeneratorShaders : planetGeneratorShaders;
    for(auto &shader : generatorShaders) {
        planetGeneratorProgram->addShader(shader);
    }
    planetGeneratorProgram->addFeedbackVariable("gsPosition", 3);
    planetGeneratorProgram->addFeedbackVariable("gsNoise", 1);
    return planetGeneratorProgram;
}


void PlanetGenerator::generatePalette(Planet::Type type) {
    int resolution = 1024;
    float fResolution = resolution;
    auto config = configs[type];

    auto palette = std::make_shared<Texture>("palette");
    palette->setWrap(Texture::CLAMP_TO_EDGE);
    palette->setFormat(Texture::RGB);
    palette->setDimensions(resolution, resolution);

    std::sort(config->colorPalette.begin(), config->colorPalette.end(),
              [](const ColorPalette &a, const ColorPalette &b) {
                  return a.heightStart < b.heightStart;
              });
    auto max = config->colorPalette.back().heightStart;
    auto min = config->colorPalette.front().heightStart;

    //Create the gradients for each color in the palette
    std::vector<std::map<double, glm::vec3>> biomeSteps;

    for (auto& biome: config->colorPalette) {
        std::map<double, glm::vec3> steps;
        std::sort(biome.colors.begin(), biome.colors.end(), [](auto &a, auto &b) {
            return a.endPoint < b.endPoint;
        });
        steps[0] = fromHex(biome.colors.front().color);
        for (auto& color: biome.colors) {
            steps[color.endPoint] = fromHex(color.color);
        }
        biomeSteps.push_back(steps);
    }

    std::map<float, std::pair<double, std::vector<glm::vec3>>> paletteMap;
    for (auto [i, color]: enumerate(config->colorPalette)) {
        auto normalised = (float) ((color.heightStart - min) / (max - min));
        auto biome = biomeSteps[i];
        std::vector<glm::vec3> columnColors;
        auto currentColor = biome.begin();
        auto nextColor = std::next(currentColor);
        for (int x = 0; x < resolution; x++) {
            auto progress = (float)x / fResolution;
            auto interpolationProgress = (progress - currentColor->first) / (nextColor->first - currentColor->first);
            auto result = glm::mix(currentColor->second, nextColor->second, interpolationProgress);
            columnColors.push_back(result);
            if (progress >= nextColor->first) {
                currentColor = nextColor;
                nextColor = std::next(currentColor);
            }
        }
        paletteMap[normalised] = {color.transition, columnColors};
    }
    auto data = std::vector<unsigned char>(resolution * resolution * 3);

    for (int y = 0; y < resolution; y++) {
        //Interpolate the column
        float rowProgress = (float) y / (float) resolution;
        auto color = paletteMap.upper_bound(rowProgress);
        if (color != paletteMap.begin()) {
            color--;
        }
        auto next = std::next(color);
        if (color == paletteMap.end()) {
            next = color;
        }
        for (int x = 0; x < resolution; x++) {
            auto index = (y * resolution + x) * 3;
            auto currentColor = color->second.second[x];
            auto nextColor = next->second.second[x];
            //Interpolate only if the transition is not 0 and the progress is greater than the transition
            auto progress = (double) x / (double) resolution;
            auto interpolator = (rowProgress - color->first) / (next->first - color->first);
            auto mixPercentage = color->second.first;
            glm::vec3 result = currentColor;
            if (interpolator > mixPercentage) {
                interpolator = (interpolator - mixPercentage) / (1 - mixPercentage);
                result = glm::mix(currentColor, nextColor, interpolator);
            }
            data[index] = (unsigned char) (result.r * 255);
            data[index + 1] = (unsigned char) (result.g * 255);
            data[index + 2] = (unsigned char) (result.b * 255);
        }
    }
    palette->setData(data);
    colorPalette[type] = palette;
}

glm::vec3 PlanetGenerator::fromHex(const std::string &hex) {
    if (hex[0] == '#') {
        return {
                (float) std::stoi(hex.substr(1, 2), nullptr, 16) / 255.0f,
                (float) std::stoi(hex.substr(3, 2), nullptr, 16) / 255.0f,
                (float) std::stoi(hex.substr(5, 2), nullptr, 16) / 255.0f
        };
    }
    return {0, 0, 0};
}

int PlanetGenerator::rndInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

float PlanetGenerator::rndFloat(float min, float max) {
	return min + static_cast <float> (rand()) /(static_cast <float>(RAND_MAX)/(max-min));
}


