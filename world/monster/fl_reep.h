/*
 * 	fl_reep.h
 *
 * 	A type of flying monster.
 *
 */

#ifndef FL_REEP_H_
#define FL_REEP_H_

#include "fl_monster.h"

class FLReep : public FLMonster {
  public:
	FLReep(float x, float y);

	virtual void per_frame_update();
  protected:
	virtual void move();
	virtual void attack();
	virtual void attack1();
	virtual void attack2();

	int attack_index;
};

#endif
