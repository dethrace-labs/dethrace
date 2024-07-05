#!/bin/sh
set -e

SOURCE_IMAGE=$1
TEMP_ICON_IMAGE=icon.png

# resize to 412x with black canvas
convert ${SOURCE_IMAGE} -resize 412x412 -background Black -gravity center -extent 412x412 ${TEMP_ICON_IMAGE}

# add rounded corners
convert ${TEMP_ICON_IMAGE} \
  \( +clone  -alpha extract \
    -draw 'fill black polygon 0,0 0,50 50,0 fill white circle 50,50 50,0' \
    \( +clone -flip \) -compose Multiply -composite \
    \( +clone -flop \) -compose Multiply -composite \
  \) -alpha off -compose CopyOpacity -composite ${TEMP_ICON_IMAGE}

# add margin
convert ${TEMP_ICON_IMAGE} -bordercolor transparent -border 40x40 ${TEMP_ICON_IMAGE}

# add drop shadow
convert ${TEMP_ICON_IMAGE} \
  \( +clone -background black -shadow 100x5+0+0 \) +swap \
  -background none -layers merge +repage ${TEMP_ICON_IMAGE}


rm -r dethrace.iconset || true
mkdir -p dethrace.iconset
sips -z 16 16     ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_16x16.png
sips -z 32 32     ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_16x16@2x.png
sips -z 32 32     ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_32x32.png
sips -z 64 64     ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_32x32@2x.png
sips -z 128 128   ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_128x128.png
sips -z 256 256   ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_128x128@2x.png
sips -z 256 256   ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_256x256.png
sips -z 512 512   ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_256x256@2x.png
sips -z 512 512   ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_512x512.png
sips -z 1024 1024 ${TEMP_ICON_IMAGE} --out dethrace.iconset/icon_512x512@2x.png
iconutil -c icns dethrace.iconset
rm -r dethrace.iconset ${TEMP_ICON_IMAGE}
