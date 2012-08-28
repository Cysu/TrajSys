#include "appspage.h"
#include "mainwizard.h"
#include "utils/const.h"

AppsPage::AppsPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Apps"));
    setSubTitle(tr("Select the app and parameters for each file."));

    release();
}

void AppsPage::release()
{
    nrSources = 0;
    sourcesLabels = NULL;
    appsTypesComboBoxes = NULL;
    appsParamsLineEdits = NULL;
}

void AppsPage::initializePage()
{
    MainWizard *mainWizard = (MainWizard *)wizard();
    QStringList sources;
    mainWizard->getSources(sources);

    release();

    nrSources = sources.size();

    sourcesLabels = new QLabel[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        QFileInfo file(sources[i]);
        sourcesLabels[i].setText(file.fileName());
        sourcesLabels[i].setToolTip(file.absoluteFilePath());
    }

    appsTypesComboBoxes = new QComboBox[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        for (int j = 0; j < NR_APPS; j ++)
            appsTypesComboBoxes[i].addItem(APPS_TYPES[j]);
    }

    appsParamsLineEdits = new QLineEdit[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        appsParamsLineEdits[i].setText(APPS_DEFAULT_PARAMS[0]);
    }

    QGridLayout *subLayout = new QGridLayout;
    for (int i = 0; i < nrSources; i ++) {
        subLayout->addWidget(&sourcesLabels[i], i, 0);
        subLayout->addWidget(&appsTypesComboBoxes[i], i, 1);
        subLayout->addWidget(&appsParamsLineEdits[i], i, 2);
    }

    QWidget *subWidget = new QWidget;
    subWidget->setLayout(subLayout);

    QScrollArea *scrollPanel = new QScrollArea;
    scrollPanel->setWidget(subWidget);
    scrollPanel->setWidgetResizable(true);

    QGridLayout *wholeLayout = new QGridLayout;
    wholeLayout->addWidget(scrollPanel, 0, 0);

    delete layout();
    setLayout(wholeLayout);
}

void AppsPage::getApps(QStringList &appsTypes,
                      QStringList &appsParams)
{
    for (int i = 0; i < nrSources; i ++) {
        appsTypes.push_back(appsTypesComboBoxes[i].currentText());
        appsParams.push_back(appsParamsLineEdits[i].text());
    }
}
