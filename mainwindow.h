#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include "widget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int init();
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Widget* m_widget;


};

#endif // MAINWINDOW_H
