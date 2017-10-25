#include "clientsocket.h"

ClientSocket::ClientSocket(SOCKET socket,sockaddr_in addr):
    clientSocket(socket),
    clientAddr(addr)
{

}

ClientSocket::~ClientSocket(){

}
int ClientSocket::send(string sendbuf){
    if (::send(clientSocket, sendbuf.c_str(), sendbuf.length() + 1, 0) == SOCKET_ERROR) {
        sprintf(error,"send error!\tcode:%d\n",WSAGetLastError());
        qDebug()<<error;
        return -1;
    }else
        return 1;
}
int ClientSocket::send(){
    qDebug()<<sendBuf;
    return send(sendBuf);
}
string ClientSocket::recv(){
    memset(recvBuf,'\0',BUFLEN);
    int nRC = ::recv(clientSocket, recvBuf, BUFLEN, 0);
    if(nRC == SOCKET_ERROR)
        return string("");
    recvBuf[nRC] = '\0';
    qDebug()<<"recvBuf = "<<recvBuf<<"\n";
    string res(recvBuf);
    return res;
}


void ClientSocket::carry(){//客户端链接服务器后预处理：注册登陆找回密码
    int result = 0;
    while(0 == result){
        string prepro = recv();
        while(prepro.length()==0)
            prepro = recv();
        QJsonParseError *error = new QJsonParseError;
        QString qStr= QString::fromStdString(prepro);
        QJsonArray qArr = QJsonDocument::fromJson(qStr.toLatin1(),error).array();
        qDebug()<<qArr;
        switch (qArr.at(0).toInt(-1)) {
        case 0:result = tologin(qArr.at(1).toString(),qArr.at(2).toString());break;
        case 1:result = toregister(qArr);break;
        case 2:result = toretrieve(qArr);break;
        default:break;
        }
    }


//    int index = login.find("#*#*2461538790*#*#");
//    if (index != string::npos) {//接收密码
//                string username = login.substr(0,index);
//                string password = login.substr(index + 18);
//                QString uuid = DataBaseUtil::searchUuid(username,password);
//                if(uuid.length()==0){
//                    strcpy(error,"userName or password error!login fail!");
//                    qDebug()<<error;
//                }
//                if (password.length() > 0 && username.length() > 0)
//                    qDebug() << "username = " << username.c_str() << ",password = " << password.c_str();
//                    //printf("username = %s,password = %s", username.c_str(), password);
//                sprintf(sendBuf,"#*#*2461538790*#*#id:7654321\0");
//                send();
//            }
}

int ClientSocket::tologin(QString email, QString password){
    QString Uuid = DataBaseUtil::searchUuid(email,password);
    if(Uuid.length()==0){
        QJsonArray arr;arr.insert(0,-1);
        arr.insert(1,"userName or password error!login fail!");
        QJsonDocument document;document.setArray(arr);
        QByteArray byteArr = document.toJson(QJsonDocument::Compact);
        sprintf(sendBuf,"%s",byteArr.toStdString().c_str());
        send();
        return 0;
    }
    qDebug()<<Uuid;
    QJsonArray arr;arr.insert(0,1);
    arr.insert(1,Uuid);
    QJsonDocument document;document.setArray(arr);
    QByteArray byteArr = document.toJson(QJsonDocument::Compact);
    sprintf(sendBuf,"%s",byteArr.toStdString().c_str());
    send();
    //QByteArray userInfo = DataBaseUtil::searchUser(Uuid);
    //qDebug()<<userInfo;
    return 1;
}

int ClientSocket::toregister(QJsonArray qArr){
    return 0;
}

int ClientSocket::toretrieve(QJsonArray qArr){
    return 0;
}
