//
// Created by Tavo Annus on 28/05/2020.
//

#include <iostream>
#include <string>

#include "cam/picam.h"

int main(int argc, char** argv) {
    std::cout << "Starting PiCam..." << std::endl;

    bool blur = false;
    bool grayscale = false;
    int port = 8080;

    for (int i = 1; i < argc; i++) {
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        string arg(argv[i]);

        if (arg == "-p" || arg == "--port") {
            if (i < argc) {
                port = std::stoi(argv[i + 1]);
                i++;
                // TODO: Error handling for strings
            } else {
                std::cout << "Please specify port number! Example: -p 8080" << std::endl;
                return 1;
            }
        }

        if (arg == "-b" || arg == "--blur") {
            blur = true;
        } else if (arg == "-g" || arg == "--greyscale") {
            grayscale = true;
        }
    }

    PiCam piCam(0, port);
    piCam.setBlur(blur);
    piCam.setGrayscale(grayscale);

    piCam.startServer();
    piCam.run();
    piCam.stopServer();

    return 0;
}

