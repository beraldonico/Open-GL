/*
 * Triangle.h
 *
 *  Created on: 26/09/2017
 *      Author: gustavo
 */

#pragma once

#include "SceneNode.h"

class Triangle : public SceneNode {
public:
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 n, glm::mat4x4 model, Material material);
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 na, glm::vec3 nb, glm::vec3 nc, glm::mat4x4 model, Material material);
	virtual ~Triangle();

	virtual float hit(Ray ray, float min, float max, glm::vec3 &vertex, glm::vec3 &normal, Material &material);
	virtual bool hit(Ray ray, float min, float max);

	void setMaterial(const Material& material);

private:

	float hitTriangle(Ray ray, float min, float max, glm::vec3 &abg);

	glm::vec3 _a, _b, _c;
	glm::vec3 _na, _nb, _nc;
	Material _material;
};

