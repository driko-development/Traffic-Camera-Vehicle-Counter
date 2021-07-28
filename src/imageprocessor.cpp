#include "imageprocessor.h"

ImageProcessor::ImageProcessor(VideoFileInfo *vfInfo) : videoFile{vfInfo}
{
    // MOG2 Settings - Constructor parameters
    const int bgHistory = 250;
    const float bgThreshold = 12;
    const bool bgShadowDetection = false;

    // Initialize the MOG2 background subtractor
    pMOG2 = cv::createBackgroundSubtractorMOG2(bgHistory,bgThreshold,bgShadowDetection);


    // Set the points used for the traffic lines
    // if the center of the rectangle crosses this area then increase the count
    firstROIPoint = cv::Point((videoFile->width / 4), 0);
    secondROIPoint = cv::Point((videoFile->width/ 4) + 25, (videoFile->height / 2) - 110);
    regionOfInterest = cv::Rect(firstROIPoint, secondROIPoint);
}

void ImageProcessor::ProcessImage(cv::Mat *currentFrame)
{   
    // Create a grayscale image from the current frame
    cv::Mat imgGray;
    cv::cvtColor(*currentFrame, imgGray, cv::COLOR_BGR2GRAY);

    // Apply Gaussian Blur
    cv::Mat imgBlur;
    cv::GaussianBlur(imgGray, imgBlur, cv::Size(5,5), 0);

    // Normalize parameters
    const int rangeMin = 0;
    const int rangeMax = 255;

    // Apply normalization to create new image  
    cv::Mat imgNorm;
    cv::normalize(imgBlur, imgNorm, rangeMin, rangeMax, cv::NORM_MINMAX, CV_8UC1);

    // extract the foreground mask from image
    //  this will create the outlines of the cars in B&W
    cv::Mat imgMask;
    pMOG2->apply(imgNorm, imgMask);

    // Canny Edge Settings - Threshold changes the amount of edges discovered
    const int cannyThreshold1 = 50;
    const int cannyThreshold2 = 100;
    const int cannyAperture = 3;     

    // Apply the canny edge detector to new image 
    cv::Mat imgCanny;
    cv::Canny(imgMask, imgCanny, cannyThreshold1, cannyThreshold2, cannyAperture);

     // erode and dilate the edges to remove noise
    int morphologySize = 1;
    cv::Mat imgDil;
    cv::dilate(imgCanny, imgDil, cv::Mat(), cv::Point(-1, -1), morphologySize); 

    cv::Mat imgErode;
    cv::erode(imgDil, imgErode, cv::Mat(), cv::Point(-1, -1), morphologySize);
    
    // Vector to hold the contours that were found in the image
    std::vector<std::vector<cv::Point> > contours;

    // locate the image contours (after applying a threshold or canny)
    // Using RETR_EXTERNAL we grab only the outer contours
    cv::findContours(imgDil, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // Will hold the approximation curve for the bounding box
    std::vector<std::vector<cv::Point> > contoursPoly{contours.size()};

    // The list of bounding boxes
    std::vector<cv::Rect> boundRect{ contours.size()};

    // With contours ready create copy of the latest processed image
    cv::Mat imgContour = imgErode.clone();

    // Draw the rectangle for the ROI just for visual - not included in release
    //cv::rectangle(imgContour, regionOfInterest, cv::Scalar(255,0,0), 3);

    // For each contour we only deal with areas that are large enough to be a vehicle
    // Then we create bounding boxes via polygon approximation
    // Again only boxes that are large enough to be a vehicle should be considered
    for(int i = 0; i < contours.size(); i++)
    {
        if(cv::contourArea(contours[i]) > 950)
        {            
            // Get the approximation curve
            cv::approxPolyDP(cv::Mat(contours[i]), contoursPoly[i], 3, true);

            // Create the bounding box
            boundRect[i] = cv::boundingRect(cv::Mat(contoursPoly[i]));

            if (boundRect[i].height >= 125 && boundRect[i].width >= 170 && boundRect[i].width < 700) 
            {                
                // Color will be green for westbound and red for eastbound
                cv::Scalar color = cv::Scalar(255, 255, 255);

                // The center point of the current bounding box
                cv::Point center = cv::Point((boundRect[i].x + boundRect[i].width)/2, (boundRect[i].y + boundRect[i].height)/2);
                
                // Check if the center is in the upper half of the image
                if (0 < center.y && center.y < (imgContour.size().height / 2) - 265)
                {
                    color = cv::Scalar(0,255,0); // set color to green

                    // Check if within the ROI
                    if (firstROIPoint.x < center.x && center.x < secondROIPoint.x)
                    {
                        videoFile->IncreaseWestBoundCount();
                        videoFile->DisplayTrafficCount();
                    }
                }
                else
                {
                    color = cv::Scalar(0,0,255); // set color to red

                    // Check if within the ROI
                    if (firstROIPoint.x < center.x && center.x < secondROIPoint.x)
                    {
                        videoFile->IncreaseEastBoundCount();
                        videoFile->DisplayTrafficCount();
                    }
                }

                // Draw the rectangles onto the original image so it is displayed to the user
                rectangle( *currentFrame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            }            
        }
    }
}