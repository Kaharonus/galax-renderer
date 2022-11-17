//
// Created by tomas on 16.11.22.
//


#pragma once

#include <renderer/impl/Node.h>

namespace Galax::Orbital{
	using namespace Galax::Renderer::SceneObjects;
	class Sun : public Node {
	public:
		Sun(const std::string& name);

		void draw() override;
	};

}
