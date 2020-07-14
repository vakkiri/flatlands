/*
 * 	npcs.h
 *
 * 	in-game NPCs to interact with.
 *
 */

#ifndef NPC_H_
#define NPC_H_

#include "../../rendering/animated_object.h"
#include "../../common/game_object.h"

class FLNpc : virtual public FLGameObject {
  public:
	FLNpc(float x, float y, float s, float t, float w, float h);
	virtual ~FLNpc();
	virtual void interact_with();
};

#endif
