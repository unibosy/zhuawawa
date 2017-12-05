#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget* parent)
{
  prepareDatas();
  prepareConnects();
  prepareUI();
}

Widget::~Widget()
{

}

void Widget::prepareDatas()
{
    m_leaveBtn = new QPushButton(this);
}
void Widget::prepareConnects()
{
    connect(this->m_leaveBtn, SIGNAL(clicked()), this, SLOT(leaveChannel()));
}


void Widget::prepareUI()
{
    m_leaveBtn->setObjectName("LeaveChannel");
    m_leaveBtn->setText("LeaveChannel");
    m_leaveBtn->setGeometry(QRect(250, 500, 110, 30));
    m_leaveBtn->show();
}


void Widget::leaveChannel()
{
    qDebug()<<"leaveChannel"<<endl;
    this->close();
}





