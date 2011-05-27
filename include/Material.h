/*
    Material.h
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

#include "Vectors.h"

enum {
	MATERIAL_AMBIENT	= 1,
	MATERIAL_DIFFUSE	= 2,
	MATERIAL_SPECULAR	= 4
};

enum {
	TEXTURE_AMBIENT	= 1,
	TEXTURE_DIFFUSE	= 2,
	TEXTURE_SPECULAR	= 4,
	TEXTURE_ALPHA		= 8
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
	 * Destruktor usuwa wszystkie przypisane do niego tekstury.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
	 */
	virtual ~Material();
	
	/**
	 * Ładuje teksturę z pliku.
	 * @param fileName Nazwa pliku z teksturą.
	 * @param type Typ tekstury, który ma być załadowany. I tak:
	 *		&bump; TEXTURE_AMBIENT - ładuje tAmbient;
	 *		&bump; TEXTURE_DIFFUSE - ładuje tDiffuse;
	 *		&bump; TEXTURE_SPECULAR - ładuje tSpecular;
	 *		&bump; TEXUTRE_ALPHA - ładuje tAlpha;
	 * @return False, jeżeli coś poszło nie tak.
	 */
	bool loadTexture(const std::string&, const unsigned&);
	
	/**
	 * Ustawia podany parametr materiału.
	 * @param param Wartość parametru.
	 * @param type Typ materiału, którego dotyczy wartość podana w argumencie
	 * <i>param</i>. I tak:
	 *		&bump; MATERIAL_AMBIENT - wartość mAmbient;
	 *		&bump; MATERIAL_DIFFUSE - wartość mDiffuse;
	 *		&bump; MATERIAL_SPECULAR - wartość mSpecular.
	 */
	void loadMaterial(const sColor&, const unsigned&);
	
	/**
	 * Ustawia parametr mAlpha_.
	 * @param alpha Przezroczystość ([0; 1]).
	 */
	void loadAlpha(const GLfloat&);
	
	/**
	 * Ustawia odbicie.
	 * @param shininess Odbijanie materiału.
	 */
	void loadShininess(const GLint&);
	
	/**
	 * Sprawdza, czy materiał posiada jakąś teksturę.
	 * @return True, jeżeli ma jakąkolwiek.
	 */
	bool hasAnyTexture();
	
	/**
	 * Ustawia parametry tekstury do renderowania.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glBindTexture.xml
	 * http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml
	 */
	void setTexture();
	
	/**
	 * Ustawia parametry materiału do renderowania.
	 * http://www.opengl.org/sdk/docs/man/xhtml/glMaterial.xml
	 */
	void setMaterial();
	
	/**
	 * Pozwala modyfikować zmienną __wrapping;
	 * @param wrapping Enum odpowiadający pożądanemu zaginaniu tekstury.
	 */
	void setWrapping(const GLenum&);
	
	/* Nazwa materiału. */
	std::string name;
	
private:
	
	/* Materiał */
	sColor		__mAmbient;
	sColor		__mDiffuse;
	sColor		__mSpecular;
	GLfloat		__mAlpha;
	GLint		__mShininess;
	
	/* Tekstury */
	GLuint		__tAmbient;
	GLuint		__tDiffuse;
	GLuint		__tSpecular;
	GLuint		__tAlpha;
	
	GLenum		__wrapping;
	
	
};

#endif // MATERIAL_H
