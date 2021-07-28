#ifndef __IMAGEPROCESSOR_H
#define __IMAGEPROCESSOR_H

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <cstdlib>
#include "videofileinfo.h"

class ImageProcessor
{
    public:
        ImageProcessor(VideoFileInfo *);
        void ProcessImage(cv::Mat *);
    private:
        cv::Ptr<cv::BackgroundSubtractor> pMOG2;
        VideoFileInfo *videoFile;
        cv::Point firstROIPoint;
        cv::Point secondROIPoint;
        cv::Rect regionOfInterest;
};
#endif