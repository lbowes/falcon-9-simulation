#!/bin/bash
# This script installs dependencies

if [ $(id -u) != "0" ]; then
echo "Must be superuser to run this script" >&2
exit 1
fi

apt-get update

# Build tools
apt-get -y install build-essential

# Setup ProjectChrono library
mkdir ../deps
cd ../deps
git clone https://github.com/projectchrono/chrono.git
cd chrono
mkdir chrono_build
# todo, complete local build
# http://api.projectchrono.org/4.0.0/tutorial_install_chrono.html
