#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([toolwrap],[1.0.8],[bmonthy@users.sourceforge.net])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/assert.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LEX
AC_PROG_YACC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([memset putenv strcasecmp strchr strdup strerror strndup strsep setenv basename dirname])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/toolwrap-policies
                 doc/Makefile])
AC_OUTPUT
