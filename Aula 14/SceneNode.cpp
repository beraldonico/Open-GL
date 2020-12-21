/*
 * SceneNode.cpp
 *
 *  Created on: 28/09/2017
 *      Author: gustavo
 */

#include "SceneNode.h"

#include <iostream>

std::vector<glm::mat4x4> SceneNode::_modelStack;

SceneNode::SceneNode()
: _model(glm::mat4x4(1.0)) {

}

SceneNode::SceneNode(glm::mat4x4 model)
: _model(model) {
	// TODO Auto-generated constructor stub

}

SceneNode::~SceneNode() {
	for (auto &c: _child) {
		delete c;
	}
}

float SceneNode::hit(Ray ray, float min, float max, glm::vec3 &vertex, glm::vec3 &normal, Material &material) {
	float t = FLT_MAX;
	for (auto &c: _child) {

		_modelStack.push_back(getCurrModelMatrix());

		glm::vec3 v, n;
		Material m;
		float tmp = c->hit(ray, min, max, v, n, m);
		if (tmp < t) {
			t = tmp;
			vertex = v;
			normal = n;
			material = m;
		}

		_modelStack.pop_back();
	}

	return t;
}

bool SceneNode::hit(Ray ray, float min, float max) {
	float t = FLT_MAX;
	for (auto &c: _child) {

		_modelStack.push_back(getCurrModelMatrix());

		bool b = c->hit(ray, min, max);

		_modelStack.pop_back();

		if (b) return true;
	}

	return false;
}

void SceneNode::addChild(SceneNode *sceneNode) {
	_child.push_back(sceneNode);
}

void SceneNode::setModelMatrix(glm::mat4x4 model) {
	_model = model;
}

glm::mat4x4 SceneNode::getCurrModelMatrix() {
	if (_modelStack.empty()) return _model;
	else return _modelStack.back() * _model;
}

const glm::mat4x4& SceneNode::getModelMatrix() const {
	return _model;
}
