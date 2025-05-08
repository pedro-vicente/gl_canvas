#!/bin/bash

mkdir -p build/ext/wxWidgets-3.2.8
pushd build

# wxwidgets (-DwxBUILD_SAMPLES=SOME | ALL | OFF )
pushd ext
pushd wxWidgets-3.2.8
cmake ../../../ext/wxWidgets-3.2.8 -DwxBUILD_SHARED=OFF -DwxBUILD_SAMPLES=OFF
cmake --build .
popd
popd
popd

pushd build
cmake .. 
cmake --build .
popd


