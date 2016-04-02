/*
 * Node.cpp
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#include "Node.h"
#include "../com.h"
#include "../host.h"

extern Host* this_host;

Node::Node() {
	id = this_host->create_node_id();
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

