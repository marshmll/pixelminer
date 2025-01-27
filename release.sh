#!/usr/bin/bash

if [ ! -d "Release/" ]
then
    mkdir Release
    cd Release/
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cd ../
fi

rm -rf \
Release/bin/Assets/

cp -r src/Assets/ Release/bin/

cd Release/ &&
make &&
cd bin/ &&
clear &&
./pixelminer &&
cd ../../

cp -rf Release/bin/Assets src/