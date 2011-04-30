/* 
 * File:   SceneManager.cpp
 * Author: Michał Garapich
 * 
 * Created on 2 april 2011, 18:05
 */

#include "../include/SceneManager.h"
#include "../include/Scene.h"

using namespace std;

SceneManager::SceneManager() :
		sceneList_(0),
		activeScene_(NULL) {}

SceneManager::~SceneManager() {
	while (!sceneList_.empty())
		delete sceneList_.back(), sceneList_.pop_back();
}

Scene *
SceneManager::createScene(const string &_newSceneName) {
	Scene *newScene = new Scene(_newSceneName);
	sceneList_.push_back(newScene);
	if (sceneList_.size() == 1) {
		activeScene_ = newScene;
	}
	return newScene;
}

bool
SceneManager::displayActiveScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
#ifdef __DEBUG_STRONG__
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	if (activeScene_) {
		activeScene_ -> show();
		return true;
	}
	return false;
}

bool
SceneManager::setActive(const string &_sceneName) {
	if (sceneList_.size() == 0)
		return false;
	for (int i = 0; i < (int)sceneList_.size(); i++) {
		if (sceneList_[i] -> name == _sceneName) {
			activeScene_ = sceneList_[i];
			return true;
		}
	}
	return false;
}

bool
SceneManager::setActive(const Scene *_scenePointer) {
	if (sceneList_.size() == 0)
		return false;
	for (int i = 0; i < (int)sceneList_.size(); i++) {
		if (sceneList_[i] == _scenePointer) {
			activeScene_ = sceneList_[i];
			return true;
		}
	}
	return false;
 }

Scene *
SceneManager::getSceneByName(const string &_sceneName) {
	if (sceneList_.size() == 0)
		return 0;
	for (int i = 0; i < (int)sceneList_.size(); i++) {
		if (sceneList_[i] -> name == _sceneName)
			return sceneList_[i];
	}
	return 0;
}

Scene *
SceneManager::getActiveScene() {
	return activeScene_;
}