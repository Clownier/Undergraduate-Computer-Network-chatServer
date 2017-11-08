#include "clientsocket.h"
ClientSocket::ClientSocket(SOCKET socket,sockaddr_in addr):
    clientSocket(socket),
    clientAddr(addr)
{
}
//vector<QString> DataBaseUtil::offline;
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
    if(nRC == SOCKET_ERROR||nRC == 0)
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

QString ClientSocket::carry(){//客户端链接服务器后预处理：注册登陆找回密码
    int result = 0;
    while(0 == result){
        QString prepro;
        while(prepro.length()==0)
            prepro = Qrecv();
        QJsonParseError *error = new QJsonParseError;
        QJsonArray qArr = QJsonDocument::fromJson(prepro.toLatin1(),error).array();
        qDebug()<<qArr;
        switch (qArr.at(0).toInt(-1)) {
        case 111:result = tologin(qArr.at(1).toString(),qArr.at(2).toString());break;
        case 600:result = toregister(qArr);break;
        case 710:
        case 700:result = toretrieve(qArr);break;
        default:break;
        }
    }
    qDebug()<<inet_ntoa(clientAddr.sin_addr)<<":"<<clientAddr.sin_port<<"login success!";


    vector<QString>::iterator iter = DataBaseUtil::offline.begin();
    for(;iter!=DataBaseUtil::offline.end();iter++){
        Qsend(*iter);
    }


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


//    QFile UserFile("history2.txt");//发送离线消息
//    if(!UserFile.open(QIODevice::ReadOnly|QIODevice::Text)){
//        qDebug()<<"open history File fail!";
//        return "";
//    }
//    QTextStream txtOutput(&UserFile);
//    while(!txtOutput.atEnd()){
//        QString userInfo = txtOutput.readLine().toLatin1();
//        int index = userInfo.indexOf(Uuid);
//        if(index!=-1){
//            QJsonArray info = QJsonDocument::fromJson(userInfo.toLatin1(),NULL).array();
//            QString aimUuid = info.at(1).toString();
//            QString MyUuid = info.at(2).toString();
//            QString text = info.at(3).toString();
//            QJsonArray arr;arr.insert(0,257);
//            arr.insert(1,aimUuid);arr.insert(2,MyUuid);arr.insert(3,text);
//            QJsonDocument doc;doc.setArray(arr);
//            QString send = doc.toJson(QJsonDocument::Compact);
//            Qsend(send);
//        }
//    }
//    UserFile.close();
//    QJsonArray arr;arr.insert(0,256);
//    QJsonDocument doc;doc.setArray(arr);
//    Qsend(QString(doc.toJson(QJsonDocument::Compact)));

    return Uuid;
    //doRequest();
}

int ClientSocket::doRequest(){//处理用户发送消息等功能
    while(1){//
        QString req = Qrecv();
        if(req.length() == 0)
            continue;
        QJsonParseError *error = new QJsonParseError;
        QJsonArray array = QJsonDocument::fromJson(req.toLatin1(),error).array();
        if(array.at(0).toInt() == 584){
//            ClientSocket *obj = FindByUuid(array.at(1).toString());
//                qDebug()<<obj->Uuid.toStdString().data();
            if(0!=DataBaseUtil::writeAtEnd(req))//保存到历史记录中
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
        QString byteArr = document.toJson(QJsonDocument::Compact);
        Qsend(byteArr);
        return 0;
    }
    qDebug()<<Uuid;
    QJsonArray arr;arr.insert(0,1);
    arr.insert(1,Uuid);arr.insert(2,inet_ntoa(clientAddr.sin_addr));
    arr.insert(3,clientAddr.sin_port);
    QJsonDocument document;document.setArray(arr);
    QString byteArr = document.toJson(QJsonDocument::Compact);
    Qsend(byteArr);
    return 1;
}

int ClientSocket::toregister(QJsonArray qArr){
    if(DataBaseUtil::searchUuid(qArr.at(6).toString())){
        qArr.replace(0,602);
        QJsonDocument document;document.setArray(qArr);
        QString byteArr = document.toJson(QJsonDocument::Compact);
        Qsend(byteArr);
    }else{
        DataBaseUtil::saveRegister(qArr);
        qArr.replace(0,601);
        QJsonDocument document;document.setArray(qArr);
        QString byteArr = document.toJson(QJsonDocument::Compact);
        Qsend(byteArr);
    }
    return 0;
}

int ClientSocket::toretrieve(QJsonArray qArr){
    if(qArr.at(0).toInt() == 700){
        if(DataBaseUtil::searchUuid(qArr.at(1).toString())){
            QString info = DataBaseUtil::searchInfo(qArr.at(1).toString());
            QJsonArray infoArr = QJsonDocument::fromJson(info.toLatin1(),NULL).array();
            QJsonArray arr;arr.insert(0,701);
            arr.insert(1,infoArr.at(3).toString());
            arr.insert(2,infoArr.at(4).toString());
            arr.insert(3,infoArr.at(0).toString());
            QJsonDocument doc;doc.setArray(arr);
            info = doc.toJson(QJsonDocument::Compact);
            Qsend(info);
        }
        else{
            QJsonArray arr;arr.insert(0,702);
            QJsonDocument doc;doc.setArray(arr);
            QString info = doc.toJson(QJsonDocument::Compact);
            Qsend(info);
        }
    }else{
        QJsonArray arr;arr.insert(0,710);
        arr.insert(1,DataBaseUtil::searchPassword(qArr.at(1).toString()));
        QJsonDocument doc;doc.setArray(arr);
        QString send = doc.toJson(QJsonDocument::Compact);
        Qsend(send);
    }
    return 0;
}
