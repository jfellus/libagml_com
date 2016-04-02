/*
 * host.h
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#ifndef AGML_HOST_H_
#define AGML_HOST_H_

#include "tcp/Socket.h"

class Host {
public:
	int id;
public:
	Host() { id = 0; }
	Host(Socket* s) {}

	~Host() {

	}


	long create_node_id();


private:
	int next_node_id = 0;
};



#endif /* AGML_HOST_H_ */
