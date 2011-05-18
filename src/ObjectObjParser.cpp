/* 
 * ObjectObjParser.cpp
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

#include <iostream>
#include <fstream>
#include <string>

#include "../include/Object.h"
#include "../include/Material.h"
#include "../include/functions.h"
#include "../include/defines.h"

using namespace std;

void
Object::__parseObj(const string &_fileName, const unsigned int &_whatToLoad) {
	unsigned int lastSlash = 0;
	while (_fileName.find('/', lastSlash+1) != string::npos) {
		lastSlash = _fileName.find('/', lastSlash+1);
	}
	
	string loc = _fileName.substr(0, lastSlash+1);
	
	
	// kilka tymczasowych zmiennych
	vector< sVertex > vertices; // tymczasowy wektor współrzędnych wierzchołków
	vector< sCoords > texture; // tymczasowy wektor współrzędnych tekstury
	vector< sVertex > normals; // tymczasowy wektor normalnych
	string buffer;
	string temp;
	sVertex v;
	sCoords t;
	sVertex n;
	unsigned int currentG = 0;
	
	string mtlFileName;
	
	unsigned long int i = 0, j = 0;
#ifdef __DEBUG__
	unsigned int p = 0;
#endif
	
	fstream objFile(_fileName.c_str(), ios::in);
	
	while (!objFile.eof()) {
		getline (objFile, buffer);
		
		if (buffer[0] == '#')
			continue;	// komentarz, idziemy dalej
		
		istringstream line(buffer);
		
		if (buffer.substr(0, 6) == "mtllib") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			line >> temp >> mtlFileName;
			__parseMtl(loc + mtlFileName);
			continue;
		} else if (buffer[0] == 'g') {
			if (buffer.length() <= 2)
				continue;
			if (currentG == 0 && __pGroups[currentG] -> name == "") {
				__pGroups[currentG] -> name = buffer.substr(2);
			} else {
				PolygonGroup *tg = new PolygonGroup(buffer.substr(2));
				__pGroups.push_back(tg);
				currentG++;
			}
			i = j = 0;
			vertices.clear();
			normals.clear();
			texture.clear();
			continue;
		} else if (buffer.substr(0, 6) == "usemtl") {
			if (!(_whatToLoad & GET_MATERIAL))
				continue;
			string mtlName;
			line >> temp >> mtlName;
			for (unsigned int y = 0; y < __materials.size(); y++) {
				if (__materials[y] -> name == mtlName) {
					__pGroups[currentG] -> __material = __materials[y];
					break;
				}
			}
		} else if (buffer[0] == 's') {
			string tf;
			line >> temp >> tf;
			if (tf != "off")
				__pGroups[currentG] -> __smooth = true;
			else
				__pGroups[currentG] -> __smooth = false;
		} else if (buffer.substr(0, 2) == "v ") {
			line >> temp >> v[0] >> v[1] >> v[2];
			vertices.push_back(v);
			continue;
		} else if (buffer.substr(0, 2) == "vt") {
			line >> temp >> t[0] >> t[1];
			texture.push_back(t);
			continue;
		} else if (buffer.substr(0, 2) == "vn") {
			line >> temp >> n[0] >> n[1] >> n[2];
			normals.push_back(n);
			continue;
		} else if (buffer[0] == 'f') {
			if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) == (GET_VERTICES | GET_TEXTURE | GET_NORMALS)) {
				char d;
				int vx, nx, tx, vy, ny, ty, vz, nz, tz;
				
				line >> temp >>
						vx >> d >> tx >> d >> nx >>
						vy >> d >> ty >> d >> ny >>
						vz >> d >> tz >> d >> nz;
				
				__pGroups[currentG] -> __pointers.resize(i + 9);
				__pGroups[currentG] -> __texCoords.resize(j + 6);
				__pGroups[currentG] -> __normals.resize(i + 9);
				
				__pGroups[currentG] -> __pointers[i] = vertices[--vx][0];
				__pGroups[currentG] -> __normals[i++] = normals[--nx][0];
				__pGroups[currentG] -> __pointers[i] = vertices[vx][1];
				__pGroups[currentG] -> __normals[i++] = normals[nx][1];
				__pGroups[currentG] -> __pointers[i] = vertices[vx][2];
				__pGroups[currentG] -> __normals[i++] = normals[nx][2];
				
				__pGroups[currentG] -> __pointers[i] = vertices[--vy][0];
				__pGroups[currentG] -> __normals[i++] = normals[--ny][0];
				__pGroups[currentG] -> __pointers[i] = vertices[vy][1];
				__pGroups[currentG] -> __normals[i++] = normals[ny][1];
				__pGroups[currentG] -> __pointers[i] = vertices[vy][2];
				__pGroups[currentG] -> __normals[i++] = normals[ny][2];
				
				__pGroups[currentG] -> __pointers[i] = vertices[--vz][0];
				__pGroups[currentG] -> __normals[i++] = normals[--nz][0];
				__pGroups[currentG] -> __pointers[i] = vertices[vz][1];
				__pGroups[currentG] -> __normals[i++] = normals[nz][1];
				__pGroups[currentG] -> __pointers[i] = vertices[vz][2];
				__pGroups[currentG] -> __normals[i++] = normals[nz][2];
				
				__pGroups[currentG] -> __texCoords[j++] = texture[--tx][0];
				__pGroups[currentG] -> __texCoords[j++] = texture[tx][1];
				__pGroups[currentG] -> __texCoords[j++] = texture[--ty][0];
				__pGroups[currentG] -> __texCoords[j++] = texture[ty][1];
				__pGroups[currentG] -> __texCoords[j++] = texture[--tz][0];
				__pGroups[currentG] -> __texCoords[j++] = texture[tz][1];
				
#ifdef __DEBUG__
				p++;
#endif
				
				continue;
			} else if ((_whatToLoad & (GET_VERTICES | GET_TEXTURE)) == (GET_VERTICES | GET_TEXTURE)) {
				char d;
				int vx, vy, vz, tx, ty, tz;
				
				line >> temp >>
						vx >> d >> tx >>
						vy >> d >> ty >>
						vz >> d >> tz;
				
				__pGroups[currentG] -> __pointers.resize(i + 9);
				__pGroups[currentG] -> __texCoords.resize(j + 6);
				
				__pGroups[currentG] -> __pointers[i++] = vertices[--vx][0];
				__pGroups[currentG] -> __pointers[i++] = vertices[vx][1];
				__pGroups[currentG] -> __pointers[i++] = vertices[vx][2];
				
				__pGroups[currentG] -> __pointers[i++] = vertices[--vy][0];
				__pGroups[currentG] -> __pointers[i++] = vertices[vy][1];
				__pGroups[currentG] -> __pointers[i++] = vertices[vy][2];
				
				__pGroups[currentG] -> __pointers[i++] = vertices[--vz][0];
				__pGroups[currentG] -> __pointers[i++] = vertices[vz][1];
				__pGroups[currentG] -> __pointers[i++] = vertices[vz][2];
				
				__pGroups[currentG] -> __texCoords[j++] = texture[--tx][0];
				__pGroups[currentG] -> __texCoords[j++] = texture[tx][1];
				__pGroups[currentG] -> __texCoords[j++] = texture[--ty][0];
				__pGroups[currentG] -> __texCoords[j++] = texture[ty][1];
				__pGroups[currentG] -> __texCoords[j++] = texture[--tz][0];
				__pGroups[currentG] -> __texCoords[j++] = texture[tz][1];
				
#ifdef __DEBUG__
				p++;
#endif
				
				continue;
			} else if ((_whatToLoad & (GET_VERTICES | GET_NORMALS)) == (GET_VERTICES | GET_NORMALS)) {
				char d;
				int vx, nx, vy, ny, vz, nz;
				
				line >> temp >>
						vx >> d >> d >> nx >>
						vy >> d >> d >> ny >>
						vz >> d >> d >> nz;
				
				__pGroups[currentG] -> __pointers.resize(i + 9);
				__pGroups[currentG] -> __normals.resize(i + 9);
				
				__pGroups[currentG] -> __pointers[i] = vertices[--vx][0];
				__pGroups[currentG] -> __normals[i++] = normals[--nx][0];
				__pGroups[currentG] -> __pointers[i] = vertices[vx][1];
				__pGroups[currentG] -> __normals[i++] = normals[nx][1];
				__pGroups[currentG] -> __pointers[i] = vertices[vx][2];
				__pGroups[currentG] -> __normals[i++] = normals[nx][2];
				
				__pGroups[currentG] -> __pointers[i] = vertices[--vy][0];
				__pGroups[currentG] -> __normals[i++] = normals[--ny][0];
				__pGroups[currentG] -> __pointers[i] = vertices[vy][1];
				__pGroups[currentG] -> __normals[i++] = normals[ny][1];
				__pGroups[currentG] -> __pointers[i] = vertices[vy][2];
				__pGroups[currentG] -> __normals[i++] = normals[ny][2];
				
				__pGroups[currentG] -> __pointers[i] = vertices[--vz][0];
				__pGroups[currentG] -> __normals[i++] = normals[--nz][0];
				__pGroups[currentG] -> __pointers[i] = vertices[vz][1];
				__pGroups[currentG] -> __normals[i++] = normals[nz][1];
				__pGroups[currentG] -> __pointers[i] = vertices[vz][2];
				__pGroups[currentG] -> __normals[i++] = normals[nz][2];
				
#ifdef __DEBUG__
				p++;
#endif
				
				continue;
			}
		}
	}
	
	objFile.close();
	
	if ((_whatToLoad & GET_NORMALS) == GET_NORMALS) {
		for (unsigned int y = 0; y < __pGroups.size(); y++) {
				__pGroups[y] -> __hasNormals = true;
		}
	}
	
#ifdef __DEBUG__
	cout << LOG_INFO << "Załadowano " << p << " poligonów.";
#endif
}

void
Object::__parseMtl(const string &_fileName) {
	fstream mtlFile(_fileName.c_str(), ios::in);
	
	string buffer, temp;
	
	Material *current;
	
	while (!mtlFile.eof()) {
		getline (mtlFile, buffer);
		
		if (buffer[0] == '#')
			continue;	// komentarz, idziemy dalej
		
		istringstream line(buffer);
		
		if (buffer.substr(0, 6) == "newmtl") {
			string newMtlName;
			line >> temp >> newMtlName;
			Material *newMtl = new Material(newMtlName);
			__materials.push_back(newMtl); // Object zarządza materiałami
			current = newMtl;
		} else if (buffer.substr(0, 2) == "Ka") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			param[3] = 0.0;
			current -> loadMaterial(param, MATERIAL_AMBIENT);
		} else if (buffer.substr(0, 2) == "Kd") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			param[3] = 0.0;
			current -> loadMaterial(param, MATERIAL_DIFFUSE);
		} else if (buffer.substr(0, 2) == "Ks") {
			sColor param;
			line >> temp >> param[0] >> param[1] >> param[2];
			param[3] = 0.0;
			current -> loadMaterial(param, MATERIAL_SPECULAR);
		} else if (buffer.substr(0, 2) == "Ns") {
			GLint param;
			line >> temp >> param;
			current -> loadShininess(param);
		} else if (buffer.substr(0, 6) == "map_Kd") {
			string texfile;
			line >> temp >>texfile;
			current ->loadTexture("texture/" + texfile, TEXTURE_DIFFUSE);
		}
	}
	
	mtlFile.close();
}