#!/bin/bash
RETVAL=0

function printUsage() {
    echo "Usage:"
    echo "./run_all_samples.sh <option> where option is either of:"
    echo " --debug      # Save OpenGL ES API Debug output in a file per demo"
    echo " --profile    # Save OpenGL ES API Profile output in a file per demo"
}

function runDemos() {
    echo "*******************************************"
    echo "******* Running Demos...*******************"
    echo "*******************************************"

    CMDDIR="$(pwd)"
    #  choose the demo samples to be played
    SAMP2RUN="$CMDDIR/triangle2d_one_color \
              $CMDDIR/triangle2d_split_colors \
              $CMDDIR/circle2d_sdf \
              $CMDDIR/texture2d_color \
              $CMDDIR/cube3d_vertexcolors \
              $CMDDIR/cube3d_textures \
              $CMDDIR/render_to_texture_filter_gamma \
              $CMDDIR/render_to_texture_filter_invert \
              $CMDDIR/render_to_texture_filter_grayscale \
              $CMDDIR/render_to_texture_filter_sobel \
              $CMDDIR/render_to_texture_filter_boxblur"

    # display short description of the sample and run it
    for file in $SAMP2RUN
    do
        # get name of the sample source file; need to remove .c from name
        BNAME=$(basename $file)
        echo "SAMPLE:  ./$BNAME"

        # run the built sample;
        RNAME=./${BNAME}

        if   [ $DEBUG == "true" ] && [ $PROFILE == "true" ]; then
            $RNAME >${BNAME}_gl_profile.log 2>${BNAME}_gl_error.log
        elif [ $DEBUG == "true" ]; then
            $RNAME 2>${BNAME}_gl_error.log
        elif [ $PROFILE == "true" ]; then
            $RNAME >${BNAME}_gl_profile.log
        else
            $RNAME
        fi
    done
}

DEBUG=false
PROFILE=false

for option in "$@"
do
    case $option in
        --help)
            printUsage
            exit 1
            ;;
        --debug)
            DEBUG=true
            echo "Enable Debug   ($option)"
            ;;
        --profile)
            PROFILE=true
            echo "Enable Profile ($option)"
            ;;
        *)
            printUsage
            exit 1
            ;;
    esac
    shift
done

runDemos

exit $RETVAL
