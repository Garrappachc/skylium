/*
    Hud.cpp
    Copyright (C) 2011  Michał Garapich garrappachc@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "../include/Hud.h"

#include "../include/defines.h"

using namespace std;

Hud::Hud(const FontBase *_font) : 
		__instance(Skylium::GetSingletonPtr()),
		__font(_font) {
#ifdef __DEBUG__
	cout << LOG_INFO << "Konstruktor: Hud()";
#endif
}

Hud::~Hud() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Desktruktor: ~Hud()";
#endif
}

void
Hud::draw() {
	// włączamy Hud Mode
	hudMode(true);
	
	glColor4f(0.6f, 0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
		glVertex2f(0.2, 0.0);
		glVertex2f(0.2, 0.4);
		glVertex2f(0.8, 0.4);
		glVertex2f(0.8, 0.0);
	glEnd();
	glColor4f(0.6f, 0.2f, 0.2f, 0.9f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.2, 0.0);
		glVertex2f(0.2, 0.4);
		glVertex2f(0.8, 0.4);
		glVertex2f(0.8, 0.0);
	glEnd();
	
	hudMode(false);
}

void
Hud::hudMode(bool flag) {
	if (flag) {
		int w, h;
		__instance -> getWindowSize(w, h);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, w, 0, h);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
	} else {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}