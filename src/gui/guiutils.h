#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <opencv2/opencv.hpp>

#define CREATE_ACTION(act, text, shortcut, func) \
    QAction *act = new QAction(tr(text), this); \
    if (shortcut != QString("")) act->setShortcut(tr(shortcut)); \
    connect(act, SIGNAL(triggered()), this, SLOT(func()));

const cv::Scalar EXAMPLE_COLOR[6] = {
    CV_RGB(0,255,0),
    CV_RGB(0,0,255),
    CV_RGB(255,255,0),
    CV_RGB(255,0,255),
    CV_RGB(0,255,255),
    CV_RGB(255,0,0),
};

#endif // GUIUTILS_H
