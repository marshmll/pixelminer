#!/bin/bash

cd src/

echo "target_sources(pixelminer PRIVATE" > CMakeLists.txt

for fname in $(find . -maxdepth 10 -type f)
    do 
        if [ ${fname##*\.} == "cxx" ];
        then
            echo "${fname}" >> CMakeLists.txt 
        fi 
done

echo ")" >> CMakeLists.txt
