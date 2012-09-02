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
                 QStringList &appsParams,
                 QVector<bool> &needDisplay);

private:
    int nrSources;
    QLabel *sourcesLabels;
    QCheckBox *needDisplayCheckBoxes;
    QComboBox *appsTypesComboBoxes;
    QLineEdit *appsParamsLineEdits;

    
};

#endif // APPSPAGE_H
