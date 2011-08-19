# Copyright: Xavier Lange
# License: GPLv3

AUTOMAKE_OPTIONS = foreign 1.4
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST=src/database/database/code/q_parser.c src/database/database/include/q_parser.h

lib_LTLIBRARIES = libddsos.la libddsutil.la libddsdatabase.la

# TODO: Automagical platform discovery (with sweet override!)
ARCH=darwin
PA=x86_64

# There's a kooky convention that the abstract files should only require a #include "os_direct_name.h"
# but if you have a platform specific file you will need to do #include "include/os_direct_name.h"
INCLUDES := -Isrc/abstraction/os-net/code -Isrc/abstraction/os-net/include -Isrc/abstraction/os-net/$(ARCH) \
 -Isrc/abstraction/os/code -Isrc/abstraction/os/include -Isrc/abstraction/os/$(ARCH) \
 -Isrc/abstraction/pa/$(PA) \
 -Isrc/abstraction -I src/abstraction/os \
 -Isrc/utilities -Isrc/utilities/include \
 -Isrc/database/database/include

# os__time.h may not be necessary for use of library
os_dir=src/abstraction/os/code
libddsos_la_SOURCES = $(os_dir)/os_abstract.c $(os_dir)/os_diag.c $(os_dir)/os_init.c $(os_dir)/os_memMapFile.c $(os_dir)/os_report.c $(os_dir)/os_sharedmem.c $(os_dir)/os_stdlib.c $(os_dir)/os_time.c $(os_dir)/os_cond.c $(os_dir)/os_heap.c $(os_dir)/os_iterator.c $(os_dir)/os_mutex.c $(os_dir)/os_rwlock.c $(os_dir)/os_signal.c $(os_dir)/os_thread.c $(os_dir)/os_usrClock.c
# libddsos_la_SOURCES = src/abstraction/os/code/os_abstract.c

util_dir=src/utilities/code
libddsutil_la_SOURCES = $(util_dir)/ut_avltree.c $(util_dir)/ut_collection.c $(util_dir)/ut_crc.c \
  $(util_dir)/ut_stack.c $(util_dir)/ut_tmplExp.c $(util_dir)/ut_trace.c

db_dir=src/database/database/code
libddsdatabase_la_SOURCES =	$(db_dir)/c_avltree.c $(db_dir)/c_base.c $(db_dir)/c_collection.c \
	$(db_dir)/c_extent.c $(db_dir)/c_field.c $(db_dir)/c_iterator.c $(db_dir)/c_laptime.c $(db_dir)/c_metabase.c \
	$(db_dir)/c_metafactory.c $(db_dir)/c_misc.c $(db_dir)/c_mmbase.c $(db_dir)/c_mmCache.c $(db_dir)/c_querybase.c \
	$(db_dir)/c_scope.c $(db_dir)/c_serialize.c $(db_dir)/c_stringSupport.c $(db_dir)/c_sync.c $(db_dir)/c_time.c \
	$(db_dir)/c_typebase.c $(db_dir)/q_expr.c \
	$(db_dir)/q_parser.c

$(db_dir)/../include/q_parser.h : $(db_dir)/q_parser.l
	$(LEX) $(LFLAGS) $(AM_LFLAGS) -Pq_parser_yy -o$(db_dir)/../include/q_parser.h $<
$(db_dir)/q_parser.c : $(db_dir)/q_parser.y $(db_dir)/../include/q_parser.h
	$(YACC) $(YFLAGS) $< -o $@

# .l.c :
#	asdfasdf $(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).c $@ -- $(LEXCOMPILE)

# gcc -m32 -static-libgcc -L/home/xlange/code/OpenSplice/lib/x86.linux2.6-dev -shared -fpic c_avltree.o c_base.o c_collection.o c_extent.o c_field.o c_iterator.o c_laptime.o c_metabase.o c_metafactory.o c_misc.o c_mmbase.o c_mmCache.o c_querybase.o c_scope.o c_serialize.o c_stringSupport.o c_sync.o c_time.o c_typebase.o q_expr.o q_parser.o -lc -lm -lpthread  -lddsos -lddsutil -o libddsdatabase.so
