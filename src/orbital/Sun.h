//
// Created by tomas on 16.11.22.
//


#pragma once

#include <renderer/impl/Node.h>

#include <renderer/impl/Uniform.h>
#include <renderer/impl/Mesh.h>

namespace Galax::Orbital{
	using namespace Galax::Renderer::SceneObjects;
	class Sun : public Node {
	public:
		Sun(const std::string& name);

		//void setMesh(std::shared_ptr<IMesh> mesh) override;
		void draw(glm::mat4 parentModel) override;

	protected:
		std::shared_ptr<Renderer::SceneObjects::Mesh> mesh;
		std::shared_ptr<Renderer::SceneObjects::Uniform> viewTowardsCamera;
	};

}
