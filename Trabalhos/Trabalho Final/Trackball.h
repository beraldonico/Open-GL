/*
 * Trackball.h
 *
 *  Created on: 04/10/2017
 *      Author: gustavo
 */

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/quaternion.hpp>

#define USE_MY_TRACKBALL

class Trackball {
public:
	Trackball();
	virtual ~Trackball();

	void reset();
	void mouseDown(glm::vec2 pos);
	void mouseUp(glm::vec2 pos);
	void mouseMotion(glm::vec2 pos);

	glm::mat4x4 getMatrix();

	static glm::vec2 getPos(glm::i32vec2 screenPos, glm::i32vec2 screenSize);

private:

	float projectToSphereOrHyberbola(glm::vec2 p);
	glm::quat trackball(glm::vec2 p1, glm::vec2 p2);

	float _radius;
	glm::quat _stableQuat, _motionQuat;
	glm::vec2 _downPos;
	bool _isDown;

#ifndef USE_MY_TRACKBALL
	float g_quaternion[4];
	float g_quaternionTemp[4];
#endif
};

