/*
     ___                  __  __                                           
    / __| __ ___ _ _  ___|  \/  |__ _ _ _  __ _ __ _ ___ _ _  __ _ __ _ __ 
    \__ \/ _/ -_) ' \/ -_) |\/| / _` | ' \/ _` / _` / -_) '_|/ _| '_ \ '_ \
    |___/\__\___|_||_\___|_|  |_\__,_|_||_\__,_\__, \___|_|(_)__| .__/ .__/
                                               |___/            |_|  |_|   
                                               
    Copyright (C) 2011  Michał Garapich garrappachc@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <GL/gl.h>

#include "../include/SceneManager.h"

#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

SceneManager::SceneManager() :
		__sceneList(0),
		__activeScene(NULL) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "SceneManager constructed.";
}

SceneManager::~SceneManager() {
	while (!__sceneList.empty())
		delete __sceneList.back(), __sceneList.pop_back();
	
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Scene Manager destructed.";
}

Scene *
SceneManager::createScene(const string &_name) {
	Scene *newScene = new Scene(_name);
	__sceneList.push_back(newScene);
	if (!__activeScene)
		__activeScene = newScene;
	return newScene;
}

void
SceneManager::displayActiveScene() {
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glColor4f(0.0, 0.0, 0.0, 1.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glLoadIdentity( );
	checkGLErrors(AT);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_LINE_SMOOTH);
	
	
	if (__activeScene) {
		__activeScene -> show();	
	}
	
	glDisable(GL_BLEND);
}

bool
SceneManager::setActive(const Scene *_toSet) {
	if (__sceneList.size() <= 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "There are no scenes! Active scene was NOT set.";
		return false;
	}
	
	for (unsigned i = 0; i < __sceneList.size(); i++) {
		if (__sceneList[i] == _toSet) {
			__activeScene = __sceneList[i];
			return true;
		}
	}
	if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
		cout << LOG_WARN << "There is no such scene in the vector! Remember to create new scenes via SceneManager::createScene().";
	return false;
}