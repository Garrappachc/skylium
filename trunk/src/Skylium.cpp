/*
     ___ _        _ _                           
    / __| |___  _| (_)_  _ _ __    __ _ __ _ __ 
    \__ \ / / || | | | || | '  \ _/ _| '_ \ '_ \
    |___/_\_\\_, |_|_|\_,_|_|_|_(_)__| .__/ .__/
             |__/                    |_|  |_|   

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
#include "../include/utils.h"

using namespace std;


Skylium::Skylium() : 
		Scenes(__sceneManagement),
		Textures(__textureManagement),
		TheHud(__hud),
		__sceneManagement(new SceneManager()),
		__textureManagement(new TextureManager()),
		__pendingKeys(KEY_NOKEY),
		__isMouseMotionEnabled(false),
		__surfDisplay(NULL),
		__lastMousePositionX(0),
		__lastMousePositionY(0),
		__shaderList(0),
		__hud(new Hud()) {
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Konstruktor: Skylium()";
}

Skylium::~Skylium() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Destruktor: ~Skylium()";

	// Uwalniamy kontekst renderowania
	SDL_FreeSurface(__surfDisplay);
	SDL_Quit();
	
	// Niszczymy SceneMangagera i TextureManagera
	delete __textureManagement;
	delete __sceneManagement;
	delete __hud;
	
	while (!__shaderList.empty())
		delete __shaderList.back(), __shaderList.pop_back();
}

bool
Skylium::init(const string &_windowName) {
	
	__loadConfig("skylium.cfg");
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Nie udało się utworzyć kontekstu renderowania! Zamykam.\n";
		return false;
	}
	
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, sGlobalConfig::GL_RED_SIZE );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, sGlobalConfig::GL_GREEN_SIZE );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, sGlobalConfig::GL_BLUE_SIZE );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, sGlobalConfig::GL_DEPTH_SIZE );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	if (sGlobalConfig::ENABLE_KEY_REPEAT)
		SDL_EnableKeyRepeat(1, 1);
	
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	
	if (!info) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Nie udało się utworzyć kontekstu renderowania! Zamykam.\n";
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
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Nie udało się utworzyć okna! Zamykam.\n";
		return false;
	}
	
	SDL_WM_SetCaption(_windowName.c_str(), NULL);

	if (!sGlobalConfig::MOUSE_VISIBLE)
		SDL_ShowCursor(SDL_DISABLE);

	SDL_WarpMouse(__windowWidth / 2, __windowHeight / 2);
	
	// inicjalizujemy GLEWa
	glewInit();
	
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	return true;
	
}

void
Skylium::execute() {
	__catchEvents();
	__render();
	if (TheHud -> visible())
		TheHud -> draw();
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
					case SDLK_BACKQUOTE:
						__pendingKeys = KEY_BACKQUOTE;
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
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Nie znaleziono pliku konfiguracyjnego! W użyciu wartości domyślne.";
		return;
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
				cout << LOG_ERROR << "Nieznana wartość " << value <<
					". Dostępne wartości dla parametru \"fullscreen\" to 0, 1, false lub true.";
		} else if (param == "using_vbo") {
			if (value == "false" || value == "0")
				sGlobalConfig::USING_VBO = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::USING_VBO = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value <<
					". Dostępne wartości dla parametru \"using_vbo\" to 0, 1, false lub true.";
		} else if (param == "hud_exists") {
			if (value == "false" || value == "0")
				sGlobalConfig::HUD_EXISTS = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::HUD_EXISTS = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value <<
					". Dostępne wartości dla parametru \"hud_exists\" to 0, 1, false lub true.";
		} else if (param == "enable_key_repeat") {
			if (value == "false" || value == "0")
				sGlobalConfig::ENABLE_KEY_REPEAT = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::ENABLE_KEY_REPEAT = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value <<
					". Dostępne wartości dla parametru \"" << param << "\" to 0, 1, false lub true.";
		} else if (param == "mouse_visible") {
			if (value == "false" || value == "0")
				sGlobalConfig::MOUSE_VISIBLE = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::MOUSE_VISIBLE = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value <<
					". Dostępne wartości dla parametru \"" << param << "\" to 0, 1, false lub true.";
		} else if (param == "gl_red_size") {
			sGlobalConfig::GL_RED_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_RED_SIZE < 0)
				cout << LOG_WARN << "Rozmiar bufora ustawiony na wartość ujemną! To nie wróży niczego dobrego.";
		} else if (param == "gl_blue_size") {
			sGlobalConfig::GL_BLUE_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_BLUE_SIZE < 0)
				cout << LOG_WARN << "Rozmiar bufora ustawiony na wartość ujemną! To nie wróży niczego dobrego.";
		} else if (param == "gl_green_size") {
			sGlobalConfig::GL_GREEN_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_GREEN_SIZE < 0)
				cout << LOG_WARN << "Rozmiar bufora ustawiony na wartość ujemną! To nie wróży niczego dobrego.";
		} else if (param == "gl_depth_size") {
			sGlobalConfig::GL_DEPTH_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_DEPTH_SIZE < 0)
				cout << LOG_WARN << "Rozmiar bufora ustawiony na wartość ujemną! To nie wróży niczego dobrego.";
		} else if (param == "tellmeabout") {
			sGlobalConfig::DEBUGGING = D_NOTHING;
			
			vector< string > flags;
			
			explode(value, ',', flags);
			
			for (vector< string >::const_iterator it = flags.begin(); it != flags.end(); it++) {
				if (*it == "nothing") {
					sGlobalConfig::DEBUGGING = D_NOTHING;
					break;
				} else if (*it == "constructors")
					sGlobalConfig::DEBUGGING |= D_CONSTRUCTORS;
				else if (*it == "all_constructors")
					sGlobalConfig::DEBUGGING |= D_ALL_CONSTRUCTORS;
				else if (*it == "destructors")
					sGlobalConfig::DEBUGGING |= D_DESTRUCTORS;
				else if (*it == "params")
					sGlobalConfig::DEBUGGING |= D_PARAMS;
				else if (*it == "all_params")
					sGlobalConfig::DEBUGGING |= D_ALL_PARAMS;
				else if (*it == "shaders")
					sGlobalConfig::DEBUGGING |= D_SHADERS;
				else if (*it == "buffer")
					sGlobalConfig::DEBUGGING |= D_BUFFER;
				else if (*it == "warnings")
					sGlobalConfig::DEBUGGING |= D_WARNINGS;
				else if (*it == "errors")
					sGlobalConfig::DEBUGGING |= D_ERRORS;
				else if (*it == "everything") {
					sGlobalConfig::DEBUGGING = D_CONSTRUCTORS |
									D_ALL_CONSTRUCTORS |
									D_DESTRUCTORS |
									D_PARAMS |
									D_ALL_PARAMS |
									D_SHADERS |
									D_BUFFER |
									D_WARNINGS |
									D_ERRORS;
					break;
				} else {
					cout << LOG_WARN << "ConfigParser: nierozpoznana opcja: " << *it;
				}
			}
		} else if (param != "") {
			cout << LOG_WARN << "ConfigParser: nierozpoznana opcja: \"" << param << "\"";
		}
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