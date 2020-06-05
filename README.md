# PiCam

## How to run

## Clone
```bash
git clone https://github.com/kilpkonn/PiCam.git && cd PiCam
```
### Build OpenCV binaries or development
```bash
./script/install-opencv.sh
```
### Build armhf binaries
```bash
sudo bash ./script build-in-docker.sh
```
### Copy build armhf binaries to Raspberry
```bash
scp -r build pi@picam.local:~/
```
### Run install script on Raspberry
```bash
ssh pi@picam.local
sudo bash ~/build/pi-setup.sh
```
### Cleanup after reboot
```bash
sudo rm -rf ~/build
```

### OpenCV installation
See [this](https://www.learnopencv.com/install-opencv-4-on-ubuntu-18-04/) tutorial.  
Another great tutorial for cross compiling can be found [here](https://solarianprogrammer.com/2018/12/18/cross-compile-opencv-raspberry-pi-raspbian/).