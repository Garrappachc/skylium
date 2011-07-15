/*
          _   _ _                   
     _  _| |_(_) |___  __ _ __ _ __ 
    | || |  _| | (_-<_/ _| '_ \ '_ \
     \_,_|\__|_|_/__(_)__| .__/ .__/
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


#include "../include/utils.h"
#include "../include/config.h"

using namespace std;

void explode(const std::string &_text, char _delim, std::vector< std::string > &_dest) {
	_dest.clear();
	string temp = "";
	
	for (unsigned i = 0; i < _text.length(); i++) {
		if (_text[i] == _delim) {
			_dest.push_back(temp);
			temp = "";
		} else {
			temp += _text[i];
		}
	}
	if (temp != "")
		_dest.push_back(temp);
}