#!/bin/bash

#######################################################
# Splits a Watcom "exedump" output file into a separate
# file per module for easier debugging.
# exedump: https://github.com/jeff-1amstudios/open-watcom-v2/tree/master/bld/exedump
#
# Writes per module files into "./_generated_dump" directory
#
# Usage: split_dump.sh <path to dump file>
#######################################################

FILE=$1
SECTION=-1

mkdir -p _generated_dump

while IFS= read -r line
do
  if [[ "$line" == *") Name:   "* ]]; then
    (( SECTION++ ))
  fi
  if [ ${SECTION} -gt -1 ]; then
    echo "$line" >> "_generated_dump/${SECTION}_dump.txt"
  fi
done <$FILE
