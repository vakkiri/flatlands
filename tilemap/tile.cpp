/*
 * 	tile.cpp
 *
 */

#include "tile.h"

FLTile::FLTile() : FLTexturedObject(), FLWorldObject() { }

FLTile::FLTile( float x, float y, float w, float h ) : FLTexturedObject(), FLWorldObject( x, y, w, h ) { }

float FLTile::x() { return FLWorldObject::x(); }
float FLTile::y() { return FLWorldObject::y(); }
float FLTile::w() { return FLWorldObject::w(); }
float FLTile::h() { return FLWorldObject::h(); }

