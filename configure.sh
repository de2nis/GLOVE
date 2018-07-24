#!/bin/bash

set -e

BASEDIR=$PWD
BUILD_TYPE=Release
VULKAN_LIBRARY=""
VULKAN_INCLUDE_PATH=""
TRACE_BUILD=OFF
TOOLCHAIN_FILE=""
SYSROOT=""
INSTALL_PATH="/usr/local"
BUILD_FOLDER=build
CROSS_COMPILATION_ARM=false
WORKAROUNDS=ON

#########################################################
####################### GLOVE ###########################
#########################################################
function buildGlove() {
    mkdir -p $BUILD_FOLDER
    cd $BUILD_FOLDER

    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
          -DVULKAN_LIBRARY=$VULKAN_LIBRARY \
          -DTRACE_BUILD=$TRACE_BUILD \
          -DWORKAROUNDS=$WORKAROUNDS \
          -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
          -DCMAKE_SYSROOT=$SYSROOT \
          -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH ..

    cd ..
}

if [ "$#" == 0 ]; then
    echo "Building GLOVE"
else
    # Parse options
    while [[ $# > 0 ]]
    do
        option="$1"

        case $option in
            # option to cross compile
            -a|--arm-compile)
                CROSS_COMPILATION_ARM=true
                BUILD_FOLDER=cross_build
                TOOLCHAIN_FILE=$BASEDIR/CMake/toolchain-arm.cmake
                INSTALL_PATH=""
                echo "Cross compiling for ARM"
                ;;
            # option to build with Debug option
            -d|--debug)
                BUILD_TYPE=Debug
                echo "Building with Debug option"
                ;;
            # option to set install path
            -i|--install-path)
                shift
                INSTALL_PATH=$1
                echo "Setting installation path to $INSTALL_PATH"
                ;;
            # option to set sysroot
            -s|--sysroot)
                shift
                SYSROOT=$1
                echo "Setting SYSROOT"
                ;;
            # option to activate GL & Vulkan logs
            -t|--trace-build)
                TRACE_BUILD=ON
                echo "Activating logs"
                ;;
            # option to set Vulkan include path
            -u|--vulkan-include-path)
                shift
                VULKAN_INCLUDE_PATH=$1
                echo "Setting Vulkan include path"
                ;;
            # option to set Vulkan loader
            -v|--vulkan-loader)
                shift
                VULKAN_LIBRARY=$1
                echo "Setting Vulkan loader"
                ;;
            # option to disable usage of workarounds
            -w|--no-workarounds)
                WORKAROUNDS=OFF
                echo "Disabling workarounds"
                ;;
            *)
                echo "Unrecognized option: $option"
                echo "Try the following:"
                echo " -a | --arm-compile                   # cross build for ARM platform (default OFF)"
                echo " -d | --debug                         # build in Debug mode (default Release)"
                echo " -i | --install-path        (dir)     # set custom installation path"
                echo " -s | --sysroot             (dir)     # set sysroot for cross compilation"
                echo " -t | --trace-build                   # activate logs (default OFF)"
                echo " -u | --vulkan-include-path (dir)     # set custom Vulkan include path"
                echo " -v | --vulkan-loader       (lib)     # set custom Vulkan loader library"
                echo " -w | --no-workarounds                # not use workarounds (default OFF)"
                exit 1
                ;;
        esac
        shift
    done
fi

if [ ! -d "$BASEDIR/External/glslang" ] || [ ! -d "$BASEDIR/External/glslang/.git" ]; then
    echo -e "\e[0;31mExternal sources are missing. Run './update_external_sources.sh'.\e[0m"
    exit 1
fi

if [ -n "$VULKAN_LIBRARY" ] && { [ ! -e $VULKAN_LIBRARY ] || echo $VULKAN_LIBRARY | grep -q -v ".so" ; }; then
    echo -e "\e[0;31mCannot find Vulkan loader '$VULKAN_LIBRARY'.\e[0m"
    exit 1
fi

if [ -n "$VULKAN_INCLUDE_PATH" ] && [ ! -e $VULKAN_INCLUDE_PATH ]; then
    echo -e "\e[0;31mCannot find Vulkan include path '$VULKAN_INCLUDE_PATH'.\e[0m"
    exit 1
fi

if [ ! -d $INSTALL_PATH ]; then
    echo -e "\e[0;31mCannot find '$INSTALL_PATH' path to install the libraries.\e[0m"
    exit 1
fi

if [ $CROSS_COMPILATION_ARM == "true" ]; then
    if [ ! -d $SYSROOT ]; then
        echo -e "\e[0;31mCannot find '$SYSROOT' path for sysroot.\e[0m"
        exit 1
    fi
    if [ -z "$INSTALL_PATH" ]; then
        echo -e "\e[0;31mSet custom installation path (use '-i' option).\e[0m"
        exit 1
    fi
    if [ ! -d "$BASEDIR/External/glslang" ] || [ ! -d "$BASEDIR/External/googletest/cross_build" ]; then
        echo -e "\e[0;31mExternal sources are not cross-compiled. Run './update_external_sources.sh -s $SYSROOT -i $INSTALL_PATH'.\e[0m"
        exit 1
    fi
fi

buildGlove
