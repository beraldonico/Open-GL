/*
 * Ray.h
 *
 *  Created on: 25/09/2017
 *      Author: gustavo
 */

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Ray {
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	virtual ~Ray();

	void setRay(glm::vec3 origin, glm::vec3 direction);
	Ray getRay(glm::mat4x4 model);

	glm::vec3 getOrigin();
	glm::vec3 getDirection();

	Ray reflect(glm::vec3 vertex, glm::vec3 normal);
	bool refract(Ray &ray, glm::vec3 vertex, glm::vec3 normal, float ni, float nt);
	float fresnel(glm::vec3 normal, float ni, float nt);

private:
	glm::vec3 _origin;
	glm::vec3 _direction;
};

