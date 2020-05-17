/*
 *	world_environment.h
 *
 * 	Singleton class to hold the game environment such as tiles, 
 * 	monsters, the player etc.
 *
 */

#ifndef FL_WORLD_ENVIRONMENT_H_
#define FL_WORLD_ENVIRONMENT_H_

#include <vector>

class FLTilemap;
class FLPlayer;
class FLWorldObject;
class FLCollidingObject;
class FLInteractableObject;

class FLWorldEnvironment {
	public:
		static FLWorldEnvironment& getInstance() {
			static FLWorldEnvironment instance;
			return instance;
		}

		virtual void update();

		void set_player(FLPlayer* player);
		FLPlayer* player();

		FLTilemap* tilemap();
		void set_tilemap(FLTilemap* tilemap);
		void reset_tilemap();

		bool solid_at(float x, float y);

		void mark_reset();

		void add_interactable_object( FLInteractableObject* object );
		void remove_interactable_object( FLInteractableObject* object );
		FLCollidingObject* find_colliding_object( FLWorldObject* object );
		std::vector<FLCollidingObject*> find_colliding_objects( FLWorldObject* object );
		void interact( FLWorldObject* object );

		virtual void load_next_level();

	protected:
		FLTilemap* _tilemap;
		FLPlayer* _player;

		std::vector<FLInteractableObject*> interactable_objects;
		virtual void reset_environment();

		int level;
		bool reset;
	private:
		// Disallow copying/construction
		// We start at level -1 as it will be incremented to
		// 0 when the first level loads.
		FLWorldEnvironment() : level(-1), reset(false) {};
		FLWorldEnvironment(FLWorldEnvironment const&) = delete;
		FLWorldEnvironment& operator=(FLWorldEnvironment const&) = delete;
};

#endif
