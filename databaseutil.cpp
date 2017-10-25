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
