/*
 * Sphere.cpp
 *
 *  Created on: 25/09/2017
 *      Author: gustavo
 */

#include "Sphere.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Sphere::Sphere(glm::vec3 center, float radius, glm::mat4x4 model, Material material)
: SceneNode(model),
  _center(center),
  _radius(radius),
  _material(material) {

}


Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

float Sphere::hit(Ray ray, float min, float max, glm::vec3 &vertex, glm::vec3 &normal, Material &material) {

	float ret = hitSphere(ray, min, max);

	if (ret < FLT_MAX) {
		glm::mat4x4 model = getCurrModelMatrix();
		vertex = ray.getOrigin() + ret * ray.getDirection();
		glm::vec4 center(_center,1.0f);
		center = model*center;
		center = center/center.w;
		normal = glm::normalize(vertex-glm::vec3(center));
		material = _material;
	}
	return ret;

}


bool Sphere::hit(Ray ray, float min, float max) {
	if (hitSphere(ray, min, max) < 1.0f) return true;
	return false;
}

float Sphere::hitSphere(Ray ray, float min, float max) {
	glm::mat4x4 model = getCurrModelMatrix();

	Ray tRay = ray.getRay(model);
	glm::vec3 e = tRay.getOrigin();
	glm::vec3 d = tRay.getDirection();

	float k = glm::dot(d,(e-_center));
	float l = glm::dot(e-_center,e-_center);
	float A = glm::dot(d,d);
	float Delta = k * k - A * (l - _radius * _radius);

	float ret = FLT_MAX;
	if (Delta == 0) {
		float t = dot(-d,e-_center)/A;
		if (t >= min && t <= max) ret = t;
	} else if (Delta > 0){
		float sqrtDelta = std::sqrt(Delta);
		float t = (dot(-d,e-_center) - sqrtDelta)/A;
		if (t >= min && t <= max)
			ret = t;
		t = (dot(-d,e-_center) + sqrtDelta)/A;
		if (t < ret && t >= min && t <= max)
			ret = t;
	}

	return ret;
}

void Sphere::setMaterial(const Material& material) {
	_material = material;
}
