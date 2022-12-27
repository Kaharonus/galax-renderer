//
// Created by tomas on 5.12.22.
//
#include <glbinding/gl/gl.h>

#include <orbital/Atmosphere.h>
#include <Extensions.h>

using namespace Galax::Orbital;
using namespace gl;

Atmosphere::Atmosphere(const std::string &name) : Galax::Renderer::RenderNode(name) {
	init();
}

Atmosphere::Atmosphere() : Galax::Renderer::RenderNode() {
	init();
}

void Atmosphere::init() {
	this->atmosphereColor = std::make_shared<Uniform>("beta_ray", Uniform::VEC3, glm::vec3(1.0f, 2.0f, 3.0f));
	this->atmosphereScale = std::make_shared<Uniform>("atmo_scale", Uniform::FLOAT, 1.15f);
	this->planetRadius = std::make_shared<Uniform>("planet_radius", Uniform::FLOAT, 0.8f);
	this->atmosphereDensity = std::make_shared<Uniform>("density_multiplier", Uniform::FLOAT, 0.25f);

	this->addUniform(this->atmosphereColor);
	this->addUniform(this->atmosphereScale);
	this->addUniform(this->planetRadius);
	this->addUniform(this->atmosphereDensity);

}


void Atmosphere::draw(glm::mat4 parentModel) {
	//Extract position and scale from model matrix

	auto position = glm::vec3(1);
	auto scale = glm::vec3(1);

	if (this->parent) {
		position = this->parent->getRelativePosition();
		scale = this->parent->getScale();
	}

	//Recreate model matrix without rotation
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::scale(model, scale);
	this->currentDrawMatrix = model;

	//Used to deal with the fact that I can scale up or down the planet
	this->planetRadius->setValue(parent ? parent->getScale().x : this->getRelativeScale().x);


	// Flip faces if camera is outside the planet
	// This is done, so I don't have to disable depth test and I can render the atmosphere from inside the planet
	auto cameraPosition = camera->getPosition();
	auto cameraDistance = glm::distance(cameraPosition, position);
	if (cameraDistance > this->getScale().x * 2) {
		gl::glFrontFace(gl::GL_CW);
		RenderNode::draw(model);
		gl::glFrontFace(gl::GL_CCW);
	} else {
		GL_DISABLE(gl::GL_DEPTH_TEST, {
			RenderNode::draw(model);
		});
	}


}




void Atmosphere::setDensity(float density) {
	this->atmosphereDensity->setValue(density);
}

void Atmosphere::setRadius(float radius) {
	this->atmosphereScale->setValue(radius);
}

void Atmosphere::setColor(glm::vec3 color) {
	this->atmosphereColor->setValue(color);
}

float Atmosphere::getDensity() {
	return this->atmosphereDensity->getValue<float>();
}

glm::vec3 Atmosphere::getColor() {
	return this->atmosphereColor->getValue<glm::vec3>();
}

float Atmosphere::getRadius() {
	return this->atmosphereScale->getValue<float>();
}


