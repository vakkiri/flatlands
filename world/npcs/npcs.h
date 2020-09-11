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

struct fl_npc_message {
	std::string text;
	bool player;
};

class FLNpc : virtual public FLGameObject {
	public:
		FLNpc(float x, float y, float w, float h,
				FLAnimatedObjectParams animation_params);
		virtual ~FLNpc();
		virtual void interact();
	protected:
		int script_id;
};

#endif
