 
EXTRA_DIST=ref.sgml header-html styles.css license.sgml

#all: ref.html

%.html: ${srcdir}/%.sgml
	linuxdoc -B html -H header-html $< 

