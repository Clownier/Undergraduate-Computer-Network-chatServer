#ifndef DATABASEUTIL_H
#define DATABASEUTIL_H
#include"baseheader.h"

class DataBaseUtil
{
    QFile *qF,qFile;
    //QTextStream *qTS;
public:
    DataBaseUtil();
    //DataBaseUtil(const char *);
    ~DataBaseUtil();
    int openReadFile();
    int openWriteFile();
    static QString searchUuid(QString email,QString password);
    static bool searchUuid(QString email);
    static QString getAllUsersName(QString uuid);
    static int writeAtEnd(QString context);
    static void saveRegister(QJsonArray arr);
    static QString searchInfo(QString email);
    static QString searchPassword(QString Uuid);
};

#endif // DATABASEUTIL_H
