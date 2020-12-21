/*
 * Light.cpp
 *
 *  Created on: 05/10/2017
 *      Author: gustavo
 */

#include "Light.h"

Light::Light(glm::vec3 position, glm::vec4 color) {
	setPosition(position);
	setColor(color);
	setAttenuation(0.0,0.0,1.0);
}

Light::~Light() {
	// TODO Auto-generated destructor stub
}

glm::vec4 Light::getColor() const {
	return _color;
}

void Light::setColor(glm::vec4 color) {
	_color = color;
}

glm::vec3 Light::getPosition() const {
	return _position;
}

void Light::setPosition(glm::vec3 position) {
	_position = position;
}

void Light::setAttenuation(float constant, float linear, float quadratic) {
	_constAttenuation = constant;
	_linAttenuation = linear;
	_quadAttenuation = quadratic;
}

void Light::getAttenuation(float &constant, float &linear, float &quadratic) {
	constant = _constAttenuation;
	linear = _linAttenuation;
	quadratic = _quadAttenuation;
}
