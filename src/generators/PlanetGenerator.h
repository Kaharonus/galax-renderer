//
// Created by tomas on 7.9.22.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <orbital/Planet.h>
#include <assets/AssetLoader.h>
#include <renderer/impl/Shader.h>
#include <renderer/impl/Program.h>
#include <renderer/impl/Texture.h>
#include <assets/PlanetConfig.h>
#include <renderer/impl/Mesh.h>
#include <orbital/Atmosphere.h>
#include <orbital/FeedbackProgram.h>

namespace Galax::Generators {

    using namespace Galax::Orbital;
    using namespace Galax::Assets;
    using namespace Galax::Renderer;
    class PlanetGenerator {
    public:

		explicit PlanetGenerator(const std::shared_ptr<AssetLoader> &loader);

        std::shared_ptr<Planet> createFromType(Planet::Type type);

		std::shared_ptr<Atmosphere> createAtmosphere();

    private:
		void prepareAtmosphere();


		int planetCount = 0;

		std::shared_ptr<AssetLoader> loader;
        std::unordered_map<Planet::Type, std::shared_ptr<Program>> programs;
        std::unordered_map<Planet::Type, std::shared_ptr<PlanetConfig>> configs;
        std::unordered_map<Planet::Type, std::shared_ptr<Texture>> colorPalette;
        std::shared_ptr<Mesh> planetMesh;
		std::shared_ptr<Mesh> atmosphereMesh;
		std::shared_ptr<Program> atmosphereProgram;
        std::array<std::shared_ptr<Shader>, 5> planetGeneratorShaders;
		std::array<std::shared_ptr<Shader>, 5> moonGeneratorShaders;

        glm::vec3 fromHex(const std::string &hex);

        Planet::Type fromStr(const std::string &str);

        void generatePalette(Planet::Type type);


		void prepareMoon();

		std::shared_ptr<FeedbackProgram> createPlanetGenerator(bool moon);
	};

}
