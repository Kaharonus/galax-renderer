//
// Created by tomas on 7.9.22.
//

#include <glbinding/gl/gl.h>
#include <iostream>

#include <orbital/Planet.h>
#include <orbital/OrbitAnimation.h>
#include <Extensions.h>
#include <physics/impl/SphereCollider.h>

using namespace Galax::Orbital;
using namespace gl;

Planet::Planet(const std::string &name, Planet::Type type) : Physics::PhysicalNode(name) {
	this->type = type;
	this->colorOffsetUniform = std::make_shared<Uniform>("colorOffset", Uniform::FLOAT, 0.0f);

	this->lightenAnimation = std::make_shared<Animation>("Lighten planet", Animation::Target::CUSTOM,
														 Animation::Repeat::ONCE);
	this->lightenAnimation->setLength(300);
	this->lightenAnimation->addKeyFrame(0, 0.f);
	this->lightenAnimation->addKeyFrame(300, 1.f);
	this->lightenAnimation->setStartType(Animation::StartType::MANUAL);
	this->lightenAnimation->setEase(Animation::Ease::QUADRATIC_IN);
	this->lightenAnimation->setCustomTarget(this->colorOffsetUniform);
	this->addAnimation(this->lightenAnimation);

	this->darkenAnimation = std::make_shared<Animation>("Darken planet", Animation::Target::CUSTOM,
														Animation::Repeat::ONCE);
	this->darkenAnimation->setLength(300);
	this->darkenAnimation->addKeyFrame(0, 1.0f);
	this->darkenAnimation->addKeyFrame(300, 0.f);
	this->darkenAnimation->setStartType(Animation::StartType::MANUAL);
	this->darkenAnimation->setEase(Animation::Ease::QUADRATIC_IN);
	this->darkenAnimation->setCustomTarget(this->colorOffsetUniform);
	this->addAnimation(this->darkenAnimation);

	this->tessLevel = std::make_shared<Uniform>("tessLevel", Uniform::INT, 1);
	this->addUniform(this->tessLevel);
	this->orbitAnimation = std::make_shared<OrbitAnimation>();

	this->noiseConfig = std::make_shared<SSBO>("noiseConfig");


	this->clampUnder = std::make_shared<Uniform>("clampUnder", Uniform::FLOAT, 0.0f);
	this->addUniform(this->clampUnder);

}

void Planet::generatePlanet() {
	generatorProgram->bind();

	mesh->bind();
	for (auto &uniform: uniforms) {
		generatorProgram->setUniform(uniform);
	}
	useDefaultUniforms();

	startSizeQuery();

	generatorProgram->setSSBO(this->noiseConfig);
	//Prepare for transform feedback rendering - disable rasterization and begin the transform feedback
	//glEnable(GL_RASTERIZER_DISCARD);
	glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, generatorProgram->getFeedbackBufferId(), 0, 32'000'000);
	glBeginTransformFeedback(GL_TRIANGLES);

	//Normal mesh drawing
	auto drawMode = GL_TRIANGLES;
	if (generatorProgram->hasTesslation()) {
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		drawMode = GL_PATCHES;
	}
	glDrawElements(drawMode, (int) mesh->size(), GL_UNSIGNED_INT, nullptr);

	//End transform feedback and re-enable rasterization
	glEndTransformFeedback();
	//glDisable(GL_RASTERIZER_DISCARD);



	// Get query result
	prims = getSizeQueryResult();
	glFlush();

	glBindVertexArray(0);

}

void Planet::drawPlanet(glm::mat4 mat) {
	program->bind();

	for (auto &uniform: uniforms) {
		program->setUniform(uniform);
	}
	program->setUniform(mouseOverUniform);


	auto texCount = program->getTextureCount();
	for (auto [i, texture]: enumerate(textures)) {
		program->setTexture(texture, texCount + i);
	}
	updateAnimations();

	this->currentDrawMatrix = mat * this->modelMatrix;

	this->modelMatrixUniform->setValue(currentDrawMatrix);
	this->transposeInverseModelUniform->setValue(glm::transpose(glm::inverse(currentDrawMatrix)));
	useDefaultUniforms();
	glBindVertexArray(generatorProgram->getFeedbackVaoId());
	glBindBuffer(GL_ARRAY_BUFFER, generatorProgram->getFeedbackBufferId());
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	//glDrawTransformFeedback(GL_TRIANGLES, generatorProgram->getTransformFeedbackId());
	glDrawArrays(GL_TRIANGLES, 0, prims * 3);

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


void Planet::draw(glm::mat4 parentModel) {

	if (this->parent) {
		auto position = this->parent->getPosition();
		parentModel = glm::translate(glm::mat4(1.0f), position);
	}
	auto level = calculateLod(parentModel);

	if (tessLevel->getValue<int>() != level) {
		tessLevel->setValue(level);
		shouldGenerate = true;
	}

	if (shouldGenerate || generatorProgram->shadersUpdated()) {
		generatePlanet();
		shouldGenerate = false;
	} else {
		drawPlanet(parentModel);
	}
}


std::vector<std::shared_ptr<Galax::Renderer::IProgram>> Planet::getPrograms() const {
	return {program, generatorProgram};
}


void Planet::setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program) {
	generatorProgram = program;
}

void Planet::startSizeQuery() {
	if (sizeQuery == 0) {
		glGenQueries(1, &sizeQuery);
	}
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, sizeQuery);
}

int Planet::getSizeQueryResult() {
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	GLuint64 result = 0;
	glGetQueryObjectui64v(sizeQuery, GL_QUERY_RESULT, &result);
	return result;

}

void Planet::setScale(const glm::vec3 &scale) {
	RenderNode::setScale(scale);
	auto radius = scale.x;
	auto collider = std::make_shared<Physics::SphereCollider>(radius);
	Physics::RigidBody::clearColliders();
	Physics::RigidBody::addBodyCollider(collider);
}

void Planet::setIsMouseOver(bool isMouseOver) {
	PhysicalNode::setIsMouseOver(isMouseOver);

	if (previousMouseOver == isMouseOver) {
		return;
	}
	previousMouseOver = isMouseOver;

	if (isMouseOver) {
		darkenAnimation->stop();
		lightenAnimation->start();
	} else {
		lightenAnimation->stop();
		darkenAnimation->start();
	}

}


Planet *Planet::withSeed(float seed) {
	this->seedUniform = std::make_shared<Uniform>("inputSeed", Uniform::FLOAT, seed);
	addUniform(this->seedUniform);
	return this;
}

Planet *Planet::withRotation(float rotation) {
	this->rotationAnimation = std::make_shared<Animation>();
	rotationAnimation->setLength(rotation);
	rotationAnimation->setRepeat(Animation::Repeat::LOOP);
	rotationAnimation->setEase(Animation::Ease::LINEAR);
	rotationAnimation->setTarget(IAnimation::Target::ROTATION);
	rotationAnimation->addKeyFrame(0, glm::vec3(0, 0, 0));
	rotationAnimation->addKeyFrame(rotation, glm::vec3(0, 360, 0));
	addAnimation(rotationAnimation);
	return this;
}

Planet *Planet::withRadius(float radius) {
	setScale(glm::vec3(radius));
	return this;
}

Planet *Planet::withOrbit(glm::vec3 orbit, std::shared_ptr<IUniform> orbitCenter) {
	this->orbitAnimation->setPlanetRadius(this->scaleUniform);
	this->orbitAnimation->setSunPosition(orbitCenter);
	this->orbitAnimation->setInitialPosition(orbit);
	this->addAnimation(this->orbitAnimation);

	return this;

}

std::string Planet::getTypeName(){
	switch(type){
		case Type::TEMPERATE:
			return "Temperate";
		case Type::MOON:
			return "Moon";
		case Type::ROCKY:
			return "Rocky";

		case Type::HOT:
			return "Hot";
		case Type::OCEAN:
			return "Ocean";
	}
	return "";
}

Planet *Planet::withAtmosphere(std::shared_ptr<Atmosphere> atmosphere) {
	if (atmosphere) {
		this->addChild(atmosphere);
		this->atmosphere = atmosphere;
	}
	return this;
}

Planet *Planet::withCamera(std::shared_ptr<SpaceCamera> camera) {
	if (camera) this->setCamera(camera);
	return this;
}

Planet *Planet::configure() {
	return this;
}

Planet *Planet::withPosition(glm::vec3 position) {
	setPosition(position);
	return this;
}

int Planet::calculateLod(glm::mat4 parentModel) {


	// Calculate distance from camera
	auto cameraPos = this->camera->getPosition();
	//Grab position from parent model
	auto planetPos = glm::vec3(parentModel * this->modelMatrix * glm::vec4(0, 0, 0, 1));
	auto distance = glm::distance(cameraPos, planetPos);

	auto logMin = glm::log(minTessDistance);
	auto logMax = glm::log(maxTessDistance);
	auto logDistance = glm::clamp(glm::log(distance), logMin, logMax);
	auto logScale = (logDistance - logMin) / (logMax - logMin);
	auto level = glm::mix(maxTess, minTess, logScale);
	//Clamp level to power of 2
	auto iLevel = (int) glm::pow(2.0f, glm::round(glm::log2(level)));

	// Slight optimization for moons. When tess is at max, the difference is small and the stutter is noticeable.
	// Noticeable on a GTX 1080
	return iLevel;
	return type == Type::MOON ? iLevel / 2 : iLevel;
}

float Planet::getSeed() {
	return this->seedUniform->getValue<float>();
}

void Planet::setSeed(float seed) {
	this->seedUniform->setValue(seed);
	this->shouldGenerate = true;
}

float Planet::getSize() {
	return this->scale.x;
}

void Planet::setSize(float size) {
	setScale(glm::vec3(size));
}

float Planet::getSpeed() {
	return orbitAnimation->getSpeed();
}

void Planet::setSpeed(float speed) {
	orbitAnimation->setSpeed(speed);
}

Planet *Planet::withMoon(std::shared_ptr<Planet> moon) {
	moons.push_back(moon);
	children.push_back(moon);
	return this;
}

void Planet::addNoiseLevel(float roughness, float strength) {
	noiseConfig->addData(std::pair<float, float>(roughness, strength));
}

void Planet::setClampUnder(float value) {
	this->clampUnder->setValue(value);
}




