#!/bin/bash

set -e

BOARD_DIR="$(dirname $0)"
BOARD_NAME="$(basename ${BOARD_DIR})"
GENIMAGE_CFG="${BOARD_DIR}/genimage-${BOARD_NAME}.cfg"
GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"

#
# Add lines to config.txt
#

cat << __EOF__ >> "${BINARIES_DIR}/rpi-firmware/config.txt"

# Enable UART serial console
enable_uart=1

# Enable Pimorino PHAT DAC (PCM5102A)
enable_uart=1
dtparam=i2c_arm=on
dtparam=spi=on
dtoverlay=hifiberry-dac
__EOF__

rm -rf "${GENIMAGE_TMP}"

genimage                           \
	--rootpath "${TARGET_DIR}"     \
	--tmppath "${GENIMAGE_TMP}"    \
	--inputpath "${BINARIES_DIR}"  \
	--outputpath "${BINARIES_DIR}" \
	--config "${GENIMAGE_CFG}"

exit $?
