/*
 * Material.h
 *
 *  Created on: 05/10/2017
 *      Author: gustavo
 */

#pragma once

#include <glm/vec4.hpp>

class Material {
public:
	Material();
	virtual ~Material();

	glm::vec4 getAmbient() const;
	void setAmbient(glm::vec4 ambient);
	glm::vec4 getDiffuse() const;
	void setDiffuse(glm::vec4 diffuse);
	float getShininess() const;
	void setShininess(float shininess);
	glm::vec4 getSpecular() const;
	void setSpecular(glm::vec4 specular);
	float getReflectionTerm() const;
	void setReflectionTerm(float reflectionTerm);
	bool isDielectric();
	void setDielectric(bool value);

private:
	bool _isDielectric;
	glm::vec4 _ambient;
	glm::vec4 _diffuse;
	glm::vec4 _specular;
	float _shininess;
	float _reflectionTerm;
};

