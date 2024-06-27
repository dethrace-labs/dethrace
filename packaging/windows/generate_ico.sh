#!/bin/sh

convert -background transparent source.png -define icon:auto-resize=16,24,32,48,64,72,96,128,256 "dethrace.ico"
