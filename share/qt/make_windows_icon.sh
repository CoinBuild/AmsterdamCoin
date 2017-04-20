#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/amsterdamcoin.ico

convert ../../src/qt/res/icons/amsterdamcoin-16.png ../../src/qt/res/icons/amsterdamcoin-32.png ../../src/qt/res/icons/amsterdamcoin-48.png ${ICON_DST}
