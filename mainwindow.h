#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include "serversocket.h"
#include "clientsocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getHostInfoMation();

private:
    Ui::MainWindow *ui;
    QLineEdit *lineEditAddress,*lineEditLoacalHostName;
    void carryClient(ClientSocket socket);
};

#endif // MAINWINDOW_H
