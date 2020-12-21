/*
 * Trackball.cpp
 *
 *  Created on: 04/10/2017
 *      Author: gustavo
 */

#include "Trackball.h"

#include "Trackball_DONOTUSE.h"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

Trackball::Trackball()
: _radius(0.8f) {
	reset();
}

Trackball::~Trackball() {
	// TODO Auto-generated destructor stub
}

void Trackball::reset() {
	_stableQuat = glm::angleAxis(0.0f, glm::vec3(0.0f,0.0f,1.0f));
	_downPos = glm::vec2(0.0f,0.0f);
	_isDown = false;

#ifndef USE_MY_TRACKBALL
	float axis[] = {0.0f,0.0f,1.0f};
	axis_to_quat(axis, 0.0f, g_quaternion);
#endif
}

void Trackball::mouseDown(glm::vec2 pos) {
	_downPos = pos;
	_isDown = true;
	_motionQuat = glm::angleAxis(0.0f, glm::vec3(0.0f,0.0f,1.0f));

#ifndef USE_MY_TRACKBALL
	float axis[] = {0.0f,0.0f,1.0f};
	axis_to_quat(axis, 0.0f, g_quaternionTemp);
#endif
}

void Trackball::mouseUp(glm::vec2 pos) {
	_isDown = false;
	_stableQuat = glm::normalize(_motionQuat * _stableQuat);

#ifndef USE_MY_TRACKBALL
	add_quats(g_quaternionTemp, g_quaternion, g_quaternion);
#endif
}

void Trackball::mouseMotion(glm::vec2 pos) {
	_motionQuat = trackball(_downPos,pos);

#ifndef USE_MY_TRACKBALL
	tball(g_quaternionTemp, _downPos.x, _downPos.y, pos.x, pos.y);
#endif
}


float Trackball::projectToSphereOrHyberbola(glm::vec2 p) {
    float z = 0.0f;

    float d = glm::length(p);
    if (d < _radius * 0.70710678118654752440) {
    	/* project to sphere */
        z = std::sqrt(_radius*_radius - d*d);
    } else {
    	/* project to hyberbola */
        float t = _radius / 1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}

glm::quat Trackball::trackball(glm::vec2 p1, glm::vec2 p2) {

	if (p1.x == p2.x && p1.y == p2.y) {
		return glm::angleAxis(0.0f,glm::vec3(0.0f,0.0f,1.0f));
	}

	glm::vec3 point1 = glm::vec3(p1.x, p1.y, projectToSphereOrHyberbola(p1));
	glm::vec3 point2 = glm::vec3(p2.x, p2.y, projectToSphereOrHyberbola(p2));

	glm::vec3 axis = glm::cross(point2, point1);
	float t = glm::length(point1-point2) / (2.0*_radius);

	t = std::max(std::min(t,1.0f),-1.0f);
	float phi = 2.0 * std::asin(t);

	return glm::angleAxis(phi, glm::normalize(axis));
}


glm::mat4x4 Trackball::getMatrix() {

	glm::quat rot;
	if (_isDown) rot = glm::normalize(_motionQuat * _stableQuat);
	else rot = _stableQuat;

#ifdef USE_MY_TRACKBALL
	return glm::toMat4(rot);

	/*
	std::cout << rotMat[0].x << " " << rotMat[1].x << " " << rotMat[2].x << " " << rotMat[3].x << std::endl;
	std::cout << rotMat[0].y << " " << rotMat[1].y << " " << rotMat[2].y << " " << rotMat[3].y << std::endl;
	std::cout << rotMat[0].z << " " << rotMat[1].z << " " << rotMat[2].z << " " << rotMat[3].z << std::endl;
	std::cout << rotMat[0].w << " " << rotMat[1].w << " " << rotMat[2].w << " " << rotMat[3].w << std::endl;
	*/
#else
	float m[4][4];
	float quaternion[4];
	if (_isDown) {
		add_quats(g_quaternionTemp, g_quaternion, quaternion);
		build_rotmatrix(m, quaternion);
	} else {
		build_rotmatrix(m, g_quaternion);
	}

	glm::mat4x4 rotMat;
	rotMat[0].x = m[0][0];rotMat[1].x = m[0][1];rotMat[2].x = m[0][2];rotMat[3].x = m[0][3];
	rotMat[0].y = m[1][0];rotMat[1].y = m[1][1];rotMat[2].y = m[1][2];rotMat[3].y = m[1][3];
	rotMat[0].z = m[2][0];rotMat[1].z = m[2][1];rotMat[2].z = m[2][2];rotMat[3].z = m[2][3];
	rotMat[0].w = m[3][0];rotMat[1].w = m[3][1];rotMat[2].w = m[3][2];rotMat[3].w = m[3][3];

	return rotMat;
#endif
}

glm::vec2 Trackball::getPos(glm::i32vec2 screenPos, glm::i32vec2 screenSize) {
	glm::vec2 pos;
	pos.x = (float)2.0*((screenSize.x*0.5)-screenPos.x)/(float)screenSize.y;
	pos.y = (2.0*(float)screenPos.y/(float)screenSize.y - 1.0);
	return pos;
}
