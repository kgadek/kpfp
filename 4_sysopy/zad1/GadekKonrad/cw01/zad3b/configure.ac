#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.50])
AC_INIT([macierze], [0.1], [])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([src/include/config.h])
AC_CONFIG_FILES([Makefile
				src/Makefile])

#AC_ARG_ENABLE(fileoutput,
#	AC_HELP_STRING([--enable-file-output], [Directs output to a file]),
#	[if test "x$enable_file_output" = "xyes"; then
#		AC_DEFINE([WEWY_PLIKI], [], [Output to a file rather than to a console]),
#		AC_MSG_RESULT([yes])
#	else
#		AC_MSG_RESULT([no])
#	fi],
#	AC_MSG_RESULT([no]))

#AC_ARG_ENABLE(debug-info,
#	AC_HELP_STRING([--enable-debug-info], [Directs output to a file]),
#	[if test "x$enable_debug_info" = "xyes"; then
#		AC_DEFINE([KOMUNIKATY], [1], [Output to a file rather than to a console]),
#		AC_MSG_RESULT([yes])
#	else
#		AC_MSG_RESULT([no])
#	fi],
#	AC_MSG_RESULT([no]))

AC_ARG_ENABLE([file-output],
	AC_HELP_STRING([--enable-file-output], [Output to a file rather than to a console]))
AS_IF([test "x$enable_file_output" == "xyes"],[
	AC_DEFINE([WEWY_PLIKI], [1], [Output to a file rather than to a console])
])

AC_ARG_ENABLE([debug-info],
	AC_HELP_STRING([--enable-debug-info], [Show additional informations]))
AS_IF([test "x$enable_debug_info" == "xyes"], [
	AC_DEFINE([KOMUNIKATY], [1], [Show additional informations])
])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT
