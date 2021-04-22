/*
 * 	fl_game_object.cpp
 *
 */

#include <iostream>
#include "fl_static_buffer.h"
#include "fl_object.h"
#include "components/components.h"
#include "resources/fl_collections.h"

#define DEFAULT_NUM_OBJECTS 10000

namespace FLObjects {
	FLStaticBuffer<FLObject> objects(DEFAULT_NUM_OBJECTS);

	fl_handle create(fl_handle parent) {
		fl_handle handle = objects.create();

		// Resetting attributes is done lazilly, so it is here
		// instead of destroy
		if (handle != NULL_HANDLE) {
			objects[handle].handle = handle;
			objects[handle].parent = parent;
			objects[handle].physics_body = NULL_HANDLE;
			objects[handle].x = 0;
			objects[handle].y = 0;
			objects[handle].states.clear();
			objects[handle].textures.clear();
			objects[handle].vars.clear();
			objects[handle].colliders.clear();
		}

		return handle;
	}

	void add_collision_target(
		fl_handle handle,
		std::string collider_name,
		std::string group_name
	) {
		if (handle == NULL_HANDLE) {
			std::cout << "Warning: Tried to add null object to collision group.\n";
			return;
		}
		if (objects[handle].colliders.find(collider_name) == objects[handle].colliders.end()) {
			std::cout << "Warning: Tried to add non-existant collider " << collider_name << " to group " << group_name << std::endl;
			return;
		}

		FLColliders::add_target_group(
			objects[handle].colliders[collider_name],
			group_name
		);
	}

	fl_handle create() {
		return create(NULL_HANDLE);
	}

	FLObject* get(fl_handle handle) {
		if (handle != NULL_HANDLE) {
			return &objects[handle];
		} else {
			std::cout << "Tried to access null object handle.\n";
			return nullptr;
		}
	}

	void destroy(fl_handle handle) {
		// TODO: should have a global gamestate, with a "RESET"
		// flag which prevents on_death scripts from running
		if (objects[handle].on_death) {
			objects[handle].on_death(objects[handle]);
		}

		objects[handle].on_death = std::function<void(FLObject&)>();

		// TODO: should we keep track of children, so they
		// can be destroyed as well?
		for (auto [name, texture] : objects[handle].textures) {
			FLTextures::destroy(texture);
		}
		for (auto [name, animator] : objects[handle].animators) {
			FLAnimators::destroy(animator);
		} for (auto [name, collider] : objects[handle].colliders) {
			FLColliders::destroy(collider);
		}
		if (objects[handle].physics_body != NULL_HANDLE) {
			FLPhysicsBodies::destroy(objects[handle].physics_body);
		}	

		objects[handle].scripts.clear();
		objects.destroy(handle);
	}

	void update() {
		for (FLObject& obj : objects) {
			for (auto s : obj.scripts) {
				s(obj);
			}
		}
	}

	void render() {
		for (auto obj : objects) {
			for (auto [name, handle] : obj.textures) {
				FLTextures::render(handle);
			}
		}
	}

	void set_pos(fl_handle handle, float x, float y) {
		if (handle != NULL_HANDLE) {
			objects[handle].x = x;
			objects[handle].y = y;
		} else {
			std::cout << "Error: tried to position of object with null handle.";
		}
	}

	void add_script(
		fl_handle handle,
		std::function<void(FLObject&)> f
	) {
		objects[handle].scripts.push_back(f);
	}

	void add_death_script(
		fl_handle handle,
		std::function<void(FLObject&)> f
	) {
		objects[handle].on_death = f;
	}

	void add_collider(
		fl_handle handle,
		std::string name,
		float x,
		float y,
		float w,
		float h
	) {
		if (handle == NULL_HANDLE) {
			std::cout << "Warning: tried to add collider to null object.\n";
			return;
		}

		fl_handle collider = FLColliders::create(handle, x, y, w, h);

		if (collider != NULL_HANDLE) {
			objects[handle].colliders[name] = collider;
		} else {
			std::cout << "Warning: could not create new collider for object.\n";
		}
	}

	void set_texture(
		fl_handle handle,
		std::string name,
		std::string collection,
		int index
	) {
		FLObject* obj = get(handle);
		if (obj->textures.find(name) != obj->textures.end()) {
			FLCollection& c = FLCollections::get(collection);
			FLCollectionElement e = c.elements[index];
			fl_handle tex_handle = obj->textures[name];
			FLTexture * tex = FLTextures::get(tex_handle);
			tex->s = e.s;
			tex->t = e.t;
			tex->w = e.w;
			tex->h = e.h;
		} else {
			std::cout << "Warning: Texture " << name << " does not exist to modify.\n";
		}
	}

	void add_var(
		fl_handle handle,
		std::string name,
		fl_var value
	) {
		if (objects[handle].vars.find(name) == objects[handle].vars.end()) {
			objects[handle].vars[name] = value;
		} else {
			std::cout << "Warning: tried to add duplicate var " << name << std::endl;
		}
	}

	void add_state(
		fl_handle handle,
		std::string name,
		fl_state state
	) {
		if (objects[handle].states.find(name) == objects[handle].states.end()) {
			objects[handle].states[name] = state;
		} else {
			std::cout << "Warning: tried to add duplicate state " << name << std::endl;
		}
	}

	void set_animation (
		fl_handle handle,
		std::string name,
		std::string collection
	) {
		if (objects[handle].animators.find(name) != objects[handle].animators.end()) {
			fl_handle animator = objects[handle].animators[name];
			FLAnimators::set_collection(animator, collection);
		} else {
			std::cout << "Warning: tried to access missing animation " << name << std::endl;
		}
	}

	void add_animator(
		fl_handle handle,
		std::string name,
		std::string texture,
		std::string collection,
		unsigned int ticks_per_frame,
		bool repeats
	){
		if (objects[handle].textures.find(texture) == objects[handle].textures.end()) {
			std::cout << "Warning: tried to create animator for texture which does not exist: " << texture << "\n";
			return;
		}

		fl_handle texture_handle = objects[handle].textures[texture];
		fl_handle animator_handle = FLAnimators::create(texture_handle, collection, ticks_per_frame, repeats);
		if (animator_handle != NULL_HANDLE) {
			objects[handle].animators[name] = animator_handle;
		} else {
			std::cout << "Warning: could not create animator.\n";
		}
	}

	void add_animator(
		fl_handle handle,
		std::string name,
		std::string texture,
		std::string collection,
		unsigned int ticks_per_frame
	){
		add_animator(handle, name, texture, collection, ticks_per_frame, true);
	}

	void set_texture(
		fl_handle handle,
		std::string name,
		std::string collection
	) {
		set_texture(handle, name, collection, 0);
	}

	void add_texture(
		fl_handle handle,
		std::string name,
		std::string surface,
		std::string collection,
		int index
	) {
		FLCollection& c = FLCollections::get(collection);
		FLCollectionElement e = c.elements[index];
		add_texture(handle, name, surface, e.s, e.t, e.w, e.h);
	}

	void add_texture(
		fl_handle handle,
		std::string name,
		std::string surface,
		std::string collection
	) {
		add_texture(handle, name, surface, collection, 0);
	}

        void add_texture(
                fl_handle handle,
		std::string name,
                std::string surface,
                float s,
                float t,
		float w,
		float h,
		bool reversed
        ) {
		fl_handle texture = FLTextures::create(surface, handle, s, t, w, h, reversed);

		objects[handle].textures[name] = texture;
	}

        void add_texture(
                fl_handle handle,
		std::string name,
                std::string surface,
                float s,
                float t,
		float w,
		float h
        ) {
		add_texture(handle, name, surface, s, t, w, h, false);
	}

	void add_physics_body(
		fl_handle handle,
		std::string collider_name
	) {
		if (handle == NULL_HANDLE) {
			std::cout << "Warning: tried to add physics body to null object.\n";
			return;
		}
		
		if (objects[handle].colliders.find(collider_name) == objects[handle].colliders.end()) {
			std::cout << "Warning: tried to add physics body using non existant collider " << collider_name << std::endl;
			return;
		}
		
		fl_handle collider = objects[handle].colliders[collider_name];

		fl_handle body = FLPhysicsBodies::create(
			handle,
			collider
		);

		objects[handle].physics_body = body;
	}

	void accelerate(fl_handle handle, float x, float y) {
		if (objects[handle].physics_body == NULL_HANDLE) {
			std::cout << "Warning: tried to accelerate object with no physics body." << std::endl;
			return;
		}

		FLPhysicsBodies::accelerate(objects[handle].physics_body, x, y);
	}

	float x(fl_handle handle) {
		float x = objects[handle].x;

		if (objects[handle].parent != NULL_HANDLE) {
			x += objects[objects[handle].parent].x;
		}

		return x;
	}

	float y(fl_handle handle) {
		float y = objects[handle].y;

		if (objects[handle].parent != NULL_HANDLE) {
			y += objects[objects[handle].parent].y;
		}

		return y;
	}

}
