/*
 * 	net_object.h
 *
 * 	Objects with information that needs to be shared across the network.
 *
 */

#ifndef FL_NET_NETOBJECT_H_
#define FL_NET_NETOBJECT_H_

#include <stdint.h>

enum FLNetObjectType { FLNetObjectAmmo, FLNumNetObects };

class FLNetObject {
  public:
	FLNetObject();
	FLNetObject(uint16_t id);
	virtual ~FLNetObject();

	uint16_t get_net_id();

  private:
	uint16_t net_id;
};

bool net_object_exists(uint16_t id);
void add_net_obj(uint16_t id, FLNetObject *obj);
void del_net_obj(uint16_t id);

#endif
