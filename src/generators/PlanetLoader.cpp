//
// Created by tomas on 7.9.22.
//

#include <generators/PlanetLoader.h>
#include <Extensions.h>
#include <memory>
#include <map>
#include <unordered_map>
#include <glm/gtx/hash.hpp>

using namespace Galax::Orbital;

using namespace Galax::Generators;


bool PlanetLoader::initialized = false;
std::unordered_map<Planet::Type, std::shared_ptr<Program>> PlanetLoader::programs;
std::unordered_map<Planet::Type, std::shared_ptr<PlanetConfig>> PlanetLoader::configs;
std::unordered_map<Planet::Type, std::shared_ptr<Texture>> PlanetLoader::colorPalette;
std::shared_ptr<Mesh> PlanetLoader::baseMesh;
std::array<std::shared_ptr<Shader>, 5> PlanetLoader::generatorShaders;
std::shared_ptr<Program> PlanetLoader::atmosphereProgram;

std::shared_ptr<Planet> PlanetLoader::fromType(const std::string &name, Galax::Orbital::Planet::Type type) {
    if (!initialized) {
        assert(!initialized);
    }
    auto planet = std::make_shared<Planet>(name, type);
    planet->setProgram(programs[type]);
    planet->setGeneratorProgram(createPlanetGenerator());
    planet->setMesh(baseMesh);
    planet->setBodyMass(5);
    planet->addTexture(colorPalette[type]);
	planet->addChild(createAtmosphere());
    return planet;
}


std::shared_ptr<Node> PlanetLoader::createAtmosphere(){
	auto atmosphere = std::make_shared<Node>("Atmosphere");
	atmosphere->setMesh(baseMesh);
	atmosphere->setScale(glm::vec3(1.2f));
	atmosphere->setProgram(atmosphereProgram);
	atmosphere->setTransparent(true);
	return atmosphere;
}

void PlanetLoader::prepareAtmosphere(const std::shared_ptr<AssetLoader> &loader){
	auto fs = loader->getShader("shaders/planets/atmosphere/atmosphere.fs.shader", Shader::Type::FRAGMENT);
	auto vs = loader->getShader("shaders/planets/atmosphere/atmosphere.vs.shader", Shader::Type::VERTEX);
	auto program = std::make_shared<Program>("Atmosphere program",vs, fs);
	atmosphereProgram = program;
}

Planet::Type PlanetLoader::fromStr(const std::string &str) {
    if (str == "rocky") return Planet::Type::ROCKY;
    if (str == "gas_giant") return Planet::Type::GAS_GIANT;
    if (str == "ice") return Planet::Type::ICE;
    if (str == "hot") return Planet::Type::HOT;
    if (str == "temperate") return Planet::Type::TEMPERATE;
    if (str == "ocean") return Planet::Type::OCEAN;
    return Planet::Type::TEMPERATE;
}


void PlanetLoader::init(const std::shared_ptr<AssetLoader> &loader) {
    if (initialized) return;
    initialized = true;

    std::vector<std::string> types = {"temperate"};

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
        generatePalette(type);
    }

	prepareAtmosphere(loader);

    //Init the planet mesh generator
    generateMesh(loader);
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
        generatorShaders[i] = loader->getShader(paths[i], pathTypes[i]);
    }
}


std::shared_ptr<FeedbackProgram> PlanetLoader::createPlanetGenerator(){
    auto planetGeneratorProgram = std::make_shared<FeedbackProgram>();
    for(auto &shader : generatorShaders) {
        planetGeneratorProgram->addShader(shader);
    }
    planetGeneratorProgram->addFeedbackVariable("gsPosition", 3);
    planetGeneratorProgram->addFeedbackVariable("gsNoise", 1);
    return planetGeneratorProgram;
}


void PlanetLoader::generatePalette(Planet::Type type) {
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

glm::vec3 PlanetLoader::fromHex(const std::string &hex) {
    if (hex[0] == '#') {
        return {
                (float) std::stoi(hex.substr(1, 2), nullptr, 16) / 255.0f,
                (float) std::stoi(hex.substr(3, 2), nullptr, 16) / 255.0f,
                (float) std::stoi(hex.substr(5, 2), nullptr, 16) / 255.0f
        };
    }
    return {0, 0, 0};
}

void PlanetLoader::generateMesh(const std::shared_ptr<AssetLoader> &loader) {
    auto planet = loader->getMesh("models/planet.obj", "Base planet");
    baseMesh = planet;
}
