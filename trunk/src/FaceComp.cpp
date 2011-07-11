/*
     ___             ___                               
    | __|_ _ __ ___ / __|___ _ __  _ __   __ _ __ _ __ 
    | _/ _` / _/ -_) (__/ _ \ '  \| '_ \_/ _| '_ \ '_ \
    |_|\__,_\__\___|\___\___/_|_|_| .__(_)__| .__/ .__/
                                  |_|       |_|  |_|  
                                  
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


#include "../include/FaceComp.h"

Face::Face() :
		v(0),
		t(0),
		n(0) {}

Face::Face(const long &_v, const long &_t, const long &_n) :
		v(_v),
		t(_t),
		n(_n) {}

bool
FaceComp::operator ()(const Face &_a, const Face &_b) const {
	if (_a.v < _b.v) return true;
	else if (_a.v == _b.v ) {
		if (_a.t < _b.t) return true;
		else if (_a.t == _b.t)
			return _a.n < _b.n;
		return false;
	}
	return false;
}