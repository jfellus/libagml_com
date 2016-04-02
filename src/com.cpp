/*
 * com.cpp
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */


#include "com.h"
#include "tcp/Socket.h"
#include "host.h"
#include "util/array.h"
#include "node/Node.h"


Host* this_host;
array<Host*> hosts;

array<Node*> nodes;




long com_get_neighbor(long src_node, long iNeighbor) {
	if(NODE_HOST(src_node)!=this_host->id) {
		ERROR("Can't get neighbors addresses for remote nodes");
		return -1;
	} else {
		if(nodes[NODE_LOCALID(src_node)]->is_fully_connected())
			return iNeighbor;
		else return nodes[NODE_LOCALID(src_node)]->neighbors[iNeighbor];
	}
}





void com_init() {
	this_host = new Host();
}


#define MAX_BUFS 10

void com_connection_thread(Socket* socket) {
	Host* h = new Host(socket);
	hosts.add(h);

	ComHeader header;
	unsigned char* bufs[MAX_BUFS];
	size_t bufsizes[MAX_BUFS];
	unsigned char* curbuf = 0;
	size_t nread = 0;

	while(socket->isConnected()) {
		socket->read(&header, sizeof(header));

		if(bufs[msgtype - 'A'] == 0) {
			socket->read(&bufsizes[msgtype - 'A'], sizeof(size_t));
		}
		socket->read();
	}
}
