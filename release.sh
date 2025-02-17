#!/usr/bin/bash

if [ ! -d "Release/" ]
then
    mkdir Release
    cd Release/
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang-18 -DCMAKE_CXX_COMPILER=clang++-18 ..
    cd ../
fi

rm -rf \
Release/bin/.pixelminer/

cp -r src/.pixelminer/ Release/bin/

cd Release/ &&
ninja -j4 &&
cd bin/ &&
clear &&
./pixelminer &&
cd ../../

cp -rf Release/bin/.pixelminer src/

cp Release/compile_commands.json .