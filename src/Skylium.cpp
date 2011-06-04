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
#include <fstream>
#include <sstream>

#include <sys/stat.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"

using namespace std;

/**
 * Funkcje pomocnicze.
 */
template < typename T >
inline T string2T(const std::string &_s) {
	T temp;
	istringstream ss(_s);
	ss >> temp;
	return temp;
}

Skylium::Skylium() : 
		Scenes(__sceneManagement),
		Textures(__textureManagement),
		__sceneManagement(NULL),
		__pendingKeys(KEY_NOKEY),
		__isMouseMotionEnabled(false),
		__surfDisplay(NULL),
		__lastMousePositionX(0),
		__lastMousePositionY(0),
		__shaderList(0) {
	__sceneManagement = new SceneManager();
	__textureManagement = new TextureManager();
	
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Skylium()";
#endif
}

Skylium::~Skylium() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Destruktor: ~Skylium()";
#endif
	// Uwalniamy kontekst renderowania
	SDL_FreeSurface(__surfDisplay);
	SDL_Quit();
	
	// Niszczymy SceneMangagera i TextureManagera
	delete __textureManagement;
	delete __sceneManagement;
	
#ifdef __DEBUG__
	cout << LOG_INFO << "C'ya!\n";
#endif
}

bool
Skylium::init(const string &_windowName) {
	
	__loadConfig("skylium.cfg");
	
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
	
	bool _fullScreen = sGlobalConfig::FULLSCREEN_RENDERING;
	
	Uint32 flags = SDL_HWSURFACE;
	flags |= SDL_GL_DOUBLEBUFFER;
	flags |= SDL_OPENGL;
	if (!_fullScreen) {
		flags |= SDL_RESIZABLE;
		__windowWidth = info -> current_w - 50;
		__windowHeight = info -> current_h - 100;
	} else {
		flags |= SDL_FULLSCREEN;
		__windowWidth = info -> current_w;
		__windowHeight = info -> current_h;
	}
	
	if ((__surfDisplay = SDL_SetVideoMode(__windowWidth, __windowHeight, bpp, flags)) == NULL) {
#ifdef __DEBUG__
		cout << LOG_ERROR << "Nie udało się utworzyć okna! Zamykam.";
#endif
		return false;
	}
	
	SDL_WM_SetCaption(_windowName.c_str(), NULL);
#ifndef __DEBUG__
	SDL_ShowCursor(SDL_DISABLE);
#endif
	SDL_WarpMouse(__windowWidth / 2, __windowHeight / 2);
	
	// inicjalizujemy GLEWa
	glewInit();
	
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, __windowWidth, __windowHeight);
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

Shader *
Skylium::createShader(const unsigned &_type, const string &_vertFile, const string &_fragFile) {
	string vertFile, fragFile;
	switch (_type) {
		case IDENTITY:
			vertFile = "shaders/identity.vert";
			fragFile = "shaders/identity.frag";
			break;
		case PHONG_SHADING:
			vertFile = "shaders/shadow.vert";
			fragFile = "shaders/shadow.frag";
			break;
		case TOON:
			vertFile = "shaders/toon.vert";
			fragFile = "shaders/toon.frag";
			break;
		case CUSTOM:
			vertFile = _vertFile;
			fragFile = _fragFile;
			break;
		default:
			vertFile = "shaders/identity.vert";
			fragFile = "shaders/identity.frag";
			break;
	}
	
	Shader *newShader = new Shader(vertFile, fragFile);
	__shaderList.push_back(newShader);
	return newShader;
}

bool
Skylium::isSupported(char *_ext) {
	const unsigned char *pszExtensions = NULL, *pszStart;
	unsigned char *pszWhere, *pszTerminator;
	pszWhere = (unsigned char *) strchr(_ext, ' ' );
	if( pszWhere || *_ext == '\0' )
		return false;
      // Pobierz łańcuch z rozszerzeniami dostępnymi na danej karcie graficznej
	pszExtensions = glGetString( GL_EXTENSIONS );
      // Sprawdź, czy w łańcuchu z rozszerzeniami jest dokładna kopia szTargetExtension
	pszStart = pszExtensions;
	for(;;) {
		pszWhere = (unsigned char *)strstr((const char *) pszStart, _ext);
		if(!pszWhere)
			break;
		pszTerminator = pszWhere + strlen(_ext);
		if(pszWhere == pszStart || *(pszWhere - 1) == ' ')
			if(*pszTerminator == ' ' || *pszTerminator == '\0')
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

void
Skylium::__render() {
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
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
					case SDLK_x:
						__pendingKeys = KEY_X;
						break;
					case SDLK_z:
						__pendingKeys = KEY_Z;
						break;
					case SDLK_F1:
						__pendingKeys = KEY_F1;
						break;
					case SDLK_F2:
						__pendingKeys = KEY_F2;
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
				const SDL_VideoInfo *info = SDL_GetVideoInfo();
				__windowWidth = info -> current_w;
				__windowHeight = info -> current_h;
				__sceneManagement->getActiveScene()->getActiveCamera()->setProjection();
				break;
		}
	}
	
}

void
Skylium::__loadConfig(const string &_fileName) {
	
	if (!__fileExists(_fileName)) {
#ifdef __DEBUG__
		cout << LOG_WARN << "Nie znaleziono pliku konfiguracyjnego! W użyciu wartości domyślne.";
		return;
#endif
	}
	
	fstream configFile(_fileName.c_str(), ios::in);
	
	string buffer, param, value;
	
	while (!configFile.eof()) {
		param = "";
		getline(configFile, buffer);
		
		if (buffer[0] == '#')
			continue;
		
		istringstream line(buffer);
		
		line >> param >> value;
		
		if (param == "min_vbo_size") {
			sGlobalConfig::MIN_VBO_SIZE = string2T< unsigned >(value);
		} else if (param == "max_vbo_size") {
			sGlobalConfig::MAX_VBO_SIZE = string2T< unsigned >(value);
		} else if (param == "fullscreen") {
			if (value == "false" || value == "0")
				sGlobalConfig::FULLSCREEN_RENDERING = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::FULLSCREEN_RENDERING = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value << ". Dostępne wartości dla parametru \"fullscreen\" to 0, 1, false lub true.";
		} else if (param == "using_vbo") {
			if (value == "false" || value == "0")
				sGlobalConfig::USING_VBO = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::USING_VBO = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value << ". Dostępne wartości dla parametru \"using_vbo\" to 0, 1, false lub true.";
		} else
			continue;
	}
	configFile.close();
}

bool
Skylium::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}