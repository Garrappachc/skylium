/* 
 * Skylium.cpp
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <GLee.h>
#include <SDL/SDL.h>

#include "../include/Skylium.h"
#include "../include/SceneManager.h"
#include "../include/Object.h"

#include "../include/functions.h"
#include "../include/defines.h"


using namespace std;

Skylium::Skylium() :
		__lastMousePositionX(0),
		__lastMousePositionY(0),
		__isRunning(true),
		__isMouseMotionEnabled(false),
		__surfDisplay(NULL) {
	__sceneManagement = new SceneManager();
#ifdef __DEBUG__
	cout << LOG_INFO << "Utworzono instancję klasy Skylium...";
#endif
}

Skylium::~Skylium() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Czyszczenie... ";
#endif
	delete __sceneManagement;
	cleanup();
#ifdef __DEBUG__
	cout << LOG_INFO << "C'ya!\n";
#endif
}

void
Skylium::execute() {
	render();
}

bool
Skylium::init(const string &_windowName) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
		return false;
	}

	int bpp = info -> vfmt -> BitsPerPixel;

	int windowWidth = info -> current_w - 50;
	int windowHeight = info -> current_h - 100;

    if((__surfDisplay = SDL_SetVideoMode(windowWidth, windowHeight, bpp, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
        return false;
    }
	
	SDL_WM_SetCaption(_windowName.c_str(), NULL);

	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, windowWidth, windowHeight);
	glLoadIdentity();

	return true;
}

void
Skylium::loop() {
	
}

void
Skylium::render() {
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT_AND_BACK);

	__sceneManagement -> displayActiveScene();

	SDL_GL_SwapBuffers();
}

void
Skylium::cleanup() {
	SDL_FreeSurface(__surfDisplay);
	SDL_Quit();
}

Scene *
Skylium::createScene(const string &name) {
	Scene *newScene = __sceneManagement -> createScene(name);
	return newScene;
}

sKey
Skylium::sEvent() {
	sKey result;
	SDL_Event tempEvent;
	while (SDL_PollEvent(&tempEvent)) {
		switch (tempEvent.type) {
			case SDL_KEYDOWN:
				switch(tempEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						result = KEY_ESC;
						break;
					case SDLK_w:
						result = KEY_UP;
						break;
					case SDLK_s:
						result = KEY_DOWN;
						break;
					case SDLK_a:
						result = KEY_LEFT;
						break;
					case SDLK_d:
						result = KEY_RIGHT;
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
			case SDL_QUIT:
				cleanup();
		}
	}
	return result;
}

void
Skylium::enableMouseCamera() {
	__isMouseMotionEnabled = true;
}

void
Skylium::disableMouseCmaera() {
	__isMouseMotionEnabled = false;
}