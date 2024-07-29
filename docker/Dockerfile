FROM arm64v8/python:3.11.2-bullseye
LABEL authors="Tobias Madlberger"

RUN apt-get update && apt-get install -y git wget libx11-dev zlib1g-dev cmake swig ninja-build \
                          build-essential libc-dev libasio-dev libc6-dev libpthread-stubs0-dev libssl-dev  \
                          libzbar-dev libopencv-dev libjpeg-dev python-dev doxygen yasm git build-essential  \
                          cmake libzbar-dev libopencv-dev libjpeg-dev python-dev doxygen swig yasm \
                          libcurl4-openssl-dev

WORKDIR /app
ENV CMAKE_GENERATOR=Ninja
ENV CMAKE_BUILD_PARALLEL_LEVEL=12