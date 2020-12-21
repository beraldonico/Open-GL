/*
 * SceneNode.h
 *
 *  Created on: 28/09/2017
 *      Author: gustavo
 */

#pragma once

#include <vector>
#include <list>

#include <glm/glm.hpp>

#include "Ray.h"
#include "Material.h"

class SceneNode {
public:
	SceneNode();
	SceneNode(glm::mat4x4 model);
	virtual ~SceneNode();

	virtual float hit(Ray ray, float min, float max, glm::vec3 &vertex, glm::vec3 &normal, Material &material);
	virtual bool hit(Ray ray, float min, float max);


	void addChild(SceneNode *sceneNode);
	void setModelMatrix(glm::mat4x4 model);
	const glm::mat4x4& getModelMatrix() const;

protected:
	glm::mat4x4 _model;
	static std::vector<glm::mat4x4> _modelStack;
	std::list<SceneNode*> _child;

	glm::mat4x4 getCurrModelMatrix();
};

