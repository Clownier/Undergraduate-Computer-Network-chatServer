#include "databaseutil.h"

DataBaseUtil::DataBaseUtil(){

}

//DataBaseUtil::DataBaseUtil(const char * name)
//{
//    qF = new File(name);

//    //fileStream()
//}
DataBaseUtil::~DataBaseUtil(){
    qF->close();
}
int DataBaseUtil::openReadFile(){
    if(!qF->open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"open read file error!";
        return -1;
    }
//    QTextStream qTextStream(qF);
//    qTS = &qTextStream;
    return 0;
}
int DataBaseUtil::openWriteFile(){
    if(!qF->open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"open write file error!";
        return -1;
    }
    QFile q;

    //QTextStream
    return 0;
}

QString DataBaseUtil::searchUuid(QString email, QString password){
    QFile loginFile("loginFile.txt");
    if(!loginFile.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"open login file fail!";
        return "";
    }
    QTextStream txtOutput(&loginFile);
    QJsonParseError *error=new QJsonParseError;
    QByteArray user;
    QJsonArray infoArr;
    while(!txtOutput.atEnd()){
        user = txtOutput.readLine().toLatin1();
        infoArr = QJsonDocument::fromJson(user,error).array();
        if(infoArr.at(0).toString()==email){
            if(infoArr.at(1).toString()==password){
                loginFile.close();
                return infoArr.at(2).toString();
            }else{
                loginFile.close();
                return "";
            }
        }
    }
    loginFile.close();
    return "";
}
QString DataBaseUtil::getAllUsersName(QString Uuid){
    //获取全部用户的信息，用#分隔本人姓名，用；分隔用户，以昵称{Uuid}形式保存
    QFile UserFile("userFile.txt");
    if(!UserFile.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"open user File fail!";
        return "";
    }
    QTextStream txtOutput(&UserFile);
    QJsonParseError *error = new QJsonParseError;
    QByteArray userInfo;
    QJsonArray infoarr;
    QString myName;
    QStringList strlist;
    while(!txtOutput.atEnd()){
        userInfo = txtOutput.readLine().toLatin1();
        infoarr = QJsonDocument::fromJson(userInfo,error).array();
        if(infoarr.at(0).toString()==Uuid){
            myName = infoarr.at(1).toString();
            continue;
        }
        QString info = infoarr.at(1).toString() + infoarr.at(0).toString();
        strlist.append(info);
    }
    QString res = strlist.join(";");
    res.append("#");
    res.append(myName);
    return res;
}

int DataBaseUtil::writeAtEnd(QString context){
    QFile writeFile("history.txt");
    if(!writeFile.open(QIODevice::Append|QIODevice::Text|QIODevice::WriteOnly)){
        qDebug()<<"open history fail!";
        return -1;
    }
    QTextStream txtInput(&writeFile);
    txtInput<<context<<endl;
    return 0;
}
