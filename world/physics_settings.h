/*
 *	physics_settings.h
 *
 *	Class for maintaining the game's physics state (ie. gravity,
 *	friction etc.).
 *
 */

#ifndef PHYSICS_SETTINGS_H_
#define PHYSICS_SETTINGS_H_

#include <unordered_map>

class FLPhysics {
	public:
		static FLPhysics& getInstance() {
			static FLPhysics instance;
			return instance;
		}

		void set_gravity( float val );
		void set_friction( float val );
		void set_attribute( std::string name, float val );

		float gravity() { return _gravity; }
		float friction() { return _friction; }
		float air_resist() { return _air_resist; }
	protected:
		float _gravity;
		float _friction;
		float _air_resist;
		std::unordered_map<std::string, float*> attr_dict;

	private:
		// Disallow copying/construction
		FLPhysics();
		FLPhysics(FLPhysics const&) = delete;
		FLPhysics& operator=(FLPhysics const&) = delete;
};

#endif

