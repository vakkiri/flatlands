/*
 * 	fl_resources.cpp
 *
 */

#include <fstream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <SDL2/SDL_opengl.h>

#include "../logging/logging.h"
#include "fl_resources.h"

#define IMAGE_RESOURCE_PATH "test-assets/image-resources.csv"

bool FLResources::init() {
	log_progress( "Initializing assets" );

	init_il();

	if (!init_images())
		return false;

	return true;
}

void FLResources::close() {
	for (auto& it : image_dict)
		delete it.second;
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

