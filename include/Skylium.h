/*
     ___ _        _ _              _    
    / __| |___  _| (_)_  _ _ __   | |_  
    \__ \ / / || | | | || | '  \ _| ' \ 
    |___/_\_\\_, |_|_|\_,_|_|_|_(_)_||_|
             |__/             
             
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


#ifndef SKYLIUM_H
#define SKYLIUM_H

#include <GL/glx.h>

#include "Singleton.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Shader.h"
#include "Hud.h"
#include "Timer.h"
#include "MatricesManager.h"

#include "keysdef.h"

/* Shaders' types */
enum {
	IDENTITY = 1,
	PHONG_SHADING,
	TOON,
	CUSTOM
};


class Skylium : public Singleton < Skylium > {

public:
	
	/**
	 * Creates new SceneManager (singleton).
	 */
	Skylium();
	
	virtual ~Skylium();
	
	/**
	 * Sets rendering context and does some standard jobs.
	 * @param windowName Window name;
	 * @return False if something went wrong.
	 */
	bool init(const std::string&);
	
	/**
	 * Catches events from the queue and renders activeScene.
	 */
	void execute();
	
	/**
	 * Swaps buffers.
	 */
	void swapBuffers();
	
	/**
	 * Returns pressed keys.
	 */
	sKey sEvent() { return __pendingKeys; }
	
	/**
	 * If mouse camera is active, it enables the possibility of controling the camera with mouse.
	 */
	void toggleMouseCamera() { __isMouseMotionEnabled = !__isMouseMotionEnabled; }
	
	/**
	 * Creates the new scene.
	 * @param name Name of the new scene - must be unique!
	 * @return Pointer to the newly created scene.
	 */
	Scene * createScene(const std::string&);
	
	/**
	 * Creates the new shader.
	 * @param type Type of the new shader. Possible values are: IDENTITY, PHONG_SHADING, TOON, CUSTOM.
	 * @param vertFile Source file of the vertex shader, if type == CUSTOM.
	 * @param fragFile Source file of the fragment shader, if type == CUSTOM.
	 */
	Shader * createShader(const unsigned&, const std::string& = "", const std::string& = "");
	
	/**
	 * Returns the current window's dimensions.
	 */
	void getWindowSize(int &_a, int &_b) { _a = __windowWidth; _b = __windowHeight; }
	
	/**
	 * Checks if particular GL's extension is available.
	 */
	bool isSupported(const std::string&);
	
	/* Struct that keeps info about the rendering context.
	 * Context can be get by getContext() and getContextPtr() methods. */
	typedef struct sContextStruct {
		
		/* Pointer to an active X server display */
		Display *	display;
		
		/* GLX version */
		int	GLXVersionMajor;
		int	GLXVersionMinor;
		
		/* Window attribs */
		XSetWindowAttributes	winAttribs;
		
		/* Pointer to the window */
		Window	window;
		
		/* Window dimensions */
		int	winHeight;
		int	winWidth;
		
		int	mousePosX;
		int	mousePosY;
		
		GLXContext	context;
		
	} sContextStruct;
	
	const sContextStruct & getContext() { return __GLXContext; }
	const sContextStruct * getContextPtr() { return &__GLXContext; }
	
	
	/* Access to managers from outside the class */
	SceneManager*& Scenes;
	TextureManager*& Textures;
	Hud*& TheHud;
	Timer*& GlobalTimer;
	MatricesManager*& Matrices;
	
	
private:
	
	/**
	 * Renders the scene.
	 */
	void __render();
	
	/**
	 * Catches events from the queue.
	 */
	void __catchEvents();
	
	/**
	 * Loads the config file.
	 * @param fileName File location.
	 */
	void __loadConfig(const std::string&);
	
	/**
	 * Sets GL's extensions pointer.
	 */
	void __earlyInitGLXFnPointers();
	
	/**
	 * Gets list of available extensions.
	 */
	void __getExtensionList();
	
	/* Init functions */
	bool __openXServerConnection();
	bool __getGLXVersion();
	bool __getBestConfig(XVisualInfo*&, GLXFBConfig&);
	bool __openXWindow(XVisualInfo*, const std::string&);
	bool __createGLXContext(GLXFBConfig&);
	void __hideMousePointer();
	
	void __destroyContextAndWindow();
	
	/**
	 * @param fileName Name of the file.
	 * @return False if file could not be found, otherwise true.
	 */
	bool __fileExists(const std::string&);
	
	/* SceneManager instance */
	SceneManager * __sceneManagement;
	
	/* TextureManager instance */
	TextureManager * __textureManagement;
	
	/* MatricesManager instance */
	MatricesManager * __matricesManagement;
	
	/* Keeps the rendering context */
	sContextStruct __GLXContext;
	
	/* Keeps pressed keys */
	sKey __pendingKeys;
	
	/* For mouse-controlled camera */
	bool __isMouseMotionEnabled;
	
	/* Keeps mouse position */
	int __lastMousePositionX;
	int __lastMousePositionY;
	
	/* Keeps window dimensions */
	int & __windowWidth;
	int & __windowHeight;
	
	std::vector< Shader* > __shaderList;
	
	/* The Hud's only instance */
	Hud * __hud;
	
	/* Global Timer */
	Timer * __timer;
	
	/* Vector with available extensions */
	std::vector< std::string* > __extensions;
	
	/* Pointers to some GL's extensions */
	GLXContext	(*glXCreateContextAttribsARB)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
	GLXFBConfig *	(*glXChooseFBConfig)(Display*, int, const int*, int*);
	XVisualInfo *	(*glXGetVisualFromFBConfig)(Display*, GLXFBConfig);
	int		(*glXGetFBConfigAttrib)(Display*, GLXFBConfig, int, int*);
	const GLubyte *	(*glGetStringi)(GLenum, GLuint);
	
};

#endif // SKYLIUM_H
