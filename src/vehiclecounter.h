#ifndef __VEHICLECOUNTER_H
#define __VEHICLECOUNTER_H

#define DISPLAY_WINDOW_NAME "Traffic Counter"


#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "videofileinfo.h"
#include "imageprocessor.h"

class VehicleCounter
{
    public:
        VehicleCounter(std::string inputImage);
        void Run();     
        VideoFileInfo videoFile;        
        bool failed;
    private:
        cv::VideoCapture capture;
        ImageProcessor* processor;
        
};
#endif