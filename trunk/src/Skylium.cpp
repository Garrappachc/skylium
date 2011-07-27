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
#include <algorithm>

#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
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
		__lastMousePositionX(0),
		__lastMousePositionY(0),
		__windowWidth(__GLXContext.winWidth),
		__windowHeight(__GLXContext.winHeight),
		__shaderList(0),
		__hud(new Hud()) {
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Konstruktor: Skylium()";
	cout.flush();
}

Skylium::~Skylium() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Destruktor: ~Skylium()";
	
	// Niszczymy SceneMangagera i TextureManagera
	delete __textureManagement;
	delete __sceneManagement;
	delete __hud;
	
	while (!__shaderList.empty())
		delete __shaderList.back(), __shaderList.pop_back();
	
	glXMakeCurrent(__GLXContext.display, None, NULL);
	glXDestroyContext(__GLXContext.display, __GLXContext.context);
	__GLXContext.context = NULL;
	
	XDestroyWindow(__GLXContext.display, __GLXContext.window);
	__GLXContext.window = (Window)NULL;
	
	XCloseDisplay(__GLXContext.display);
	__GLXContext.display = 0;
}

bool
Skylium::init(const string &_windowName) {
	
	__loadConfig("skylium.cfg");
	
	__earlyInitGLXFnPointers();
	
	sContextStruct *rcx = &__GLXContext;
	
	rcx->mousePosX = 0;
	rcx->mousePosY = 0;
	
	XVisualInfo *visualInfo;
	
	/* Otwieramy połączenie z serwerem X */
	rcx->display = XOpenDisplay(NULL);
	if (rcx->display == NULL) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Nie udało się połączyć z serwerem X!";
		return false;
	}
	checkGLErrors(AT);
	
	/* Sprawdzamy wersję GLX */
	glXQueryVersion(rcx->display, &rcx->GLXVersionMajor, &rcx->GLXVersionMinor);
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Rozpoznano wersję GLX: " << rcx->GLXVersionMajor << "." << rcx->GLXVersionMinor << ".";
	checkGLErrors(AT);
	
	if (rcx->GLXVersionMajor <= 1 && rcx->GLXVersionMinor < 3) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Wersja GLX zainstalowana w tym systemie jest zbyt stara!";
		return false;
	}
	checkGLErrors(AT);
	
	/* Szukamy odpowiedniej konfiguracji */
	GLXFBConfig *fbConfigs;
	int numConfigs = 0;
	static int fbAttribs[] = {
		GLX_RENDER_TYPE,	GLX_RGBA_BIT,
		GLX_X_RENDERABLE,	True,
		GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER,	True,
		GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
		GLX_RED_SIZE,		sGlobalConfig::GL_RED_SIZE,
		GLX_BLUE_SIZE,		sGlobalConfig::GL_BLUE_SIZE,
		GLX_GREEN_SIZE,	sGlobalConfig::GL_GREEN_SIZE,
		GLX_DEPTH_SIZE,	sGlobalConfig::GL_DEPTH_SIZE,
		GLX_STENCIL_SIZE,	8,
		GLX_ALPHA_SIZE,	8,
		0 };
	/* Pobieramy nową konfigurację */
	fbConfigs = glXChooseFBConfig(rcx->display, DefaultScreen(rcx->display), fbAttribs, &numConfigs);
	if (!fbConfigs) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Nie udało się pobrać konfiguracji bufora ramki!";
		return false;
	}
	checkGLErrors(AT);
	
	/* Wybieramy najlepszą konfigurację */
	int bestFbConfigNum = -1, worstFbConfigNum = -1, bestNumSamp = -1, worstNumSamp = 999;
	for (int i = 0; i < numConfigs; i++) {
		XVisualInfo *vi = glXGetVisualFromFBConfig(rcx->display, fbConfigs[i]);
		if (vi) {
			int sampBuf, samples;
			glXGetFBConfigAttrib(rcx->display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampBuf);
			glXGetFBConfigAttrib(rcx->display, fbConfigs[i], GLX_SAMPLES, &samples);
			
			if ((bestFbConfigNum < 0 || sampBuf) && samples > bestNumSamp)
				bestFbConfigNum = i, bestNumSamp = samples;
			if (worstFbConfigNum < 0 || !sampBuf || samples < worstNumSamp)
				worstFbConfigNum = i, worstNumSamp = samples;
		}
		XFree(vi);
	}
	checkGLErrors(AT);
	
	GLXFBConfig bestFbConfig = fbConfigs[bestFbConfigNum];
	XFree(fbConfigs);
	
	visualInfo = glXGetVisualFromFBConfig(rcx->display, bestFbConfig);
	
	rcx -> winAttribs.event_mask =	ExposureMask
							|	VisibilityChangeMask
							|	KeyPressMask
							|	PointerMotionMask
							|	StructureNotifyMask
							;
	rcx -> winAttribs.border_pixel = 0;
	rcx -> winAttribs.background_pixmap = None;
	rcx -> winAttribs.bit_gravity = StaticGravity;
	rcx -> winAttribs.colormap = XCreateColormap(rcx->display,
										RootWindow(rcx->display, visualInfo->screen),
										visualInfo->visual,
										AllocNone
						    );
	checkGLErrors(AT);
	
	GLint winmask = CWBorderPixel | CWBitGravity | CWEventMask | CWColormap;
		
	
	rcx->winWidth = DisplayWidth(rcx->display,  DefaultScreen(rcx->display));
	rcx->winHeight = DisplayHeight(rcx->display,  DefaultScreen(rcx->display));
	
	if (!sGlobalConfig::FULLSCREEN_RENDERING) {
		rcx->winWidth -= 50;
		rcx->winHeight -= 100;
	}
	checkGLErrors(AT);
	
	/* Tworzymy nowe okno */
	rcx->window = XCreateWindow(rcx->display,
						   DefaultRootWindow(rcx->display),
						   20, 20,
						   rcx->winWidth, rcx->winHeight, 0,
						   visualInfo->depth, InputOutput,
						   visualInfo->visual, winmask,
						   &rcx->winAttribs
			    );
	checkGLErrors(AT);
	
	if (sGlobalConfig::FULLSCREEN_RENDERING) {
		XGrabKeyboard(rcx->display, rcx->window, True, GrabModeAsync, GrabModeAsync, CurrentTime);                                     
		XGrabPointer(rcx->display, rcx->window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, rcx->window, None, CurrentTime); 
	}
	
	XStoreName(rcx->display, rcx->window, _windowName.c_str());
	XMapWindow(rcx->display, rcx->window);
	checkGLErrors(AT);
	
	/* Tworzymy nowy kontekst OpenGL'a do renderowania */
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Inicjalizacja OpenGL " << sGlobalConfig::OPENGL_VERSION_MAJOR
			<< "." << sGlobalConfig::OPENGL_VERSION_MINOR;
			
	GLint attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB,	sGlobalConfig::OPENGL_VERSION_MAJOR,
		GLX_CONTEXT_MINOR_VERSION_ARB,	sGlobalConfig::OPENGL_VERSION_MINOR,
		//GLX_CONTEXT_FLAGS_ARB,			GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0 };
	
	rcx->context = glXCreateContextAttribsARB(rcx->display, bestFbConfig,
									  0, True, attribs);
	XSync(rcx->display, False);
	checkGLErrors(AT);
	
	glXMakeCurrent(rcx->display, rcx->window, rcx->context);
	checkGLErrors(AT);
	
	if (!sGlobalConfig::MOUSE_VISIBLE) {
		Cursor invisibleCursor;
		Pixmap bitmapEmpty;
		XColor black;
		static char noData[] = { 0,0,0,0,0,0,0,0 };
		black.red = black.green = black.blue = 0;
		
		bitmapEmpty = XCreateBitmapFromData(rcx->display, rcx->window, noData, 8, 8);
		invisibleCursor = XCreatePixmapCursor(rcx->display, bitmapEmpty, bitmapEmpty, &black, &black, 0, 0);
		XDefineCursor(rcx->display, rcx->window, invisibleCursor);
		
		XFreeCursor(rcx->display, invisibleCursor);
	}
	
	swapBuffers();
	checkGLErrors(AT);
	
	glViewport(0, 0, rcx->winWidth, rcx->winHeight);
	checkGLErrors(AT);
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	checkGLErrors(AT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkGLErrors(AT);
	glEnable(GL_DEPTH_TEST);
	checkGLErrors(AT);
	glDepthFunc(GL_LEQUAL);
	checkGLErrors(AT);
	
	return true;
	
}

void
Skylium::execute() {
	__catchEvents();
	__render();
	if (TheHud -> visible())
		TheHud -> draw();
}

void
Skylium::swapBuffers() {
	glXSwapBuffers(__GLXContext.display, __GLXContext.window);
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
Skylium::isSupported(const string &_ext) {
	auto comparator = [](const string *a, const string *b) -> bool {
		return (*a) < (*b);
	};
	
	if (binary_search(__extensions.begin(), __extensions.end(), &_ext, comparator))
		return true;
	return false;
}

void
Skylium::__render() {
	
	__sceneManagement -> displayActiveScene();
	
}

void
Skylium::__catchEvents() {
	__pendingKeys = KEY_NOKEY;
	
	XEvent xev;
	
	while(XPending(__GLXContext.display)) {
		XNextEvent(__GLXContext.display, &xev);
		
		switch (xev.type) {
			case  KeyPress:
				switch(XLookupKeysym(&xev.xkey, 0)) {
					case XK_Escape:
						__pendingKeys = KEY_ESC;
						break;
					case XK_w:
						__pendingKeys = KEY_UP;
						break;
					case XK_s:
						__pendingKeys = KEY_DOWN;
						break;
					case XK_a:
						__pendingKeys = KEY_LEFT;
						break;
					case XK_d:
						__pendingKeys = KEY_RIGHT;
						break;
					case XK_Tab:
						__pendingKeys = KEY_TAB;
						break;
					case XK_x:
						__pendingKeys = KEY_X;
						break;
					case XK_z:
						__pendingKeys = KEY_Z;
						break;
					case XK_F1:
						__pendingKeys = KEY_F1;
						break;
					case XK_F2:
						__pendingKeys = KEY_F2;
						break;
					case XK_quoteleft:
						__pendingKeys = KEY_BACKQUOTE;
						break;
					default:
						break;
				}
				break;
			case MotionNotify:
				if (__isMouseMotionEnabled) {
					int x = xev.xmotion.x;
					int y = xev.xmotion.y;
					const GLdouble moveX = (__lastMousePositionX - x);
					const GLdouble moveY = (__lastMousePositionY - y);
					__sceneManagement->getActiveScene()->getActiveCamera()->rotateCamera(moveX, moveY, 0.0);
					__lastMousePositionX = x;
					__lastMousePositionY = y;
				}
				break;
			case ConfigureNotify:
				XWindowAttributes winData;
				XGetWindowAttributes(__GLXContext.display, __GLXContext.window, &winData);
				__GLXContext.winWidth = winData.width;
				__GLXContext.winHeight = winData.height;
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
		} else if (param == "create_mipmaps") {
			if (value == "false" || value == "0")
				sGlobalConfig::CREATE_MIPMAPS = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::CREATE_MIPMAPS = true;
			else
				cout << LOG_ERROR << "Nieznana wartość " << value <<
					". Dostępne wartość dla parametru \"" << param << "\" to 0, 1, false lub true.";
		} else if (param == "opengl_version") {
			sGlobalConfig::OPENGL_VERSION_MAJOR = string2T< unsigned >(value);
			if (line.eof())
				sGlobalConfig::OPENGL_VERSION_MINOR = 0;
			else {
				line >> value;
				sGlobalConfig::OPENGL_VERSION_MINOR = string2T< unsigned >(value);
			}
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
					sGlobalConfig::DEBUGGING = D_CONSTRUCTORS 
								|	D_ALL_CONSTRUCTORS
								|	D_DESTRUCTORS
								|	D_PARAMS
								|	D_ALL_PARAMS
								|	D_SHADERS
								|	D_BUFFER
								|	D_WARNINGS
								|	D_ERRORS;
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

void
Skylium::__earlyInitGLXFnPointers() {
	glXCreateContextAttribsARB = getProcAddr< decltype(glXCreateContextAttribsARB) >("glXCreateContextAttribsARB");
	glXChooseFBConfig = getProcAddr< decltype(glXChooseFBConfig) >("glXChooseFBConfig");
	glXGetVisualFromFBConfig = getProcAddr< decltype(glXGetVisualFromFBConfig) >("glXGetVisualFromFBConfig");
	glXGetFBConfigAttrib = getProcAddr< decltype(glXGetFBConfigAttrib) >("glXGetFBConfigAttrib");
	glGetStringi = getProcAddr< decltype(glGetStringi) >("glGetStringi");
}

void
Skylium::__getExtensionList() {
	int n;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (int i = 0; i < n; i++) {
		const char *temp = (const char*)glGetStringi(GL_EXTENSIONS, i);
		__extensions.push_back(new string(temp));
	}
	
	auto comparator = [](string *a, string *b) -> bool {
		return (*a) < (*b);
	};
	
	sort(__extensions.begin(), __extensions.end(), comparator);
}

bool
Skylium::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}