#!/bin/bash
# This file should not be run manually, it is used by the Dockerfile

cmake -Bbuild "$@" -Dserver=OFF
cd build || exit 1
ninja
cpack
mkdir -p "/app/binaries"
mv create3-0.1.0-Linux.deb /app/binaries