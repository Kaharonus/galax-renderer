#include <iostream>
#include "InputHandler.h"
#include <tuple>

using namespace Galax::Renderer;
using namespace std::chrono;

InputHandler::InputHandler() {

	for (bool &key: keys) {
		key = false;
	}

	for (bool &mouseButton: mouseButtons) {
		mouseButton = false;
	}
}

InputHandler::~InputHandler() {
}


void InputHandler::mouseMove(float x, float y) {
	if (absoluteX == 0 && absoluteY == 0) {
		absoluteX = x;
		absoluteY = y;
		return;
	}
	deltaX = x - absoluteX;
	deltaY = y - absoluteY;
	absoluteX = x;
	absoluteY = y;

}

void InputHandler::mousePress(MouseButton button, float x, float y) {
	mouseButtons[button] = true;
	mousePressTime[button] = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	this->absoluteX = x;
	this->absoluteY = y;
}

void InputHandler::mouseRelease(MouseButton button, float x, float y) {
	mouseButtons[button] = false;
	this->absoluteX = x;
	this->absoluteY = y;
	auto time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	auto timeDiff = time - mousePressTime[button];
	// 175 ms was chosen as a reasonable time for a single click
	// It might seem like a lot, but I tried clicking the same way my grandma does, and it was about 167 ms
	// Tried 200, but sometimes it was too slow
	if (timeDiff < milliseconds(175)) {
		std::cout << timeDiff.count() << std::endl;
		for (auto &callback: mouseClickCallbacks) {
			callback(button, x, y);
		}
	}

}

void InputHandler::mouseWheel(float x, float y) {
	scrollDelta = {x, y};
	scrollDeltaSet = true;
}

void InputHandler::keyPress(Key key) {
	keys[key] = true;
}

void InputHandler::keyRelease(Key key) {
	keys[key] = false;
}

bool InputHandler::isKeyPressed(Key key) {
	return keys[key];
}

bool InputHandler::isKeyUp(Key key) {
	return !keys[key];
}

bool InputHandler::isMouseButtonPressed(MouseButton button) {
	return mouseButtons[button];
}

bool InputHandler::isMouseButtonUp(MouseButton button) {
	return !mouseButtons[button];
}

glm::vec2 InputHandler::getPositionDelta() {
	return glm::vec2(deltaX, deltaY);
}

glm::vec2 InputHandler::getAbsolutePosition() {
	return {absoluteX, absoluteY};
}

glm::vec2 InputHandler::getScrollDelta() {
	if(scrollDeltaSet){
		scrollDeltaSet = false;
		return scrollDelta;
	}
	return {0, 0};
}

InputHandler::Key InputHandler::fromQtKey(int key) {
	if (key >= KEY_SPACE && key <= KEY_RIGHT_BRACKET) {
		return static_cast<Key>(key);
	}
	switch (key) {
		case 0x01000000:
			return KEY_ESCAPE;
		case 0x01000005:
			return KEY_ENTER;
		case 0x01000006:
			return KEY_INSERT;
		case 0x01000020:
			return KEY_SHIFT;
		case 0x01000021:
			return KEY_CONTROL;

	}

	return KEY_UNKNOWN;
}

glm::vec2 InputHandler::getRendererPosition() {
	return {absoluteX / rendererWidth, absoluteY / rendererHeight};
}

void InputHandler::setRendererSize(int width, int height) {
	rendererWidth = width;
	rendererHeight = height;
}

void InputHandler::registerMouseClickCallback(std::function<void(MouseButton, float, float)> callback) {
	mouseClickCallbacks.push_back(callback);
}
