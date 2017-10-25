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
    //QTextStream
    return 0;
}
