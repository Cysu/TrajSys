#ifndef MODESPAGE_H
#define MODESPAGE_H

#include <QtGui>

class ModesPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ModesPage(QWidget *parent = 0);

    void getMode(QString &mode);

private:
    QRadioButton *offlineModeRadioButton;
    QRadioButton *onlineModeRadioButton;
    QGroupBox *modesRadioButtonsGroupBox;
    
};

#endif // MODESPAGE_H
