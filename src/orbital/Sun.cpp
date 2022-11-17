//
// Created by tomas on 16.11.22.
//

#include <glbinding/gl/gl.h>

#include <orbital/Sun.h>
using namespace Galax::Orbital;
using namespace gl;

Sun::Sun(const std::string &name) : Node(name) {

}


void Sun::draw() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Node::draw();
	glDisable(GL_BLEND);
}
