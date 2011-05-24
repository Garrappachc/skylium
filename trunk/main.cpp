/*
	File:	main.cpp
	Author:	Michał Garapich
*/

#include <iostream>

#include "include/Camera.h"
#include "include/Light.h"
#include "include/Object.h"
#include "include/Scene.h"
#include "include/Shader.h"
#include "include/Skylium.h"
#include "include/Timer.h"

using namespace std;

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
	
	Object *table = scenka -> createObject("table");
	table -> loadFromObj("objects/table.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL | GET_TEXTURE); // akurat stolik jest źle wymodelowany - nie przejmować się nim
	table -> move(0, -2, -10);
	table -> scale (6, 6, 6);
	table -> setColor(240, 240, 255, 1);
	table -> loadIntoVBO();
	
	Object *malpka = scenka -> createObject("malpka"); // małpka
	if (!malpka -> loadFromObj("objects/monkey.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL)) // znowu .obj
		exit(1); // nie chcemy brzydali na ekranie
	malpka -> move(0, 7, -10);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(0, -45, 40);
	malpka -> setColor(136, 47, 0);
	malpka -> loadIntoVBO();
	
	Camera* kamerka = scenka -> createCamera(0.0, 4.0, 10.0); // kamerka na pozycji (5, 6, 0)
	kamerka -> lookAt(0, 3, -10); // kamerka skierowana na punkt (1, 4, -1)
	
	cienie -> bind(malpka); // przyłączmy shadera z cieniowanem do małpki
	cienie -> bind(table); // do stolika
	
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
			kamerka -> moveCamera(0.0, 0.0, -0.1); // KEY_DOWN to nie kursor w dół, tylko S
		if (klawisz == KEY_UP)
			kamerka -> moveCamera(0.0, 0.0, 0.1); // W
		if (klawisz == KEY_RIGHT)
			kamerka -> moveCamera(0.1, 0.0, 0.0); // A
		if (klawisz == KEY_LEFT)
			kamerka -> moveCamera(-0.1, 0.0, 0.0); // D
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
			malpka -> rotate(0, 0.1, 0);
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
