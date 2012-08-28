#ifndef APPSPAGE_H
#define APPSPAGE_H

#include <QtGui>

class AppsPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit AppsPage(QWidget *parent = 0);

    void release();

    void initializePage();

    void getApps(QStringList &appsTypes,
                 QStringList &appsParams);

private:
    int nrSources;
    QLabel *sourcesLabels;
    QComboBox *appsTypesComboBoxes;
    QLineEdit *appsParamsLineEdits;

    
};

#endif // APPSPAGE_H
