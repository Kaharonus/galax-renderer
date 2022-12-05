//
// Created by tomas on 5.12.22.
//
#include <glbinding/gl/gl.h>

#include <orbital/Atmosphere.h>

void Galax::Orbital::Atmosphere::draw(glm::mat4 parentModel) {
	//Extract position and scale from model matrix
	auto position = glm::vec3(parentModel[3]);
	auto scale = glm::vec3(glm::length(parentModel[0]), glm::length(parentModel[1]), glm::length(parentModel[2]));

	//Recreate model matrix without rotation
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::scale(model, scale);

	// Flip faces
	gl::glFrontFace(gl::GL_CW);
	Node::draw(model);
	gl::glFrontFace(gl::GL_CCW);

}