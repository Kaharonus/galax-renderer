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

std::shared_ptr<Galax::Physics::IRenderNode> findMouseOverRecursive(std::shared_ptr<IRenderNode> node){
	std::shared_ptr<Galax::Physics::IRigidBody> body = std::dynamic_pointer_cast<Galax::Physics::IRigidBody>(node);
	if(body && body->isMouseOver()){
		return node;
	}
	for(auto child : node->getChildren()){
		body = std::dynamic_pointer_cast<Galax::Physics::IRigidBody>(child);
		if(body && body->isMouseOver()){
			return child;
		}
		auto result = findMouseOverRecursive(child);
		if(result != nullptr){
			return result;
		}
	}
	return nullptr;
}


void SolarSystem::setInputHandler(std::shared_ptr<InputHandler> inputHandler) {
	inputHandler->registerMouseClickCallback([&](InputHandler::MouseButton button, float x, float y){
		std::shared_ptr<Physics::IRenderNode> body;
		for(const auto& model : this->models){
			auto result = findMouseOverRecursive(model);
			if(result != nullptr){
				body = result;
				break;
			}
		}
		for(auto model : this->models){
			auto camera = std::dynamic_pointer_cast<Orbital::SpaceCamera>(model->getCamera());
			if(camera != nullptr){
				camera->follow(body);
			}
		}
		std::cout << "Mouse click: " << x << ", " << y << std::endl;
	});
	Scene::setInputHandler(inputHandler);
}
