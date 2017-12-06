#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "readfile.h"


#include "signal.h"

#include "qprocess.h"
#include <QDesktopWidget>

using namespace std;

//using namespace agora::ServerSDK2;


bool quit = false;
void term_handler(int sig_no)
{
    qDebug()<<"term_handler called"<<endl;
    quit = true;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (ui)
    {
        delete ui;
        ui = nullptr;
    }
    if (m_widget)
    {
        qDebug()<<"delete widget!"<<endl;
        delete m_widget;
        m_widget = nullptr;

    }
}

void MainWindow::on_pushButton_clicked()
{

    std::string config = "./config.ini";
    ReadConfig fConfig(config);

    fConfig.readText();

    //./demo --appId STRING --channelKey STRING --channel STRING --url STRING
#if 0
    QString program2 = "./myqt";

    std::string appid = fConfig.getAppId();
    std::string channelKey = "";
    std::string channel = fConfig.getChannel();
    std::string cameraId = "0";
    cameraId = fConfig.getCameraId();
    std::string live = fConfig.getLive();

    QStringList arguments2;
    arguments2 << "--appId" << appid.c_str()<<"--channel"<<channel.c_str() <<"--cameraId"<<cameraId.c_str()<<"--live"<< live.c_str();

    QProcess *myProcess2 = new QProcess(this);
    myProcess2->start(program2, arguments2);
#endif
#if 0
    //QObject *parent;
    QString program = "tar";
    QStringList arguments;
    arguments << "czvf" << "backup.tar.gz" << "/home/pi/Agora_WaWa/test";
//    QProcess *myProcess = new QProcess(this);
//    QProcess->start(program, arguments);
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
#endif
#if 1
    //QObject *parent;
    QString program = "./pushDemo";

    std::string appid = fConfig.getAppId();
    std::string channelKey = "";
    std::string channel = fConfig.getChannel();
    std::string cameraId = "0";
    cameraId = fConfig.getCameraId();
    std::string live = fConfig.getLive();

    QStringList arguments;
    arguments << "--appId" << appid.c_str()<<"--channel"<<channel.c_str() <<"--cameraId"<<cameraId.c_str()<<"--live"<< live.c_str();
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    qDebug()<<"fConfig appid:"<<fConfig.getAppId().c_str()<<endl;
    qDebug()<<"fps:"<<fConfig.getFPS().c_str()<<",live:"<<live.c_str()<<"cameraId:"<<cameraId.c_str()<<",channel:"<<channel.c_str()<<endl;

    if(!myProcess->waitForStarted(12))
    {
        qDebug()<<"not Started"<<endl;
    }
    else
    {
        qDebug()<<"Started"<<endl;
    }
    if(myProcess->waitForFinished(6))
    {
        qDebug()<<"finished!"<<endl;
        if(0 == myProcess->exitCode())
        {
            qDebug()<<"normal"<<endl;
        }
        else
        {
            qDebug()<<"error"<<endl;
        }
    }
    else
    {
        qDebug()<<"timeout"<<endl;
    }
#endif
    // setup signal handler
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT,  term_handler);
    signal(SIGTERM, term_handler);


    int ret = 0; /*ChatEng.Init(eConfig);*/
    if(ret == 0)
    {
        this->hide();
        m_widget = new Widget(this);

        m_widget->resize(600,600);
        m_widget->move((QApplication::desktop()->width() - m_widget->width())/2,
                (QApplication::desktop()->height()- m_widget->height())/2);
        m_widget->show();
    }
}



