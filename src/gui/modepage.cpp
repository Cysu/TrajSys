#include "modepage.h"
#include "mainwizard.h"

ModePage::ModePage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Mode Options"));
    setSubTitle(tr("Specify the trajectory analysis mode."));


    onlineModeRadioButton = new QRadioButton(tr("O&nline"));
    offlineModeRadioButton = new QRadioButton(tr("O&ffline"));
    modeRadioButtonsGroupBox = new QGroupBox(tr("&Modes"));

    offlineModeRadioButton->setChecked(true);


    QVBoxLayout *modeRadioButtonsGroupBoxLayout = new QVBoxLayout;
    modeRadioButtonsGroupBoxLayout->addWidget(onlineModeRadioButton);
    modeRadioButtonsGroupBoxLayout->addWidget(offlineModeRadioButton);
    modeRadioButtonsGroupBox->setLayout(modeRadioButtonsGroupBoxLayout);


    QGridLayout *modePageLayout = new QGridLayout;
    modePageLayout->addWidget(modeRadioButtonsGroupBox, 0, 0);
    setLayout(modePageLayout);
}

int ModePage::nextId() const
{
    if (onlineModeRadioButton->isChecked()) {
        return -1;
    } else if (offlineModeRadioButton->isChecked()) {
        return MainWizard::PAGE_OFFLINE_FILELIST;
    }
}
