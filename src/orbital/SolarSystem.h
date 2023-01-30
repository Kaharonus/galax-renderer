//
// Created by tomas on 11.12.22.
//


#pragma once

#include <renderer/Scene.h>

namespace Galax::Orbital{

	using namespace Galax::Renderer;

	class SolarSystem : public Galax::Renderer::Scene {
	public:
		SolarSystem();

		void setInputHandler(std::shared_ptr<InputHandler> inputHandler) override;

	};
}

