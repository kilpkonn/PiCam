//
// Created by Tavo Annus on 28/05/2020.
//

#include <iostream>

#include "cam/picam.h"

int main(int argc, char** argv) {
    std::cout << "Starting PiCam..." << std::endl;

    bool blur = false;
    bool grayscale = false;

    for (int i = 1; i < argc; i++) {
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        string arg(argv[i]);
        if (arg == "-blur") {
            blur = true;
        } else if (arg == "-gray") {
            grayscale = true;
        }
    }

    PiCam piCam(0, 8080);
    piCam.setBlur(blur);
    piCam.setGrayscale(grayscale);

    piCam.startServer();
    piCam.run();
    piCam.stopServer();

    return 0;
}

