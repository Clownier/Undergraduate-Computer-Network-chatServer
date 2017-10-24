#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "baseheader.h"
#include "clientsocket.h"

#define SERVER_PORT 5050
#define MAXCONN 5
#define BUFLEN 255

using namespace std;
typedef list<SOCKET> ListConnect;
typedef list<SOCKET> ListConErr;
class ServerSocket{
    WSADATA wsaData;
    int result;
    QString errorStr;
    sockaddr_in serverAddr;
    int sizeofAddr = sizeof(sockaddr);

    FD_SET readfds,writefds;//point sockets that are waiting for check
    ListConnect connected_List;//store connected socket
    ListConnect::iterator connectList_itor;
    ListConErr conErrList;//store lose efficacy socket
    ListConErr::iterator conErrList_itor;

public:
    SOCKET serverSocket;
    ServerSocket();
    ~ServerSocket();
    ClientSocket poll();
    void showErrorStr(const QString error);
};

#endif // SERVERSOCKET_H
