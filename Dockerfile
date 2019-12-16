FROM debian:stable

MAINTAINER Jingru <1961295051@qq.com>
COPY install_skip.sh .
RUN apt-get remove python-* \
    remove ros-* \
    ros-melodic-* \
    && apt-get autoremove \
    && apt update  && apt-get install -y python3 \
    python3-dev \
    python3-pip \
    build-essentia \
    wget \
    && pip3 install rosdep \ 
    rospkg \
    rosinstall_generator \
    rosinstall \
    wstool \
    vcstools \
    catkin_tools \
RUN catkin_pkg \
    && rosdep init \
    && rosdep update \
    && cd ~ \
    && mkdir ros_catkin_ws \
    && cd ros_catkin_ws \
    && catkin config --init -DCMAKE_BUILD_TYPE=Release --blacklist rqt_rviz rviz_plugin_tutorials librviz_tutorial --install \
    && rosinstall_generator desktop_full --rosdistro melodic --deps --tar > melodic-desktop-full.rosinstall \
    && wstool init -j8 src melodic-desktop-full.rosinstall \
    && export ROS_PYTHON_VERSION=3 \
    && pip3 install wxPython==4.0.0b1

RUN ./install_skip.sh `rosdep check --from-paths src --ignore-src | grep python | sed -e "s/^apt\t//g" | sed -z "s/\n/ /g" | sed -e "s/python/python3/g"` \
    && rosdep install --from-paths src --ignore-src -y --skip-keys="`rosdep check --from-paths src --ignore-src | grep python | sed -e "s/^apt\t//g" | sed -z "s/\n/ /g"`" \
    && find . -type f -exec sed -i 's/\/usr\/bin\/env[ ]*python/\/usr\/bin\/env python3/g' {} + \
    && catkin build \
    && echo "export PYTHONPATH=/usr/lib/python3/dist-packages" >> ~/.bashrc \
    && echo "source ~/ros_catkin_ws/install/setup.bash" >> ~/.bashrc
