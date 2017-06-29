
bin_PROGRAMS=toolwrap

# do not install man page to the standard location
#man_MANS=toolwrap.1

toolwrap_SOURCES=assert.c assert.h hash.c hash.h list.c list.h log.c log.h policy.c policy.h \
	toolwrap.c toolwrap.h \
	env_parser.y env_lex.c env_lex.h  env.c env.h \
	getopt.c getopt1.c getopt.h getopt_int.h \
	utils.c utils.h 

BUILT_SOURCES=env_parser.h

toolwrap_CFLAGS=-DTOOLWRAP_ROOT=\"@prefix@\" -D_GNU_SOURCE -D_DEBUG 

toolwrap.1:
	help2man --output=${srcdir}/$@ ./toolwrap

EXTRA_DIST=env.default toolwrap-policies.in toolwrap.1 env_parser.c env_parser.h


install-data-local:
	${INSTALL_PROGRAM} -d ${DESTDIR}${prefix}/etc ${DESTDIR}${prefix}/env ${DESTDIR}${prefix}/bin ${DESTDIR}${prefix}/pkgs
	@INSTALL_DATA@ -b ${srcdir}/env.default  ${DESTDIR}${prefix}/env/__default__
	@INSTALL_DATA@ -b toolwrap-policies ${DESTDIR}${prefix}/etc/toolwrap-policies
	${INSTALL_PROGRAM} -d ${DESTDIR}${prefix}/pkgs/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/man/man1
	@INSTALL_DATA@ ${srcdir}/toolwrap.1 ${DESTDIR}${prefix}/pkgs/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/man/man1/toolwrap.1
	
