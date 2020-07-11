//
// Created by Tavo Annus on 28/05/2020.
//

#include <iostream>
#include <string>

#include "../include/PiCam.h"

inline bool isNumeric(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

int main(int argc, char** argv) {
    std::cout << "Starting PiCam..." << std::endl;

    bool blur = false;
    bool grayscale = false;
    int port = 8080;
    int height = 720;
    int width = 1280;

    for (int i = 1; i < argc; i++) {
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        string arg(argv[i]);

        if (arg == "-p" || arg == "--port") {
            if (i < argc && isNumeric(argv[i + 1])) {
                port = std::stoi(argv[i + 1]);
                i++;
            } else {
                std::cout << "Please specify port number! Example: -p 8080" << std::endl;
                return 1;
            }
        } else if (arg == "-h" || arg == "--height") {
            if (i < argc && isNumeric(argv[i + 1])) {
                height = std::stoi(argv[i + 1]);
                i++;
            } else {
                std::cout << "No valid height given! Example: -h 720" << std::endl;
                return 1;
            }
        } else if (arg == "-w" || arg == "--width") {
            if (i < argc && isNumeric(argv[i + 1])) {
                width = std::stoi(argv[i + 1]);
                i++;
            } else {
                std::cout << "No valid width given! Example: -w 1280" << std::endl;
                return 1;
            }
        } else if (arg == "-b" || arg == "--blur") {
            blur = true;
        } else if (arg == "-g" || arg == "--greyscale") {
            grayscale = true;
        }
    }

    PiCam piCam(0, port);
    piCam.setBlur(blur);
    piCam.setGrayscale(grayscale);
    piCam.setFrameSize(width, height);

    piCam.startServer();
    piCam.run();
    piCam.stopServer();

    return 0;
}

