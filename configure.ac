#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([A time zone library that can handle both POSIX style and IANA time zone database style strings for C++11],
    [2018-08-19.05], , [integrated_tz],
    [https://github.com/trueroad/integrated_tz])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([integrated_tz.hh])
AC_CONFIG_HEADERS([config.h])

PACKAGE_COPYRIGHT="Copyright (C) 2018 Masamichi Hosoda. All rights reserved."
PACKAGE_LICENSE="License: BSD-2-Clause"

AC_SUBST([PACKAGE_COPYRIGHT])
AC_SUBST([PACKAGE_LICENSE])
AC_DEFINE_UNQUOTED([PACKAGE_COPYRIGHT], ["$PACKAGE_COPYRIGHT"],
    [Define to the copyright of this package.])
AC_DEFINE_UNQUOTED([PACKAGE_LICENSE], ["$PACKAGE_LICENSE"],
    [Define to the license of this package.])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for libraries.
AC_ARG_WITH([tzdb],
    AS_HELP_STRING([--with-tzdb=@<:@system|remote@:>@],
                   [Time zone database to use (default: system)]),
                   ,
                   [with_tzdb=system])
AS_CASE(["$with_tzdb"],
        ["system"], [],
        ["remote"],
            [PKG_PROG_PKG_CONFIG
             AC_MSG_NOTICE([--with-tzdb=remote requires libcurl-devel])
             PKG_CHECK_MODULES([LIBCURL], [libcurl])],
        [AC_MSG_ERROR([unrecognized --with-tzdb argument: $with_tzdb])])
AM_CONDITIONAL([USE_TZDB_SYSTEM], [test $with_tzdb = system])
AM_CONDITIONAL([USE_TZDB_REMOTE], [test $with_tzdb = remote])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_11([ext], [mandatory])

# Checks for library functions.

AC_CONFIG_FILES([Makefile tz/Makefile])
AC_OUTPUT
