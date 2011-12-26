/*
     ___             ___                  _    
    | __|_ _ __ ___ / __|___ _ __  _ __  | |_  
    | _/ _` / _/ -_) (__/ _ \ '  \| '_ \_| ' \ 
    |_|\__,_\__\___|\___\___/_|_|_| .__(_)_||_|
                                  |_|   
    
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


#ifndef FACECOMP_H
#define FACECOMP_H

/* For std::map */
struct Face {
	long v;
	long t;
	long n;
	
	Face();
	Face(const long&, const long&, const long&);
};

typedef struct FaceComp {
	
	bool operator()(const Face&, const Face&) const;
	
} FaceComp;

#endif // FACECOMP_H
