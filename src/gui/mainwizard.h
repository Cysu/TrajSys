#ifndef MAINWIZARD_H
#define MAINWIZARD_H

#include <QtGui>

#include "modepage.h"
#include "offlinefilelistpage.h"
#include "offlinetrackerpage.h"

class MainWizard : public QWizard
{
    Q_OBJECT

public:
    enum {
        PAGE_MODE,
        PAGE_OFFLINE_FILELIST,
        PAGE_OFFLINE_TRACKER
    };

    explicit MainWizard(QWidget *parent = 0);

    void accept();

    QStringList getOfflineFilePaths();

private:
    ModePage *modePage;
    OfflineFilelistPage *offlineFilelistPage;
    OfflineTrackerPage *offlineTrackerPage;
    
};

#endif // MAINWIZARD_H
