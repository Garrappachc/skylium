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
	
	KEY_Esc, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9,
		KEY_F10, KEY_F11, KEY_F12,
	
	KEY_backquote, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
	
	KEY_Tab, KEY_q, KEY_w, KEY_e, KEY_r, KEY_t, KEY_y, KEY_u, KEY_i, KEY_o, KEY_p,
	KEY_a, KEY_s, KEY_d, KEY_f, KEY_g, KEY_h, KEY_j, KEY_k, KEY_l,
	KEY_z, KEY_x, KEY_c, KEY_v, KEY_b, KEY_n, KEY_m,
															 KEY_UP,
				KEY_Space,							KEY_LEFT, KEY_DOWN, KEY_RIGHT,
} sKey;

#endif