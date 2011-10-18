/*
	 _                   _      __  _    
	| |_____ _  _ ___ __| |___ / _|| |_  
	| / / -_) || (_-</ _` / -_)  _|| ' \ 
	|_\_\___|\_, /__/\__,_\___|_|(_)_||_|
	         |__/                                
             
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

#ifndef KEYSDEF_H
#define KEYSDEF_H

/* Obsługa klawiszy */
typedef enum {
	KEY_NOKEY = 0,
	KEY_ESC,
	KEY_UP,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_TAB,
	KEY_X,
	KEY_Z,
	KEY_F1,
	KEY_F2,
	KEY_BACKQUOTE
} sKey;

#endif