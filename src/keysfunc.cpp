/*
	 _                __                             
	| |_____ _  _ ___/ _|_  _ _ _  __   __ _ __ _ __ 
	| / / -_) || (_-<  _| || | ' \/ _|_/ _| '_ \ '_ \
	|_\_\___|\_, /__/_|  \_,_|_||_\__(_)__| .__/ .__/
	         |__/                         |_|  |_|   

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

#include <X11/Xutil.h>

#include "../include/keysfunc.h"

/* Oh, man, this is shitty job */
void
doYourJob(const KeySym& _key, sKey& _k) {
	switch (_key) {
		case XK_Escape:
			_k = KEY_Esc;
			break;
		case XK_F1:
			_k = KEY_F1;
			break;
		case XK_F2:
			_k = KEY_F2;
			break;
		case XK_F3:
			_k = KEY_F3;
			break;
		case XK_F4:
			_k = KEY_F4;
			break;
		case XK_F5:
			_k = KEY_F5;
			break;
		case XK_F6:
			_k = KEY_F6;
			break;
		case XK_F7:
			_k = KEY_F7;
			break;
		case XK_F8:
			_k = KEY_F8;
			break;
		case XK_F9:
			_k = KEY_F9;
			break;
		case XK_F10:
			_k = KEY_F10;
			break;
		case XK_F11:
			_k = KEY_F11;
			break;
		case XK_F12:
			_k = KEY_F12;
			break;
		
		case XK_quoteleft:
			_k = KEY_backquote;
			break;
		case XK_1:
			_k = KEY_1;
			break;
		case XK_2:
			_k = KEY_2;
			break;
		case XK_3:
			_k = KEY_3;
			break;
		case XK_4:
			_k = KEY_4;
			break;
		case XK_5:
			_k = KEY_5;
			break;
		case XK_6:
			_k = KEY_6;
			break;
		case XK_7:
			_k = KEY_7;
			break;
		case XK_8:
			_k = KEY_8;
			break;
		case XK_9:
			_k = KEY_9;
			break;
		case XK_0:
			_k = KEY_0;
			break;
		
		case XK_q:
			_k = KEY_q;
			break;
		case XK_w:
			_k = KEY_w;
			break;
		case XK_e:
			_k = KEY_e;
			break;
		case XK_r:
			_k = KEY_r;
			break;
		case XK_t:
			_k = KEY_t;
			break;
		case XK_y:
			_k = KEY_y;
			break;
		case XK_u:
			_k = KEY_u;
			break;
		case XK_i:
			_k = KEY_i;
			break;
		case XK_o:
			_k = KEY_o;
			break;
		case XK_p:
			_k = KEY_p;
			break;
		case XK_a:
			_k = KEY_a;
			break;
		case XK_s:
			_k = KEY_s;
			break;
		case XK_d:
			_k = KEY_d;
			break;
		case XK_f:
			_k = KEY_f;
			break;
		case XK_g:
			_k = KEY_g;
			break;
		case XK_h:
			_k = KEY_h;
			break;
		case XK_j:
			_k = KEY_j;
			break;
		case XK_k:
			_k = KEY_k;
			break;
		case XK_l:
			_k = KEY_l;
			break;
		case XK_z:
			_k = KEY_z;
			break;
		case XK_x:
			_k = KEY_x;
			break;
		case XK_c:
			_k = KEY_c;
			break;
		case XK_v:
			_k = KEY_v;
			break;
		case XK_b:
			_k = KEY_b;
			break;
		case XK_n:
			_k = KEY_n;
			break;
		case XK_m:
			_k = KEY_m;
			break;
		
		case XK_Tab:
			_k = KEY_Tab;
			break;
		case XK_bar:
			_k = KEY_Space;
			break;
		
		case XK_Left:
			_k = KEY_LEFT;
			break;
		case XK_Right:
			_k = KEY_RIGHT;
			break;
		case XK_Up:
			_k = KEY_UP;
			break;
		case XK_Down:
			_k = KEY_DOWN;
			break;
	}
}