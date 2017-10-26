#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include "baseheader.h"
#include "databaseutil.h"
using namespace std;
class ClientSocket{
    int tologin(QString email,QString password);
    int toregister(QJsonArray qArr);
    int toretrieve(QJsonArray qArr);
    QString Uuid;
public:
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    ClientSocket(SOCKET socket,sockaddr_in addr);
    ~ClientSocket();
    int send(string sendbuf);
    int send();
    int Qsend(QString buf);
    void close();
    string recv();
    void carry();
    char sendBuf[BUFLEN], recvBuf[BUFLEN],error[BUFLEN];
};

#endif // CLIENTSOCKET_H
