/*
 * host.cpp
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#include "host.h"


long Host::create_node_id() {
	return (((long)id) << 48) | next_node_id++;
}
