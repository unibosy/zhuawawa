#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "readfile.h"

#include "avengine.h"
#include "enginehandler.h"

#include "signal.h"

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


#if 0
    qDebug()<<"on_pushButton_clicked join"<<endl;

    std::string config = "config.ini";
    ReadConfig fConfig(config);

    fConfig.readText();

    AVEngineConfig eConfig;
    eConfig.appid = fConfig.getAppId();
    eConfig.channel = fConfig.getChannel();
    eConfig.logPath = ".";
    eConfig.role = CLIENT_ROLE_BROADCASTER;
    std::string live = fConfig.getLive();
    eConfig.profile = atoi(live.c_str())?(CHANNEL_PROFILE_LIVE_BROADCASTING):(CHANNEL_PROFILE_COMMUNICATION);
    eConfig.dual = false;

    AVEngine ChatEng(new EngineHandler);
    int ret = ChatEng.Init(eConfig);
    if(ret == 0)
    {
        this->hide();
        m_widget = new Widget(this);
        m_widget->show();
        m_widget->resize(600,600);
    }

    // setup signal handler
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT,  term_handler);
    signal(SIGTERM, term_handler);


#endif


#if 1
    std::string config = "config.ini";
    ReadConfig fConfig(config);

    fConfig.readText();

    qDebug()<<"fConfig appid:"<<fConfig.getAppId().c_str()<<endl;
    qDebug()<<"fps:"<<fConfig.getFPS().c_str()<<endl;

    // setup signal handler
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT,  term_handler);
    signal(SIGTERM, term_handler);

#endif

    int ret = 0; /*ChatEng.Init(eConfig);*/
    if(ret == 0)
    {
        this->hide();
        m_widget = new Widget(this);
        m_widget->show();
        m_widget->resize(600,600);
    }
}



