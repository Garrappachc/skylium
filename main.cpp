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
	/* compilation + link */
	if (!shadow -> make())
		exit(1);
	
	/* create the scene */
	Scene *sScene = s_main -> createScene("SampleScene");

	/* create the object */
	Object *surface = sScene -> createObject("surface_0");
	/* read the vertices from the .obj file */
	if (!surface -> loadFromObj("objects/surface.obj"))
		exit(1);
	/* translate */
	surface -> move(-6, -2.65, -10);
	surface -> scale(4, 4, 4);
	
	/* let's crate the larger surface */
	for (int k = 0; k < 5; k++ ) {
		for (int i = -1; i < 4; i++) {
			ostringstream temp;
			temp << (int)((i+2) + k * 5);
			string str = "surface_" + temp.str();
			Object *surface_new = sScene -> createObject(str, surface);
			surface_new -> move(10*i, 0, 10*k);
		}
	}
	
	/* create second object and load its data from file */
	Object *table = sScene -> createObject("table");
	table -> loadFromObj("objects/table.obj");
	table -> move(0, -2, 0);
	table -> scale (6, 6, 6);
	table -> setColor(0.7f, 0.7f, 0.7f);
	
	Object *monkey = sScene -> createObject("monkey"); // monkey
	if (!monkey -> loadFromObj("objects/monkey.obj"))
		exit(1);
	monkey -> move(0, 7, 0);
	monkey -> scale(3, 3, 3);
	monkey -> rotate(0, -45, 40);
	monkey -> setColor(54, 18, 0);
	
	/* Camera of (0, 4, -20) position, looking at (0, 7, 20) */
	Camera* fppCamera = sScene -> createCamera(0, 4.0, -20, FPP);
	fppCamera -> lookAt(0, 7, 20);
	
	Camera *sphereCamera = sScene -> createCamera(0, 4, -20, SPHERICAL);
	sphereCamera -> lookAt(0, 7, 0);
	
	/* Let's bind our shading shader to the monkey and the table */
	shadow -> bind(monkey);
	shadow -> bind(table);
	
	/* Light on (7, 3, 0) position */
	int light = sScene -> createLight(7, 3, 0);
	/* Set ambient light */
	sScene -> setAmbientLight(light, 0.5, 0.5, 0.5, 1.0);
	/* Switch the light on. By default, all lights are off */
	sScene -> toggleLight();
	
	/* Timer for animation */
	Timer *Tanimation = new Timer();
	/* Timer for fps counting */
	Timer *Tfps = new Timer();
	/* We have to count the time when the key is down to avoid "flashing-like" effect */
	Timer *Ttabulator = new Timer();
	Timer *Thud = new Timer();
	
	/* Let's create new bitmap font */
	FontBase *sFont = new FontBase("-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1");
	
	/* Make the "data package" for Hud. */
	HudData fpsCounter(sXY(-0.75, 0.9), sColor(SCOLORS_WHITE), "", sFont);
	s_main -> TheHud -> attachData(&fpsCounter);
	
	HudData cameraInfo(sXY(-0.75, 0.8), sColor(SCOLORS_WHITE), "Camera: FPP", sFont);
	s_main -> TheHud -> attachData(&cameraInfo);
	
	HudData cameraLookAt(sXY(0.0, 0.9), sColor(SCOLORS_WHITE), "LookAt: ", sFont);
	s_main -> TheHud -> attachData(&cameraLookAt);
	sVector camLookAt;
	
	HudData cameraCenter(sXY(0.0, 0.8), sColor(SCOLORS_WHITE), "Center: ", sFont);
	s_main -> TheHud -> attachData(&cameraCenter);
	sVector camCenter;
	
	s_main -> TheHud -> setColor(sColor(0.0f, 0.0f, 1.0f, 0.5f), sColor(0.0f, 0.0f, 1.0f, 0.8f));
	
	/* FPS counting */
	short fps = 0;
	
	/* Switch on steering the camera with mouse. */
	s_main -> toggleMouseCamera();
	
	/* Capture keys pressed */
	sKey keyPressed;
	while ((keyPressed = s_main -> sEvent()) != KEY_Esc) { // to make exit possible
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
		if (keyPressed == KEY_Tab && Ttabulator -> passed(250000, MICROSECONDS)) {
			if (shadow -> isBound(monkey))
				shadow -> unbind(monkey);
			else
				shadow -> bind(monkey);
		}
		
		if (Tanimation -> passed(2500, MICROSECONDS)) {
			table -> rotate(0, 0.1, 0);
			monkey -> rotate(0, 0.1, 0);
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
		
		s_main -> execute(); // Skylium::execute() captures some events and renders the scene.
		
		s_main -> swapBuffers(); // Must be. Swaps the buffers.
		
	}
	
	/* We have to delete everything that was made on our own. */
	delete Tanimation;
	delete Tfps;
	delete Ttabulator;
	delete Thud;
	delete sFont;
	
	/* Skylium handles the rest. */
	delete s_main;
	
	
	cout << endl;

	return 0;
	
}
