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
    wget \
    python-catkin-tools python3-dev python3-numpy

RUN wget http://repo.continuum.io/miniconda/Miniconda3-latest-Linux-armv7l.sh
COPY Miniconda3-latest-Linux-armv7l.sh
RUN ./Miniconda3-latest-Linux-armv7l.sh -b
    python -V
RUN mkdir ~/catkin_build_ws && cd ~/catkin_build_ws 
    catkin config -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.6m.so
    catkin config --install
    mkdir src
    cd src
    git clone -b melodic https://github.com/ros-perception/vision_opencv.git
    cd ~/catkin_build_ws
    catkin build cv_bridge
    source install/setup.bash --extend
    cd ~/catkin_ws/src
    git clone https://github.com/ros/ros
    cd ..
    catkin_make_isolated --install --pkg rosunit -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/melodic
