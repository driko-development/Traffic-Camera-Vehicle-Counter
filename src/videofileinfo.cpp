#include "videofileinfo.h"

VideoFileInfo::VideoFileInfo()
{
}

VideoFileInfo::VideoFileInfo(cv::VideoCapture capture) : westboundCount{0}, eastboundCount{0}
{
    width = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    height =  static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps = static_cast<int>(capture.get(cv::CAP_PROP_FPS));
}

void VideoFileInfo::IncreaseEastBoundCount()
{
    eastboundCount++;
}

void VideoFileInfo::IncreaseWestBoundCount()
{
    westboundCount++;
}

void VideoFileInfo::DisplayTrafficCount()
{
    std::cout << "WESTBOUND COUNT: " << westboundCount << std::endl;
    std::cout << "EASTBOUND COUNT: " << eastboundCount << std::endl;
}