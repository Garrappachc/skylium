/*
     _____        _                __  __                              _    
    |_   _|____ _| |_ _  _ _ _ ___|  \/  |__ _ _ _  __ _ __ _ ___ _ _ | |_  
      | |/ -_) \ /  _| || | '_/ -_) |\/| / _` | ' \/ _` / _` / -_) '_|| ' \ 
      |_|\___/_\_\\__|\_,_|_| \___|_|  |_\__,_|_||_\__,_\__, \___|_|(_)_||_|
                                                        |___/          
                                                        
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


#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <vector>

#include "Singleton.h"

class Texture;

class TextureManager : public Singleton< TextureManager > {

public:
	
	/**
	 * Konstruktor domyślny.
	 */
	TextureManager();
	
	/**
	 * Desktruktor niszczy wszystkie załadowane tekstury.
	 */
	virtual ~TextureManager();

	/**
	 * Pozwala dobrać się do tekstury za pomocą jej nazwy.
	 * @param name Nazwa tekstury, która będzie szukana.
	 * @return Wskaźnik do tej tekstury.
	 */
	Texture * getTextureByName(const std::string&);
	
	/**
	 * Wrzuca teksturę do swojego wektora.
	 * @param texture Wskaźnik na teksturę, która zostanie wrzucona do wektora.
	 */
	void pushBack(Texture*);

private:
	
	/* Wektor tekstur */
	std::vector< Texture* > __textureList;
};

#endif // TEXTUREMANAGER_H
