//
//    Copyright 2021 Christopher D. McMurrough
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

/*******************************************************************************************************************//**
 * @file program3.cpp
 * @brief Stationary traffic camera video - count eastbound and westbound cars with bounding boxes around the cars 
 * @author Edrik Aguilera
 **********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <cstdio>
#include "vehiclecounter.h"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1

/*******************************************************************************************************************//**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Edrik A. Aguilera
 **********************************************************************************************************************/
int main(int argc, char **argv)
{
    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }
    else
    {
    
        // Create a new vehicle counter using the provided video file
        VehicleCounter vehicleCounter{argv[1]};

        // If the file failed to open then exit the program
        if (vehicleCounter.failed)
        {			
            std::cout << "Unable to open video source" << std::endl;
            return 0;
        }

        // Process the provided video
        vehicleCounter.Run();
    }
}
