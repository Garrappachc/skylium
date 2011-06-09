/*
    config.h
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

#ifndef CONFIG_H
#define CONFIG_H

enum {
	D_NOTHING			= 0,
	D_CONSTRUCTORS		= 1,
	D_ALL_CONSTRUCTORS	= 2,
	D_DESTRUCTORS		= 4,
	D_PARAMS			= 8,
	D_ALL_PARAMS		= 16,
	D_SHADERS			= 32,
	D_BUFFER			= 64,
	D_WARNINGS			= 128,
	D_ERRORS			= 256,
	D_EVERYTHING		= 512
};

struct sGlobalConfig {
	
	/* Definiuje (za pomocą enuma) elementy do wyświetlania na standardowym
	 * wyjściu. */
	static unsigned	DEBUGGING;
	
	/* Jeżeli true, Skylium będzie używało VBO. Jeżeli false, wszystkie
	 * dane do renderowania będą przechowywane w pamięci RAM. Jeżeli karta
	 * graficzna obsługuje VBO, zaleza się ustawienie tej wartość na
	 * true. */
	static bool		USING_VBO;
	
	/* Maksymalny rozmiar tablicy wierzchołków, aby znaleźć się w buforze
	 * karty graficznej. Tablice powyżej tej wartości będą renderowane
	 * z pamięci RAM. Domyślna wartość to 256 MB. */
	static unsigned	MAX_VBO_SIZE;
	
	/* Minimalny rozmiar tablicy wierzchołków, aby znaleźć się w buforze
	 * karty graficznej. Nie zaleza się tworzenia zbyt małych buforów,
	 * dlatego domyślna wartość to 256 bajtów. */
	static unsigned	MIN_VBO_SIZE;
	
	/* Jeżeli true, Skylium będzie renderował na fullscreenie. */
	static bool		FULLSCREEN_RENDERING;
};

#endif