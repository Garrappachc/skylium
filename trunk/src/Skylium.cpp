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

#ifdef _WIN32
#include <GL/GLee.h>
#else
#include <GLee.h>
#endif
#include <SDL/SDL.h>

#include "../include/Skylium.h"
#include "../include/SceneManager.h"
#include "../include/Object.h"

#include "../include/functions.h"
#include "../include/defines.h"


using namespace std;

static const int KEY_ESC = 27;
static const int KEY_A = 97;
static const int KEY_D = 100;
static const int KEY_S = 115;
static const int KEY_W = 119;

Skylium::Skylium() {
	sceneManagement_ = new SceneManager();
	lastMousePositionX_ = 0;
	lastMousePositionY_ = 0;
	surfDisplay_ = NULL;
	isRunning_ = true;
#ifdef __DEBUG__
	cout << LOG_INFO << "Utworzono instancję klasy Skylium...";
#endif
}

Skylium::~Skylium() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Czyszczenie... ";
#endif
	delete sceneManagement_;
	cleanup();
#ifdef __DEBUG__
	cout << LOG_INFO << "C'ya!\n";
#endif
}

void
Skylium::execute() {
	SDL_Event tempEvent;
	while (isRunning_) {
		while (SDL_PollEvent(&tempEvent))
			event(&tempEvent);
//		loop();
		render();
	}
}

bool
Skylium::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
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

    if((surfDisplay_ = SDL_SetVideoMode(windowWidth, windowHeight, bpp, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
        return false;
    }

	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, windowWidth, windowHeight);
	glLoadIdentity();

	return true;
}

void
Skylium::event(SDL_Event *tempEvent) {
	switch (tempEvent->type) {
		case SDL_KEYDOWN:
			switch(tempEvent -> key.keysym.sym) {
				case SDLK_ESCAPE:
					isRunning_ = false;
					break;
				case SDLK_w:
					sceneManagement_->getActiveScene()->getActiveCamera()->moveCamera(-0.1, 0.0, 0.0);
					break;
				case SDLK_s:
					sceneManagement_->getActiveScene()->getActiveCamera()->moveCamera(0.1, 0.0, 0.0);
					break;
				case SDLK_a:
					sceneManagement_->getActiveScene()->getActiveCamera()->moveCamera(0.0, 0.0, 0.1);
					break;
				case SDLK_d:
					sceneManagement_->getActiveScene()->getActiveCamera()->moveCamera(0.0, 0.0, -0.1);
					break;
				default:
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			int x = tempEvent -> motion.x;
			int y = tempEvent -> motion.y;
			const GLdouble moveX = (lastMousePositionX_ - x);
			const GLdouble moveY = (lastMousePositionY_ - y);
			sceneManagement_->getActiveScene()->getActiveCamera()->rotateCamera(moveX, moveY, 0.0);
			lastMousePositionX_ = x;
			lastMousePositionY_ = y;
			break;
	}
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

	sceneManagement_ -> displayActiveScene();

	SDL_GL_SwapBuffers();
}

void
Skylium::cleanup() {
	SDL_FreeSurface(surfDisplay_);
	SDL_Quit();
}

Scene *
Skylium::createScene(const string &name) {
	Scene *newScene = sceneManagement_ -> createScene(name);
	return newScene;
}