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
	 * @return false, jeżeli coś poszło nie tak.
	 */
	bool init();

	void event(SDL_Event *event);

	void loop();

	void render();

	void cleanup();

	/**************************************************************************/


	/**
	 * Tworzy scenę.
	 */
	Scene * createScene(const std::string&);
	
private:

	/* Jedyna instancja SceneManagera. */
	SceneManager * sceneManagement_;

	/* Przechowuje pozycję x i y myszy. */
	int lastMousePositionX_;
	int lastMousePositionY_;

	/* Do pętli while. */
	bool isRunning_;

	/* Taka tam zmienna, potrzebna SDL-owi. */
	SDL_Surface * surfDisplay_;

};
#endif	/* Skylium_H */

