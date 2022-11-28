//
// Created by tomas on 16.11.22.
//

#include <glbinding/gl/gl.h>

#include <orbital/Sun.h>
using namespace Galax::Orbital;
using namespace gl;

Sun::Sun(const std::string &name) : Node(name) {
	auto mesh = std::make_shared<Mesh>("Sun Quad (" + this->name + ")");
	mesh->addVertex({-1.0f, -1.0f, 0.0f});
	mesh->addVertex({1.0f, -1.0f, 0.0f});
	mesh->addVertex({-1.0f, 1.0f, 0.0f});
	mesh->addVertex({1.0f, 1.0f, 0.0f});

	mesh->addIndex(1);
	mesh->addIndex(0);
	mesh->addIndex(2);
	mesh->addIndex(1);
	mesh->addIndex(2);
	mesh->addIndex(3);

	this->setMesh(mesh);

}


void Sun::draw(glm::mat4 parentModel) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program->setUniform(scaleUniform);

	Node::draw(parentModel);
	glDisable(GL_BLEND);
}
