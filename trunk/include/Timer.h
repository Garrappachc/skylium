/* 
 * Timer.h
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

#ifndef TIMER_H
#define	TIMER_H

#include <iostream>

enum {
	SECONDS			= 1,
	MICROSECONDS
};

/* Niskopoziomowy timer, liczący z dokładnością do ~10 mikrosekund. */
class Timer {
	
public:
	/**
	 * Domyślny konstruktor - wszystko 0.
     */
	Timer();
	
	/**
	 * Pyta się system o aktualny czas, zwraca go w postaci long long int.
     * @param clock Dokładność mierzenia rzasu - SECONDS lub MICROSECONDS.
     * @return Czas.
     */
	long long int update(const unsigned&);
	
	/**
	 * Sprawdza, czy od ostatniego wywołania funkcji minęła określona ilość
	 * czasu.
     * @param howMuch Ile czasu ma minąć?
     * @param clock Dokładność mierzenia czasu - SECONDS lub MICROSECONDS.
     * @return True, jeżeli minęło już tyle czasu, w przeciwnym wypadku - false.
     */
	bool passed(const long int&, const unsigned&);
	
private:
	
	struct timeval __tv;
	
	time_t __curtime;
	
	long long int __temp;

};

#endif	/* TIMER_H */

