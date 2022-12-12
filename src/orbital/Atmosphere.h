//
// Created by tomas on 5.12.22.
//


#pragma once

#include <renderer/impl/Node.h>
#include <renderer/impl/Uniform.h>

namespace Galax::Orbital {
	using namespace Galax::Renderer;

	class Atmosphere : public Galax::Renderer::Node {
	public:
		Atmosphere();
		Atmosphere(const std::string& name);

		void draw(glm::mat4 parentModel) override;

		void setColor(glm::vec3 color);
		void setRadius(float radius);
		void setDensity(float density);


	private:
		std::shared_ptr<Uniform> atmosphereColor;
		std::shared_ptr<Uniform> atmosphereScale;
		std::shared_ptr<Uniform> planetRadius;
		std::shared_ptr<Uniform> atmosphereDensity;
		std::shared_ptr<Uniform> lightOffset;

		float radius;

		void init();

	};
}

