noinst_LIBRARIES = libtz.a
libtz_a_SOURCES = tz.cpp date/tz_private.h date/tz.h date/date.h
libtz_a_CPPFLAGS =

if USE_TZDB_SYSTEM
libtz_a_CPPFLAGS += -DUSE_AUTOLOAD=0 -DHAS_REMOTE_API=0 -DUSE_OS_TZDB=1
endif

if USE_TZDB_REMOTE
libtz_a_CPPFLAGS += -DUSE_AUTOLOAD=1 -DHAS_REMOTE_API=1 -DUSE_OS_TZDB=0 \
	@LIBCURL_CFLAGS@
endif

EXTRA_DIST = README.md LICENSE.txt
