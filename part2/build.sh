#!/bin/bash

pwd=$(pwd)
mkdir -p dbg
mkdir -p rls

cd dbg
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
cd $pwd
cd rls
cmake -DCMAKE_BUILD_TYPE=Release ../
make
cd $pwd
