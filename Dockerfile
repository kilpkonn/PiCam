FROM debian:buster

ENV DEBIAN_FRONTEND noninteractive

VOLUME /export

ADD ./script/install-opencv-armh.sh /install-opencv-armh.sh

ENTRYPOINT ["/bin/bash", "./install-opencv-armh.sh"]
