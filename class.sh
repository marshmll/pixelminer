#!/usr/bin/bash

if [[ ! -z $1 && ! -z $2 ]]
then
    if [ ! -d "src/$1" ]
    then
        mkdir "src/$1"
    fi
    if [ ! -d "include/$1" ]
    then
        mkdir "include/$1"
    fi

    if [ ! -f "src/${1}/${2}.cxx" ]
    then
        touch "src/${1}/${2}.cxx"
        echo -e "#include \"stdafx.hxx\"\n#include \"$1/$2.hxx\"\n" > "src/${1}/${2}.cxx"
    fi

    if [ ! -f "include/${1}/${2}.hxx" ]
    then
        touch "include/${1}/${2}.hxx"
        echo -e "#pragma once" > "include/${1}/${2}.hxx"
    fi

    ./updatesrcdeps.sh
else
    echo "Usage: <folder> <Class>"
fi