/*
	File:	main.cpp
	Author:	Michał Garapich
*/

#include <iostream>
#include <sstream>

#include <skylium.h>

using namespace std;

int
main() {
	
	Skylium *s_main = new Skylium();
	
	if (!s_main -> init("Skylium sample")) {
		return 1;
	}
	
	
	Shader *shadow = s_main -> createShader(PHONG_SHADING);
	if (!shadow -> make()) // compilation + linking
		exit(1);
	
	Scene *sScene = s_main -> createScene("SampleScene"); // we create the scene

	Object *surface = sScene -> createObject("surface_0");
	if (!surface -> loadFromObj("objects/surface.obj"))
		exit(1);
	
	surface -> move(-6, -2.65, -10);
	surface -> scale(4, 4, 4);
	
	// we crate the larger surface
	for (int k = 0; k < 5; k++ ) {
		for (int i = -1; i < 4; i++) {
			ostringstream temp;
			temp << (int)((i+2) + k * 5);
			string str = "surface_" + temp.str();
			Object *surface_new = sScene -> createObject(str, surface);
			surface_new -> move(10*i, 0, 10*k);
		}
	}
	
	
	Object *table = sScene -> createObject("table");
	table -> loadFromObj("objects/table.obj");
	table -> move(0, -2, 0);
	table -> scale (6, 6, 6);
	table -> setColor(0.7f, 0.7f, 0.7f);
	
	Object *malpka = sScene -> createObject("monkey"); // monkey
	if (!malpka -> loadFromObj("objects/monkey.obj"))
		exit(1);
	malpka -> move(0, 7, 0);
	malpka -> scale(3, 3, 3);
	malpka -> rotate(0, -45, 40);
	malpka -> setColor(54, 18, 0);
	
	Camera* fppCamera = sScene -> createCamera(0, 4.0, -20, FPP); // kamerka na pozycji (5, 6, 0)
	fppCamera -> lookAt(0, 7, 0); // kamerka skierowana na punkt (0, 7, 0)
	
	Camera *sphereCamera = sScene -> createCamera(0, 4, -20, SPHERICAL);
	sphereCamera -> lookAt(0, 7, 0);
	
	shadow -> bind(malpka); // przyłączmy shadera z cieniowanem do małpki
	shadow -> bind(table); // do stolika
	
	int light = sScene -> createLight(7, 3, 0); // światło na pozycji (6, 6, 0)
	sScene -> setAmbientLight(light, 0.5, 0.5, 0.5, 1.0); // ustawiamy ambient Light.
	sScene -> toggleLight(); // włączamy światło. Domyślnie każde światło jest wyłączone!
	
	Timer *Tanimation = new Timer(); // zegarek dla obracającego się stolika + samolotu
	Timer *Tfps = new Timer(); // liczymy fps'y
	Timer *Ttabulator = new Timer(); // nie chcemy efektu "jarzeniówki" przy przęłączaniu shaderów :)
	Timer *Thud = new Timer();
	
	//Timer matrixTimer;
	
	FontBase *sFont = new FontBase("-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1"); // tworzymy nowego fonta
	
	HudData fpsCounter(sXY(-0.75, 0.9), sColor(SCOLORS_WHITE), "", sFont);  // Tworzymy "paczkę" danych dla Huda -
													// pozycję tekstu, kolor tekstu, tekst (póki co
													// pusty, w pętli renderującej będzie apdejtowany)
													// oraz czcionkę.
	s_main -> TheHud -> attachData(&fpsCounter); // TheHud jest referencją do wskaźnika na instancję Huda, do którego to dołączmy
								// dopiero co utworzoną paczkę.
	
	
	HudData cameraInfo(sXY(-0.75, 0.8), sColor(SCOLORS_WHITE), "Camera: FPP", sFont);
	s_main -> TheHud -> attachData(&cameraInfo);
	
	HudData cameraLookAt(sXY(0.0, 0.9), sColor(SCOLORS_WHITE), "LookAt: ", sFont);
	s_main -> TheHud -> attachData(&cameraLookAt);
	sVector camLookAt;
	
	HudData cameraCenter(sXY(0.0, 0.8), sColor(SCOLORS_WHITE), "Center: ", sFont);
	s_main -> TheHud -> attachData(&cameraCenter);
	sVector camCenter;
	
	s_main -> TheHud -> setColor(sColor(0.0f, 0.0f, 1.0f, 0.5f));
	
	
	short fps = 0; // licznik fps.
	
	s_main -> toggleMouseCamera(); // Ha! Włączamy obracanie kamerą za pomocą myszy
	
	sKey keyPressed; // tutaj przechwytujemy klawisze
	while ((keyPressed = s_main -> sEvent()) != KEY_Esc) { // żeby się dało czymś wyjść
		if (keyPressed == KEY_s)
			sScene -> getActiveCamera() -> moveCamera(0.0, 0.0, -0.1);
		if (keyPressed == KEY_w)
			sScene -> getActiveCamera() -> moveCamera(0.0, 0.0, 0.1);
		if (keyPressed == KEY_d)
			sScene -> getActiveCamera() -> moveCamera(0.1, 0.0, 0.0);
		if (keyPressed == KEY_a)
			sScene -> getActiveCamera() -> moveCamera(-0.1, 0.0, 0.0);
		if (keyPressed == KEY_z)
 			sScene -> getActiveCamera() -> moveCamera(0.0, -0.7, 0.0);
		if (keyPressed == KEY_x)
			sScene -> getActiveCamera() -> moveCamera(0.0, 0.7, 0.0);
		
		if (keyPressed == KEY_q)
			sScene -> getActiveCamera() -> setRange((sScene -> getActiveCamera() -> getRange()) + 0.5);
		if (keyPressed == KEY_e)
			sScene -> getActiveCamera() -> setRange((sScene -> getActiveCamera() -> getRange()) - 0.5);
		
		if (keyPressed == KEY_F1) {
			sScene -> setActiveCamera(fppCamera);
			cameraInfo.text = "Camera: FPP";
		}
		if (keyPressed == KEY_F2) {
			sScene -> setActiveCamera(sphereCamera);
			cameraInfo.text = "Camera: SPHERICAL";
		}
		if (keyPressed == KEY_backquote && Thud -> passed(250000, MICROSECONDS))
			s_main -> TheHud -> toggle();
		if (keyPressed == KEY_Tab && Ttabulator -> passed(250000, MICROSECONDS)) { // używamy dodatkowego zegarka, bo nigdy nie 
																	// przytrzymamy taba tak krótko, żeby się
																	// po prostu raz włączył lub wyłączył
			if (shadow -> isBound(malpka))
				shadow -> unbind(malpka);
			else
				shadow -> bind(malpka);
		}
		
		if (Tanimation -> passed(2500, MICROSECONDS)) {
			table -> rotate(0, 0.1, 0);
			malpka -> rotate(0, 0.1, 0);
		}
		
		fps++;
		
		if (Tfps -> passed(1, SECONDS)) {
			fpsCounter.text = "FPS: " + T2String(fps) + " (approximately)";
			fps = 0;
		}
		
		camLookAt = sScene -> getActiveCamera() -> getCenter();
		cameraLookAt.text = "LookAt: (" + T2String(camLookAt.x) +
				", " + T2String(camLookAt.y) +
				", " + T2String(camLookAt.z) + ")";
		
		camCenter = sScene -> getActiveCamera() -> getEye();
		cameraCenter.text = "Center: (" + T2String(camCenter.x) +
				", " + T2String(camCenter.y) +
				", " + T2String(camCenter.z) + ")";
				
		//if (matrixTimer.passed(1, SECONDS)) {
		//	cout << "\n" << scenka -> getActiveCamera() -> getModelViewMatrix() << "\n";
		//}
		
		s_main -> execute(); // Skylium::execute() przechwytuje i obsługuje stosowne eventy i renderuje scenę.
		
		s_main -> swapBuffers(); // musi być! Zamieniamy bufory.
		
	}
	
	// musimy wywalić ręcznie wszystko, co utworzyliśmy ręcznie (operatorem new w źródle)
	delete Tanimation;
	delete Tfps;
	delete Ttabulator;
	delete Thud;
	delete sFont;
	
	// całą resztę wywali za nas Skylium.
	delete s_main;
	
	
	cout << endl;

	return 0;
	
}
