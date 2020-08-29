SUMMARY = "The canonical example of init scripts"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYRIGHT;md5=5111f52a6925efff79682aec8b3b32da"
FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI = "file://telemetry \
	   file://main.c \
           file://udp.c \
	   file://udp.h \
	   file://serial.c \
	   file://serial.h \
	   file://connection.conf \
	   file://COPYRIGHT \
	   "
TARGET_LDFLAGS_append = " -pthread "
do_compile () {
	${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/main.c  ${WORKDIR}/udp.c ${WORKDIR}/serial.c  -o ${WORKDIR}/udpuart-bridge
}

INITSCRIPT_NAME = "telemetry"
INITSCRIPT_PARAMS = "start 80 2 3 4 5 ."

inherit update-rc.d

do_install () {
	install -d ${D}${sysconfdir}/init.d
	# modification du fichier pour modifer les path /etc /usr /var /usr par ceux contenu dans les variables systemes ${sysconfdir} ${sbindir} {localstatedir}${bindir} etc..
	cat ${WORKDIR}/telemetry | \
	  sed -e 's,/etc,${sysconfdir},g' \
	      -e 's,/usr/sbin,${sbindir},g' \
	      -e 's,/var,${localstatedir},g' \
	      -e 's,/usr/bin,${bindir},g' \
	      -e 's,/usr,${prefix},g' > ${D}${sysconfdir}/init.d/telemetry
	#rendre le script service executable
	chmod a+x ${D}${sysconfdir}/init.d/telemetry
		

	install -d ${D}${sbindir}
	install -m 0755 ${WORKDIR}/udpuart-bridge ${D}${sbindir}/

	install -d ${D}${sysconfdir}
	install -m 0755 ${WORKDIR}/connection.conf ${D}${sysconfdir}/
}

RDEPENDS_${PN} = "initscripts"

CONFFILES_${PN} += "${sysconfdir}/init.d/telemetry"
