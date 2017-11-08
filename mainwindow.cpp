#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QWidget *widget = new QWidget();
//    this->setCentralWidget(widget);

//    lineEditAddress = new QLineEdit;
//    lineEditLoacalHostName = new QLineEdit;

//    QGridLayout *mainLayout= new QGridLayout(this);
//    mainLayout->addWidget(new QLabel(tr("IP address: ")),1,0);
//    mainLayout->addWidget(lineEditAddress,1,1);
//    mainLayout->addWidget(new QLabel(tr("Local HostName: ")),0,0);
//    mainLayout->addWidget(lineEditLoacalHostName,0,1);

//    centralWidget()->setLayout(mainLayout);

    QString localHostName = QHostInfo::localHostName();
//    lineEditLoacalHostName->setText(localHostName);
    QHostInfo hostInfo = QHostInfo::fromName(localHostName);
    QList<QHostAddress> listAddress = hostInfo.addresses();
    QString Address;
    if(listAddress.length()>6){
//        lineEditAddress->setText(listAddress.at(6).toString());
        Address = listAddress.at(6).toString();
        qDebug()<<listAddress.at(6).toString();
    }else{
        Address = listAddress.last().toString();
        qDebug()<<listAddress.last().toString();
    }

    QLabel *per1 = new QLabel(Address, this);
    statusBar()->addPermanentWidget(per1);
    setWindowTitle("CoolChat服务器窗口");
//    for(int i =0;i<listAddress.length();i++)
//        qDebug()<<"the "<<i<<" address:"<<listAddress.at(i).toString()<<"\n";
//    //getHostInfoMation();
    std::thread serverThread(&MainWindow::getHostInfoMation,this);
    serverThread.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getHostInfoMation(){
    ServerSocket serverSocket;
    while(1){
        ClientSocket clientSocket = serverSocket.poll();
        std::thread clientThread(&MainWindow::carryClient,this,clientSocket);
        clientThread.detach();
    }
}


void MainWindow::carryClient(ClientSocket socket){
    QString address;
    address.sprintf( "the visitor from %s:%d login!\n",
                     inet_ntoa(socket.clientAddr.sin_addr),socket.clientAddr.sin_port);
    qDebug()<<address;
    ui->userListWidget->addItem(address.sprintf("%s:%d",
                                                inet_ntoa(socket.clientAddr.sin_addr),socket.clientAddr.sin_port));
//    ui->userListWidget->clear();
    QString Uuid = socket.carry();
    onLineCS.insert(Uuid,socket);
    while(1){
        QString req = socket.Qrecv();
        if(req.length() == 0)
            continue;
        QJsonParseError *error = new QJsonParseError;
        QJsonArray array = QJsonDocument::fromJson(req.toLatin1(),error).array();
        if(array.at(0).toInt()==583){
            QMap<QString,ClientSocket>::iterator res = onLineCS.find(array.at(1).toString());

            if(res != onLineCS.end()){
                ClientSocket *obj = &(res.value());
                qDebug()<<inet_ntoa(obj->clientAddr.sin_addr)<<":"<<obj->clientAddr.sin_port;
//                QJsonArray addrArray;
//                addrArray.insert(0,583);
//                addrArray.insert(1,inet_ntoa(socket.clientAddr.sin_addr));
//                addrArray.insert(2,socket.clientAddr.sin_port);
                QString Ip = inet_ntoa(obj->clientAddr.sin_addr);
                array.replace(1,Ip);
                array.insert(2,obj->clientAddr.sin_port);
                QJsonDocument doc;doc.setArray(array);
                QString send = doc.toJson(QJsonDocument::Compact);
                socket.Qsend(send);
                //qDebug()<<obj->Uuid.toStdString().data()<<"addr ="<<address;
            }else{
                QJsonArray addrArray;
                qDebug()<<"Aim user is lost";
                addrArray.insert(0,584);
                QJsonDocument doc;doc.setArray(addrArray);
                QString send = doc.toJson(QJsonDocument::Compact);
                socket.Qsend(send);
            }
        }
        else if(array.at(0).toInt() == 584){
            if(0!=DataBaseUtil::writeAtEnd(req))//保存到历史记录中
                qDebug()<<"write into file fail!";
        }
    }
}

