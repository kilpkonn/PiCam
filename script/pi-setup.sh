#!/bin/bash

apt install libgtk-3-dev libcanberra-gtk3-dev
apt install libtiff-dev zlib1g-dev
apt install libjpeg-dev libpng-dev
apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
apt-get install libxvidcore-dev libx264-dev

echo "Extracting opencv..."
tar xfv opencv-4.3.0-armhf.tar.bz2
echo "Done, moving to /opt ..."
mv opencv-4.3.0 /opt
echo "Done."

echo "Moving opencv.pc..."
mv opencv.pc /usr/lib/arm-linux-gnueabihf/pkgconfig
echo "Done."

echo "Cleaning up..."
rm opencv-4.3.0-armhf.tar.bz2
echo "Done."

echo "Adding OpenCV to PATH..."
echo "export LD_LIBRARY_PATH=/opt/opencv-4.3.0/lib:$LD_LIBRARY_PATH" >> .bashrc
source .bashrc
echo "Done."

echo "Making symlinks..."
ln -s /opt/opencv-4.3.0/lib/python2.7/dist-packages/cv2 /usr/lib/python2.7/dist-packages/cv2
ln -s /opt/opencv-4.3.0/lib/python3.7/dist-packages/cv2 /usr/lib/python3/dist-packages/cv2
echo "Done."

echo "Reboot is required!"
echo "Rebooting..."
reboot