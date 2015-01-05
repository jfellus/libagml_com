/*
 * Server.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: neuro
 */

#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Socket.h"
#include <netinet/in.h>
#include "../util/utils.h"

struct ConnectionCallbackData {
	Socket* socket;
	void (*callback)(Socket*);
};


static void* _runServer(void* s) {
	((Server*)s)->run();
	return 0;
}

static void* _onConnect(void* s) {
	struct ConnectionCallbackData* ccd = (struct ConnectionCallbackData*)s;
	ccd->callback(ccd->socket);
	return 0;
}


Server::Server(int port, void(*connectionCallback)(Socket*)) {
	readBlocking = true; readTimeout = 0;
	this->connectionCallback = connectionCallback;
	if(port==0) port = DEFAULT_PORT;
	bClosed = false;

	// Create socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) throw "ERROR opening server socket";

    // Create socket address
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    char yes = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));


    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
             throw "ERROR on binding";
    listen(sockfd,5);


    // Launch server thread
    pthread_create(&serverThread, NULL, _runServer, this);
}

Server::~Server() {
}

void Server::close() {
	bClosed = true;
	//pthread_cancel(serverThread);
	shutdown(sockfd, SHUT_RDWR);
	::close(sockfd);
}

void Server::join() {
	pthread_join(serverThread, NULL);
}

void Server::run() {
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    while(!bClosed) {
    	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    	if (newsockfd < 0) break;
    	pthread_t connectionThread;

    	struct ConnectionCallbackData* d = new struct ConnectionCallbackData;
    	d->socket = new Socket(newsockfd, &cli_addr, clilen);
    	d->socket->setReadBlocking(readBlocking);
    	if(!readBlocking) d->socket->setReadTimeout(readTimeout);
    	d->callback = connectionCallback;

    	pthread_create(&connectionThread, NULL, _onConnect, d);
    }
}
