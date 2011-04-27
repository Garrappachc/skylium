/* 
 * File:   Shader.h
 * Author: Michał Garapich
 *
 * Created on 20 april 2011, 20:34
 */

#ifndef SHADER_H
#define	SHADER_H

#include "Object.h"

/**
 * Program shadera wysokopoziomowego.
 */
class Shader {
public:
	/**
	 * Konstruktor, przyjmuje dwa argumenty.
     * @param vertFileName Nazwa pliku z kodem vertex shadera.
     * @param fragFileName Nazwa pliku z kodem fragment shadera.
     */
	Shader(const std::string&, const std::string&);
	
	virtual ~Shader();

	/**
	 * Kompiluje i linkuje program.
     * @return False, jeżeli coś poszło nie tak.
     */
	bool make();

	/**
	 * Włącza program, jeżeli jest wyłączony i wyłącza, jeżeli jest włączony.
     * @return Zawsze true.
     */
	bool toggle();
	
	/**
	 * Przypina shadera do danego obiektu.
     * @param dest Wskaźnik obiektu, do którego ma być przypięty shader.
     * @return false, jeżeli coś poszło nie tak.
     */
	bool bind(Object*);

private:
	std::string vertFile_;
	std::string fragFile_;

	GLint vertexShader_;
	GLint fragmentShader_;

	GLint shaderProgram_;

	GLboolean isRunning_;

	/**
	 * Funkcja pomocnicza - sprawdza, czy istnieje podany plik.
     * @param Nazwa pliku.
     * @return True, jeżeli istnieje, w przeciwnym przypadku false.
     */
	bool fileExists(const std::string&);

};

#endif	/* SHADER_H */

