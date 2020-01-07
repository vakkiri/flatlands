/*
 *	world_environment.h
 *
 * 	Singleton class to hold the game environment such as tiles, 
 * 	monsters, the player etc.
 *
 */

#ifndef FL_WORLD_ENVIRONMENT_H_
#define FL_WORLD_ENVIRONMENT_H_

class FLTilemap;
class FLPlayer;

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
	protected:
		FLTilemap* _tilemap;
		FLPlayer* _player;
	private:
		// Disallow copying/construction
		FLWorldEnvironment(){};
		FLWorldEnvironment(FLWorldEnvironment const&) = delete;
		FLWorldEnvironment& operator=(FLWorldEnvironment const&) = delete;
};

#endif
