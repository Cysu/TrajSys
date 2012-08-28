#include "modespage.h"
#include "mainwizard.h"

ModesPage::ModesPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Modes Options"));
    setSubTitle(tr("Specify the trajectory analysis mode."));


    onlineModeRadioButton = new QRadioButton(tr("O&nline"));
    offlineModeRadioButton = new QRadioButton(tr("O&ffline"));
    modesRadioButtonsGroupBox = new QGroupBox(tr("Modes"));
    offlineModeRadioButton->setChecked(true);


    QVBoxLayout *modesRadioButtonsGroupBoxLayout = new QVBoxLayout;
    modesRadioButtonsGroupBoxLayout->addWidget(onlineModeRadioButton);
    modesRadioButtonsGroupBoxLayout->addWidget(offlineModeRadioButton);
    modesRadioButtonsGroupBox->setLayout(modesRadioButtonsGroupBoxLayout);


    QGridLayout *modesPageLayout = new QGridLayout;
    modesPageLayout->addWidget(modesRadioButtonsGroupBox, 0, 0);
    setLayout(modesPageLayout);
}

void ModesPage::getMode(QString &mode)
{
    if (onlineModeRadioButton->isChecked()) mode = "Online";
    else if (offlineModeRadioButton->isChecked()) mode = "Offline";
}
