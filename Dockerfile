FROM debian:stable

MAINTAINER Jingru <1961295051@qq.com>
# RUN apt-get update && apt-get install -y --no-install-recommends \
#     # to build and install
#     unzip git \
#     build-essential cmake pkg-config \
#     # to work with images
#     libjpeg-dev libtiff5-dev \
#     wget 
#     # to work with videos
#     # libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
#     # wget \
#     # python-catkin-tools python3-dev python3-numpy

RUN apt-get remove python-*
    apt-get remove ros-* \
    ros-melodic-*
    apt-get autoremove
    apt update  && apt-get install -y python3 \
    python3-dev \
    python3-pip \
    build-essentia
    pip3 install rosdep \ 
    rospkg \
    rosinstall_generator \
    rosinstall \
    wstool \
    vcstools \
    catkin_tools \
    catkin_pkg
    rosdep init
    rosdep update
    cd ~
    mkdir ros_catkin_ws
    cd ros_catkin_ws
    catkin config --init -DCMAKE_BUILD_TYPE=Release --blacklist rqt_rviz rviz_plugin_tutorials librviz_tutorial --install
    rosinstall_generator desktop_full --rosdistro melodic --deps --tar > melodic-desktop-full.rosinstall
    wstool init -j8 src melodic-desktop-full.rosinstall
    export ROS_PYTHON_VERSION=3
    pip3 install -U -f https://extras.wxpython.org/wxPython4/extras/linux/gtk3/ubuntu-18.04 wxPython

COPY install_skip.sh .
RUN ./install_skip.sh `rosdep check --from-paths src --ignore-src | grep python | sed -e "s/^apt\t//g" | sed -z "s/\n/ /g" | sed -e "s/python/python3/g"`
    rosdep install --from-paths src --ignore-src -y --skip-keys="`rosdep check --from-paths src --ignore-src | grep python | sed -e "s/^apt\t//g" | sed -z "s/\n/ /g"`"
    find . -type f -exec sed -i 's/\/usr\/bin\/env[ ]*python/\/usr\/bin\/env python3/g' {} +
    catkin build
    echo "export PYTHONPATH=/usr/lib/python3/dist-packages" >> ~/.bashrc
    echo "source ~/ros_catkin_ws/install/setup.bash" >> ~/.bashrc

# RUN wget http://repo.continuum.io/miniconda/Miniconda3-latest-Linux-armv7l.sh
# COPY Miniconda3-latest-Linux-armv7l.sh .
# RUN ./Miniconda3-latest-Linux-armv7l.sh -b
#     python -V
# RUN mkdir ~/catkin_build_ws && cd ~/catkin_build_ws 
#     catkin config -DPYTHON_EXECUTABLE=/usr/bin/python3 -DPYTHON_INCLUDE_DIR=/usr/include/python3.6m -DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython3.6m.so
#     catkin config --install
#     mkdir src
#     cd src
#     git clone -b melodic https://github.com/ros-perception/vision_opencv.git
#     cd ~/catkin_build_ws
#     catkin build cv_bridge
#     source install/setup.bash --extend
#     cd ~/catkin_ws/src
#     git clone https://github.com/ros/ros
#     cd ..
#     catkin_make_isolated --install --pkg rosunit -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/melodic
