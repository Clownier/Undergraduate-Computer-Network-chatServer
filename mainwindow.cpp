#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget();
    this->setCentralWidget(widget);

    lineEditAddress = new QLineEdit;
    lineEditLoacalHostName = new QLineEdit;

    QGridLayout *mainLayout= new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("IP address: ")),1,0);
    mainLayout->addWidget(lineEditAddress,1,1);
    mainLayout->addWidget(new QLabel(tr("Local HostName: ")),0,0);
    mainLayout->addWidget(lineEditLoacalHostName,0,1);

    centralWidget()->setLayout(mainLayout);

    QString localHostName = QHostInfo::localHostName();
    lineEditLoacalHostName->setText(localHostName);
    QHostInfo hostInfo = QHostInfo::fromName(localHostName);
    QList<QHostAddress> listAddress = hostInfo.addresses();
    if(listAddress.length()>6){
        lineEditAddress->setText(listAddress.at(6).toString());
        qDebug()<<listAddress.at(6).toString();
    }else{
        lineEditAddress->setText(listAddress.last().toString());
        qDebug()<<listAddress.last().toString();
    }

    for(int i =0;i<listAddress.length();i++)
        qDebug()<<"the "<<i<<" address:"<<listAddress.at(i).toString()<<"\n";
    //getHostInfoMation();
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
        clientSocket.onlineCS.push_back(clientSocket);
        std::thread clientThread(&MainWindow::carryClient,this,clientSocket);
        //std::thread clientThread(&clientSocket.carry,this);
        clientThread.detach();
    }
}


void MainWindow::carryClient(ClientSocket socket){
    QString address;
    address.sprintf( "the visitor from %s:%d login!\n",
                     inet_ntoa(socket.clientAddr.sin_addr),socket.clientAddr.sin_port);
    qDebug()<<address;
    socket.carry();
}
