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
#include "../include/Skylium.h"
#include "../include/FontBase.h"
#include "../include/HudData.h"

#include "../include/defines.h"
#include "../include/config.h"

using namespace std;

Hud::Hud() :
		__visible(false),
		__toDisplay(0) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "Konstruktor: Hud()";
}

Hud::~Hud() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Destruktor: ~Hud()";
}

void
Hud::draw() {
	// włączamy Hud Mode
	__hudMode(true);
	
	glColor4f(1.0f, 0.0f, 0.0f, 0.6f);
	glBegin(GL_QUADS);
		glVertex2f(-0.8, 1.0);
		glVertex2f(-0.8, 0.6);
		glVertex2f(0.8, 0.6);
		glVertex2f(0.8, 1.0);
	glEnd();
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-0.8, 1.0);
		glVertex2f(-0.8, 0.6);
		glVertex2f(0.8, 0.6);
		glVertex2f(0.8, 1.0);
	glEnd();
	
	__displayList = __toDisplay.begin();
	while (__displayList != __toDisplay.end()) {
		glColor4f(
				(*__displayList) -> color.r,
				(*__displayList) -> color.g,
				(*__displayList) -> color.b,
				(*__displayList) -> color.a
			);
		(*__displayList) -> font -> print(
				(*__displayList) -> position.x,
				(*__displayList) -> position.y,
				(*__displayList) -> text
		);
		__displayList++;
	}
	
	__hudMode(false);
}

void
Hud::attachData(HudData *_newdata) {
	__toDisplay.push_back(_newdata);
	if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
		cout << LOG_INFO << "Hud: dołączono (\"" << _newdata -> text << "\")";
}

void
Hud::__hudMode(bool flag) {
	if (flag) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
	} else {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
	}
}

