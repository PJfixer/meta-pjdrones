#install custom openvpn configuration
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYRIGHT;md5=5111f52a6925efff79682aec8b3b32da"
FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI = "file://openvpn.conf \
	   file://auth.txt \
	   file://COPYRIGHT \
	   "
do_install () {
	install -d ${D}/${sysconfdir}/openvpn
	install -m 0644 ${WORKDIR}/openvpn.conf ${D}${sysconfdir}/openvpn/openvpn.conf
	install -m 0644 ${WORKDIR}/auth.txt ${D}${sysconfdir}/openvpn/auth.txt
}
