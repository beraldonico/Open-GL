/*
 * RayTracer.cpp
 *
 *  Created on: 27/09/2017
 *      Author: gustavo
 */

#include "RayTracer.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <CImg.h>

#include "Sphere.h"
#include "Triangle.h"
#include "Trackball.h"
#include "plylib.h"

#define XMIN -0.1f
#define XMAX 0.1f
#define YMIN 0.0f
#define YMAX 0.2f
#define ZMIN -1.0f
#define ZMAX 1.0f

glm::vec3 tmin;
glm::vec3 tmax;
glm::vec3 dist;
int contadorpcc;

RayTracer* RayTracer::_instance = nullptr;

RayTracer::RayTracer()
    : _resolution(glm::i32vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)),
      _buffer(nullptr),
      _bias(0.001f),
      _maxRecursionDepth(0),
      _scroolFactor(0.0002f),
      _cameraAngle(45.0f),
      _superSampleJittered(false),
      _sampleResolution(1)
{

    _view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));
    _projection = glm::perspective(_cameraAngle, (float)_resolution.x/(float)_resolution.y, 0.1f, 100000.0f);//FLT_MAX))
}

RayTracer::~RayTracer()
{
    delete [] _buffer;

    for (auto l: _light)
    {
        delete l;
    }
}

RayTracer* RayTracer::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new RayTracer();
    }
    return _instance;
}

void RayTracer::init(char *filename)
{

    _buffer = new glm::u8vec4[_resolution.x*_resolution.y];

    glm::mat4x4 model(1.0);
    Material material;
    material.setAmbient(glm::vec4(0.0f,0.0f,0.0f,1.0f));
    material.setDiffuse(glm::vec4(1.0f,0.0f,0.0f,1.0f));
    material.setSpecular(glm::vec4(0.2f,0.2f,0.2f,1.0f));
    material.setShininess(1.0f);
    material.setReflectionTerm(0.0f);

    ply_model *mdl = NULL;
    if (filename) mdl = load_ply_model(filename);
    if (mdl)
    {
        model = glm::mat4x4(1.0f);
        model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
        for (int i=0; i<mdl->nTriangles; i++)
        {
            vec3f A = mdl->vertices[mdl->triangles[i].a];
            vec3f B = mdl->vertices[mdl->triangles[i].b];
            vec3f C = mdl->vertices[mdl->triangles[i].c];
            vec3f N = mdl->triangles[i].normal;
            _scene.addChild(new Triangle(glm::vec3(A.x,A.y,A.z),glm::vec3(B.x,B.y,B.z),glm::vec3(C.x,C.y,C.z),glm::vec3(N.x,N.y,N.z), model, material));
        }
        delete_ply_model(mdl);
    }
    else
    {
        model = glm::mat4x4(1.0f);
        model = glm::translate(model, glm::vec3(-0.5,-0.25,0.0));
        Material material;
        material.setAmbient(glm::vec4(0.0f,0.0f,0.0f,1.0f));
        material.setDiffuse(glm::vec4(1.0f,0.0f,0.0f,1.0f));
        material.setSpecular(glm::vec4(0.2f,0.2f,0.2f,1.0f));
        material.setShininess(1.0f);
        material.setReflectionTerm(0.0f);
        _scene.addChild(new Sphere(glm::vec3(0.0f,0.0f,0.0f), 0.2f, model, material));

        model = glm::mat4x4(1.0f);
        model = glm::translate(model, glm::vec3(0.5,-0.25,0.0));
        material.setDiffuse(glm::vec4(1.0f,1.0f,1.0f,1.0f));
        material.setReflectionTerm(0.3f);
        _scene.addChild(new Sphere(glm::vec3(0.0f,0.0f,0.0f), 0.2f, model, material));

        material.setDielectric(true);

        model = glm::mat4x4(1.0f);
        model = glm::translate(model, glm::vec3(0.0,-0.25,0.5));
        material.setDiffuse(glm::vec4(0.0f,0.0f,1.0f,1.0f));
        material.setReflectionTerm(0.7f);
        _scene.addChild(new Sphere(glm::vec3(0.0f,0.0f,0.0f), 0.2f, model, material));
        material.setDielectric(false);
    }

    model = glm::mat4x4(1.0f);
    model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
    material.setDiffuse(glm::vec4(1.0f,1.0f,1.0f,1.0f));

    model = glm::mat4x4(1.0);
    model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
    material.setDiffuse(glm::vec4(0.8,0.8,0.8,1.0));
    material.setReflectionTerm(0.0f);

#if 1
    // bottom
    _scene.addChild(new Triangle(glm::vec3(-1.0,-1.0,1.0),glm::vec3(1.0,-1.0,1.0),glm::vec3(1.0,-1.0,-1.0),glm::vec3(0.0,1.0,0.0), model, material));
    _scene.addChild(new Triangle(glm::vec3(1.0,-1.0,-1.0),glm::vec3(-1.0,-1.0,-1.0),glm::vec3(-1.0,-1.0,1.0),glm::vec3(0.0,1.0,0.0), model, material));

    // back
    _scene.addChild(new Triangle(glm::vec3(-1.0,-1.0,-1.0),glm::vec3(1.0,-1.0,-1.0),glm::vec3(1.0,1.0,-1.0),glm::vec3(0.0,0.0,1.0), model, material));
    _scene.addChild(new Triangle(glm::vec3(1.0,1.0,-1.0),glm::vec3(-1.0,1.0,-1.0),glm::vec3(-1.0,-1.0,-1.0),glm::vec3(0.0,0.0,1.0), model, material));

    // top
    _scene.addChild(new Triangle(glm::vec3(-1.0,1.0,1.0),glm::vec3(-1.0,1.0,-1.0),glm::vec3(1.0,1.0,-1.0),glm::vec3(0.0,-1.0,0.0), model, material));
    _scene.addChild(new Triangle(glm::vec3(1.0,1.0,-1.0),glm::vec3(1.0,1.0,1.0),glm::vec3(-1.0,1.0,1.0),glm::vec3(0.0,-1.0,0.0), model, material));

    // left
    material.setDiffuse(glm::vec4(0.0,0.8,0.0,1.0));
    _scene.addChild(new Triangle(glm::vec3(-1.0,-1.0,1.0),glm::vec3(-1.0,-1.0,-1.0),glm::vec3(-1.0,1.0,-1.0),glm::vec3(1.0,0.0,0.0), model, material));
    _scene.addChild(new Triangle(glm::vec3(-1.0,1.0,-1.0),glm::vec3(-1.0,1.0,1.0),glm::vec3(-1.0,-1.0,1.0),glm::vec3(1.0,0.0,0.0), model, material));

    // right
    material.setDiffuse(glm::vec4(0.8,0.0,0.0,1.0));
    _scene.addChild(new Triangle(glm::vec3(1.0,-1.0,-1.0),glm::vec3(1.0,-1.0,1.0),glm::vec3(1.0,1.0,1.0),glm::vec3(-1.0,0.0,0.0), model, material));
    _scene.addChild(new Triangle(glm::vec3(1.0,1.0,1.0),glm::vec3(1.0,1.0,-1.0),glm::vec3(1.0,-1.0,-1.0),glm::vec3(-1.0,0.0,0.0), model, material));
#endif
    _light.push_back(new Light(glm::vec3(0.0f,0.8f,0.8f), glm::vec4(1.0f,1.0f,1.0f,1.0f)));
    //_light.push_back(new Light(glm::vec3(-1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,0.0f,1.0f)));
    //_light.push_back(new Light(glm::vec3(-1.0f,1.0f,-1.0f), glm::vec4(0.0f,1.0f,0.0f,1.0f)));
    //_light.push_back(new Light(glm::vec3(1.0f,1.0f,-1.0f), glm::vec4(0.0f,0.0f,1.0f,1.0f)));

}

void RayTracer::display()
{
    _scene.setModelMatrix(_trackball.getMatrix());

    glm::mat4x4 invView = glm::inverse(_view);
    glm::vec4 tmp = invView * glm::vec4(0.0,0.0,0.0,1.0);
    glm::vec3 eye = glm::vec3(tmp/tmp.w);

    float maxPixels = _resolution.x * _resolution.y;
    float currPixel = 0;

    glm::i32vec2 pixel(0,0);
    float refP = 0.0;
    for (pixel.x=0; pixel.x<_resolution.x; pixel.x++)
    {
        for (pixel.y=0; pixel.y<_resolution.y; pixel.y++)
        {

            glm::vec4 color(0.0f);

            glm::i32vec2 sample(0,0);
            for (sample.x=0; sample.x<_sampleResolution; sample.x++)
            {
                for (sample.y=0; sample.y<_sampleResolution; sample.y++)
                {
                    tmp = invView * samplePixel(pixel, sample);
                    glm::vec3 dir = glm::vec3(tmp/tmp.w) - eye;
                    Ray ray(eye, dir);
                    
                    // nosso codigo
                    //std::cout <<"dir = x: " << dir.x << "\ty: "<< dir.y << "\tz: " << dir.z << std::endl;

                    dist.x = 1 / dir.x;
                    dist.y = 1 / dir.y;
                    dist.z = 1 / dir.z;
                    //std::cout <<"dist = x: " << dist.x << "\ty: "<< dist.y << "\tz: " << dist.z << std::endl;
                    if(dist.x >= 0)
                    {
                        tmin.x = dist.x*(XMIN - eye.x);
                        tmax.x = dist.x*(XMAX - eye.x);
                    }
                    else
                    {
                        tmin.x = dist.x*(XMAX - eye.x);
                        tmax.x = dist.x*(XMIN - eye.x);
                    }
                    if(dist.y >= 0)
                    {
                        tmin.y = dist.y*(YMIN - eye.y);
                        tmax.y = dist.y*(YMAX - eye.y);
                    }
                    else
                    {
                        tmin.y = dist.y*(YMAX - eye.y);
                        tmax.y = dist.y*(YMIN - eye.y);
                    }
                    if(dist.z >= 0)
                    {
                        tmin.z = dist.z*(ZMIN - eye.z);
                        tmax.z = dist.z*(ZMAX - eye.z);
                    }
                    else
                    {
                        tmin.z = dist.z*(ZMAX - eye.z);
                        tmax.z = dist.z*(ZMIN - eye.z);
                    }
                    //std::cout <<"tmin = x:  << XMIN <<", "<< XMAX <<"\ty: "<< YMIN<<", "<< YMAX <<"\tz: "<< ZMIN<<", "<<ZMAX << std::endl;
                    

                    //fim nosso codigo
                    float mint, maxt;
                    
                    mint = std::max(std::max(tmin.x,tmin.y),tmin.z);
                    maxt = std::min(std::min(tmax.x,tmax.y),tmax.z);

					if (mint < maxt) {
                    	//std::cout <<"contador = x: " << contadorpcc++ << std::endl;
                    	color += shootRay(ray, 0.0f, FLT_MAX, _maxRecursionDepth);
					}

                    /*if(tmin.z < tmax.y)
                    {
                        if(tmin.y < tmax.x)
                        {	
                           	std::cout <<"contador = x: " << contadorpcc++ << std::endl;
                           	color += shootRay(ray, 0.0f, FLT_MAX, _maxRecursionDepth);
                        }
                    }*/
                }
            }

            color = glm::clamp(color/(float)(_sampleResolution*_sampleResolution), 0.0f, 1.0f);

            _buffer[(pixel.y * _resolution.x + pixel.x)] = float2ubyte(color);

            currPixel += 1.0f;
            float p = 100.0f*currPixel/maxPixels;
            if (p > refP+5.0f || p == 100.0f)
            {
                refP = p;
                std::cout << p << "%" << std::endl;
            }
        }
    }

    std::cout << "Done!" << std::endl;

    glDrawPixels(_resolution.x, _resolution.y, GL_RGBA, GL_UNSIGNED_BYTE, _buffer);

    glutSwapBuffers();

}

void RayTracer::reshape(int w, int h)
{
    if (w == _resolution.x && h == _resolution.y) return;

    _resolution = glm::i32vec2(w,h);
    delete[] _buffer;
    _buffer = new glm::u8vec4[_resolution.x*_resolution.y];

    _projection = glm::perspective(45.0f, (float)_resolution.x/(float)_resolution.y, 1.0f, 100.0f);//FLT_MAX))

    glutPostRedisplay();
}

void RayTracer::mouse(int button, int state, int x, int y)
{
    bool redisplay = false;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            _trackball.mouseDown(Trackball::getPos(glm::i32vec2(x,y),_resolution));
        }
        else
        {
            _trackball.mouseUp(Trackball::getPos(glm::i32vec2(x,y),_resolution));
        }
        break;
    case 3:
        _cameraAngle *= 1.0f-_scroolFactor;
        _projection = glm::perspective(_cameraAngle, (float)_resolution.x/(float)_resolution.y, 0.1f, 100000.0f);//FLT_MAX))
        redisplay = true;
        break;
    case 4:
        _cameraAngle *= 1.0f+_scroolFactor;
        _projection = glm::perspective(_cameraAngle, (float)_resolution.x/(float)_resolution.y, 0.1f, 100000.0f);//FLT_MAX))
        redisplay = true;
        break;
    default:
        break;
    }

    if (redisplay) glutPostRedisplay();
}

void RayTracer::motion(int x, int y)
{

    _trackball.mouseMotion(Trackball::getPos(glm::i32vec2(x,y),_resolution));

    glutPostRedisplay();
}

void RayTracer::keyboard(unsigned char key, int x, int y)
{
    bool redisplay = false;
    if (key == '+' || key == '-')
    {
        if (key == '+')
        {
            _maxRecursionDepth++;
        }
        else
        {
            _maxRecursionDepth = std::max(_maxRecursionDepth-1,0);
        }
        std::cout << "Max recursion depth: " << _maxRecursionDepth << std::endl;
        redisplay = true;
    }
    else if (key == 'w' || key == 's')
    {
        if (key == 'w')
        {
            _sampleResolution++;
        }
        else
        {
            _sampleResolution = std::max(_sampleResolution-1,1);
        }
        std::cout << "Sample resolution: " << _sampleResolution << std::endl;
        redisplay = true;
    }
    else if (key == 'o')
    {
        cimg_library::CImg<unsigned char> img(_resolution.x, _resolution.y, 1, 4);
        glm::i32vec2 pixel;
        for (pixel.x=0; pixel.x<_resolution.x; pixel.x++)
        {
            for (pixel.y=0; pixel.y<_resolution.y; pixel.y++)
            {
                img(pixel.x,pixel.y,0) = _buffer[((_resolution.y - pixel.y - 1) * _resolution.x + pixel.x)].r;
                img(pixel.x,pixel.y,1) = _buffer[((_resolution.y - pixel.y - 1) * _resolution.x + pixel.x)].g;
                img(pixel.x,pixel.y,2) = _buffer[((_resolution.y - pixel.y - 1) * _resolution.x + pixel.x)].b;
                img(pixel.x,pixel.y,3) = _buffer[((_resolution.y - pixel.y - 1) * _resolution.x + pixel.x)].a;
            }
        }
        img.save("./img/output.jpg");
    }
    else if (key == 'j')
    {
        _superSampleJittered = !_superSampleJittered;
        redisplay = true;
        std::cout << "Super sample Jittered: " << _superSampleJittered << std::endl;
    }

    if (redisplay)
        glutPostRedisplay();
}

glm::vec4 RayTracer::shootRay(Ray ray, float min, float max, int recDepth)
{
    glm::vec4 color(0.0f);
    if (recDepth < 0) return color;

    glm::vec3 vertex;
    glm::vec3 normal;
    Material material;
    float t = _scene.hit(ray, min, max, vertex, normal, material);
    if (t < FLT_MAX)
    {
        if (material.isDielectric())
        {
            Ray r = ray.reflect(vertex,normal);
            Ray t(glm::vec3(0.0), glm::vec3(0.0));
            // air 1.0
            // water 1.3
            // glass 1.5
            // diamond 1.8
            ray.refract(t,vertex,normal,1.0f,1.5f);
            Ray rRay = ray.reflect(vertex,normal);
            float fr = ray.fresnel(normal, 1.0f, 1.5f);

            color += fr * shootRay(rRay, _bias,  FLT_MAX, recDepth-1) + (1.0f-fr) * shootRay(t, _bias, FLT_MAX, recDepth-1);

        }
        else
        {

            for (auto l: _light)
            {
                glm::vec4 lPos4(l->getPosition(),1.0f);
                lPos4 = _scene.getModelMatrix() * lPos4;
                lPos4 = lPos4/lPos4.w;
                glm::vec3 lPos3(lPos4);
                Ray shadowRay(vertex,lPos3-vertex);
                if (!shootShadowRay(shadowRay))
                    color += BlinnPhong(ray.getOrigin(), vertex, normal, material, *l);
            }

            // perfect reflection
            Ray rRay = ray.reflect(vertex, normal);
            float fr = material.getReflectionTerm();
            color = (1.0f-fr)*color + fr*shootRay(rRay, _bias,  FLT_MAX, recDepth-1);
        }

        //color = glm::clamp(color, 0.0f, 1.0f);
    }

    return color;
}

bool RayTracer::shootShadowRay(Ray ray)
{
    return _scene.hit(ray, _bias, 1.0f);
}

glm::vec4 RayTracer::BlinnPhong(glm::vec3 eye, glm::vec3 vertex, glm::vec3 normal, Material material, Light light)
{

    glm::vec4 color(0.0f);
    float a,b,c;
    light.getAttenuation(a,b,c);
    glm::vec4 lPos4(light.getPosition(),1.0f);
    lPos4 = _scene.getModelMatrix() * lPos4;
    lPos4 = lPos4/lPos4.w;
    glm::vec3 lPos3(lPos4);
    float d = glm::length(lPos3 - vertex);
    float fatt = 1.0f / (a + b * d + c * d * d);

    glm::vec3 E = normalize(eye - vertex);

    if (glm::dot(E,normal) < 0) return color;

    glm::vec3 L = normalize(lPos3 - vertex);
    //glm::vec3 R = normalize(-L + 2.0f * glm::dot(L,normal) * normal);
    glm::vec3 H = normalize(L + E);

    glm::vec4 Iamb = material.getAmbient() * light.getColor();

    glm::vec4 Idiff = fatt * material.getDiffuse() * light.getColor() * std::max(glm::dot(normal,L), 0.0f);
    //Idiff = glm::clamp(Idiff, 0.0f, 1.0f);

    glm::vec4 Ispec = fatt * material.getSpecular() * light.getColor() * std::pow(std::max(glm::dot(H,normal),0.0f), material.getShininess());
    //Ispec = glm::clamp(Ispec, 0.0f, 1.0f);

    color =  Iamb + Idiff + Ispec;

    return color;
}

glm::u8vec4 RayTracer::float2ubyte(glm::vec4 v)
{
    glm::u8vec4 ret;
    ret.r = v.r * 255;
    ret.g = v.g * 255;
    ret.b = v.b * 255;
    ret.a = v.a * 255;
    return ret;
}

glm::vec4 RayTracer::samplePixel(glm::i32vec2 pixel, glm::i32vec2 sample)
{

    float factor = 1.0f/(float)_sampleResolution;
    glm::vec2 spix = glm::vec2(sample)*factor;

    if (_superSampleJittered)
    {
        glm::vec2 d(0.0f,0.0f);
        std::srand(time(NULL));
        d.x = (float)std::rand()/(float)RAND_MAX;
        d.y = (float)std::rand()/(float)RAND_MAX;
        spix += factor * d;
    }
    else
    {
        glm::vec2 center(0.5f,0.5f);
        spix += factor * center;
    }

    glm::vec4 canPos(-1+2*((pixel.x+spix.x)/(float)_resolution.x),-1+2*((pixel.y+spix.y)/(float)_resolution.y), -1.0f, 1.0f);

    return glm::inverse(_projection) * canPos;
}
