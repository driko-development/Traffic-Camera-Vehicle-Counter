#ifndef __VIDEOFILEINFO_H
#define __VIDEOFILEINFO_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

class VideoFileInfo
{
    public:
        VideoFileInfo();
        VideoFileInfo(cv::VideoCapture);
        int width;
        int height;
        int fps;
        void IncreaseWestBoundCount();
        void IncreaseEastBoundCount();
        void DisplayTrafficCount();
    private:
        int westboundCount;
        int eastboundCount;
};
#endif