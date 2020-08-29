SUMMARY = "The canonical example of init scripts"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYRIGHT;md5=5111f52a6925efff79682aec8b3b32da"
FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI = "file://serial.c \
	   file://serial.h \
	   file://main.c \
	   file://COPYRIGHT \
	   "

#CFLAGS_append = ""
#LDLAGS_append = ""

do_compile () {
	${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/main.c ${WORKDIR}/serial.c -o ${WORKDIR}/rx_poll
}



do_install () {

	install -d ${D}${sbindir}
	install -m 0755 ${WORKDIR}/rx_poll ${D}${sbindir}/
}

RDEPENDS_${PN} = ""


