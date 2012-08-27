#ifndef MODEPAGE_H
#define MODEPAGE_H

#include <QtGui>

class ModePage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ModePage(QWidget *parent = 0);

    int nextId() const;

private:
    QRadioButton *offlineModeRadioButton;
    QRadioButton *onlineModeRadioButton;
    QGroupBox *modeRadioButtonsGroupBox;
    
};

#endif // MODEPAGE_H
