/*
 * Server.h
 *
 *  Created on: Dec 31, 2012
 *      Author: neuro
 */

#ifndef AGML_TCP_SERVER_H_
#define AGML_TCP_SERVER_H_

#include <pthread.h>
#include "Socket.h"


class Server {
public:
	pthread_t serverThread;
private:
	int sockfd;

	void (*connectionCallback)(Socket*);

	bool bClosed;
	int readTimeout;
	bool readBlocking;
public:
	Server(int port, void(*connectionCallback)(Socket*));
	virtual ~Server();

	void setReadBlocking(bool rb) {readBlocking = rb;}
	void setReadTimeout(int timeout) {setReadBlocking(false); readTimeout = timeout;}

	void close();
	void join();

	void run();
};

#endif /* AGML_TCP_SERVER_H_ */
