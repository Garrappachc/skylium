/* 
 * File:   SceneManager.cpp
 * Author: Michał Garapich
 * 
 * Created on 2 april 2011, 18:05
 */

#include "../include/SceneManager.h"
#include "../include/Scene.h"

using namespace std;

SceneManager::SceneManager() { // nad tym będziemy mysleć
}

SceneManager::~SceneManager() {
	for (int i = sceneList_.size() - 1; i >= 0; i--) {
		delete sceneList_[i];
		sceneList_.pop_back();
	}
}

Scene *
SceneManager::createScene(const string &newSceneName) {
	Scene *newScene = new Scene(newSceneName);
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
#ifdef _DEBUG_STRONG
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	//glEnable(GL_COLOR_MATERIAL);

	if (activeScene_) {
		activeScene_ -> showAllObjects();
		return true;
	}
	return false;
}

bool
SceneManager::setActive(const std::string& sceneName) {
	if (sceneList_.size() == 0)
		return false;
	for (int i = 0; i < (int)sceneList_.size(); i++) {
		if (sceneList_[i] -> name == sceneName) {
			activeScene_ = sceneList_[i];
			return true;
		}
	}
	return false;
}

bool
SceneManager::setActive(const Scene* scenePointer) {
	if (sceneList_.size() == 0)
		return false;
	for (int i = 0; i < (int)sceneList_.size(); i++) {
		if (sceneList_[i] == scenePointer) {
			activeScene_ = sceneList_[i];
			return true;
		}
	}
	return false;
 }

Scene *
SceneManager::getSceneByName(const std::string& sceneName) {
	if (sceneList_.size() == 0)
		return 0;
	for (int i = 0; i < (int)sceneList_.size(); i++) {
		if (sceneList_[i] -> name == sceneName)
			return sceneList_[i];
	}
	return 0;
}

Scene *
SceneManager::getActiveScene() {
	return activeScene_;
}