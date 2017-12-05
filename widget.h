#ifndef WIDGET_H
#define WIDGET_H

#include <qdialog.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <QPushButton>


class Widget : public QWidget/*, public QObject*/
{
  Q_OBJECT

  friend class QTCBHandler;
public:
public slots :

signals:

public:
  //Widget(QWidget* parent = 0);
  Widget(QWidget* parent = 0);
  ~Widget();

  void prepareConnects();
  void prepareDatas();
  void prepareUI();
public slots :
  void leaveChannel();

public:

  QPushButton* m_leaveBtn;

//protected:

private:


};


#endif // WIDGET_H
