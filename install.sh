#!/bin/bash

mkdir build
cd build
cmake ..
cmake --build .
sudo cmake --install . --prefix /usr