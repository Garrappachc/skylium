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

void checkGLErrors(const string &_at) {
#ifdef __DEBUG__
	bool iserror = false;
	GLenum err = glGetError(); // pobieramy błędy
	while (err != GL_NO_ERROR) {
		if ((sGlobalConfig::DEBUGGING & D_WARNINGS) == D_WARNINGS) {
			cout << "\n\e[33m(WW)\e[0m " << "OpenGL error!" 
				<< "\nAt: " << _at << "\nError code: " << err
				<< "\nError message: " << getErrorString(err);
			cout.flush();
		}
		err = glGetError();
		iserror = true;
	}
	if (iserror)
		sleep(1);
#endif
}

string getErrorString(GLenum _err) {
	switch (_err) {
		case 0x0500:
			return "GL_INVALID_ENUM";
		case 0x0501:
			return "GL_INVALID_VALUE";
		case 0x0502:
			return "GL_INVALID_OPERATION";
		case 0x0503:
			return "GL_OUT_OF_MEMORY";
		case 0x0506:
			return "GL_INVALID_FRAMEBUFFER_OPERATION​";
		default:
			return "unknown error code";
	}
}