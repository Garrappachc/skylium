/* 
 * File:   Box.h
 * Author: Michał Garapich
 *
 * Created on 3 april 2011, 17:59
 */

#ifndef BOX_H
#define	BOX_H

#include "Object.h"

class Box : public Object {
public:
	/**
	 * @param nazwa pudełeczka.
     */
	Box(const std::string&);
	~Box();

	/**
	 * Zastępuje metodę show() z klasy Object. Wyświetla pudełko.
	 */
	void show();
};

#endif	/* BOX_H */

