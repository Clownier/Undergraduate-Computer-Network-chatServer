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
QString DataBaseUtil::searchInfo(QString email){
    QFile UserFile("userFile.txt");
    if(!UserFile.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"open login file fail!";
        return "";
    }
    QTextStream txtOutput(&UserFile);
    QJsonParseError *error = new QJsonParseError;
    QByteArray user;
    QJsonArray infoArr;
    while(!txtOutput.atEnd()){
        user = txtOutput.readLine().toLatin1();
        infoArr = QJsonDocument::fromJson(user,error).array();
        if(infoArr.at(2).toString()==email){
            UserFile.close();
            return user;
        }
    }
    UserFile.close();
    return "";
}

bool DataBaseUtil::searchUuid(QString email){
    QFile loginFile("loginFile.txt");
    if(!loginFile.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"open login file fail!";
        return false;
    }
    QTextStream txtOutput(&loginFile);
    QJsonParseError *error=new QJsonParseError;
    QByteArray user;
    QJsonArray infoArr;
    while(!txtOutput.atEnd()){
        user = txtOutput.readLine().toLatin1();
        infoArr = QJsonDocument::fromJson(user,error).array();
        if(infoArr.at(0).toString()==email){
            loginFile.close();
            return true;
        }
    }
    loginFile.close();
    return false;
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
QString DataBaseUtil::searchPassword(QString Uuid){
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
        if(infoArr.at(2).toString()==Uuid){
            loginFile.close();
            return infoArr.at(1).toString();
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
vector<QString> DataBaseUtil::offline;
int DataBaseUtil::writeAtEnd(QString context){
    offline.push_back(context);
    QFile writeFile("history2.txt");
    if(!writeFile.open(QIODevice::Append|QIODevice::Text|QIODevice::WriteOnly)){
        qDebug()<<"open history fail!";
        return -1;
    }
    QTextStream txtInput(&writeFile);
    txtInput<<context<<endl;
    return 0;
}

void DataBaseUtil::saveRegister(QJsonArray arr){
    QString Uuid = QUuid::createUuid().toString();
    QFile writeHis("loginFile.txt");
    if(!writeHis.open(QIODevice::Append|QIODevice::Text|QIODevice::WriteOnly)){
        qDebug()<<"open history fail!";
        return;
    }
    QTextStream txtInHis(&writeHis);
    QJsonArray loginarr;
    loginarr.insert(0,arr.at(2).toString());
    loginarr.insert(1,arr.at(3).toString());
    loginarr.insert(2,Uuid);
    QJsonDocument doc;doc.setArray(loginarr);
    QString login = doc.toJson(QJsonDocument::Compact);
    txtInHis<<login<<endl;
    writeHis.close();


    QFile writeUser("userFile.txt");
    if(!writeUser.open(QIODevice::Append|QIODevice::Text|QIODevice::WriteOnly)){
        qDebug()<<"open history fail!";
        return;
    }
    QTextStream txtInUser(&writeUser);
    QJsonArray Userarr;
    Userarr.insert(0,Uuid);
    Userarr.insert(1,arr.at(1).toString());
    Userarr.insert(2,arr.at(2).toString());
    Userarr.insert(3,arr.at(4).toString());
    Userarr.insert(4,arr.at(5).toString());
    QJsonDocument doc1;doc1.setArray(Userarr);
    QString User = doc1.toJson(QJsonDocument::Compact);
    txtInUser<<User<<endl;
    writeUser.close();
}
