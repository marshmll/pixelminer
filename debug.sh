#!/usr/bin/bash

if [ ! -d "Debug/" ]
then
    mkdir Debug
    cd Debug/
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cd ../
fi

cp Debug/compile_commands.json .

rm -rf \
Debug/bin/Assets/ \
# Debug/bin/Config/ \
# Debug/bin/Fonts/ \
# Debug/bin/Maps \
# Debug/bin/Shaders \
# Debug/bin/Logs

cp -r src/Assets/ Debug/bin/
# cp -r src/Config/ Debug/bin/
# cp -r src/Fonts/ Debug/bin/
# cp -r src/Maps/ Debug/bin/
# cp -r src/Shaders/ Debug/bin/

cd Debug/ &&
make &&
cd bin/ &&
clear &&
./pixelminer &&
cd ../../

# rm -rf \
# src/Assets/ \
# src/Config/ \
# src/Fonts/ \
# src/Maps/ \
# src/Shaders \
# src/Logs

cp -rf Debug/bin/Assets src/
# cp -r Debug/bin/Config src/
# cp -r Debug/bin/Fonts src/
# cp -r Debug/bin/Maps src/
# cp -r Debug/bin/Shaders src/