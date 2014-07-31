#include "frameio.h"
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>

using std::string;
using std::vector;
using cv::Mat;
using cv::resize;
using cv::imread;
using cv::imwrite;
using cv::VideoCapture;

FrameIO::FrameIO(const string& source)
{
    videoCapture = NULL;
    frameIdx = 0;

    if (source == "Camera") {
        videoCapture = new VideoCapture(0);
    } else {
        QFileInfo src(source.c_str());
        if (src.isDir()) {
            QDir dir(source.c_str());
            QStringList imgFilters;
            imgFilters << "*.bmp" << "*.jpg"<< "*.tiff" << "*.jpeg" << "*.png";
            dir.setNameFilters(imgFilters);
            QFileInfoList list(dir.entryInfoList());
            for (int i = 0; i < list.size(); ++i) {
                imgFiles.push_back(list[i].absoluteFilePath().toStdString());
            }
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
    videoCapture = NULL;
    imgFiles.clear();
    frameIdx = 0;
}

int FrameIO::getNrFrame()
{
    if (videoCapture != NULL) return -1;
    return imgFiles.size();
    return -1;
}

int FrameIO::readNextFrame(Mat& frame)
{
    if (videoCapture != NULL) {
        if (!videoCapture->isOpened()) return -1;
        (*videoCapture) >> frame;
        CvSize size = {0, 0};
        resize(frame, frame, size, 0.5, 0.5);
        return frameIdx ++;
    } else if (!imgFiles.empty()) {
        if (frameIdx >= imgFiles.size()) return -1;
        frame = imread(imgFiles[frameIdx]);
        return frameIdx ++;
    }
    return -1;
}

bool FrameIO::writeFrame(const Mat& frame, string& filePath)
{
    return imwrite(filePath, frame);
}
