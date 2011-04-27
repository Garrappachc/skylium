/* 
 * File:   Skylium.h
 * Author: Michał Garapich
 *
 * Created on 1 kwiecień 2011, 20:51
 *
 ******
 * Klasa-matka.
 ******
 */

#ifndef Skylium_H
#define	Skylium_H

#include <SDL/SDL.h>

#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

class Skylium {

public:

	static Skylium* instance();

	static void clean();

	/**************************************************************************/
	/*                           OBSŁUGA    SDL                               */
	/**
	 * Ustawienie rozmiarów okna i różnych innych dziwnych opcji.
     * @return true, jeżeli wszystko działa, false, jeżeli coś poszło nie tak.
     */
	bool execute();

	/**
	 * Inicjacja okna.
     * @return false, jeżeli coś poszło nie tak.
     */
	bool init();

	bool event(SDL_Event *event);

	bool loop();

	bool render();

	bool cleanup();

	/**************************************************************************/


	/**
	 * Tworzy scenę.
     */
	Scene * createScene(const std::string&);
	
private:
	/**
	 * Konstruktor tworzy nowego SceneManagera.
     */
	Skylium();

	virtual ~Skylium();

	/* Jedyna instancja SceneManagera. */
	SceneManager * sceneManagement_;

	static Skylium* instance_;

	/* Przechowuje pozycję x i y myszy. */
	int lastMousePositionX_;
	int lastMousePositionY_;

	/* Do pętli while. */
	bool isRunning_;

	/* Taka tam zmienna, potrzebna SDL-owi. */
	SDL_Surface * surfDisplay_;

};
#endif	/* Skylium_H */

