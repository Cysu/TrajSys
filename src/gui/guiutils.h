#ifndef GUIUTILS_H
#define GUIUTILS_H

#define CREATE_ACTION(act, text, shortcut, func) \
    QAction *act = new QAction(tr(text), this); \
    if (shortcut != QString("")) act->setShortcut(tr(shortcut)); \
    connect(act, SIGNAL(triggered()), this, SLOT(func()));

#endif // GUIUTILS_H
