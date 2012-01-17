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
#include "../include/keysfunc.h"

using namespace std;


Skylium::Skylium() : 
		Scenes(__sceneManagement),
		Textures(__textureManagement),
		TheHud(__hud),
		GlobalTimer(__timer),
		Matrices(__matricesManagement),
		__sceneManagement(new SceneManager()),
		__textureManagement(new TextureManager()),
		__matricesManagement(new MatricesManager()),
		__pendingKeys(KEY_NOKEY),
		__isMouseMotionEnabled(false),
		__lastMousePositionX(0),
		__lastMousePositionY(0),
		__windowWidth(__GLXContext.winWidth),
		__windowHeight(__GLXContext.winHeight),
		__shaderList(0),
		__hud(new Hud()),
		__timer(new Timer()) {
	
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Skylium constructed.";
	cout.flush();
}

Skylium::~Skylium() {
	delete __textureManagement;
	delete __sceneManagement;
	delete __hud;
	
	while (!__shaderList.empty())
		delete __shaderList.back(), __shaderList.pop_back();
	
	__destroyContextAndWindow();
	
	while (!__extensions.empty())
		delete __extensions.back(), __extensions.pop_back();
	
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Skylium destructed.";
}

bool
Skylium::init(const string &_windowName) {
	
	__loadConfig("skylium.cfg");
	
	__earlyInitGLXFnPointers();
	
	__GLXContext.mousePosX = 0;
	__GLXContext.mousePosY = 0;
	
	XVisualInfo *visualInfo = nullptr;
	GLXFBConfig bestFbConfig;
	
	if (!__openXServerConnection())
		return false;
	
	if (!__getGLXVersion())
		return false;
	
	if (!__getBestConfig(visualInfo, bestFbConfig))
		return false;
	 
	if (!__openXWindow(visualInfo, _windowName))
		return false;
	
	if (!__createGLXContext(bestFbConfig))
		return false;
	
	__getExtensionList();
	
	if (!sGlobalConfig::MOUSE_VISIBLE)
		__hideMousePointer();
	
	swapBuffers();
	checkGLErrors(AT);
	
	glViewport(0, 0, __GLXContext.winWidth, __GLXContext.winHeight);
	checkGLErrors(AT);
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	checkGLErrors(AT);
	
	if (sGlobalConfig::HUD_EXISTS)
		TheHud -> prepare();
	
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
			vertFile = "identity.vert";
			fragFile = "identity.frag";
			break;
		case PHONG_SHADING:
			vertFile = "shadow.vert";
			fragFile = "shadow.frag";
			break;
		case TOON:
			vertFile = "toon.vert";
			fragFile = "toon.frag";
			break;
		case CUSTOM:
			vertFile = _vertFile;
			fragFile = _fragFile;
			break;
		default:
			vertFile = "identity.vert";
			fragFile = "identity.frag";
			break;
	}
	
	Shader *newShader = new Shader(vertFile, fragFile);
	__shaderList.push_back(newShader);
	return newShader;
}

bool
Skylium::isSupported(const string &_ext) {
	// lambda expression
	auto comparator = [](const string *a, const string *b) -> bool {
		return (*a) < (*b);
	};
	
	return binary_search(__extensions.begin(), __extensions.end(), &_ext, comparator);
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
				doYourJob(XLookupKeysym(&xev.xkey, 0), __pendingKeys);
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
			cout << LOG_WARN << "No config file found! Default values in use.";
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
				cout << LOG_ERROR << "Value " << value <<
					" invalid. Possible values for \"fullscreen\" are 0, 1, false or true.";
		} else if (param == "using_vbo") {
			if (value == "false" || value == "0")
				sGlobalConfig::USING_VBO = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::USING_VBO = true;
			else
				cout << LOG_ERROR << "Value " << value <<
					" invalid. Possible values for \"using_vbo\" are 0, 1, false or true.";
		} else if (param == "hud_exists") {
			if (value == "false" || value == "0")
				sGlobalConfig::HUD_EXISTS = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::HUD_EXISTS = true;
			else
				cout << LOG_ERROR << "Value " << value <<
					" invalid. Possible valuse for \"hud_exists\" are 0, 1, false or true.";
		} else if (param == "mouse_visible") {
			if (value == "false" || value == "0")
				sGlobalConfig::MOUSE_VISIBLE = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::MOUSE_VISIBLE = true;
			else
				cout << LOG_ERROR << "Value " << value <<
					" invalid. Possible values for \"" << param << "\" are 0, 1, false or true.";
		} else if (param == "gl_red_size") {
			sGlobalConfig::GL_RED_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_RED_SIZE < 0)
				cout << LOG_WARN << "Buffer size is less than 0! It is not going to work as expected.";
		} else if (param == "gl_blue_size") {
			sGlobalConfig::GL_BLUE_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_BLUE_SIZE < 0)
				cout << LOG_WARN << "Buffer size is less than 0! It is not going to work as expected.";
		} else if (param == "gl_green_size") {
			sGlobalConfig::GL_GREEN_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_GREEN_SIZE < 0)
				cout << LOG_WARN << "Buffer size is less than 0! It is not going to work as expected.";
		} else if (param == "gl_depth_size") {
			sGlobalConfig::GL_DEPTH_SIZE = string2T< short >(value);
			if (sGlobalConfig::GL_DEPTH_SIZE < 0)
				cout << LOG_WARN << "Buffer size is less than 0! It is not going to work as expected.";
		} else if (param == "create_mipmaps") {
			if (value == "false" || value == "0")
				sGlobalConfig::CREATE_MIPMAPS = false;
			else if (value == "true" || value == "1")
				sGlobalConfig::CREATE_MIPMAPS = true;
			else
				cout << LOG_ERROR << "Value " << value <<
					" invalid. Possible values for \"" << param << "\" are 0, 1, false or true.";
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
					cout << LOG_WARN << "ConfigParser: unrecognized option: " << *it;
				}
			}
		} else if (param != "") {
			cout << LOG_WARN << "ConfigParser: unrecognized option: \"" << param << "\"";
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
	if (sGlobalConfig::OPENGL_VERSION_MAJOR >= 3) {
		int n;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n);
		checkGLErrors(AT);
		if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
			cout << LOG_INFO << "List of available extensions: " << n;
		for (int i = 0; i < n; i++) {
			const char *temp = (const char*)glGetStringi(GL_EXTENSIONS, i);
			__extensions.push_back(new string(temp));
			if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
				cout << LOG_INFO << "Found an extension: " << temp;
		}
	} else {
		const char* pszExtensions = (const char*)glGetString(GL_EXTENSIONS);
		checkGLErrors(AT);
		string tempExt(pszExtensions);
		string temp = "";
		for (unsigned i = 0; i < tempExt.length(); i++) {
			if (tempExt[i] == ' ') {
				__extensions.push_back(new string(temp));
				if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
					cout << LOG_INFO << "Found an extension: " << temp;
				cout.flush();
				temp = "";
			} else {
				temp += tempExt[i];
			}
		}
		
		if (temp != "") {
			__extensions.push_back(new string(temp));
			if ((sGlobalConfig::DEBUGGING & D_ALL_PARAMS) == D_ALL_PARAMS)
					cout << LOG_INFO << "Found an extension: " << temp;
		}
	}
	
	auto comparator = [](string *a, string *b) -> bool {
		return (*a) < (*b);
	};
	
	sort(__extensions.begin(), __extensions.end(), comparator);
}

bool
Skylium::__openXServerConnection() {
	/* Open connection with the X server */
	__GLXContext.display = XOpenDisplay(NULL);
	if (__GLXContext.display == NULL) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Connection with X server failed! Exiting.";
		return false;
	}
	checkGLErrors(AT);
	return true;
}

bool
Skylium::__getGLXVersion() {
	/* Get the GLX version */
	glXQueryVersion(__GLXContext.display, &__GLXContext.GLXVersionMajor, &__GLXContext.GLXVersionMinor);
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "GLX version: " << __GLXContext.GLXVersionMajor << "." << __GLXContext.GLXVersionMinor << ".";
	checkGLErrors(AT);
	
	if (__GLXContext.GLXVersionMajor <= 1 && __GLXContext.GLXVersionMinor < 3) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "GLX version too old. Exiting.";
		return false;
	}
	checkGLErrors(AT);
	return true;
}

bool
Skylium::__getBestConfig(XVisualInfo*& _visualInfo, GLXFBConfig& _bestFbConfig) {
	/* Look for the best configuration */
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
	/* Gets new configuration */
	fbConfigs = glXChooseFBConfig(__GLXContext.display, DefaultScreen(__GLXContext.display), fbAttribs, &numConfigs);
	if (!fbConfigs) {
		if ((sGlobalConfig::DEBUGGING & D_ERRORS) == D_ERRORS)
			cout << LOG_ERROR << "Frame buffer configuration failed to be fetched! Exiting.";
		return false;
	}
	checkGLErrors(AT);
	
	/* Choose the best configuration */
	int bestFbConfigNum = -1, worstFbConfigNum = -1, bestNumSamp = -1, worstNumSamp = 999;
	for (int i = 0; i < numConfigs; i++) {
		XVisualInfo *vi = glXGetVisualFromFBConfig(__GLXContext.display, fbConfigs[i]);
		if (vi) {
			int sampBuf, samples;
			glXGetFBConfigAttrib(__GLXContext.display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampBuf);
			glXGetFBConfigAttrib(__GLXContext.display, fbConfigs[i], GLX_SAMPLES, &samples);
			
			if ((bestFbConfigNum < 0 || sampBuf) && samples > bestNumSamp)
				bestFbConfigNum = i, bestNumSamp = samples;
			if (worstFbConfigNum < 0 || !sampBuf || samples < worstNumSamp)
				worstFbConfigNum = i, worstNumSamp = samples;
		}
		XFree(vi);
	}
	checkGLErrors(AT);
	
	_bestFbConfig = fbConfigs[bestFbConfigNum];
	XFree(fbConfigs);
	
	_visualInfo = glXGetVisualFromFBConfig(__GLXContext.display, _bestFbConfig);
	
	__GLXContext.winAttribs.event_mask =	ExposureMask
							|	VisibilityChangeMask
							|	KeyPressMask
							|	PointerMotionMask
							|	StructureNotifyMask
							;
	__GLXContext.winAttribs.border_pixel = 0;
	__GLXContext.winAttribs.background_pixmap = None;
	__GLXContext.winAttribs.bit_gravity = StaticGravity;
	__GLXContext.winAttribs.colormap = XCreateColormap(__GLXContext.display,
										RootWindow(__GLXContext.display, _visualInfo->screen),
										_visualInfo->visual,
										AllocNone
						    );
	checkGLErrors(AT);
	return true;
}

bool
Skylium::__openXWindow(XVisualInfo* _visualInfo, const string& _windowName) {
	GLint winmask = CWBorderPixel | CWBitGravity | CWEventMask | CWColormap;	
	
	__GLXContext.winWidth = DisplayWidth(__GLXContext.display,  DefaultScreen(__GLXContext.display));
	__GLXContext.winHeight = DisplayHeight(__GLXContext.display,  DefaultScreen(__GLXContext.display));
	
	if (!sGlobalConfig::FULLSCREEN_RENDERING) {
		__GLXContext.winWidth -= 50;
		__GLXContext.winHeight -= 100;
	}
	checkGLErrors(AT);
	
	/* Create the new window */
	__GLXContext.window = XCreateWindow(__GLXContext.display,
						   DefaultRootWindow(__GLXContext.display),
						   20, 20,
						   __GLXContext.winWidth, __GLXContext.winHeight, 0,
						   _visualInfo -> depth, InputOutput,
						   _visualInfo -> visual, winmask,
						   &__GLXContext.winAttribs
			    );
	checkGLErrors(AT);
	
	if (sGlobalConfig::FULLSCREEN_RENDERING) {
		XGrabKeyboard(__GLXContext.display, __GLXContext.window, True, GrabModeAsync, GrabModeAsync, CurrentTime);                                     
		XGrabPointer(__GLXContext.display, __GLXContext.window, True, ButtonPressMask, GrabModeAsync,
				   GrabModeAsync, __GLXContext.window, None, CurrentTime); 
	}
	
	XStoreName(__GLXContext.display, __GLXContext.window, _windowName.c_str());
	XMapWindow(__GLXContext.display, __GLXContext.window);
	checkGLErrors(AT);
	
	return true;
}

bool
Skylium::__createGLXContext(GLXFBConfig& _bestFbConfig) {
	/* Create the new OpenGL context */
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Initializing OpenGL version " << sGlobalConfig::OPENGL_VERSION_MAJOR
			<< "." << sGlobalConfig::OPENGL_VERSION_MINOR << "...";
	
	if (glXCreateContextAttribsARB) {
		GLint attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB,	sGlobalConfig::OPENGL_VERSION_MAJOR,
			GLX_CONTEXT_MINOR_VERSION_ARB,	sGlobalConfig::OPENGL_VERSION_MINOR,
			//GLX_CONTEXT_FLAGS_ARB,			GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0 };
	
		__GLXContext.context = glXCreateContextAttribsARB(__GLXContext.display, _bestFbConfig,
									  0, True, attribs);	
		XSync(__GLXContext.display, False);
		checkGLErrors(AT);
	} else {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "glXCreateContextAttribsARB() not found. Older function in use.";
		__GLXContext.context = glXCreateNewContext(__GLXContext.display, _bestFbConfig, GLX_RGBA_TYPE, 0, True);
	}
	
	glXMakeCurrent(__GLXContext.display, __GLXContext.window, __GLXContext.context);
	checkGLErrors(AT);
	
	return true;
}

void
Skylium::__hideMousePointer() {
	// hides the mouse pointer
	Cursor invisibleCursor;
	Pixmap bitmapEmpty;
	XColor black;
	static char noData[] = { 0,0,0,0,0,0,0,0 };
	black.red = black.green = black.blue = 0;
	
	bitmapEmpty = XCreateBitmapFromData(__GLXContext.display, __GLXContext.window, noData, 8, 8);
	invisibleCursor = XCreatePixmapCursor(__GLXContext.display, bitmapEmpty, bitmapEmpty, &black, &black, 0, 0);
	XDefineCursor(__GLXContext.display, __GLXContext.window, invisibleCursor);
	
	XFreeCursor(__GLXContext.display, invisibleCursor);
}

void
Skylium::__destroyContextAndWindow() {
	glXMakeCurrent(__GLXContext.display, None, NULL);
	glXDestroyContext(__GLXContext.display, __GLXContext.context);
	__GLXContext.context = NULL;
	
	XDestroyWindow(__GLXContext.display, __GLXContext.window);
	__GLXContext.window = (Window)NULL;
	
	XCloseDisplay(__GLXContext.display);
	__GLXContext.display = 0;
}

bool
Skylium::__fileExists(const string &_fileName) {
	struct stat buf;
	if (stat(_fileName.c_str(), &buf) == 0)
		return 1;
	else
		return 0;
}
