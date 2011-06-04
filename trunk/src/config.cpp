/*
    config.cpp
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

#include "../include/config.h"

bool		sGlobalConfig::USING_VBO = true;
unsigned	sGlobalConfig::MAX_VBO_SIZE = 268435456;
unsigned	sGlobalConfig::MIN_VBO_SIZE = 256;
bool		sGlobalConfig::FULLSCREEN_RENDERING = false;