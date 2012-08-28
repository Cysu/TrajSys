#include "trackerspage.h"
#include "utils/const.h"
#include "mainwizard.h"

TrackersPage::TrackersPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Trackers"));
    setSubTitle(tr("Specify the tracker and parameters for each file."));

    release();
}

void TrackersPage::release()
{
    nrSources = 0;
    sourcesLabels = NULL;
    trackersTypesComboBoxes = NULL;
    trackersParamsLineEdits = NULL;
}

void TrackersPage::initializePage()
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

    trackersTypesComboBoxes = new QComboBox[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        for (int j = 0; j < NR_TRACKERS; j ++)
            trackersTypesComboBoxes[i].addItem(TRACKERS_TYPES[j]);
    }

    trackersParamsLineEdits = new QLineEdit[nrSources];
    for (int i = 0; i < nrSources; i ++) {
        trackersParamsLineEdits[i].setText(TRACKERS_DEFAULT_PARAMS[0]);
    }

    QGridLayout *subLayout = new QGridLayout;
    for (int i = 0; i < nrSources; i ++) {
        subLayout->addWidget(&sourcesLabels[i], i, 0);
        subLayout->addWidget(&trackersTypesComboBoxes[i], i, 1);
        subLayout->addWidget(&trackersParamsLineEdits[i], i, 2);
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

void TrackersPage::getTrackers(QStringList &trackersTypes,
                               QStringList &trackersParams)
{
    for (int i = 0; i < nrSources; i ++) {
        trackersTypes.push_back(trackersTypesComboBoxes[i].currentText());
        trackersParams.push_back(trackersParamsLineEdits[i].text());
    }
}
