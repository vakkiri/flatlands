/*
 * 	fl_resources.cpp
 *
 */

#include <IL/il.h>
#include <IL/ilu.h>
#include <SDL2/SDL_opengl.h>
#include <fstream>
#include <iostream>
#include <cstring>

#include "../environment/fl_environment.h"
#include "../logging/logging.h"
#include "../net/fl_net.h"
#include "../world/monster/fl_monster_types.h"
#include "../world/npcs/npcs.h"
#include "../world/objects/objects.h"
#include "../world/physics_settings.h"
#include "../world/player/player.h"
#include "../world/portal.h"
#include "../world/scenery.h"
#include "../world/teleporter.h"
#include "../world/water.h"
#include "../rendering/text/fl_font.h"

#include "../tilemap/tilemap.h"
#include "fl_resources.h"

#define IMAGE_RESOURCE_PATH "assets/image-resources.csv"
#define SFX_RESOURCE_PATH "assets/sfx-resources.csv"
#define BASE_RESOURCE_PATH "assets/"

bool FLResources::init() {
	bool success = true;
	log_progress("Initializing assets");

	init_physics();

	init_il();

	if (!init_images() || !init_sfx())
		success = false;

	init_fonts();

	return success;
}

void FLResources::close() {
	for (auto &it : image_dict)
		delete it.second;
	for (auto &it : sfx_dict)
		Mix_FreeChunk(it.second);
}

void FLResources::init_physics() {
	log_progress("Loading physics csv");
	std::string csv_path = "settings/physics.csv";

	FLPhysics &physics = FLPhysics::getInstance();

	std::ifstream csv_file;
	std::string line;
	std::string name;
	float value;
	std::string path;
	csv_file.open(csv_path);

	if (!csv_file.is_open())
		log_warning("Could not open image .csv file");
	else {
		while (!csv_file.eof()) {
			getline(csv_file, line);
			if (line.size() > 0) {
				int pos = line.find(",");
				name = line.substr(0, pos);
				value = std::stof(line.substr(pos + 1, line.size() - pos));
				physics.set_attribute(name, value);
			}
		}
	}

	csv_file.close();
}

void FLResources::init_il() {
	ilInit();
	iluInit();
}

bool FLResources::init_images() {
	log_progress("Initializing images");

	return load_images(IMAGE_RESOURCE_PATH);
}

bool FLResources::init_sfx() {
	log_progress("Initializing sfx");

	return load_sfx(SFX_RESOURCE_PATH);
}

bool FLResources::load_images(std::string csv_path) {
	log_progress("Loading image csv");
	bool success = true;

	std::ifstream csv_file;
	std::string line;
	std::string name;
	std::string path;
	csv_file.open(csv_path);

	if (!csv_file.is_open()) {
		log_error("Could not open image .csv file");
		success = false;
	} else {
		while (!csv_file.eof()) {
			getline(csv_file, line);
			if (line.size() > 0) {
				int pos = line.find(",");
				name = line.substr(0, pos);
				path = line.substr(pos + 1, line.size() - pos);
				load_image(path, name);
			}
		}
	}

	csv_file.close();

	return success;
}

void FLResources::load_image(std::string path, std::string name) {
	ILuint img_id = 0;
	GLuint gl_id = 0;
	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	if (!ilLoadImage((const ILstring)path.c_str())) {
		log_warning("Could not load image: " + path);
		log_warning(iluErrorString(ilGetError()));
	} else {
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		glGenTextures(1, &gl_id);
		glBindTexture(GL_TEXTURE_2D, gl_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA,
					 GL_UNSIGNED_BYTE, (GLuint *)ilGetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture *tex = new texture;
		tex->w = (float)ilGetInteger(IL_IMAGE_WIDTH);
		tex->h = (float)ilGetInteger(IL_IMAGE_HEIGHT);
		tex->id = gl_id;
		image_dict[name] = tex;

		// add the pixels to a dict storing pixel rgba data
		float *pixels = new float[int(tex->w) * int(tex->h) * 4];
		glBindTexture(GL_TEXTURE_2D, tex->id);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
		image_pixel_dict[name] = pixels;
	}

	ilDeleteImages(1, &img_id);
}

bool FLResources::load_sfx(std::string csv_path) {
	log_progress("Loading sfx csv");
	bool success = true;

	std::ifstream csv_file;
	std::string line;
	std::string name;
	std::string path;
	csv_file.open(csv_path);

	if (!csv_file.is_open()) {
		log_error("Could not open image .csv file");
		success = false;
	} else {
		while (!csv_file.eof()) {
			getline(csv_file, line);
			if (line.size() > 0) {
				int pos = line.find(",");
				name = line.substr(0, pos);
				path = line.substr(pos + 1, line.size() - pos);
				Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
				if (chunk != nullptr) {
					Mix_VolumeChunk(chunk, 100);
					sfx_dict[name] = chunk;
				} else {
					log_warning("Missing audio file: ");
					std::cout << path << std::endl;
				}
			}
		}
	}

	csv_file.close();

	return success;
}

texture *FLResources::get_image(std::string image_name) {
	return image_dict[image_name];
}

float *FLResources::get_image_pixels(std::string image_name) {
	return image_pixel_dict[image_name];
}

float FLResources::get_image_transparency(std::string image_name, float s,
										  float t) {
	float *pixels = get_image_pixels(image_name);
	texture *tex = get_image(image_name);
	int num_channels = 4;
	int alpha_channel_offset = 3;
	int index = (t * int(tex->w) * num_channels) + (s * num_channels) +
				alpha_channel_offset;
	return pixels[index];
}

Mix_Chunk *FLResources::get_sound(std::string effect_name) {
	return sfx_dict[effect_name];
}

void FLResources::load_level(int id, FLEnvironment *environment) {
	std::string filepath = BASE_RESOURCE_PATH + std::to_string(id) + ".lvl";
	std::ifstream file(filepath,
					   std::ios::in | std::ios::binary | std::ios::ate);

	FLTilemap *tilemap = environment->tilemap();
	FLPlayer *player = environment->player();
	environment->set_level_id(id);

	if (!file.is_open()) {
		log_error("Could not open map file");
	} else {
		log_progress("Loading map");
		tilemap->reset();

		clear_npcs();
		clear_scenery();
		clear_portals();
		clear_monsters();
		clear_teleporters();
		clear_geysers();
		clear_water();

		std::vector<char> buffer;
		file.seekg(0, file.end);
		int size = file.tellg();
		buffer.reserve(size);
		file.seekg(0, file.beg);
		file.read(buffer.data(), size);
		file.close();


		int16_t val;
		char* cur = buffer.data();
		std::memcpy(&val, cur, sizeof(int16_t));

		// TODO: encode width/height in map format
		tilemap->reset(4096 * 2, 4096);

		while (val != -1) {
			if (val == -2) {
				int16_t tileset;

				cur += 2;
				std::memcpy(&tileset, cur, sizeof(int16_t));
				tilemap->set_tileset((unsigned int)tileset);
				cur += 2;
			}
			else if (val == 0 || val == 1) {
				int16_t x;
				int16_t y;
				int16_t index;
				bool solid = val == 1;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&index, cur, sizeof(int16_t));

				tilemap->add_tile(x, y, 16.f, 16.f, index, solid, 1);
				
				cur += 2;
			} else if (val == 2) {
				int16_t x;
				int16_t y;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));

				player->set_x(x);
				player->set_y(y);
				player->set_reset_position(x, y);
				player->reset_camera();

				cur += 2;
			} else if (val == 3) {
				int16_t x;
				int16_t y;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));

				new FLReep(float(x), float(y));

				cur += 2;
			} else if (val == 4) {
				int16_t x;
				int16_t y;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));


				new FLOrb(float(x), float(y));

				cur += 2;
			} else if (val == 5) {
				int16_t x;
				int16_t y;

				cur += 2;

				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));
				cur += 2;

				new FLHopper(float(x), float(y));

			} else if (val == 6) {
				int16_t x;
				int16_t y;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));

				FLAnimatedObjectParams animation_params = {
					1, 7, 5, 16, 32, true
				};
				new FLNpc(float(x), float(y), 16, 32, animation_params);

				cur += 2;

			} else if (val == 7) {
				int16_t x;
				int16_t y;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));
				cur += 2;

				new FLTeleporter(x, y);
			} else if (val == 8) {
				int16_t x;
				int16_t y;

				// geyser
				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));
				cur += 2;

				new FLGeyser(x, y);
			} else if (val >= 101 && val < 300) {
				// scenery

				int16_t x;
				int16_t y;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));

				// create it ...
				new FLScenery(x, y, val);

				cur += 2;

			} else if (val == 300) {
				float x;
				float y;
				float w;
				float h;
				float destx;
				float desty;
				int16_t dest_level;

				cur += 2;
				std::memcpy(&x, cur, sizeof(float));
				cur += 4;
				std::memcpy(&y, cur, sizeof(float));
				cur += 4;
				std::memcpy(&w, cur, sizeof(float));
				cur += 4;
				std::memcpy(&h, cur, sizeof(float));
				cur += 4;
				std::memcpy(&destx, cur, sizeof(float));
				cur += 4;
				std::memcpy(&desty, cur, sizeof(float));
				cur += 4;
				std::memcpy(&dest_level, cur, sizeof(int16_t));
				cur += 4;

				new FLPortal(x, y, w, h, destx, desty, dest_level);

			} else if (val == 301) {
				int16_t x;
				int16_t y;
				int16_t w;
				int16_t h;

				cur += 2;
				std::memcpy(&x, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&y, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&w, cur, sizeof(int16_t));
				cur += 2;
				std::memcpy(&h, cur, sizeof(int16_t));
				cur += 2;

				new FLWater(x, y, w, h);
			} else {
				std::cout << "UNK: " << val << std::endl;
				val = -1;
			}

			if (val != -1) {
				std::memcpy(&val, cur, sizeof(int16_t));
			}
		}

		tilemap->update_surface();
	}
}

void FLResources::init_fonts() {
	// TODO: read fonts from a config list w/ char dimensions
	fl_add_font("fl-plain-mono", 5, 10);
}

