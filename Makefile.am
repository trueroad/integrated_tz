SUBDIRS = tz

bin_PROGRAMS = sample sample-multiple
sample_SOURCES = sample.cc \
	integrated_tz.hh integrated_tz_private.hh tz/ptz.h
sample_multiple_SOURCES = sample-multiple.cc \
	integrated_tz.hh integrated_tz_private.hh tz/ptz.h

LDADD = -ltz
LDFLAGS = -L./tz
DEPENDENCIES = ./tz/libtz.a

if USE_TZDB_REMOTE
LDADD += @LIBCURL_LIBS@
endif

dist_doc_DATA = README.md README.ja.md LICENSE

ACLOCAL_AMFLAGS = -I m4

install:
	echo Files can not be installed.
	echo Please copy `integrated_tz.hh`, `integrated_tz_private.hh`,
	echo `tz/ptz.h`, `tz/tz.cpp`,
	echo `tz/date/date.h`, `tz/tz.h`, and `tz_private.h`
	echo to your project directory.
	exit 1
