//
// Created by Tavo Annus on 28/05/2020.
//

#include <iostream>

#include "cam/picam.h"

int main(int argc, char** argv) {
    std::cout << "Starting PiCam..." << std::endl;

    PiCam piCam(0, 8080);
    piCam.startServer();
    piCam.run();
    piCam.stopServer();
    return 0;
}

