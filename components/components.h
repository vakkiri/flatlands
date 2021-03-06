/*
 * 	components.h
 *
 * 	header file for all components.
 *
 */

#ifndef FL_COMPONENTS_H_
#define FL_COMPONENTS_H_

#include <unordered_map>
#include <variant>

#include "animator/fl_animator.h"
#include "collider/fl_collider.h"
#include "collider/fl_colliders.h"
#include "collider/fl_collider_manager.h"
#include "interactable/fl_interactable.h"
#include "interactable/fl_interactable_manager.h"
#include "physics/fl_physicsbody.h"
#include "physics/fl_physics_handler.h"
#include "physics/fl_physics_handler_manager.h"
#include "texture/fl_texture.h"
#include "updator/fl_update_manager.h"

void fl_update_components();

#endif
