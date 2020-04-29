/*
 * 	fl_resources.cpp
 *
 */

#include <fstream>
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <SDL2/SDL_opengl.h>

#include "../net/fl_net.h"
#include "../logging/logging.h"
#include "../world/physics_settings.h"
#include "../world/world_environment.h"
#include "../world/player/player.h"
#include "../world/objects/objects.h"
#include "../world/npcs/npcs.h"
#include "../custom/angel.h"

#include "../tilemap/tilemap.h"
#include "fl_resources.h"

#define IMAGE_RESOURCE_PATH "test-assets/image-resources.csv"
#define SFX_RESOURCE_PATH "test-assets/sfx-resources.csv"
#define BASE_RESOURCE_PATH "test-assets/"

bool FLResources::init() {
	bool success = true;
	log_progress( "Initializing assets" );

	init_physics();

	init_il();

	if (!init_images() || !init_sfx())
		success = false;
	
	return success;
}

void FLResources::close() {
	for (auto& it : image_dict)
		delete it.second;
	for (auto& it : sfx_dict)
		Mix_FreeChunk( it.second );
}

void FLResources::init_physics() {
	log_progress( "Loading physics csv" );
	std::string csv_path = "settings/physics.csv";

	FLPhysics& physics = FLPhysics::getInstance();

	std::ifstream csv_file;
	std::string line;
	std::string name;
	float value;
	std::string path;
	csv_file.open(csv_path);

	if ( !csv_file.is_open() )
		log_warning( "Could not open image .csv file" );
	else {
		while (!csv_file.eof()) {
			getline(csv_file, line);
			if (line.size() > 0) {
				int pos = line.find(",");
				name = line.substr(0, pos);
				value = std::stof(line.substr(pos+1, line.size() - pos));
				physics.set_attribute( name, value );
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
	log_progress( "Initializing images" );

	return load_images( IMAGE_RESOURCE_PATH );
}

bool FLResources::init_sfx() {
	log_progress( "Initializing sfx" );

	return load_sfx( SFX_RESOURCE_PATH );
}

bool FLResources::load_images( std::string csv_path ) {
	log_progress( "Loading image csv" );
	bool success = true;

	std::ifstream csv_file;
	std::string line;
	std::string name;
	std::string path;
	csv_file.open(csv_path);

	if ( !csv_file.is_open() ) {
		log_error( "Could not open image .csv file" );
		success = false;
	}
	else {
		while (!csv_file.eof()) {
			getline(csv_file, line);
			if (line.size() > 0) {
				int pos = line.find(",");
				name = line.substr(0, pos);
				path = line.substr(pos+1, line.size() - pos);
				load_image( path, name );
			}
		}
	}

	csv_file.close();

	return success;
}

void FLResources::load_image( std::string path, std::string name ) {
	ILuint img_id = 0;
	GLuint gl_id = 0;
	ilGenImages( 1, &img_id );
	ilBindImage( img_id );

	if ( !ilLoadImage( (const ILstring) path.c_str() ) ) {
		log_warning( "Could not load image: " + path );
		log_warning( iluErrorString( ilGetError() ) );
	}
	else {
		ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
		glGenTextures( 1, &gl_id );
		glBindTexture( GL_TEXTURE_2D, gl_id );
		glTexImage2D( GL_TEXTURE_2D, 
				0, 
				GL_RGBA, 
				ilGetInteger( IL_IMAGE_WIDTH ),
				ilGetInteger( IL_IMAGE_HEIGHT ),
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				(GLuint*) ilGetData() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glBindTexture( GL_TEXTURE_2D, 0 );

		texture *tex = new texture;
		tex->w = (float) ilGetInteger( IL_IMAGE_WIDTH );
		tex->h = (float) ilGetInteger( IL_IMAGE_HEIGHT );
		tex->id = gl_id;
		image_dict[name] = tex;
	}

	ilDeleteImages( 1, &img_id );
}

bool FLResources::load_sfx( std::string csv_path ) {
	log_progress( "Loading sfx csv" );
	bool success = true;

	std::ifstream csv_file;
	std::string line;
	std::string name;
	std::string path;
	csv_file.open(csv_path);

	if ( !csv_file.is_open() ) {
		log_error( "Could not open image .csv file" );
		success = false;
	}
	else {
		while (!csv_file.eof()) {
			getline(csv_file, line);
			if (line.size() > 0) {
				int pos = line.find(",");
				name = line.substr(0, pos);
				path = line.substr(pos+1, line.size() - pos);
				Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
				if ( chunk != nullptr ) {
					Mix_VolumeChunk(chunk, 100);
					sfx_dict[name] = chunk;
				}
				else {
					log_warning( "Missing audio file: " );
					std::cout << path << std::endl;
				}
			}
		}
	}

	csv_file.close();

	return success;
}

texture* FLResources::get_image( std::string image_name ) {
	return image_dict[image_name];
}

Mix_Chunk* FLResources::get_sound( std::string effect_name ) {
	return sfx_dict[effect_name];
}

void FLResources::load_level( int id ) {
	std::string filepath = BASE_RESOURCE_PATH + std::to_string(id) + ".lvl";
	std::ifstream file ( filepath, std::ios::in|std::ios::binary|std::ios::ate );
	FLWorldEnvironment &environment = FLWorldEnvironment::getInstance();

	FLTilemap* tilemap = environment.tilemap();
	FLPlayer* player = environment.player();

	if ( !file.is_open() ) {
		log_error("Could not open map file");
	}
	else {
		log_progress("Loading map");
		tilemap->reset();

		// TODO: create new map format
		// This code is largely just a carryover from the format of an
		// old engine (shift).

		int current_type;
		bool done = false;
		int *input;
		int size = file.tellg();

		input = new int[size];

		file.seekg( 0, std::ios::beg );
		file.read((char*)input, size * sizeof(int));
		file.close();
		
		while (!done) {
			current_type = *input;

			if ( current_type == 0 ) {
				// input[1]: background
				// input[2]: tileset
				// input[3]: map width
				// input[4]: map height

				tilemap->reset( input[3], input[4] );
				tilemap->set_tileset( input[2] );
				input += 5;
			}
			else if ( current_type == 1 ) {
				// input[1]: player x
				// input[2]: player y
				player->set_x( input[1] );
				player->set_y( input[2] );
				player->set_reset_position( input[1], input[2] );
				input += 3;
			}
			else if ( current_type == 2 ) {
				// input[1]: x
				// input[2]: y
				// input[3]: id
				// input[4]: solid?
				// input[5]: layer
				tilemap->add_tile(
							(float)input[1],
							(float)input[2],
							16.f,
							16.f,
							(float)input[3]+1,
							(bool)input[4]
						);
				input += 6;
			}
			else if ( current_type == 3 ) {
				input += 6;
			}
			else if ( current_type == 4) {
				input += 8;
			}
			else if ( current_type == 5) {
				input += 5;
			}
			else if ( current_type == 6 ) {
				input += 3;
			}
			else if ( current_type == 7 ) {
				input += 4;
			}
			else if ( current_type == 8 ) {
				input += 7;
			}
			else if ( current_type == 9 ) {
				input += 3;
			}
			else if ( current_type == 10 ) {
				// items, or general objects
				// input[1]: x
				// input[2]: y
				// input[3]: type
				switch ( input[3] ) {
					case 0:
						// was pound powerup
						new FLAmmo( (float)input[1], (float)input[2], FL_FUSION );
						break;
					case 1:
						new FLDestroyableTile( (float)input[1], (float)input[2] );
						break;
					case 2:
						new FLWarpGate( (float)input[1], (float)input[2] );
						break;
					case 3:
						new NVAngel( (float)input[1], (float)input[2] );
						break;
					case 4:
						new FLNpc( (float)input[1], (float)input[2], 0, 160, 37, 18 );
						break;
					case 5:
						new FLToggler( (float)input[1], (float) input[2] );
						break;
					case 6:
						new FLToggleTile( (float)input[1], (float) input[2], true );
						break;
					case 7:
						new FLToggleTile( (float)input[1], (float) input[2], false );
						break;
					case 8: new FLFireball( (float)input[1], (float) input[2] );
						break;
					case 9: new FLPlatform( (float)input[1], (float) input[2] );
						break;
					case 10: new FLRhombus( (float)input[1], (float) input[2] );
						break;
					default:
						log_warning( "Unknown item type" );
						break;
				}
				input += 4;
			}
			else if ( current_type == 11 ) { 
				input += 4;
			}
			else if ( current_type == 12 ) {
				input += 3;
			}
			else if ( current_type == 13 ) {
				input += 3;
			}
			else if ( current_type == 14 ) {
				input += 4;
			}
			else if ( current_type == 15 ) {
				input += 3;
			}
			else if ( current_type == 16 ) {
				input += 4;
			}
			else {
				done = true;
			}
		}

	}

}

