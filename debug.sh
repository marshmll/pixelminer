#!/usr/bin/bash

if [ ! -d "Debug/" ]
then
    mkdir Debug
    cd Debug/
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-18 -DCMAKE_CXX_COMPILER=clang++-18 ..
    cd ../
fi

rm -rf \
Debug/bin/.pixelminer/

cp -r src/.pixelminer/ Debug/bin/

cd Debug/ &&
ninja -j10 &&
cd bin/ &&
clear &&
./pixelminer &&
cd ../../

cp -rf Debug/bin/.pixelminer src/

cp Debug/compile_commands.json .