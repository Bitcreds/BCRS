#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/bitcreds.png
ICON_DST=../../src/qt/res/icons/bitcreds.ico
convert ${ICON_SRC} -resize 16x16 bitcreds-16.png
convert ${ICON_SRC} -resize 32x32 bitcreds-32.png
convert ${ICON_SRC} -resize 48x48 bitcreds-48.png
convert bitcreds-16.png bitcreds-32.png bitcreds-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/bitcreds_testnet.png
ICON_DST=../../src/qt/res/icons/bitcreds_testnet.ico
convert ${ICON_SRC} -resize 16x16 bitcreds-16.png
convert ${ICON_SRC} -resize 32x32 bitcreds-32.png
convert ${ICON_SRC} -resize 48x48 bitcreds-48.png
convert bitcreds-16.png bitcreds-32.png bitcreds-48.png ${ICON_DST}
rm bitcreds-16.png bitcreds-32.png bitcreds-48.png
