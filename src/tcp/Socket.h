/*
 * Socket.h
 *
 *  Created on: Dec 31, 2012
 *      Author: neuro
 */

#ifndef AGML_TCP_SOCKET_H_
#define AGML_TCP_SOCKET_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 10001
//#define SOCKET_DEBUG // Uncomment for socket messages debug


#define SOCKET_WRITE_STRING_DELAY 10000



class Socket {
public:
	int socket;
private:
	bool bConnected;
	bool readBlocking;
	int readTimeout;
public:
	char ip[256];
	unsigned short int port;
public:
	Socket(const char* ip);
	Socket(const char* ip, unsigned short int port);
	Socket(int socket, struct sockaddr_in* addr, size_t len);
	virtual ~Socket();

	void setReadBlocking(bool rb) {this->readBlocking = rb;}
	void setReadTimeout(size_t timeout) {setReadBlocking(false); readTimeout = timeout;}

	bool isConnected() {return bConnected;}

	bool waitForMsg(int timeout_ms = -1);
	bool hasMsg() {return waitForMsg(0);}

	void write(void* buffer, size_t size);
	size_t read(void* buffer, size_t maxSize);

	inline void writeString(const char* str) {write((void*)str, strlen(str)+1); usleep(SOCKET_WRITE_STRING_DELAY);}

	void writeFile(const char* filename);
	void readFile(const char* out_filename);

	bool readAck();

	void ack() {writeString("OK");}


	void dump() {
		printf("%s:%i,%s", ip, port, readBlocking ? "block" : (readTimeout<0 ? "non-block" : "timeout"));
		if(!readBlocking && readTimeout>0) printf("(%ums)",readTimeout);
	}
protected:
	void connect();
};

#endif /* AGML_TCP_SOCKET_H_ */
