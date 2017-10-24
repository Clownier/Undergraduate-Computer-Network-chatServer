#include "serversocket.h"

ServerSocket::ServerSocket(){
    //initial Socket
    result = WSAStartup(0x0101,&wsaData);
    if(result)
        showErrorStr("Server initialize winsock error!");
    if(wsaData.wVersion!=0x0101){
        showErrorStr("Server's winsock version error!");
        WSACleanup();
    }
    qDebug()<<"Server's winsock initialized !\n";

    //create Socket
    serverSocket = socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket == INVALID_SOCKET){
        showErrorStr("Server create socket error!");
        WSACleanup();
    }
    qDebug()<<"Server TCP socket create OK!\n";

    //bind Socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    result = bind(serverSocket, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
    if(result == SOCKET_ERROR){
        showErrorStr("Server socket bind error!");
        closesocket(serverSocket);
        WSACleanup();
    }
    qDebug()<<"Server socket bind OK!\n";

    //initial Listen
    result = listen(serverSocket,MAXCONN);
    if(result == SOCKET_ERROR){
        showErrorStr("Server socket listen error!");
        closesocket(serverSocket);
        WSACleanup();
    }
    qDebug()<<"Server begin to listen!\n";

    //set serverSocket unBlock for listen client request
    u_long uNonBlock = 1;
   ioctlsocket(serverSocket, FIONBIO, &uNonBlock);

}

ServerSocket::~ServerSocket(){
    closesocket(serverSocket);
    WSACleanup();
}

void ServerSocket::showErrorStr(const QString error){
    errorStr.sprintf("%s%d\n",error,WSAGetLastError());
    showCritical(errorStr);
}

ClientSocket ServerSocket::poll(){
    //清空read,write套接字集合
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);

    //设置等待客户连接请求
    FD_SET(serverSocket, &readfds);
    //开始等待
    int nTotal = select(0, &readfds, &writefds, NULL, NULL);
    SOCKET connSocket;
    sockaddr_in client_Addr;
    if(FD_ISSET(serverSocket,&readfds)){
        connSocket = accept(serverSocket,
                            (LPSOCKADDR)&client_Addr,
                            &sizeofAddr);

        if (connSocket == INVALID_SOCKET)
        {
            showErrorStr("Server accept connection request error!");
            closesocket(serverSocket);
            WSACleanup();
        }
    }
    return ClientSocket(connSocket,client_Addr);
}

