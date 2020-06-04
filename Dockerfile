FROM miguelbarroeprosima/raspfarm:base

VOLUME /export

ADD ./script/install-opencv-armh.sh /install-opencv-armh.sh

ENTRYPOINT ["/bin/bash", "./install-opencv-armh.sh"]
