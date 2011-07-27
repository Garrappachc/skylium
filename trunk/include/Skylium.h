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

/* Obsługa klawiszy */
typedef enum {
	KEY_NOKEY = 0,
	KEY_ESC = 1,
	KEY_UP = 2,
	KEY_DOWN = 4,
	KEY_RIGHT = 8,
	KEY_LEFT = 16,
	KEY_TAB = 32,
	KEY_X = 64,
	KEY_Z = 128,
	KEY_F1 = 256,
	KEY_F2 = 512,
	KEY_BACKQUOTE = 1024
} sKey;

/* Róże typy shaderów */
enum {
	IDENTITY = 1,
	PHONG_SHADING,
	TOON,
	CUSTOM
};

class Skylium : public Singleton < Skylium > {

public:
	
	/**
	 * Konstruktor tworzy nowego SceneManagera - singletona
	 */
	Skylium();
	
	virtual ~Skylium();
	
	/**
	 * Metoda init ustawia kontekst renderowania i robi kilka standardowych rzeczy.
	 * @param windowName Nazwa okna.
	 * @param fullScreen Jeżeli true, będzie fullscreen.
	 * @return False, jeżeli coś poszło nie tak.
	 */
	bool init(const std::string&);
	
	/**
	 * Przechwytuje zdarzenia z kolejki, a następnie renderuje scenę.
	 */
	void execute();
	
	/**
	 * Przestawia bufory.
	 */
	void swapBuffers();
	
	sKey sEvent() { return __pendingKeys; }
	
	void toggleMouseCamera() { __isMouseMotionEnabled = !__isMouseMotionEnabled; }
	
	Scene * createScene(const std::string&);
	
	/*
	 * Tworzy nowego shadera i zwraca wskaźnik do niego.
	 */
	Shader * createShader(const unsigned&, const std::string& = "", const std::string& = "");
	
	void getWindowSize(int &_a, int &_b) { _a = __windowWidth; _b = __windowHeight; }
	
	/**
	 * Sprawdza, czy dane rozszerzenie jest dostępne.
	 */
	bool isSupported(const std::string&);
	
	/* Sktruktura przechowująca informacje o kontekście renderowania X.
	 * Aktualny kontekst jest dostępny poprzez funkcje getContext()
	 * oraz getContextPtr(). */
	typedef struct sContextStruct {
		
		/* Wskaźnik na aktywny ekran serwera X */
		Display *	display;
		
		/* Wersja GLX */
		int	GLXVersionMajor;
		int	GLXVersionMinor;
		
		/* Atrybuty okna */
		XSetWindowAttributes	winAttribs;
		
		/* Wskaźnik do okna */
		Window	window;
		
		/* Rozmiary okna */
		int	winHeight;
		int	winWidth;
		
		int	mousePosX;
		int	mousePosY;
		
		GLXContext	context;
		
	} sContextStruct;
	
	const sContextStruct & getContext() { return __GLXContext; }
	const sContextStruct * getContextPtr() { return &__GLXContext; }
	
	
	/* Dostęp do manadżerów z zewnątrze */
	SceneManager*& Scenes;
	TextureManager*& Textures;
	Hud*& TheHud;
	
	
private:
	
	/**
	 * Renderuje scenę.
	 */
	void __render();
	
	/**
	 * Łapie eventy z kolejki, obsługuje je odpowiednio, a naciśnięte klawisze
	 * wrzuca do __pendingKeys.
	 */
	void __catchEvents();
	
	/**
	 * Ładuje plik konfiguracyjny.
	 * @param fileName Lokalizacji pliku.
	 * @param 
	 */
	void __loadConfig(const std::string&);
	
	/**
	 * Ustawia wskaźniki na potrzebne funkcje.
	 */
	void __earlyInitGLXFnPointers();
	
	/**
	 * Pobiera listę dostępnych rozszerzeń */
	void __getExtensionList();
	
	/**
	 * Zwraca false, jeżeli plik nie istnieje.
	 */
	bool __fileExists(const std::string&);
	
	/* Instancja SceneManagera */
	SceneManager * __sceneManagement;
	
	/* Instancja TextureManagera */
	TextureManager * __textureManagement;
	
	/* Przechowuje kontekst renderowania */
	sContextStruct __GLXContext;
	
	/* Przechowuje wciśnięte klawisze. Patrz metoda sEvent(). */
	sKey __pendingKeys;
	
	/* Do kamerki z myszką. */
	bool __isMouseMotionEnabled;
	
	/* Przechowuje pozycję x i y myszy. */
	int __lastMousePositionX;
	int __lastMousePositionY;
	
	/* Przechowuje rozmiary okna. */
	int & __windowWidth;
	int & __windowHeight;
	
	std::vector< Shader* > __shaderList;
	
	/* Jedyna instancja huda */
	Hud * __hud;
	
	/* Wektor z dostępnymi rozszerzeniami */
	std::vector< std::string* > __extensions;
	
	/* Wskaźniki na kilka potrzebnych funkcji */
	GLXContext	(*glXCreateContextAttribsARB)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
	GLXFBConfig *	(*glXChooseFBConfig)(Display*, int, const int*, int*);
	XVisualInfo *	(*glXGetVisualFromFBConfig)(Display*, GLXFBConfig);
	int			(*glXGetFBConfigAttrib)(Display*, GLXFBConfig, int, int*);
	const GLubyte *	(*glGetStringi)(GLenum, GLuint);
	
};

#endif // SKYLIUM_H
