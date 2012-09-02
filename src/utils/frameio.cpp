#include "frameio.h"

FrameIO::FrameIO(const QString &source)
{
    videoCapture = NULL;
    imgFiles = NULL;
    frameIdx = 0;

    if (source == "Camera") {
        videoCapture = new cv::VideoCapture(0);
    } else {
        QFileInfo src(source);
        if (src.isDir()) {
            QDir dir(source);
            QStringList imgFilters;
            imgFilters << "*.bmp" << "*.jpg"<< "*.tiff" << "*.jpeg" << "*.png";
            dir.setNameFilters(imgFilters);
            imgFiles = new QFileInfoList;
            *imgFiles = dir.entryInfoList();
        } else {

        }
    }
}

FrameIO::~FrameIO()
{
    release();
}

void FrameIO::release()
{
    delete videoCapture;
    delete imgFiles;
    videoCapture = NULL;
    imgFiles = NULL;
    frameIdx = 0;
}

int FrameIO::getNrFrame()
{
    if (videoCapture != NULL) return -1;
    if (imgFiles != NULL) return imgFiles->size();
    return -1;
}

int FrameIO::readNextFrame(cv::Mat &frame)
{
    if (videoCapture != NULL) {
        if (!videoCapture->isOpened()) return -1;
        (*videoCapture) >> frame;
        CvSize size = {0, 0};
        cv::resize(frame, frame, size, 0.5, 0.5);
        return frameIdx ++;
    } else if (imgFiles != NULL) {
        if (frameIdx >= imgFiles->size()) return -1;
        frame = cv::imread(imgFiles->at(frameIdx).filePath().toStdString());
        return frameIdx ++;
    }
    return -1;
}

bool FrameIO::writeFrame(const cv::Mat &frame, QString &filePath)
{
    return cv::imwrite(filePath.toStdString(), frame);
}
