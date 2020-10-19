/*
 * 	savepoint.h
 *
 * 	Spots to... save
 *
 */

#ifndef SAVEPOINT_H_
#define SAVEPOINT_H_

#include "../../common/game_object.h"

class FLSavePoint : virtual public FLGameObject {
	public:
		FLSavePoint(float x, float y);
		virtual ~FLSavePoint();
		virtual void interact();
};

void clear_savepoints();

#endif

