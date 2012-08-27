#include "mainwindow.h"
#include "guiutils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mainWizard(NULL)
{
    setWindowTitle(tr("Trajectory Analysis System"));
    showMaximized();

    createMenu();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createMenu()
{
    CREATE_ACTION(openWizardAction, "&Wizard", "Ctrl+N", openWizard);

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openWizardAction);

    menuBar()->addAction(openWizardAction);
}

void MainWindow::openWizard()
{
    mainWizard = new MainWizard;
    mainWizard->show();
}
