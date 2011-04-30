/* 
 * File:   Skylium.h
 * Author: Michał Garapich
 *
 * Created on 1 april 2011, 20:51
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

