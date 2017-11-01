#include "clientsocket.h"

ClientSocket::ClientSocket(SOCKET socket,sockaddr_in addr):
    clientSocket(socket),
    clientAddr(addr)
{   }

ClientSocket::~ClientSocket(){  }

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
int ClientSocket::Qsend(QString buf){
    QString len = QString::number(buf.length(),10);
    sprintf(sendBuf,"%s,%s",len.toStdString().data(),buf.toStdString().data());
    return send();
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
QString ClientSocket::Qrecv(){//
    memset(recvBuf,'\0',BUFLEN);
    int nRC = ::recv(clientSocket,recvBuf,BUFLEN,0);
    QString res = QString::fromStdString(recvBuf);
    if(nRC == SOCKET_ERROR||res.length()==0)
        return "";
    int index = res.indexOf(",");
    int len = res.mid(0,index).toInt();
    qDebug()<<"len = "<<len<<"Qrecv() ="<<recvBuf<<"\n";
    return res.mid(index+1,len);
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
    qDebug()<<inet_ntoa(clientAddr.sin_addr)<<":"<<clientAddr.sin_port<<"login success!";
    QString UserInfo = DataBaseUtil::getAllUsersName(Uuid);
    qDebug()<<"userInfo ="<<UserInfo.toStdString().data();
    int num = UserInfo.length()/200;
    for(int i =0;i<=num;i++){
        QJsonArray arr;
        if(i<num)   arr.insert(0,258);//...ing
        else arr.insert(0,259);//...ed
        arr.insert(1,UserInfo.mid(i*200,200));
        QJsonDocument document;document.setArray(arr);
        Qsend(QString(document.toJson(QJsonDocument::Compact)));
    }
    doRequest();
}

int ClientSocket::doRequest(){//处理用户发送消息等功能
    while(1){//
        QString req = Qrecv();
        if(req.length() == 0)
            continue;
        QJsonParseError *error = new QJsonParseError;
        QJsonArray array = QJsonDocument::fromJson(req.toLatin1(),error).array();
        if(array.at(0).toInt() == 584){
            if(0!=DataBaseUtil::writeAtEnd(req))
                qDebug()<<"write into file fail!";
        }
    }
}

int ClientSocket::tologin(QString email, QString password){
    Uuid = DataBaseUtil::searchUuid(email,password);
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
    return 1;
}

int ClientSocket::toregister(QJsonArray qArr){
    Q_UNUSED(qArr);
    return 0;
}

int ClientSocket::toretrieve(QJsonArray qArr){
    Q_UNUSED(qArr);
    return 0;
}
