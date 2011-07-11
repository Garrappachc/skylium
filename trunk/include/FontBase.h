/*
     ___        _   ___                 _    
    | __|__ _ _| |_| _ ) __ _ ___ ___  | |_  
    | _/ _ \ ' \  _| _ \/ _` (_-</ -_)_| ' \ 
    |_|\___/_||_\__|___/\__,_/__/\___(_)_||_|                                     

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


#ifndef FONTBASE_H
#define FONTBASE_H

#include <string>

#include <GL/gl.h>
#include <GL/glx.h>

class FontBase {
	
public:
	
	FontBase(const std::string& = "-*-courier-bold-r-normal--14-*-*-*-*-*-*-*", const int& = 96);
	
	FontBase(const FontBase&);
	
	virtual ~FontBase();
	
	void print(const GLfloat&, const GLfloat&, const std::string&) const;
	
private:
	
	Font __font;
	
	GLuint __base;
	
	int __characters;
	
	
};

#endif // FONTBASE_H
