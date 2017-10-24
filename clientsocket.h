#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include "baseheader.h"
using namespace std;
class ClientSocket{

public:
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    ClientSocket(SOCKET socket,sockaddr_in addr);
    ~ClientSocket();
    int send(string sendbuf);
    int send();
    void close();
    string recv();
    void carry();
    char sendBuf[BUFLEN], recvBuf[BUFLEN],error[BUFLEN];
};

#endif // CLIENTSOCKET_H
