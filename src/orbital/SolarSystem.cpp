//
// Created by tomas on 7.9.22.
//

#include "SolarSystem.h"

using namespace Galax::Orbital;



std::vector<std::shared_ptr<Planet>> SolarSystem::getPlanets() {
	auto planets = std::vector<std::shared_ptr<Planet>>();
	for(auto& node : getModels()) {
		auto p = getPlanets(node);
		if(auto planet = std::dynamic_pointer_cast<Planet>(node)){
			p.push_back(planet);
		}
		planets.insert(planets.end(), p.begin(), p.end());
	}
    return planets;
}

std::vector<std::shared_ptr<Planet>> SolarSystem::getPlanets(const std::shared_ptr<IRenderNode> &node) {
    std::vector<std::shared_ptr<Planet>> planets;
    for (const auto &child : node->getChildren()) {
        if (auto planet = std::dynamic_pointer_cast<Planet>(child)) {
            planets.push_back(planet);
        }
        auto children = getPlanets(child);
        planets.insert(planets.end(), children.begin(), children.end());
    }
    return planets;
}
