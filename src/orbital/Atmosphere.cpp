//
// Created by tomas on 5.12.22.
//
#include <glbinding/gl/gl.h>

#include <orbital/Atmosphere.h>
#include <Extensions.h>

using namespace Galax::Orbital;
using namespace gl;

void Atmosphere::draw(glm::mat4 parentModel) {
	//Extract position and scale from model matrix

	auto position = glm::vec3(1);
	auto scale = glm::vec3(1);

	if(this->parent){
		position = this->parent->getRelativePosition();
		scale = this->parent->getRelativeScale();
	}

	//Recreate model matrix without rotation
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::scale(model, scale);

	// Flip faces if camera is outside the planet
	// This is done, so I don't have to disable depth test and I can render the atmosphere from inside the planet
	auto cameraPosition = camera->getPosition();
	auto cameraDistance = glm::distance(cameraPosition, position);
	if (cameraDistance > this->scale.x) {
		gl::glFrontFace(gl::GL_CW);
		Node::draw(model);
		gl::glFrontFace(gl::GL_CCW);
	}else{
		GL_DISABLE(gl::GL_DEPTH_TEST,{
			Node::draw(model);
		});
	}


}
