#include "vehiclecounter.h"

VehicleCounter::VehicleCounter(std::string fileName) : capture{fileName}
{
    if(!capture.isOpened())
    {
        failed = true;
    }
    else
    {
        // Defaults to false but explicitly assign false if it succeeds in opening  
        failed = false;      

        // Create the named windows for display
        cv::namedWindow(DISPLAY_WINDOW_NAME, cv::WINDOW_AUTOSIZE);

        // Save the dimensions and FPS from the captured file
        // the video file will hold the vehicle count
        videoFile = VideoFileInfo{capture};

        // Create a new image process using the video file created above
        // the processor will update the video file as it processes the frames
        processor = new ImageProcessor(&videoFile);
    }
}

void VehicleCounter::Run()
{   
    // The user should check but if they didnt make sure it doesnt run if failed to open
    if (failed)
        return;

    // process data until program termination
    bool doCapture = true;    
    int frameCount = 0;    
    int retryCount = 0;
    while(doCapture)
    {
        // openCV Matrix that will hold the captured frame
        cv::Mat captureFrame;
        
        // Attempt to open the file for reading
        bool captureSuccess = capture.read(captureFrame);

        if(captureSuccess)
        {
            // Add bounding boxes around vehicles and update vehicle counts
            processor->ProcessImage(&captureFrame);

            // Update the GUI
            cv::imshow(DISPLAY_WINDOW_NAME, captureFrame);

            // get the number of milliseconds per frame
            int delayMs = (1.0 / videoFile.fps) * 1000;

            // check for program termination
            if(((char) cv::waitKey(delayMs)) == 'q')
            {
                doCapture = false;
            }
        }
        else
        {
            retryCount++;
            if (retryCount > 3)
                break;

            std::printf("Unable to acquire image frame! \n");
            
        }
    }

    // Display the final count
    std::cout << std::endl << "Results:" << std::endl;
    videoFile.DisplayTrafficCount();

    // release program resources before returning
    capture.release();
    cv::destroyAllWindows();
}