/* 
 * Camera.h
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

#ifndef CAMERA_H
#define	CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "Vectors.h"

class Camera {

public:
	Camera();
	Camera(const GLdouble&, const GLdouble&, const GLdouble&);
	
	virtual ~Camera();
	
	/**
	 * GL_PROJECTION;
	 * gluPerspective.
	 * Tylko w razie zmiany parametrów.
	 * http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	 * http://www.felixgers.de/teaching/jogl/gluPerspective.gif
	 */
	void setProjection();

	/**
	 * GL_MODELVIEW;
	 * gluLookAt.
	 * Tylko w razie zmiany parametrów.
	 * http://pyopengl.sourceforge.net/documentation/manual/gluLookAt.3G.html
	 * http://www.toldo.info/roberto/LaboratorioGrafica/Slides/images/glulookat.gif
	 */
	void setView();

	/**
	 * Przesuwa kamerę o podane wartości, w trybie 2D.
     * @param movX oś X;
     * @param movY oś Y;
     * @param movZ oś Z.
	 */
	void moveCamera(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Obraca kamerę w jednym miejscu - obsługa myszy.
     * @param x;
     * @param y;
     * @param z.
	 */
	void rotateCamera(const GLdouble&, const GLdouble&, const GLdouble&);

	/**
	 * Ustawia punkt, na który patrzy się kamera.
	 * Jako argument przyjmuje albo trzy GLdouble, które definiują punkt (x, y, z),
	 * albo tablicę GLdouble (p[0], p[1], p[2]).
     * @param x Pozycja X.
     * @param y Pozycja Y.
     * @param z Pozycja Z.
	 */
	void lookAt(const GLdouble&, const GLdouble&, const GLdouble&);
	
	/**
	 * Ustawia kamerę w pozycji początkowej.
	 */
	void resetCameraPosition();


private:
	/*** setProjection ***/
	GLdouble __fovy;
	GLdouble __zNear;
	GLdouble __zFar;


	/*** setView ***/
	
	/* Punkt, w którym znajduje się kamera */
	sVector __eye;

	/* Punkt, na który patrzy się kamera */
	sVector __center;

	/* Wektor zmiany kierunku patrzenia kamery */
	sVector __up;
	/* Wartości, którymi będziemy zmieniać położenie kamery; */
	sVector __mov;

	/*** Inne ***/
	int __windowWidth;
	int __windowHeight;
};

#endif	/* CAMERA_H */

