#!/bin/bash
# This script installs dependencies

if [ $(id -u) != "0" ]; then
echo "Must be superuser to run this script" >&2
exit 1
fi

apt-get update

# Build tools
apt-get -y install build-essential

# Install ProjectChrono library
#todo

