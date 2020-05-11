SUMMARY = "The canonical example of init scripts"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYRIGHT;md5=5111f52a6925efff79682aec8b3b32da"
FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI = "file://main.c \
           file://bridge.c \
	   file://bridge.h \
           file://udp.c \
	   file://udp.h \
	   file://serial.c \
	   file://serial.h \
	   file://COPYRIGHT \
	   "

#CFLAGS_append = ""
#LDLAGS_append = ""

TARGET_LDFLAGS_append = " -pthread "
do_compile () {
	${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/main.c ${WORKDIR}/bridge.c ${WORKDIR}/udp.c ${WORKDIR}/serial.c  -o ${WORKDIR}/bridge-test
}



do_install () {

	install -d ${D}${sbindir}
	install -m 0755 ${WORKDIR}/bridge-test ${D}${sbindir}/
}

RDEPENDS_${PN} = ""


