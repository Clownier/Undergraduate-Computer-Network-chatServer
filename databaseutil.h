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
    static QString getAllUsersName(QString uuid);
    static int writeAtEnd(QString context);
};

#endif // DATABASEUTIL_H
