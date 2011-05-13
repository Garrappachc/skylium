/* 
 * Skylium.h
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

#ifndef Skylium_H
#define	Skylium_H

#include <SDL/SDL.h>

#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Singleton.h"

typedef enum {
	KEY_NOKEY = 0,
	KEY_ESC = 1,
	KEY_UP = 2,
	KEY_DOWN = 4,
	KEY_RIGHT = 8,
	KEY_LEFT = 16
} sKey;

class Skylium : public Singleton < Skylium > {

public:
	/**
	 * Konstruktor tworzy nowego SceneManagera.
	 */
	Skylium();

	virtual ~Skylium();

	/**************************************************************************/
	/*                           OBSŁUGA    SDL                               */
	/**
	 * Ustawienie rozmiarów okna i różnych innych dziwnych opcji.
	 */
	void execute();

	/**
	 * Inicjacja okna.
	 * @param windowName Nazwa okna.
	 * @return false, jeżeli coś poszło nie tak.
	 */
	bool init(const std::string&);

	void loop();

	void render();

	void cleanup();

	/**************************************************************************/


	/**
	 * Tworzy scenę.
	 */
	Scene * createScene(const std::string&);
	
	/**
	 * Zwraca inta związanego z jakimś wydarzeniem, np. naciśnięciem klawisza,
	 * czy ruchem myszki.
     * @return Numer zdarzenia.
     */
	sKey sEvent();
	
	void enableMouseCamera();
	
	void disableMouseCmaera();
	
private:

	/* Jedyna instancja SceneManagera. */
	SceneManager * __sceneManagement;

	/* Przechowuje pozycję x i y myszy. */
	int __lastMousePositionX;
	int __lastMousePositionY;

	/* Do pętli while. */
	bool __isRunning;
	
	/* Do kamerki z myszką */
	bool __isMouseMotionEnabled;

	/* Taka tam zmienna, potrzebna SDL-owi. */
	SDL_Surface * __surfDisplay;

};
#endif	/* Skylium_H */

