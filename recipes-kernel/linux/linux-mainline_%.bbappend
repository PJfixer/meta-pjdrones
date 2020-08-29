COMPATIBLE_MACHINE = "(nanopi-neo|orange-pi-zero)"

FILESEXTRAPATHS_prepend := "${THISDIR}/patchs:"

SRC_URI += "file://0001-uart1-uart2-usb1-usb2-dts-patch.patch \
       "

