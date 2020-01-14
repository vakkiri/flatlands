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
		void add_object( FLWorldObject* object );
		void remove_object( FLWorldObject* object );
		void add_colliding_object( FLCollidingObject* object );
		void remove_colliding_object( FLCollidingObject* object );
		FLCollidingObject* get_colliding_object( FLWorldObject* object );
		std::vector<FLCollidingObject*> get_colliding_objects( FLWorldObject* object );
		virtual void load_next_level();

	protected:
		FLTilemap* _tilemap;
		FLPlayer* _player;
		std::vector<FLWorldObject*> world_objects;
		std::vector<FLCollidingObject*> colliding_objects;
		virtual void reset_environment();

		int level;
		bool reset;
	private:
		// Disallow copying/construction
		FLWorldEnvironment() : level(-1), reset(false) {};
		FLWorldEnvironment(FLWorldEnvironment const&) = delete;
		FLWorldEnvironment& operator=(FLWorldEnvironment const&) = delete;
};

#endif
