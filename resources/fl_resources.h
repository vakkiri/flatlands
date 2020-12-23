/*
 * 	fl_resources.h
 *
 * 	Resource (ie. image/sound, not memory/processor) management
 * 	and storage.
 *
 */

#ifndef FL_RESOURCES_H_
#define FL_RESOURCES_H_

#include "../rendering/texture.h"
#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>

class FLEnvironment;

class FLResources {
  public:
	static FLResources &getInstance() {
		static FLResources instance;
		return instance;
	}

	bool init();
	void close();

	void load_level(int id, FLEnvironment *environment);
	bool load_images(std::string csv_path);
	void load_image(std::string path, std::string name);
	texture *get_image(std::string image_name);
	float *get_image_pixels(std::string image_name);
	float get_image_transparency(std::string image_name, float s, float t);
	Mix_Chunk *get_sound(std::string effect_name);


  protected:
	std::unordered_map<std::string, texture *> image_dict;
	std::unordered_map<std::string, float *> image_pixel_dict;
	std::unordered_map<std::string, Mix_Chunk *> sfx_dict;

  private:
	// Disallow copying/construction
	FLResources(){};
	FLResources(FLResources const &) = delete;
	FLResources &operator=(FLResources const &) = delete;

	bool init_images();
	bool init_backgrounds();
	bool init_sfx();
	void init_il();
	void init_physics();
	void init_fonts();

	bool load_sfx(std::string csv_path);
};

#endif
