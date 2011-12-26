/*
     ___        _   ___                              
    | __|__ _ _| |_| _ ) __ _ ___ ___   __ _ __ _ __ 
    | _/ _ \ ' \  _| _ \/ _` (_-</ -_)_/ _| '_ \ '_ \
    |_|\___/_||_\__|___/\__,_/__/\___(_)__| .__/ .__/
                                          |_|  |_|  
                                          
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
#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

FontBase::FontBase(const string &_fontName, int _characters) :
		__font(0),
		__base(0),
		__characters(_characters) {
	Display *dpy = Skylium::GetSingletonPtr() -> getContextPtr() -> display;
	XFontStruct *fontInfo;
	
	/* Room for 96 signs */
	__base = glGenLists(__characters);
	checkGLErrors(AT);
	
	fontInfo = XLoadQueryFont(dpy, _fontName.c_str());
	
	if (fontInfo == NULL) { // nie udało się
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
			cout << LOG_WARN << "Font could not be found: " << _fontName << ".";
		fontInfo = XLoadQueryFont(dpy, "fixed"); // coś musimy mieć
		if (fontInfo != NULL) {
			if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS)
				cout << LOG_WARN << "Loaded: \"fixed\".";
		} else throw;
	} else {
		if ((sGlobalConfig::DEBUGGING & D_PARAMS) == D_PARAMS)
			cout << LOG_INFO << "Font " << _fontName << " loaded.";
		cout.flush();
	}
	
	__font = fontInfo -> fid;
	glXUseXFont(__font, ' ', _characters, __base);
	
	checkGLErrors(AT);
	
	XFreeFont(dpy, fontInfo); // release unneeded memory
}

FontBase::FontBase(const FontBase &_orig) :
		__font(_orig.__font),
		__base(_orig.__base),
		__characters(_orig.__characters) {}

FontBase::~FontBase() {
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "Font destructed";
	glDeleteLists(__base, __characters);
}

void
FontBase::print(GLfloat _x, GLfloat _y, const string &_text) const {
	glPushAttrib(GL_LIST_BIT);
			glListBase(__base - ' ');
			glRasterPos3f(_x, _y, 0.0f);
			glCallLists(_text.length(), GL_BYTE, _text.c_str());
		glPopAttrib();
}