/*
 * 	net_object.h
 *
 * 	Objects with information that needs to be shared across the network.
 *
 */

#ifndef FL_NET_NETOBJECT_H_
#define FL_NET_NETOBJECT_H_

class FLNetObject {
	public:
		FLNetObject();
		FLNetObject( uint32_t id );
		virtual ~FLNetObject();

		uint32_t get_net_id();
	private:
		uint32_t net_id;
};

#endif

