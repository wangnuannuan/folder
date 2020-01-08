FROM debian:stable

MAINTAINER Jingru <1961295051@qq.com>

RUN apt update  && apt-get install -y autoconf \
    autogen \
    git
RUN git clone https://github.com/protocolbuffers/protobuf.git \
    && cd protobuf \
    && ls
RUN git submodule update --init --recursive \
    && ./autogen.sh \
    && ./configure \
    && make \
    && make check \
    && make install \
    && ldconfig
RUN apt-get update  && apt-get install -y python-wstool \
    python-rosdep \
    ninja-build
RUN mkdir cartographer_ws \
    && cd catkin_ws \
    && wstool init src \
    && wstool merge -t src https://raw.githubusercontent.com/googlecartographer/cartographer_ros/master/cartographer_ros.rosinstall \
    && wstool update -t src \
    && rosdep init \
    && rosdep update \
    && rosdep install --from-paths src --ignore-src --rosdistro=kinetic -y \
    && catkin_make_isolated --install --use-ninja \
    && source install_isolated/setup.bash
