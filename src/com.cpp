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


Host* this_host;
array<Host*> hosts;






long get_neighbor(long src_node, int iNeighbor) {

}





void com_init() {
	this_host = new Host();
}



void com_connection_thread(Socket* socket) {
	Host* h = new Host(socket);
	hosts.add(h);

	while(socket->isConnected()) {
		socket->read();
	}
}
