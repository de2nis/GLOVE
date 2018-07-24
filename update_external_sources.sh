#!/bin/bash
# Update source for glslang, googletest

set -e

BASEDIR=$PWD
EXT_DIR=$PWD/External
BUILD_FOLDER=build
INSTALL_PATH=""
TOOLCHAIN_FILE=""
SYSROOT=""

GLSLANG_REPOSITORY="https://github.com/KhronosGroup/glslang.git"
GOOGLETEST_REPOSITORY="https://github.com/google/googletest.git"

GLSLANG_REVISION=$(cat $EXT_DIR/glslang_revision)
GOOGLETEST_REVISION=$(cat $EXT_DIR/googletest_revision)
echo "GLSLANG_REVISION=$GLSLANG_REVISION"
echo "GOOGLETEST_REVISION=$GOOGLETEST_REVISION"

function create() {
    PROJECT=$1
    echo "Creating local glslang repository ($EXT_DIR/$PROJECT)."
    rm -rf $EXT_DIR/$PROJECT
    mkdir -p $EXT_DIR/$PROJECT
    cd $EXT_DIR/$PROJECT
    git clone $2 .
    git checkout $3
}

function update() {
    PROJECT=$1
    echo "Updating $EXT_DIR/$PROJECT"
    cd $EXT_DIR/$PROJECT
    git fetch --all
    git checkout --force $2
}

function build() {
    PROJECT=$1
    echo "Building $EXT_DIR/$PROJECT"
    if [ -z "$INSTALL_PATH" ]; then
        INSTALL_PREFIX="$EXT_DIR/$PROJECT"
    else
        INSTALL_PREFIX="$INSTALL_PATH"
    fi
    cd $EXT_DIR/$PROJECT
    mkdir -p $BUILD_FOLDER
    cd $BUILD_FOLDER
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
          -DCMAKE_SYSROOT=$SYSROOT \
          -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX ..
    make -j $(nproc)
    make install
}

# Parse options
while [[ $# > 0 ]]
do
    option="$1"
    case $option in
        # option to set install path
        -i|--install-path)
            shift
            INSTALL_PATH=$1
            ;;
        # option to set sysroot
        -s|--sysroot)
            shift
            SYSROOT=$1
            BUILD_FOLDER=cross_build
            TOOLCHAIN_FILE=$BASEDIR/CMake/toolchain-arm.cmake
            ;;
        *)
            echo "Unrecognized option: $option"
            echo "Try the following:"
            echo " -i | --install-path (dir)    # set custom installation path"
            echo " -s | --sysroot      (dir)    # set sysroot for cross compilation"
            exit 1
            ;;
    esac
    shift
done

if [ ! -d "$EXT_DIR/glslang" ] || [ ! -d "$EXT_DIR/glslang/.git" ]; then
    create glslang $GLSLANG_REPOSITORY $GLSLANG_REVISION
fi
update glslang $GLSLANG_REVISION
build glslang

if [ ! -d "$EXT_DIR/googletest" ] || [ ! -d "$EXT_DIR/googletest/.git" ]; then
    create googletest $GOOGLETEST_REPOSITORY $GOOGLETEST_REVISION
fi
update googletest $GOOGLETEST_REVISION
build googletest
