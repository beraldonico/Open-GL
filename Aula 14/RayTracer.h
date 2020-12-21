/*
 * RayTracer.h
 *
 *  Created on: 27/09/2017
 *      Author: gustavo
 */

#pragma once

#include <list>

#include "SceneNode.h"
#include "Light.h"
#include "Trackball.h"

#include <glm/gtc/type_ptr.hpp>

#define DEFAULT_WINDOW_WIDTH	800
#define DEFAULT_WINDOW_HEIGHT	600

class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();

	static RayTracer* getInstance();

	void init(char *filename);
	void display();
	void reshape(int w, int h);
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void keyboard(unsigned char key, int x, int y);

private:
	static RayTracer *_instance;

	glm::vec4 samplePixel(glm::i32vec2 pixel, glm::i32vec2 sample);
	glm::vec4 shootRay(Ray ray, float min, float max, int recDepth);
	bool shootShadowRay(Ray ray);
	glm::vec4 BlinnPhong(glm::vec3 eye, glm::vec3 vertex, glm::vec3 normal, Material material, Light light);

	static glm::u8vec4 float2ubyte(glm::vec4 a);

	glm::i32vec2 _resolution;
	glm::u8vec4 *_buffer;

	SceneNode _scene;
	std::list<Light*> _light;

	glm::mat4x4 _projection;
	glm::mat4x4 _view;

	Trackball _trackball;

	float _bias;

	int _maxRecursionDepth;

	float _scroolFactor;
	float _cameraAngle;

	bool _superSampleJittered;
	int _sampleResolution;

};

