# Copyright: Xavier Lange
# License: GPLv3

AUTOMAKE_OPTIONS = foreign 1.4
ACLOCAL_AMFLAGS = -I m4

# SUBDIRS = src/abstraction/os/code

lib_LTLIBRARIES = libddsos.la

ARCH=linux2.6
INCLUDES := -Isrc/abstraction/os-net/include -Isrc/abstraction/os-net/$(ARCH)/include -Isrc/abstraction/os/include -Isrc/abstraction/os/$(ARCH)/include

# os__time.h may not be necessary for use of library
libdds_dir=src/abstraction/os/code
libddsos_la_SOURCES = $(libdds_dir)/os_abstract.c $(libdds_dir)/os_diag.c $(libdds_dir)/os_init.c $(libdds_dir)/os_memMapFile.c $(libdds_dir)/os_report.c $(libdds_dir)/os_sharedmem.c $(libdds_dir)/os_stdlib.c $(libdds_dir)/os_time.c $(libdds_dir)/os_cond.c $(libdds_dir)/os_heap.c $(libdds_dir)/os_iterator.c $(libdds_dir)/os_mutex.c $(libdds_dir)/os_rwlock.c $(libdds_dir)/os_signal.c $(libdds_dir)/os_thread.c $(libdds_dir)/os_usrClock.c
# libddsos_la_SOURCES = src/abstraction/os/code/os_abstract.c
