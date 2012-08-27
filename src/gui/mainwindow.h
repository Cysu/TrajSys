#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "mainwizard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openWizard();

private:
    void createMenu();

    MainWizard *mainWizard;
};

#endif // MAINWINDOW_H
