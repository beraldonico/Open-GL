/*
 * Sphere.h
 *
 *  Created on: 25/09/2017
 *      Author: gustavo
 */

#pragma once

#include "SceneNode.h"

class Sphere : public SceneNode {
public:
	Sphere(glm::vec3 center, float radius);
	Sphere(glm::vec3 center, float radius, glm::mat4x4 model);
	Sphere(glm::vec3 center, float radius, glm::mat4x4 model, Material material);
	virtual ~Sphere();

	virtual float hit(Ray ray, float min, float max, glm::vec3 &vertex, glm::vec3 &normal, Material &material);
	virtual bool hit(Ray ray, float min, float max);

	void setMaterial(const Material& material);

private:

	float hitSphere(Ray ray, float min, float max);

	glm::vec3 _center;
	float _radius;
	Material _material;
};

