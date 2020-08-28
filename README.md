# PiCam
![](https://github.com/kilpkonn/PiCam/workflows/Build%20on%20raspberry/badge.svg)
## How to run
_Make sure you have data directory next to pi_cam executable!_
```bash
./pi_cam
```

### Some useful args
 - **--port [-p]** specifies port
 - **--height [-h]** specifies desired camera frame height
 - **--width [-w]** specifies desired camera frame width
 - **--blur [-b]** blurs background
 - **--grayscale [-g]** grayscale background
 
 **Example:**  
 ```bash
./picam --port 8090 --height 480 --width 720 -b -g 
```

### How to fake webcam
__See: https://snapcraft.io/fakecam__
```bash
sudo apt-get install v4l2loopback-dkms
 echo options v4l2loopback devices=1 video_nr=20 \
 card_label="fakecam" exclusive_caps=1 | sudo tee -a \
 /etc/modprobe.d/fakecam.conf
 echo v4l2loopback | sudo tee -a /etc/modules-load.d/fakecam.conf
 sudo modprobe -r v4l2loopback
 sudo modprobe v4l2loopback
```
Then run
```bash
ffmpeg -re -i http://picam.local:8080 -map 0:v -vf format=yuv420p -f v4l2 /dev/video20
```

## How to build

### Clone
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
scp -r build-armhf pi@picam.local:~/
```
### Run install script on Raspberry
```bash
ssh pi@picam.local
sudo bash ~/build/pi-setup.sh
```
_Note that you might have to enable Raspberry's cam as well using `raspi-config`_
### Cleanup after reboot
```bash
sudo rm -rf ~/build
```

### OpenCV installation
See [this](https://www.learnopencv.com/install-opencv-4-on-ubuntu-18-04/) tutorial.  
Another great tutorial for cross compiling can be found [here](https://solarianprogrammer.com/2018/12/18/cross-compile-opencv-raspberry-pi-raspbian/).