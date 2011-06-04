/*
	File:	main.cpp
	Author:	Michał Garapich
*/

#include <iostream>
#include <sstream>

#include "include/Camera.h"
#include "include/Light.h"
#include "include/Object.h"
#include "include/Scene.h"
#include "include/Skylium.h"
#include "include/Timer.h"
#include "include/Material.h"
#include "include/FontBase.h"
#include "include/Hud.h"

using namespace std;

int
main() {
	
	Skylium *s_main = new Skylium();
	
	if (!s_main -> init("Skylium")) { // tworzy okno, false - okno, true - fullscreen
		cout << "Błąd przy Skylium::init(). Przerywam.\n\n";
		return 1;
	}
	
	
	Shader *cienie = s_main -> createShader(PHONG_SHADING);
	if (!cienie -> make()) // kompilacja + linkowanie
		exit(1);
	
	Scene *scenka = s_main -> createScene("Scenka"); // tworzymy scenę

	Object *surface = scenka -> createObject("surface1");
	if (!surface -> loadFromObj("objects/surface.obj", GET_VERTICES | GET_NORMALS | GET_TEXTURE | GET_MATERIAL))
		exit(1);
	
	surface -> move(-6, -2.65, -10);
	surface -> scale(2, 2, 2);
	
	// we crate the larger surface
	for (int k = 0; k < 5; k++ ) {
		for (int i = -1; i < 4; i++) {
			ostringstream temp;
			temp << i;
			string str = "surface_" + temp.str();
			Object *surface_new = scenka -> createObject(str, surface);
			surface_new -> move(6*i, 0, 6*k);
		}
	}
	
	
	Object *table = scenka -> createObject("table");
	table -> loadFromObj("objects/table.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL | GET_TEXTURE); // akurat stolik jest źle wymodelowany - nie przejmować się nim
	table -> move(0, -2, 0);
	table -> scale (6, 6, 6);
	table -> setColor(0.7f, 0.7f, 0.7f);
	
	Object *malpka = scenka -> createObject("malpka"); // małpka
	if (!malpka -> loadFromObj("objects/monkey.obj", GET_VERTICES | GET_NORMALS | GET_MATERIAL)) // znowu .obj
		exit(1); // nie chcemy brzydali na ekranie
	malpka -> move(0, 7, 0);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(0, -45, 40);
	malpka -> setColor(54, 18, 0);
	
	Camera* kamerka_fpp = scenka -> createCamera(0, 4.0, -20, FPP); // kamerka na pozycji (5, 6, 0)
	kamerka_fpp -> lookAt(0, 7, 0); // kamerka skierowana na punkt (0, 7, 0)
	
	Camera *kamerka_kula = scenka -> createCamera(0, 4, -20, SPHERICAL);
	kamerka_kula -> lookAt(0, 7, 0);
	
	cienie -> bind(malpka); // przyłączmy shadera z cieniowanem do małpki
	cienie -> bind(table); // do stolika
	
	int swiatelko = scenka -> createLight(7, 3, 0); // światło na pozycji (6, 6, 0)
	scenka -> setAmbientLight(swiatelko, 0.5, 0.5, 0.5, 1.0); // ustawiamy ambient Light.
	scenka -> toggleLight(); // włączamy światło. Domyślnie każde światło jest wyłączone!
	
	Timer *zegarek_dla_animacji = new Timer(); // zegarek dla obracającego się stolika + samolotu
	Timer *zegarek_dla_fps = new Timer(); // liczymy fps'y
	Timer *zegarek_dla_taba = new Timer(); // nie chcemy efektu "jarzeniówki" przy przęłączaniu shaderów :)
	
	//FontBase *foncik = new FontBase();
	
	//Hud *hudzik = new Hud(foncik);
	
	short fps = 0; // licznik fps.
	
	s_main -> toggleMouseCamera(); // Ha! Włączamy obracanie kamerą za pomocą myszy
	
	sKey klawisz; // tutaj przechwytujemy klawisze
	while ((klawisz = s_main -> sEvent()) != KEY_ESC) { // żeby się dało czymś wyjść
		if (klawisz == KEY_DOWN)
			scenka -> getActiveCamera() -> moveCamera(0.0, 0.0, -0.1); // KEY_DOWN to nie kursor w dół, tylko S
		if (klawisz == KEY_UP)
			scenka -> getActiveCamera() -> moveCamera(0.0, 0.0, 0.1); // W
		if (klawisz == KEY_RIGHT)
			scenka -> getActiveCamera() -> moveCamera(0.1, 0.0, 0.0); // A
		if (klawisz == KEY_LEFT)
			scenka -> getActiveCamera() -> moveCamera(-0.1, 0.0, 0.0); // D
		if (klawisz == KEY_Z)
			scenka -> getActiveCamera() -> moveCamera(0.0, -0.5, 0.0);
		if (klawisz == KEY_X)
			scenka -> getActiveCamera() -> moveCamera(0.0, 0.5, 0.0);
		if (klawisz == KEY_F1)
			scenka -> setActiveCamera(kamerka_fpp);
		if (klawisz == KEY_F2)
			scenka -> setActiveCamera(kamerka_kula);
		if (klawisz == KEY_TAB && zegarek_dla_taba -> passed(250000, MICROSECONDS)) { // używamy dodatkowego zegarka, bo nigdy nie 
																	// przytrzymamy taba tak krótko, żeby się
																	// po prostu raz włączył lub wyłączył
			if (cienie -> isBound(malpka))
				cienie -> unbind(malpka);
			else
				cienie -> bind(malpka);
			
			if (cienie -> isBound(table))
				cienie -> unbind(table);
			else
				cienie -> bind(table);
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
		
		//hudzik -> draw();
		
		s_main -> swapBuffers(); // musi być! Zamieniamy bufory.
		
	}
	
	// musimy wywalić ręcznie wszystko, co utworzyliśmy ręcznie (operatorem new w źródle)
	delete zegarek_dla_animacji;
	delete zegarek_dla_fps;
	delete zegarek_dla_taba;
	
	// całą resztę wywali za nas Skylium.
	delete s_main;

	return 0;
	
}
