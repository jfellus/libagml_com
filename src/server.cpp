/*
 * server.cpp
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#include "com.h"
#include "tcp/Socket.h"
#include "tcp/Server.h"

Server* server = NULL;
unsigned short int PORT = 10001;








void server_run() {
	com_init();

	server = new Server(PORT, com_connection_thread);
	server->join();
}
