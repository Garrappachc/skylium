/* 
 * File:   main.cpp
 * Authors: Michał Garapich, Eryk Dykiel
 *
 * Created on 1 april 2011, 18:04
 */

#include <cstdlib>
#include <iostream>
#include <GLee.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include "include/Camera.h"
#include "include/Skylium.h"
#include "include/Scene.h"
#include "include/Shader.h"
#include "include/Timer.h"
#include "include/sFont.h"
#include "include/Hud.h"

using namespace std;

GLfloat boxTex[] = { // tekstury dla pudełka - do przeniesienia do frameworka w najbliższej przyszłości.
	0, 0,
	1, 0,
	1, 1,
	0, 1,

	1, 0,
	0, 0,
	0, 1,
	1, 1,

	0, 0,
	1, 0,
	1, 1,
	0, 1,

	0, 0,
	1, 0,
	1, 1,
	0, 1,

	1, 0,
	1, 1,
	0, 1,
	0, 0,

	0, 0,
	1, 0,
	1, 1,
	0, 1
};

int
main() {

	Skylium *s_main = new Skylium();
	
	if (!s_main -> init("Skylium", false)) { // tworzy okno, false - okno, true - fullscreen
		cout << "Błąd przy Skylium::init(). Przerywam.\n\n";
		return 1;
	}
	
	Shader *cienie = new Shader("shaders/shadow.vert", "shaders/shadow.frag");
	if (!cienie -> make()) // kompilacja + linkowanie
		exit(1);
	
	Scene *scenka = s_main -> createScene("Scenka"); // tworzymy scenę

	Object *pudelko = scenka -> createBox("Pudelko"); // poprzez scenkę tworzymy nowy obiekt
	pudelko -> scale(-3, -3, -3);
	pudelko -> move(-10, 0, 0);
	if (!pudelko -> loadTexture("texture/box.jpg", boxTex, 48)) // ładujemy teksturę
		cout << "Nie udało się załadować tekstury!";
	
	Object *plane = scenka -> createObject("plane"); // tworzymy nowy obiekt
	plane -> loadFromObj("objects/plane.obj", GET_VERTICES | GET_TEXTURE | GET_NORMALS); // wczytujemy z .obj'ta
	plane -> move(-10, 5.1, 10);
	plane -> scale (1.5, 1.5, 1.5);
	plane -> setColor(0, 132, 200, 1);
	plane -> rotate(0, 0, 20); // a co

	Object *malpka = scenka -> createObject("malpka"); // małpka
	if (!malpka -> loadFromObj("objects/monkey.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL)) // znowu .obj
		exit(1); // nie chcemy brzydali na ekranie
	malpka -> move(-10, 4.5, 0);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(0, -45, 40);
	malpka -> setColor(88, 25, 0, 1);
	
	Object *table = scenka -> createObject("table");
	table -> loadFromObj("objects/table.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL | GET_TEXTURE); // akurat stolik jest źle wymodelowany - nie przejmować się nim
	table -> move(-10, -2, 10);
	table -> scale (6, 6, 6);
	table -> setColor(119, 44, 0, 1);
	
	Camera* kamerka = scenka -> createCamera(5.0, 6.0, 0.0); // kamerka na pozycji (5, 6, 0)
	kamerka -> lookAt(1, 4, -1); // kamerka skierowana na punkt (1, 4, -1)
	
	cienie -> bind(malpka); // przyłączmy shadera z cieniowanem do małpki
	cienie -> bind(plane); // do samolociku
	cienie -> bind(table); // do stolika
	// do pudełka już nie, bo nie chce mi się shadera pisać na razie

	int swiatelko = scenka -> createLight(6.0, 6.0, 0.0); // światło na pozycji (6, 6, 0)
	scenka -> setAmbientLight(swiatelko, 0.5, 0.5, 0.5, 1.0); // ustawiamy ambient Light.
	scenka -> toggleLight(); // włączamy światło. Domyślnie każde światło jest wyłączone!
	
	Timer *zegarek_dla_animacji = new Timer(); // zegarek dla obracającego się stolika + samolotu
	Timer *zegarek_dla_fps = new Timer(); // liczymy fps'y
	Timer *zegarek_dla_taba = new Timer(); // nie chcemy efektu "jarzeniówki" przy przęłączaniu shaderów :)
	
	short fps = 0; // licznik fps.
	
	s_main -> toggleMouseCamera(); // Ha! Włączamy obracanie kamerą za pomocą myszy
	
	sKey klawisz; // tutaj przechwytujemy klawisze
	while ((klawisz = s_main -> sEvent()) != KEY_ESC) { // żeby się dało czymś wyjść
		if (klawisz == KEY_DOWN)
			kamerka -> moveCamera(0.1, 0.0, 0.0); // KEY_DOWN to nie kursor w dół, tylko S
		if (klawisz == KEY_UP)
			kamerka -> moveCamera(-0.1, 0.0, 0.0); // W
		if (klawisz == KEY_RIGHT)
			kamerka -> moveCamera(0.0, 0.0, -0.1); // A
		if (klawisz == KEY_LEFT)
			kamerka -> moveCamera(0.0, 0.0, 0.1); // D
		if (klawisz == KEY_TAB && zegarek_dla_taba -> passed(250000, MICROSECONDS)) { // używamy dodatkowego zegarka, bo nigdy nie 
																	// przytrzymamy taba tak krótko, żeby się
																	// po prostu raz włączył lub wyłączył
			if (cienie -> isBound(malpka))
				cienie -> unbind(malpka);
			else
				cienie -> bind(malpka);
		}
		
		if (zegarek_dla_animacji -> passed(2500, MICROSECONDS)) {
			table -> rotate(0, 0.1, 0);
			plane -> rotate(0, 0.1, 0);
		}
		
		fps++;
		
		if (zegarek_dla_fps -> passed(1, SECONDS)) {
			cout << "\n	FPS: " << fps;
			fps = 0;
		}
		
		s_main -> execute(); // Skylium::execute() przechwytuje i obsługuje stosowne eventy i renderuje scenę.
		
		s_main -> swapBuffers(); // musi być! Zamieniamy bufory.
		
	}
	
	// musimy wywalić ręcznie wszystko, co utworzyliśmy ręcznie (operatorem new w źródle)
	delete cienie; // wywalamy shadera
	delete zegarek_dla_animacji;
	delete zegarek_dla_fps;
	delete zegarek_dla_taba;

	// całą resztę wywali za nas Skylium.
	delete s_main;

	return 0;
}

