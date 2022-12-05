//
// Created by tomas on 5.12.22.
//


#pragma once

#include <renderer/impl/Node.h>

namespace Galax::Orbital {

	class Atmosphere : public Galax::Renderer::Node {
	public:
		Atmosphere() = default;
		Atmosphere(const std::string& name) : Node(name) {};

		void draw(glm::mat4 parentModel) override;
	};
}

