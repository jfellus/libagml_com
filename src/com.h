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

#define NODE_HOST(node) ((int) (node >> 32))
#define NODE_LOCALID(node) ((int) node)



void com_init();


void com_connection_thread(Socket* socket);





#endif /* AGML_COM_H_ */
