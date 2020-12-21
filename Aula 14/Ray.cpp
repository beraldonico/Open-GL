/*
 * Ray.cpp
 *
 *  Created on: 25/09/2017
 *      Author: gustavo
 */

#include "Ray.h"

#include <glm/glm.hpp>

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {

	setRay(origin, direction);

}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

void Ray::setRay(glm::vec3 origin, glm::vec3 direction) {
	_origin = origin;
	_direction = direction;
}

Ray Ray::getRay(glm::mat4x4 model) {
	glm::mat4x4 invModel = glm::inverse(model);
	glm::vec4 tmp = invModel * glm::vec4(getOrigin(),1.0f);
	glm::vec3 e = glm::vec3(tmp/tmp.w);
	tmp = glm::inverse(glm::transpose(invModel)) * glm::vec4(getDirection(),0.0f);
	glm::vec3 d = glm::vec3(tmp);
	return Ray(e,d);
}

glm::vec3 Ray::getOrigin() {
	return _origin;
}

glm::vec3 Ray::getDirection() {
	return _direction;
}

Ray Ray::reflect(glm::vec3 vertex, glm::vec3 normal) {
	glm::vec3 incident = normalize(_direction);
	return Ray(vertex,incident-2.0f*glm::dot(incident,normal)*normal);
}

bool Ray::refract(Ray &ray, glm::vec3 vertex, glm::vec3 normal, float ni, float nt) {
	float n = ni/nt;
	glm::vec3 d = normalize(_direction);
	float c1 = glm::dot(d,normal);

	if (c1 < 0) c1 = -c1;
	else {
		normal = -normal;
		n = nt/ni;
	}

	float s = 1 - (n*n * (1 - c1*c1));
	if (s < 0) return false; // total internal reflection

	float c2 = std::sqrt(s);
	glm::vec3 t = n*d + (n*c1-c2)*normal;
	ray = Ray(vertex, t);

	return true;
}

float Ray::fresnel(glm::vec3 normal, float ni, float nt) {
	glm::vec3 d = normalize(_direction);
	float ci = glm::dot(d,normal);

	if (ci > 0) {
		float tmp = ni;
		ni = nt;
		nt = tmp;
	}

	// Snell's law
	float si = ni/nt * std::sqrt(1 - ci*ci);

	if (si >= 1.0f) // total internal reflection
		return 1.0f;

	float ct = std::sqrt(1-si*si);
	if (ci < 0) ci = -ci;

	float p = (nt * ci - ni * ct) / (nt * ci + ni * ct);
	float q = (ni * ct - nt * ci) / (ni * ct + nt * ci);

	return 0.5f * (p * p + q * q);
}
