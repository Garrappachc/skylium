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

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

SceneManager::SceneManager() :
		__sceneList(0),
		__activeScene(NULL) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Konstruktor: SceneManager()";
}

SceneManager::~SceneManager() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Destruktor: ~SceneManager()";
	
	while (!__sceneList.empty())
		delete __sceneList.back(), __sceneList.pop_back(); // wywalamy cały wektor
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
	glColor4f(1.0, 1.0, 1.0, 1.0);
	//glLoadIdentity( );
	checkGLErrors(AT);
	
	
	if (__activeScene) {
		__activeScene -> show();	
	}
}

bool
SceneManager::setActive(const Scene *_toSet) {
	if (__sceneList.size() <= 0) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Nie mam żadnych scen! Nowa aktywna scena NIE została ustawiona.";
		return false;
	}
	
	for (unsigned i = 0; i < __sceneList.size(); i++) {
		if (__sceneList[i] == _toSet) {
			__activeScene = __sceneList[i];
			return true;
		}
	}
	if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
		cout << LOG_WARN << "Nie mam takiej sceny w bazie! Proszę tworzyć nowe sceny poprzez metodę Skylium::createScene().";
	return false;
}