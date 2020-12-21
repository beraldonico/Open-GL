/*
 * Material.cpp
 *
 *  Created on: 05/10/2017
 *      Author: gustavo
 */

#include "Material.h"

Material::Material()
: _isDielectric(false) {
	setAmbient(glm::vec4(.0f,.0f,.0f,1.0f));
	setDiffuse(glm::vec4(.0f,.0f,.0f,1.0f));
	setSpecular(glm::vec4(.0f,.0f,.0f,1.0f));
	setShininess(1.0f);
}

Material::~Material() {
	// TODO Auto-generated destructor stub
}

glm::vec4 Material::getAmbient() const {
	return _ambient;
}

void Material::setAmbient(glm::vec4 ambient) {
	_ambient = ambient;
}

glm::vec4 Material::getDiffuse() const {
	return _diffuse;
}

void Material::setDiffuse(glm::vec4 diffuse) {
	_diffuse = diffuse;
}

float Material::getShininess() const {
	return _shininess;
}

void Material::setShininess(float shininess) {
	_shininess = shininess;
}

glm::vec4 Material::getSpecular() const {
	return _specular;
}

void Material::setSpecular(glm::vec4 specular) {
	_specular = specular;
}

float Material::getReflectionTerm() const {
	return _reflectionTerm;
}

void Material::setReflectionTerm(float reflectionTerm) {
	_reflectionTerm = reflectionTerm;
}

bool Material::isDielectric() {
	return _isDielectric;
}

void Material::setDielectric(bool value) {
	_isDielectric = value;
}
