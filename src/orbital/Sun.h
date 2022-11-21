//
// Created by tomas on 16.11.22.
//


#pragma once

#include <renderer/impl/Node.h>
#include "renderer/interfaces/IMesh.h"

namespace Galax::Orbital{
	using namespace Galax::Renderer::SceneObjects;
	class Sun : public Node {
	public:
		Sun(const std::string& name);

		void setMesh(std::shared_ptr<IMesh> mesh) override;
		void draw() override;

	protected:
		std::shared_ptr<Quad> mesh;
	};

}
