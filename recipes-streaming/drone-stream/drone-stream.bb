SUMMARY = "The canonical example of init scripts"
SECTION = "base"
LICENSE = "MIT"
DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good mavlink-uart-udp"
LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYRIGHT;md5=5111f52a6925efff79682aec8b3b32da"
FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI = "file://stream-udp \
	   file://pj-stream.c \
	   file://COPYRIGHT \
	   "

INITSCRIPT_NAME = "stream-udp"
INITSCRIPT_PARAMS = "start 95 2 3 4 5 ."

inherit update-rc.d


do_compile () {
	${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/pj-stream.c -o ${WORKDIR}/pj-stream
}


do_install () {
	install -d ${D}${sysconfdir}/init.d
	# modification du fichier pour modifer les path /etc /usr /var /usr par ceux contenu dans les variables systemes ${sysconfdir} ${sbindir} {localstatedir}${bindir} etc..
	cat ${WORKDIR}/stream-udp | \
	  sed -e 's,/etc,${sysconfdir},g' \
	      -e 's,/usr/sbin,${sbindir},g' \
	      -e 's,/var,${localstatedir},g' \
	      -e 's,/usr/bin,${bindir},g' \
	      -e 's,/usr,${prefix},g' > ${D}${sysconfdir}/init.d/stream-udp
	#rendre le script service executable
	chmod a+x ${D}${sysconfdir}/init.d/stream-udp

	install -d ${D}${sbindir}
	install -m 0755 ${WORKDIR}/pj-stream ${D}${sbindir}/
		


}

RDEPENDS_${PN} = "initscripts"

CONFFILES_${PN} += "${sysconfdir}/init.d/stream-udp"
