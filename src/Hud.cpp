/* 
 * Hud.cpp
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


#include "../include/Hud.h"

Hud::Hud() :
	__hudFont(NULL),
	__coord1(0, 0),
	__coord2(0, 0),
	__alpha(0)
		{}
		
Hud::Hud(const sFont *_hudFont, const sSingleCoord &_coord1, const sSingleCoord &_coord2, const GLfloat &_alpha) :
	__hudFont(_hudFont),
	__coord1(_coord1),
	__coord2(_coord2),
	__alpha(_alpha)
		{}
		
void
Hud::drawBorder() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(__coord1.x, __coord2.x, __coord1.y, __coord2.y, -100.0f, 100.0f);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glColor4f(1.0, 0.0, 0.0, __alpha);
			glBegin(GL_QUADS);
				glVertex2f(__coord1.x, __coord1.y);
				glVertex2f(__coord1.x, __coord2.y);
				glVertex2f(__coord2.x, __coord2.y);
				glVertex2f(__coord2.x, __coord1.y);
			glEnd();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

}