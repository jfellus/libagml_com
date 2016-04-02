/*
 * Node.h
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#ifndef NODE_NODE_H_
#define NODE_NODE_H_

#include "../util/utils.h"
#include "../util/array.h"

class Node {
public:
	long id;
	array<long> neighbors;
	bool bFullyConnected = false;
public:
	Node();
	virtual ~Node();

	void iteration();

	void send(int iNeighbor, char* data, size_t size);
	virtual void on_receive(char* data, size_t size) = 0;

	virtual void process() = 0;


	bool is_fully_connected() 	{	return bFullyConnected;	}
	void connect_all() 			{ neighbors.clear(); bFullyConnected = true; }
	void disconnect_all() 		{ neighbors.clear(); bFullyConnected = false; }
	void connect(long id) 		{ bFullyConnected = false;		neighbors.add(id);	}
};

#endif /* NODE_NODE_H_ */
