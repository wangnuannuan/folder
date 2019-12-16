FROM debian:stable

MAINTAINER Jingru <1961295051@qq.com>
RUN apt-get update && apt-get install -y --no-install-recommends \
    # to build and install
    unzip \
    build-essential cmake pkg-config \
    # to work with images
    libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev \
    # to work with videos
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev