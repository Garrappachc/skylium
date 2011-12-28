/*
     _____        _                __  __                                      
    |_   _|____ _| |_ _  _ _ _ ___|  \/  |__ _ _ _  __ _ __ _ ___ _ _  __ _ __ _ __ 
      | |/ -_) \ /  _| || | '_/ -_) |\/| / _` | ' \/ _` / _` / -_) '_|/ _| '_ \ '_ \
      |_|\___/_\_\\__|\_,_|_| \___|_|  |_\__,_|_||_\__,_\__, \___|_|(_)__| .__/ .__/
                                                        |___/            |_|  |_|   
      

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


#include "../include/TextureManager.h"
#include "../include/Texture.h"

#include "../include/Skylium.h"

#include "../include/defines.h"
#include "../include/config.h"
#include "../include/utils.h"

using namespace std;

TextureManager::TextureManager() :
		__textureList(0) {
	if ((sGlobalConfig::DEBUGGING & D_CONSTRUCTORS) == D_CONSTRUCTORS)
		cout << LOG_INFO << "TextureManager constructed.";
}

TextureManager::~TextureManager() {
	while (!__textureList.empty())
		delete __textureList.back(), __textureList.pop_back();
	if ((sGlobalConfig::DEBUGGING & D_DESTRUCTORS) == D_DESTRUCTORS)
		cout << LOG_INFO << "TextureManager destructed.";
}

Texture *
TextureManager::getTextureByName(const string &_name) {
	for (unsigned i = 0; i < __textureList.size(); i++)
		if (__textureList[i]->name == _name) return __textureList[i];
	
	return NULL;
}

void
TextureManager::pushBack(Texture *_tex) {
	if (_tex == NULL)
		return;
	__textureList.push_back(_tex);
}