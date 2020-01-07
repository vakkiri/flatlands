/*
 * 	fl_resources.cpp
 *
 */

#include <fstream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <SDL2/SDL_opengl.h>

#include "../logging/logging.h"
#include "../world/physics_settings.h"
#include "fl_resources.h"

#define IMAGE_RESOURCE_PATH "test-assets/image-resources.csv"
#define BASE_RESOURCE_PATH "test-assets/"

bool FLResources::init() {
	log_progress( "Initializing assets" );

	init_physics();

	init_il();

	if (!init_images())
		return false;

	return true;
}

void FLResources::close() {
	for (auto& it : image_dict)
		delete it.second;
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

	load_images( IMAGE_RESOURCE_PATH );

	return true;
}

void FLResources::load_images( std::string csv_path ) {
	log_progress( "Loading image csv" );

	std::ifstream csv_file;
	std::string line;
	std::string name;
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
				path = line.substr(pos+1, line.size() - pos);
				load_image( path, name );
			}
		}
	}

	csv_file.close();
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

texture* FLResources::get_image( std::string image_name ) {
	return image_dict[image_name];
}

void FLResources::load_level( int id ) {
	std::string filepath = BASE_RESOURCE_PATH + std::to_string(id) + ".lvl";
	std::ifstream file ( filepath, std::ios::in|std::ios::binary|std::ios::ate );

	if ( !file.is_open() ) {
		log_error("Could not open map file");
	}
	else {
		log_progress("Loading map");

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

				input += 5;
			}
			else if ( current_type == 1 ) {
				// input[1]: player x
				// input[2]: player y
				input += 3;
			}
			else if ( current_type == 2 ) {
				// ADD TILES HERE!
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

