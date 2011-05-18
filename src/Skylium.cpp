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
	__catchEvents();
	render();
}

bool
Skylium::init(const string &_windowName, const bool &_fullscreen) {
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

	int windowWidth, windowHeight;
	
	Uint32 flags = SDL_HWSURFACE;
	flags |= SDL_GL_DOUBLEBUFFER;
	flags |= SDL_OPENGL;
	if (_fullscreen) {
		flags |= SDL_FULLSCREEN;
		windowWidth = info -> current_w;
		windowHeight = info -> current_h;
	} else {
		flags |= SDL_RESIZABLE;
		windowWidth = info -> current_w - 50;
		windowHeight = info -> current_h - 100;
	}

    if((__surfDisplay = SDL_SetVideoMode(windowWidth, windowHeight, bpp, flags)) == NULL) {
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
}

void
Skylium::swapBuffers() {
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

void
Skylium::toggleMouseCamera() {
	if (!__isMouseMotionEnabled)
		__isMouseMotionEnabled = true;
	else
		__isMouseMotionEnabled = false;
}

void
Skylium::__catchEvents() {
	__currentKeys = KEY_NOKEY;
	SDL_Event tempEvent;
	while (SDL_PollEvent(&tempEvent)) {
		switch (tempEvent.type) {
			case SDL_KEYDOWN:
				switch(tempEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						__currentKeys = KEY_ESC;
						break;
					case SDLK_w:
						__currentKeys = KEY_UP;
						break;
					case SDLK_s:
						__currentKeys = KEY_DOWN;
						break;
					case SDLK_a:
						__currentKeys = KEY_LEFT;
						break;
					case SDLK_d:
						__currentKeys = KEY_RIGHT;
						break;
					case SDLK_TAB:
						__currentKeys = KEY_TAB;
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
			case SDL_QUIT:
				cleanup();
		}
	}
}