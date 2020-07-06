#!/bin/bash
# This script installs dependencies

if [ $(id -u) != "0" ]; then
echo "Must be superuser to run this script" >&2
exit 1
fi

apt-get update

# Build tools
apt-get -y install build-essential

# ========== ProjectChrono ==========
# Install dependency packages
apt-get -y install libeigen3-dev

# Build and install
cd /tmp
git clone https://github.com/projectchrono/chrono.git
cd chrono
mkdir chrono_build
cd chrono_build
cmake ..
make
make install

# ========== GLFW ==========
apt-get -y install xorg-dev
apt-get -y install libgtk-3-dev
apt-get -y install libglfw3-dev

# ========== BGFX ==========
# Install dependency packages
apt-get -y install libgl1-mesa-dev
apt-get -y install x11proto-core-dev
apt-get -y install libx11-dev

# Build and install
cd /tmp
git clone https://github.com/widberg/bgfx.cmake.git
cd bgfx.cmake
git submodule init
git submodule update
mkdir bgfx_build
cd bgfx_build
cmake ..
make
make install
