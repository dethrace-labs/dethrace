#!/bin/sh
set -e

SOURCE_IMAGE=$1
TEMP_ICON_IMAGE=icon.png

# resize to 412x with black canvas
convert ${SOURCE_IMAGE} -resize 412x412 -background Black -gravity center -extent 412x412 ${TEMP_ICON_IMAGE}
# convert to ico format
convert -background transparent ${TEMP_ICON_IMAGE} -define icon:auto-resize=16,24,32,48,64,72,96,128,256 "dethrace.ico"

rm ${TEMP_ICON_IMAGE}
