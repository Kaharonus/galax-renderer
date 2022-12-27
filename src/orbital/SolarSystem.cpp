//
// Created by tomas on 11.12.22.
//

#include <orbital/SolarSystem.h>
#include <physics/impl/RigidBody.h>
#include <orbital/SpaceCamera.h>

using namespace Galax::Orbital;
using namespace Galax::Renderer;

Galax::Orbital::SolarSystem::SolarSystem() : Scene() {

}

void SolarSystem::setInputHandler(std::shared_ptr<InputHandler> inputHandler) {
	inputHandler->setClickOnDetection([&](InputHandler::MouseButton button, float x, float y){
		std::shared_ptr<Physics::IRenderNode> body;

		for(const auto& child: this->getAllNodes()){
			auto rigidBody = std::dynamic_pointer_cast<Galax::Physics::IRigidBody>(child);
			if(rigidBody && rigidBody->isMouseOver()){
				body = child;
			}
		}

		for(const auto& child: this->getAllNodes()){
			auto cam = std::dynamic_pointer_cast<Orbital::SpaceCamera>(child->getCamera());
			if(cam){
				cam->follow(body);
				break;
			}
		}
		return body;
	});
	Scene::setInputHandler(inputHandler);
}
