/*
     _____        _                  _    
    |_   _|____ _| |_ _  _ _ _ ___  | |_  
      | |/ -_) \ /  _| || | '_/ -_)_| ' \ 
      |_|\___/_\_\\__|\_,_|_| \___(_)_||_|
                                
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


#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/gl.h>

class TextureManager;

class Texture {

public:
	
	/**
	 * Konstruktor domyślny, który jako argument przyjmuje lokalizację
	 * pliku z teksturą. Konstruktor ten od razu generuje teksturę i wrzuca
	 * swoją instancję do TextureManagera. Jako name ustawiana jest nazwa
	 * plik  z teksturą, ale bez rozszerzenia.
	 * @param fileName Nazwa pliku, w którym znajduje się tekstura.
	 */
	Texture(const std::string&);
	
	/**
	 * Destruktor niszczy teksturę i usuwa ją z bufora karty graficznej.
	 */
	virtual ~Texture();
	
	/**
	 * Binduje teksturę do OpenGLa i ustawia parametry w potoku renderowania.
	 */
	void setTexture();

	/* Nazwa tekstury - ta bez rozszerzenia */
	std::string name;
	
private:
	
	/**
	 * Sprawdza, czy plik istnieje.
	 */
	bool __fileExists(const std::string&);
	
	/**
	 * Ładuje teksturę.
	 * @param fileName Nazwa pliku z teksturą.
	 * @return Tekstura.
	 */
	GLuint __loadImage(const std::string&);
	
	/* Wskaźnik na teksturę OpenGLa */
	GLuint __texture;
	
	/* Typ tekstury - 
		GL_TEXTURE_1D,
		GL_TEXTURE_2D,
		GL_TEXTURE_3D,
		GL_TEXTURE_CUBE_MAP.
	Domyślnie GL_TEXTURE_2D. */
	GLenum __type;
	
	/* Wrapping tekstury - GL_CLAMP,
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_EDGE,
		GL_MIRRORED_REPEAT,
		GL_REPEAT,
		GL_CLAMP.
	Domyślnie GL_CLAMP_TO_BORDER. */
	GLenum __wrapping;
	
	/* Lokalizacja pliku z teksturą - być może się przyda */
	std::string __file;
	
	/* Channels, default 4 (RGBA) */
	GLint __channels;
	
	/* Instancja TextureManagera */
	TextureManager * __boss;
};

#endif // TEXTURE_H
