/*
 * 	npcs.h
 *
 * 	in-game NPCs to interact with.
 *
 */

#ifndef NPC_H_
#define NPC_H_

#include "../interactable_object.h"
#include "../../rendering/animated_object.h"

class FLNpc : virtual public FLAnimatedObject, virtual public FLInteractableObject {
	public:
		FLNpc( float x, float y, float s, float t, float w, float h);
		virtual ~FLNpc();
		virtual void interact_with();

};

#endif

