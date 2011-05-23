/*
    Skylium.cpp
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

#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>

#include "../include/Skylium.h"

#include "../include/defines.h"

using namespace std;

Skylium::Skylium() : 
		Scenes(__sceneManagement),
		__sceneManagement(NULL),
		__pendingKeys(KEY_NOKEY),
		__isMouseMotionEnabled(false),
		__surfDisplay(NULL),
		__lastMousePositionX(0),
		__lastMousePositionY(0) {
	__sceneManagement = new SceneManager();
	
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Skylium()";
#endif
}

Skylium::~Skylium() {
	// Uwalniamy kontekst renderowania
	SDL_FreeSurface(__surfDisplay);
	SDL_Quit();
	
	// Niszczymy SceneMangager
	delete __sceneManagement;
#ifdef __DEBUG__
	cout << LOG_INFO << "C'ya!\n";
#endif
}

bool
Skylium::init(const string &_windowName, const bool &_fullScreen) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie udało się utworzyć kontekstu renderowania! Zamykam.";
#endif
		return false;
	}
	
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_EnableKeyRepeat(1, 1);
	
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	
	if (!info) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie udało się utworzyć kontekstu renderowania! Zamykam.";
#endif
		return false;
	}
	
	int bpp = info -> vfmt -> BitsPerPixel;
	
	int windowWidth, windowHeight;
	
	Uint32 flags = SDL_HWSURFACE;
	flags |= SDL_GL_DOUBLEBUFFER;
	flags |= SDL_OPENGL;
	if (!_fullScreen) {
		flags |= SDL_RESIZABLE;
		windowWidth = info -> current_w - 50;
		windowHeight = info -> current_h - 100;
	} else {
		flags |= SDL_FULLSCREEN;
		windowWidth = info -> current_w;
		windowHeight = info -> current_h;
	}
	
	if ((__surfDisplay = SDL_SetVideoMode(windowWidth, windowHeight, bpp, flags)) == NULL) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie udało się utworzyć okna! Zamykam.";
#endif
		return false;
	}
	
	SDL_WM_SetCaption(_windowName.c_str(), NULL);
	
	// inicjalizujemy GLEWa
	glewInit();
	
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, windowWidth, windowHeight);
	glLoadIdentity();
	
	return true;
	
}

void
Skylium::execute() {
	__catchEvents();
	__render();
}

Scene *
Skylium::createScene(const string &_sceneName) {
	Scene *newScene = __sceneManagement -> createScene(_sceneName);
	return newScene;
}

void
Skylium::__render() {
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT_AND_BACK);
	
	glEnable(GL_POLYGON_SMOOTH);
	
	__sceneManagement -> displayActiveScene();
	
}

void
Skylium::__catchEvents() {
	__pendingKeys = KEY_NOKEY;
	SDL_Event tempEvent;
	
	while (SDL_PollEvent(&tempEvent)) {
		switch (tempEvent.type) {
			case SDL_KEYDOWN:
				switch (tempEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						__pendingKeys = KEY_ESC;
						break;
					case SDLK_w:
						__pendingKeys = KEY_UP;
						break;
					case SDLK_s:
						__pendingKeys = KEY_DOWN;
						break;
					case SDLK_a:
						__pendingKeys = KEY_LEFT;
						break;
					case SDLK_d:
						__pendingKeys = KEY_RIGHT;
						break;
					case SDLK_TAB:
						__pendingKeys = KEY_TAB;
						break;
					default:
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				if (__isMouseMotionEnabled) {
					int x = tempEvent.motion.x;
					int y = tempEvent.motion.y;
					const GLdouble moveX = (__lastMousePositionX - x);
					const GLdouble moveY = (__lastMousePositionY - y);
					__sceneManagement->getActiveScene()->getActiveCamera()->rotateCamera(moveX, moveY, 0.0);
					__lastMousePositionX = x;
					__lastMousePositionY = y;
				}
				break;
			case SDL_VIDEORESIZE:
				__sceneManagement->getActiveScene()->getActiveCamera()->setProjection();
				break;
		}
	}
	
}