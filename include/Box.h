/* 
 * File:   Box.h
 * Author: Michał Garapich
 *
 * Created on 3 kwiecień 2011, 17:59
 */

#ifndef BOX_H
#define	BOX_H

#include <SDL/SDL.h>

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
	bool show();
};

#endif	/* BOX_H */

