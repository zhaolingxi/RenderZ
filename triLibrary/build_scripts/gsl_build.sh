﻿#!/bin/bash
# cmake工具
CMAKE_TOOL=$1
# Git目录
BUILD_CONFIGURE=$2
# Debug或Release
INSTALL_DIR=$3
# *******************************************************************************
cd ..
echo enter the third home dir: `pwd`
echo cmake tools: $CMAKE_TOOL
echo build config: $BUILD_CONFIGURE
BUILD_DIR_NAME=build
cd gsl2.7
echo build gsl dir: `pwd`
if [ -d $BUILD_DIR_NAME ]; then
    echo "remove the old gsl build dir......."
 	rm -rf $BUILD_DIR_NAME
fi
mkdir $BUILD_DIR_NAME
cd $BUILD_DIR_NAME
# 编译选项Debug/release
GSL_BUILD_CONFIGURE=$BUILD_CONFIGURE
# 因为3方库通常都不会一直更改,所以只要已经编译完成,则直接退出
if [ -e "$INSTALL_DIR/include/gsl/gsl_blas.h" ]; then
    echo "gsl has compiled success quit directly......."
    exit 0
fi
# gsl编译结果保存目录
if [ ! -d "$INSTALL_DIR" ]; then
 	mkdir $INSTALL_DIR
fi

$CMAKE_TOOL -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$GSL_BUILD_CONFIGURE" \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" ..
make -j8
make install
echo gsl build finished, install dir: $INSTALL_DIR
