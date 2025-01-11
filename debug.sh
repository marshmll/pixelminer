#!/usr/bin/bash

if [ ! -d "Debug/" ]
then
    mkdir Debug
    cd Debug/
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cd ../
fi

rm -rf \
Debug/bin/Assets/

cp -r src/Assets/ Debug/bin/

cd Debug/ &&
make &&
cd bin/ &&
clear &&
./pixelminer &&
cd ../../

cp -rf Debug/bin/Assets src/

cp Debug/compile_commands.json .