#ifndef DATABASEUTIL_H
#define DATABASEUTIL_H
#include"baseheader.h"

class DataBaseUtil
{
    QFile *qF;
    //QTextStream *qTS;
public:
    DataBaseUtil();
    //DataBaseUtil(const char *);
    ~DataBaseUtil();
    int openReadFile();
    int openWriteFile();
};

#endif // DATABASEUTIL_H
