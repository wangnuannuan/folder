FROM debian:stable

MAINTAINER Jingru <1961295051@qq.com>
RUN apt-get update && apt-get install -y --no-install-recommends \
    # to build and install
    unzip git \
    build-essential cmake pkg-config \
    # to work with images
    libjpeg-dev libtiff5-dev \
    # to work with videos
    # libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    wget

RUN wget http://repo.continuum.io/miniconda/Miniconda3-latest-Linux-armv7l.sh \
    /Miniconda3-latest-Linux-armv7l.sh -b
