#include "appmanager.h"

AppManager::AppManager(const QFileInfoList &files,
                       const QString &type, const QString &params,
                       const QString &outputFileName, QObject *parent) :
    QObject(parent),
    files(files),
    type(type),
    params(params),
    outputFileName(outputFileName)
{
}

void AppManager::work()
{
    if (type == "Coherent Filter") {

    }

}
