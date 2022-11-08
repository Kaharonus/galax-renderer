//
// Created by tomas on 7.9.22.
//

#pragma once

#include <renderer/Scene.h>
#include <orbital/Planet.h>

namespace Galax::Orbital {

        class SolarSystem : public Renderer::Scene {
        public:
            std::vector<std::shared_ptr<Planet>> getPlanets();
        private:
            std::vector<std::shared_ptr<Planet>> getPlanets(const std::shared_ptr<INode> &node);
        };
} // Orbital
