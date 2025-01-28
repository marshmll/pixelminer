#!/usr/bin/bash

if [ ! -d "Debug/" ]
then
    mkdir Debug
    cd Debug/
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-18 -DCMAKE_CXX_COMPILER=clang++-18 ..
    cd ../
fi

rm -rf \
Debug/bin/Assets/

cp -r src/Assets/ Debug/bin/

cd Debug/ &&
ninja -j10 &&
cd bin/ &&
clear &&
./pixelminer &&
cd ../../

cp -rf Debug/bin/Assets src/

cp Debug/compile_commands.json .