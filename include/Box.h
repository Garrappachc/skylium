/*
    Box.h
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


#ifndef BOX_H
#define BOX_H

#include <iostream>

#include "Object.h"
#include "Skylium.h"

class Box : Object {

public:
	
	Box(const std::string& = "");
	
	virtual ~Box();
	
	void show();
	
	bool loadFromObj();
	
	void loadIntoVBO();



};

#endif // BOX_H
