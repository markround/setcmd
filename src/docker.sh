#!/bin/bash

docker run \
  -it \
  --rm \
  --name amigavbcc-m68k \
  -v ${PWD}:/opt/code \
  -w /opt/code \
  walkero/docker4amigavbcc:latest-m68k \
  "$@"
