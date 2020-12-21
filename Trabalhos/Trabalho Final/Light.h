/*
 * Light.h
 *
 *  Created on: 05/10/2017
 *      Author: gustavo
 */

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


class Light {
public:
	Light(glm::vec3 position, glm::vec4 color);
	virtual ~Light();

	glm::vec4 getColor() const;
	void setColor(glm::vec4 color);
	glm::vec3 getPosition() const;
	void setPosition(glm::vec3 position);
	void setAttenuation(float constant, float linear, float quadratic);
	void getAttenuation(float &constant, float &linear, float &quadratic);

private:
	glm::vec3 _position;
	glm::vec4 _color;
	float _constAttenuation;
	float _linAttenuation;
	float _quadAttenuation;
};

