/* 
 * Material.h
 * Copyright (C) 2011 Michał Garapich
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#ifndef MATERIAL_H
#define	MATERIAL_H

#include <iostream>

#include "Vectors.h"

enum {
	MATERIAL_AMBIENT = 1,
	MATERIAL_DIFFUSE = 2,
	MATERIAL_SPECULAR = 4
};

enum {
	TEXTURE_AMBIENT = 1,
	TEXTURE_DIFFUSE = 2,
	TEXTURE_SPECULAR = 4,
	TEXTURE_ALPHA = 8,
	TEXTURE_BUMP = 16
};

class Material {
	
public:
	/**
	 * Konstruktor domyślny, wszystko puste.
     */
	Material();
	
	/**
	 * Konstruktor domyślny, który przyjmuje jako argument swoją nazwę.
     * @param name Nazwa materiału.
     */
	Material(const std::string&);
	
	/**
	 * Destruktor usuwa wszystkie przypisane do niego tekstury.
     */
	~Material();
	
	/**
	 * Ładuje teksturę z pliku.
     * @param fileName Nazwa pliku z teksturą.
     * @param type Typ tekstury, który ma być załadowany. I tak:
	 *		&bump; TEXTURE_AMBIENT - ładuje tAmbient_;
	 *		&bump; TEXTURE_DIFFUSE - ładuje tDiffuse_;
	 *		&bump; TEXTURE_SPECULAR - ładuje tSpecular_;
	 *		&bump; TEXUTRE_ALPHA - ładuje tAlpha_;
	 *		&bump; TEXTURE_BUMP - ładuje tBump_.
	 * Domyślna wartość to TEXTURE_BUMP.
     * @return False, jeżeli coś poszło nie tak.
     */
	bool loadTexture(const std::string&, const unsigned int&);
	
	/**
	 * Ustawia podany parametr materiału.
     * @param param Wartość parametru.
     * @param type Typ materiału, którego dotyczy wartość podana w argumencie
	 * <i>param</i>. I tak:
	 *		&bump; MATERIAL_AMBIENT - wartość mAmbient_;
	 *		&bump; MATERIAL_DIFFUSE - wartość mDiffuse_;
	 *		&bump; MATERIAL_SPECULAR - wartość mSpecular_.
     */
	void loadMaterial(const sColor&, const unsigned int&);
	
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
     */
	void setTexture();
	
	/**
	 * Ustawia parametry materiału do renderowania.
     */
	void setMaterial();
	
	/* Nazwa materiału. */
	std::string name;
	
private:
	/* Materiał */
	sColor mAmbient_;
	sColor mDiffuse_;
	sColor mSpecular_;
	GLfloat mAlpha_;
	GLint mShininess_;
	
	/* Tekstury */
	GLuint tAmbient_;
	GLuint tDiffuse_;
	GLuint tSpecular_;
	GLuint tAlpha_;
	GLuint tBump_;

};

#endif	/* MATERIAL_H */

