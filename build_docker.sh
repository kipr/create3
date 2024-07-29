#!/bin/bash
IMAGE_NAME="create3-builder:python-3.11.2"

if [[ "$(docker images -q ${IMAGE_NAME} 2> /dev/null)" == "" ]]; then
  docker build -t ${IMAGE_NAME} -f docker/Dockerfile .
fi

docker run --cpus="$(nproc)" \
-v "$(pwd)":/app \
${IMAGE_NAME} bash -c "bash docker/build.sh $*"

docker stop $(docker ps -a --filter "ancestor=${IMAGE_NAME}" -q)
docker rm $(docker ps -a --filter "ancestor=${IMAGE_NAME}" -q)