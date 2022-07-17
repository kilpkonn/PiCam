#pragma once

#include "piCam.h"


namespace picam {

class PiCamLite : public PiCam {
public:
    PiCamLite(const int& cameraIndex = 0, const int& port = 8080);

    bool run() override;
};

}
