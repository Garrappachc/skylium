/* 
 * File:   Scene.cpp
 * Author: Michał Garapich
 * 
 * Created on 2 kwiecień 2011, 18:07
 */

#include "../include/Scene.h"
#include "../include/ObjectManager.h"

using namespace std;

Scene::Scene(const string& _name) :
		name(_name),
		activeCamera_(0),
		ambientLight_(1.0, 1.0, 1.0, 1.0),
		diffuseLight_(1.0, 1.0, 1.0, 1.0),
		specularLight_(1.0, 1.0, 1.0, 1.0),
		lightSrc_(0.0, 0.0, 0.0) {
	objectManagement_ = new ObjectManager();
	activeCamera_ = 0;
	name = _name;
	isLightOn_ = false;
}

Scene::~Scene() {
	delete objectManagement_;
}

bool
Scene::showAllObjects() {
	glLoadIdentity();
	if (activeCamera_) {
		activeCamera_ -> setProjection();
		activeCamera_ -> setView();
	}
	if (isLightOn_) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	glLightfv(GL_LIGHT0, GL_AMBIENT, &ambientLight_[0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuseLight_[0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &specularLight_[0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightSrc_[0]);
	if (!objectManagement_ -> showAll())
		return false;
	return true;
}

Object *
Scene::createObject(const std::string &_name) {
	Object * newObject = objectManagement_ -> newObject(_name);
	return newObject;
}

Object *
Scene::createBox(const string &name) {
	Object * newBox = objectManagement_ -> newBox(name);
	return newBox;
}

Camera *
Scene::createCamera(const GLdouble& x, const GLdouble& y, const GLdouble& z) {
	Camera * newCamera = new Camera(x, y, z);
	if (!activeCamera_)
		activeCamera_ = newCamera;
	cameraList_.push_back(newCamera);
	return newCamera;
}

Camera*
Scene::getActiveCamera() {
	return activeCamera_;
}

bool
Scene::setLights(const sColor &_ambient, const sColor &_diffuse, const sColor &_specular) {
	ambientLight_ = _ambient;
	diffuseLight_ = _diffuse;
	specularLight_ = _specular;
	return true;
}

bool
Scene::setAmbientLight(const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_T) {
	ambientLight_ = sColor(_R, _G, _B, _T);
	cout << "Ambient color:\n  R: " << ambientLight_[0] << "\n  G: " << ambientLight_[1] << "\n  B: " << ambientLight_[2] << endl;
	return true;
}

bool
Scene::setDiffuseLight(const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_T) {
	diffuseLight_ = sColor(_R, _G, _B, _T);
	cout << "Diffuse color:\n  R: " << diffuseLight_[0] << "\n  G: " << diffuseLight_[1] << "\n  B: " << diffuseLight_[2] << endl;
	return true;
}

bool
Scene::setSpecularLight(const GLfloat &_R, const GLfloat &_G, const GLfloat &_B, const GLfloat &_T) {
	specularLight_ = sColor(_R, _B, _G, _T);
	cout << "Specular color:\n  R: " << specularLight_[0] << "\n  G: " << specularLight_[1] << "\n  B: " << specularLight_[2] << endl;
	return true;
}

bool
Scene::setLightPosition(const GLfloat &_x, const GLfloat &_y, const GLfloat &_z) {
	lightSrc_ = sVec3D < GLfloat >(_x, _y, _z);
	cout << "Light position: (" << lightSrc_[0] << ", " << lightSrc_[1] << ", " << lightSrc_[2] << ")\n";
	return true;
}

bool
Scene::toggleLight() {
	isLightOn_ = !isLightOn_;
	return true;
}