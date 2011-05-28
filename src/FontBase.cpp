/*
    FontBase.cpp
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

#include <iostream>

#include "../include/FontBase.h"

#include "../include/defines.h"

using namespace std;

FontBase::FontBase(const string &_fontName, const int &_characters) :
		__font(0),
		__base(0),
		__characters(_characters) {
	Display *dpy; // ekran X
	XFontStruct *fontInfo; // info o nowej czcionce
	
	/* Miejsce na 96 znaków */
	__base = glGenLists(__characters);
	dpy = XOpenDisplay(NULL);
	
	fontInfo = XLoadQueryFont(dpy, _fontName.c_str());
	
	if (fontInfo == NULL) { // nie udało się
#ifdef __DEBUG__
		cout << LOG_WARN << "Nie znaleziono czcionki: " << _fontName << ".";
#endif
		fontInfo = XLoadQueryFont(dpy, "fixed"); // coś musimy mieć
		if (fontInfo != NULL) {
#ifdef __DEBUG__
			cout << LOG_WARN << "Załadowano: \"fixed\".";
#endif
		} else throw;
	} else {
#ifdef __DEBUG__
		cout << LOG_INFO << "Załadowano czcionkę: " << _fontName << ".";
		cout.flush();
#endif
	}
	
	__font = fontInfo -> fid;
	glXUseXFont(__font, 32, _characters, __base);
	
	GLenum err = glGetError(); // pobieramy błędy
	while (err != GL_NO_ERROR) {
		cout << err << endl;
		cout.flush();
		err = glGetError();
	}
	
	XFreeFont(dpy, fontInfo); // zwalniamy niepotrzebną pamięć
	
	XCloseDisplay(dpy); // zamykamy tymczasowy ekran
}

FontBase::FontBase(const FontBase &_orig) :
		__font(_orig.__font),
		__base(_orig.__base),
		__characters(_orig.__characters) {}

FontBase::~FontBase() {
#ifdef __DEBUG__
	cout << LOG_INFO << "Desktruktor: ~FontBase()";
#endif
	glDeleteLists(__base, __characters);
}

void
FontBase::print(const GLfloat &_x, const GLfloat &_y, const string &_text) {
	glPushMatrix();
		glPushAttrib(GL_LIST_BIT);
			glListBase(__base - 32);
			
			const char *text = _text.c_str();
			
			glRasterPos2f(_x, _y);
			glColor4f(0.0, 0.0, 0.0, 1.0);
			
			glCallLists(_text.length(), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	glPopMatrix();
}