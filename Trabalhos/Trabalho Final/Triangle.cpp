/*
 * Triangle.cpp
 *
 *  Created on: 26/09/2017
 *      Author: gustavo
 */

#include "Triangle.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 na, glm::vec3 nb, glm::vec3 nc, glm::mat4x4 model, Material material)
: SceneNode(model),
  _a(a), _b(b), _c(c),
  _na(na), _nb(nb), _nc(nc),
  _material(material) {
	// TODO Auto-generated constructor stub

}

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 n, glm::mat4x4 model, Material material)
: SceneNode(model),
  _a(a), _b(b), _c(c),
  _na(n), _nb(n), _nc(n),
  _material(material) {
	// TODO Auto-generated constructor stub

}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}

float Triangle::hit(Ray ray, float min, float max, glm::vec3 &vertex, glm::vec3 &normal, Material &material) {

	glm::vec3 abg;
	float t = hitTriangle(ray, min, max, abg);

	vertex = ray.getOrigin() + t * ray.getDirection();
	glm::mat4x4 model = getCurrModelMatrix();
	normal = glm::vec3(glm::inverseTranspose(model)*glm::vec4(abg.x * _na + abg.y * _nb + abg.z * _nc,0.0f));
	//std::cout << "normal: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
	material = _material;

	return t;
}

bool Triangle::hit(Ray ray, float min, float max) {
	glm::vec3 abg;
	if (hitTriangle(ray, min, max, abg) < 1.0) return true;
	return false;
}

float Triangle::hitTriangle(Ray ray, float min, float max, glm::vec3 &abg) {
	glm::mat4x4 model = getCurrModelMatrix();

	Ray tRay = ray.getRay(model);

	float a = _a.x - _b.x;
	float b = _a.y - _b.y;
	float c = _a.z - _b.z;
	float d = _a.x - _c.x;
	float e = _a.y - _c.y;
	float f = _a.z - _c.z;
	float g = tRay.getDirection().x;
	float h = tRay.getDirection().y;
	float i = tRay.getDirection().z;
	float j = _a.x - tRay.getOrigin().x;
	float k = _a.y - tRay.getOrigin().y;
	float l = _a.z - tRay.getOrigin().z;

	float A = e*i-h*f;
	float B = g*f-d*i;
	float C = d*h-e*g;

	float detA	= a*A + b*B + c*C;
	float beta	= (j*A + k*B + l*C)/detA;

	A = a*k-j*b;
	B = j*c-a*l;
	C = b*l-k*c;

	float gamma	= (i*A + h*B + g*C)/detA;
	float t		= -(f*A + e*B + d*C)/detA;

	if (t < min || t > max) {
		return FLT_MAX;
	}
	if (gamma < 0 || gamma > 1) return FLT_MAX;
	if (beta < 0 || beta > 1-gamma) return FLT_MAX;
	float alpha = 1.0f - (beta + gamma);

	abg.x = alpha;
	abg.y = beta;
	abg.z = gamma;

	return t;
}

void Triangle::setMaterial(const Material& material) {
	_material = material;
}
