//
// Created by tomas on 6.12.22.
//


#pragma once


#include <renderer/impl/Camera.h>
#include <renderer/interfaces/IRenderNode.h>

using namespace Galax::Renderer::SceneObjects;
using namespace Galax::Renderer;

namespace Galax::Orbital{
	class SpaceCamera : public Camera {
	public:
		SpaceCamera(const std::string &name) : Camera(name) {}

		void follow(const std::shared_ptr<IRenderNode> &object);

		void update() override;
	protected:
		void calculateDistance();
		std::shared_ptr<IRenderNode> followedObject;
		float distance = 2;
		float targetDistance = 2;
	};
}

