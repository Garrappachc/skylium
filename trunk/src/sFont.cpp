/* 
 * sFont.cpp
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

#include <iostream>

#include "../include/sFont.h"

#include "../include/defines.h"

using namespace std;

sFont::sFont(const string &_fontName, const int &_characters) :
		__font(0),
		__base(0),
		__characters(0) {
	Display *dpy; // ekran X
	XFontStruct *fontInfo; // info o nowej czcionce
	
	__characters = _characters;
	
	/* Miejsce na 96 znaków */
	__base = glGenLists(__characters);
	dpy = XOpenDisplay(NULL);
	
	fontInfo = XLoadQueryFont( dpy, _fontName.c_str() );
	
	if ( fontInfo == NULL ) { // nie udało się
		cout << LOG_WARN << "Nie znaleziono czcionki: " << _fontName << ".";
		fontInfo = XLoadQueryFont( dpy, "fixed" ); // ładujemy fixed.
			if ( fontInfo == NULL )
				cout << LOG_WARN << "Załadowano \"fixed\".";
	} else {
#ifdef __DEBUG__
		cout << LOG_INFO << "Załadowano czcionkę " <<_fontName << ".";
#endif
	}
	
	__font = fontInfo -> fid;
	glXUseXFont(__font, 32, _characters, __base);
	
	XFreeFont(dpy, fontInfo); // zwalniamy niepotrzebną pamięć
	
	XCloseDisplay(dpy); // zamykamy tymczasowy wyświetlacz
}

sFont::sFont(const sFont &_orig) {
	__font = _orig.__font;
	__base = _orig.__base;
	__characters = _orig.__characters;
}

sFont::~sFont() {
	glDeleteLists(__base, __characters);
}

void
sFont::print(const int &_x, const int &_y, const string &_text) {
	glPushMatrix();
	
	glDisable(GL_DEPTH_TEST);
	
	glPushAttrib(GL_LIST_BIT);
	glListBase(__base - 32);
	
	const char *text = new char[_text.length()];
	text = _text.c_str();
	
	glRasterPos2i(_x, _y);
	
	glCallLists(_text.length(), GL_UNSIGNED_BYTE, text);
	
	glPopAttrib();
	
	glEnable(GL_DEPTH_TEST);
	
	glPopMatrix();
	
}

