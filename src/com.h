/*
 * com.h
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#ifndef AGML_COM_H_
#define AGML_COM_H_

#include "tcp/Socket.h"
#include <vector>

#define NODE_HOST(node) ((int) (node >> 48))
#define NODE_THREAD(node) ((int) (node >> 32) & ((1 << 16)-1))
#define NODE_LOCALID(node) ((int) node)



void com_init();
long com_get_neighbor(long src_node, long iNeighbor);
void com_connection_thread(Socket* socket);





typedef struct {
	char msgtype;
	size_t bufsize;
} ComHeader;

#endif /* AGML_COM_H_ */
