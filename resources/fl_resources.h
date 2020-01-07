/*
 * 	fl_resources.h
 *
 * 	Resource (ie. image/sound, not memory/processor) management
 * 	and storage.
 *
 */

#ifndef FL_RESOURCES_H_
#define FL_RESOURCES_H_

#include <unordered_map>
#include <string>
#include "../rendering/texture.h"

class FLResources {
	public:
		static FLResources& getInstance() {
			static FLResources instance;
			return instance;
		}

		bool init();
		void close();

		void load_level( int id );
		void load_images( std::string csv_path );
		void load_image( std::string path, std::string name );
		texture* get_image( std::string image_name );

	protected:
		std::unordered_map< std::string, texture* > image_dict;

	private:
		// Disallow copying/construction
		FLResources(){};
		FLResources(FLResources const&) = delete;
		FLResources& operator=(FLResources const&) = delete;

		bool init_images();
		void init_il();
		void init_physics();
};

#endif

