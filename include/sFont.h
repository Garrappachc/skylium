/* 
 * sFont.h
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
#ifndef SFONT_H
#define	SFONT_H

#include <string>

#include <GL/gl.h>
#include <GL/glx.h>

enum {
	
};

class sFont {
public:
	
	explicit sFont(const std::string& = "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1", const int& = 96);
	
	explicit sFont(const sFont&);
	
	virtual ~sFont();
	
	void print(const int&, const int&, const std::string&);
	
private:
	
	/* Nasz font */
	Font __font;
	
	GLuint __base;
	
	int __characters;

};

#endif	/* SFONT_H */

