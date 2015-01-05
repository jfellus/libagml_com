/*
 * Socket.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: neuro
 */

#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/poll.h>
#include "../util/utils.h"



///////////
// DEBUG //
///////////

#define SOCKET_DBG_READ(socket, buffer, n) do {  \
	__dbg_append_toFile((socket), "r : "); \
    if((n)==sizeof(int)) {__dbg_append_toFile((socket), "%i", *(int*) (buffer) ); } \
    else { __dbg_append_toFile((socket), (char*) (buffer), (n)); } \
    __dbg_append_toFile((socket), "\n") } while(false)

#define SOCKET_DBG_WRITE(socket, buffer, n) do { \
	__dbg_append_toFile(socket, "w : "); \
    if(n==sizeof(int)) {__dbg_append_toFile(socket, "%i", *(int*)buffer); } \
    else { __dbg_append_toFile(socket, (char*)buffer, n); } \
    __dbg_append_toFile(socket, "\n"); } while(false)





Socket::Socket(const char* ip, unsigned short int port) {
	readBlocking = true; readTimeout = 0;
	bConnected = false;

	strcpy(this->ip, ip);
	this->port = port;
	connect();
}


Socket::Socket(const char* url) {
	readBlocking = true; readTimeout = 0;
	bConnected = false;

	// Parse URL
	const char* sp;
	if((sp = strchr(url, ':'))) {
		port = atoi(sp+1);
		strncpy(this->ip, url, sp-url);
	} else {
		strcpy(this->ip, url);
		port = DEFAULT_PORT;
	}

	connect();
}

void Socket::connect() {
	// Create socket
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) throw("ERROR opening socket");

	int flag = 1;
	setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    // Create address
    struct hostent *server;
    if (!(server = gethostbyname(this->ip))) throw "ERROR, no such host";
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    // Connect socket to addr
    if (::connect(socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        throw "ERROR connecting";

    bConnected = true;
}

Socket::Socket(int socket, struct sockaddr_in* addr, size_t len) {
	readBlocking = true; readTimeout = 0;
	int flag = 1;
	setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
	this->socket = socket;
	strcpy(this->ip, inet_ntoa(addr->sin_addr));
	this->port = ntohs(addr->sin_port);

	bConnected = true;
}

Socket::~Socket() {
	shutdown(socket, SHUT_RDWR);
	close(socket);
}


void Socket::write(void* buffer, size_t size) {
    int n = ::write(socket,buffer,size);
    if (n < 0) throw "ERROR writing to socket";

#ifdef SOCKET_DEBUG
    SOCKET_DBG_WRITE(socket, buffer, n);
#endif
}


bool Socket::waitForMsg(int timeout_ms) {
	struct pollfd pfd;
	pfd.fd = socket; pfd.events = POLLIN;
	poll(&pfd, 1, timeout_ms);
	return pfd.revents & POLLIN;
}


size_t Socket::read(void* buffer, size_t maxSize) {
	int n;
	if(!readBlocking && readTimeout>0) if(!waitForMsg(this->readTimeout)) throw "Timeout exceeded";
	n = ::recv(socket,buffer,maxSize, readBlocking ? 0 : MSG_DONTWAIT);
	if (n < 0) throw "ERROR reading from socket";

#ifdef SOCKET_DEBUG
		SOCKET_DBG_READ(socket, buffer, n);
#endif
    return n;
}

void Socket::writeFile(const char* filename) {
	struct stat stat_buf;
	int f = open(filename, O_RDONLY);
		fstat(f, &stat_buf);
		write(&(stat_buf.st_size), sizeof(stat_buf.st_size));
		sendfile(socket, f, NULL, stat_buf.st_size);
	close(f);
	usleep(20000);
}

void Socket::readFile(const char* out_filename) {
	char buf[1024];
	size_t n;
	read(&n, sizeof(n));

	DBG("File is of size " << n);

	FILE* f = fopen(out_filename, "w");

	while(n>0) {
		size_t nn = read(buf, sizeof(buf));
		n -= nn;
		fwrite(buf,1,nn,f);
		DBG(n << " remaining");
	}

	fclose(f);
}

bool Socket::readAck() {
	char ack[256];
	try{read(ack, 256);}catch(const char* s) {ERROR(s << " ACK reception timeout"); throw s;}
	return !strcmp(ack, "OK");
}
