/*
 * 	fl_teleporter_box.h
 *
 * 	UI element for area selection boxes.
 *
 */

#ifndef FL_TELEPORTER_BOX_H_
#define FL_TELEPORTER_BOX_H_

#include <string>
#include <vector>

#include "fl_ui_element.h"

class FLGameObject;

struct fl_message {
	std::string text;
	FLGameObject* speaker;
	bool flipped;
};

class FLTeleporterBox : public FLUIElement {
  public:
	FLTeleporterBox() = delete;
	FLTeleporterBox(float x, float y, int area_id);

	virtual void accept() override;
	virtual void reject() override;
	std::vector<FLTexturedObject*>& get_textured_objects() override;
  protected:
	int area_id;
	virtual void init_textures();

	unsigned int width;
	unsigned int height;
	float s, t;
	float x, y;

	FLTexturedObject* tex;
};

#endif
