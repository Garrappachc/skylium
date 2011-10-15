/*
     __  __      _           _      _   _    
    |  \/  |__ _| |_ ___ _ _(_)__ _| | | |_  
    | |\/| / _` |  _/ -_) '_| / _` | |_| ' \ 
    |_|  |_\__,_|\__\___|_| |_\__,_|_(_)_||_|
                       
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


#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>

#include "Vectors.h"

class Texture;

enum {
	MATERIAL_AMBIENT	= 1,
	MATERIAL_DIFFUSE	= 2,
	MATERIAL_SPECULAR	= 4
};

class Material {
	
public:
	
	/**
	 * Konstruktor, który przyjmuje jako argument swoją nazwę.
	 * Reszta pusta.
	 * @param name Nazwa materiału.
	 */
	Material(const std::string& = "");
	
	/**
	 * Konstruktor kopiujący.
	 */
	Material(const Material&);
	
	/**
	 * Destruktor usuwa wszystkie przypisane do niego tekstury.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
	 */
	virtual ~Material();
	
	/**
	 * Ustawia podany parametr materiału.
	 * @param param Wartość parametru.
	 * @param type Typ materiału, którego dotyczy wartość podana w argumencie
	 * <i>param</i>. I tak:
	 *		&bump; MATERIAL_AMBIENT - wartość mAmbient;
	 *		&bump; MATERIAL_DIFFUSE - wartość mDiffuse;
	 *		&bump; MATERIAL_SPECULAR - wartość mSpecular.
	 */
	void loadMaterial(const sColor&, unsigned);
	
	/**
	 * Ustawia parametr mAlpha_.
	 * @param alpha Przezroczystość ([0; 1]).
	 */
	void loadAlpha(GLfloat);
	
	/**
	 * Ustawia odbicie.
	 * @param shininess Odbijanie materiału.
	 */
	void loadShininess(GLint);
	
	/**
	 * Sprawdza, czy materiał posiada jakąś teksturę.
	 * @return True, jeżeli ma jakąkolwiek.
	 */
	bool hasAnyTexture() { return __textures.size() > 0; }
	
	/**
	 * Ustawia parametry tekstury do renderowania.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glBindTexture.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml
	 */
	void setTextures();
	
	/**
	 * Ustawia parametry materiału do renderowania.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glMaterial.xml
	 */
	void setMaterial();
	
	/**
	 * Dodaje do wektora __textures wskaźnik na teksturę, który
	 * został podany jako argument.
	 * @param _tex Wskaźnik na teksturę.
	 */
	void appendTexture(Texture*);
	
	/* Nazwa materiału. */
	std::string name;
	
private:
	
	/* Materiał */
	sColor			__mAmbient;
	sColor			__mDiffuse;
	sColor			__mSpecular;
	GLfloat			__mAlpha;
	GLint				__mShininess;
	
	/* Tekstury */
	std::vector< Texture* >				__textures;
	std::vector< Texture* >::const_iterator	__texturesIterator;

	
};

#endif // MATERIAL_H
