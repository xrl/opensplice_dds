# Copyright: Xavier Lange
# License: GPLv3

# PATH=bin/:.libs/:$PATH

AUTOMAKE_OPTIONS = foreign 1.4
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST=src/database/database/code/q_parser.c src/database/database/include/q_parser.h

lib_LTLIBRARIES = libddsos.la libddsosnet.la libddsutil.la libddsdatabase.la libddskernel.la
noinst_PROGRAMS = odlpp

# TODO: Automagical platform discovery (with sweet override!)
ARCH=darwin
PA=x86_64

##############################################################################
##### Grab bag of includes
##############################################################################

# There's a kooky convention that the abstract files should only require a #include "os_direct_name.h"
# but if you have a platform specific file you will need to do #include "include/os_direct_name.h"
INCLUDES := -Isrc/abstraction/os/code -Isrc/abstraction/os/include -Isrc/abstraction/os/$(ARCH) \
 -Isrc/abstraction/os-net/code -Isrc/abstraction/os-net/include -Isrc/abstraction/os-net/$(ARCH) \
 -Isrc/abstraction/pa/$(PA) \
 -Isrc/abstraction -I src/abstraction/os \
 -Isrc/utilities -Isrc/utilities/include \
 -Isrc/database/database/include \
 -Isrc/database/odlpp/code \
 -Isrc/kernel/include

##############################################################################
##### OS Abstraction
##############################################################################

os_dir=src/abstraction/os/code
os_arch_dir=src/abstraction/os/$(ARCH)/code
libddsos_la_SOURCES = $(os_dir)/os_abstract.c $(os_dir)/os_diag.c $(os_dir)/os_init.c $(os_dir)/os_memMapFile.c \
                      $(os_dir)/os_report.c $(os_dir)/os_sharedmem.c $(os_dir)/os_stdlib.c $(os_dir)/os_time.c \
					  $(os_dir)/os_cond.c $(os_dir)/os_heap.c $(os_dir)/os_iterator.c $(os_dir)/os_mutex.c \
					  $(os_dir)/os_rwlock.c $(os_dir)/os_signal.c $(os_dir)/os_thread.c $(os_dir)/os_usrClock.c \
					  $(os_arch_dir)/os__process.c $(os_arch_dir)/os__library.c

# Why in the world are the last two out of the main code directory when all others are #include monkey patches?
					
##############################################################################
##### OS Abstraction (Network)
##############################################################################

osnet_dir=src/abstraction/os-net/code
libddsosnet_la_SOURCES = $(osnet_dir)/os_socket.c

##############################################################################
##### Utilities
##############################################################################

util_dir=src/utilities/code
libddsutil_la_SOURCES = $(util_dir)/ut_avltree.c $(util_dir)/ut_collection.c $(util_dir)/ut_crc.c \
  $(util_dir)/ut_stack.c $(util_dir)/ut_tmplExp.c $(util_dir)/ut_trace.c

##############################################################################
##### Database
##############################################################################

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

##############################################################################
##### Database - ODLPP (command line utility)
##############################################################################
# gcc  -m32 -static-libgcc -L/home/xlange/code/OpenSplice/lib/x86.linux2.6-dev -L/home/xlange/code/OpenSplice/lib/x86.linux2.6-dev c_genc.o c_odlparser.o
#     c_typenames.o  c_odlbase.o   -lc -lm -lpthread -lddsdatabase -lddsos  -o odlpp

# Current objects: c_genc.o c_odlparser.o c_typenames.o c_odlbase.o
odlpp_dir=src/database/odlpp/code
odlpp_DEPENDENCIES = libddsos.la libddsdatabase.la
odlpp_SOURCES = $(odlpp_dir)/c_genc.c $(odlpp_dir)/c_odlparser.c $(odlpp_dir)/c_typenames.c \
	$(odlpp_dir)/c_odlbase.c
odlpp_LDADD = -lddsdatabase -lddsos
	
# Ugh, a one off where the .h is in the same folder?
$(odlpp_dir)/c_odlbase.h : $(odlpp_dir)/c_odlbase.l
	$(LEX) $(LFLAGS) $(AM_LFLAGS) -Pc_odlbase_yy -o$(odlpp_dir)/c_odlbase.h $<
$(odlpp_dir)/c_odlbase.c : $(odlpp_dir)/c_odlbase.y $(odlpp_dir)/c_odlbase.h
	$(YACC) $(YFLAGS) $< -o $@

##############################################################################
##### Kernel
##############################################################################

kernel_dir=src/kernel/code
include_dir=src/include/
nodist_libddskernel_la_SOURCES = kernelModule.h
$(kernel_dir)/v_avgValue.c : kernelModule.h
libddskernel_la_DEPENDENCIES = odlpp
libddskernel_la_SOURCES = $(kernel_dir)/v_avgValue.c $(kernel_dir)/v_deliveryServiceEntry.c $(kernel_dir)/v_maxValue.c $(kernel_dir)/v_query.c \
	$(kernel_dir)/v_builtin.c $(kernel_dir)/v_deliveryWaitList.c $(kernel_dir)/v_message.c $(kernel_dir)/v_queryStatistics.c \
	$(kernel_dir)/v_cache.c $(kernel_dir)/v_durability.c $(kernel_dir)/v_messageQos.c $(kernel_dir)/v_reader.c \
	$(kernel_dir)/v_cfAttribute.c $(kernel_dir)/v_durabilityStatistics.c $(kernel_dir)/v_minValue.c $(kernel_dir)/v_readerQos.c \
	$(kernel_dir)/v_cfData.c $(kernel_dir)/v_entity.c $(kernel_dir)/v_networkChannelStatistics.c $(kernel_dir)/v_readerStatistics.c \
	$(kernel_dir)/v_cfElement.c $(kernel_dir)/v_entry.c $(kernel_dir)/v_networkQueue.c $(kernel_dir)/v_scheduler.c \
	$(kernel_dir)/v_cfNode.c $(kernel_dir)/v_filter.c $(kernel_dir)/v_networkQueueStatistics.c $(kernel_dir)/v_service.c \
	$(kernel_dir)/v_cmsoap.c $(kernel_dir)/v_fullCounter.c $(kernel_dir)/v_networkReader.c $(kernel_dir)/v_serviceManager.c \
	$(kernel_dir)/v_cmsoapStatistics.c $(kernel_dir)/v_group.c $(kernel_dir)/v_networkReaderEntry.c $(kernel_dir)/v_serviceState.c \
	$(kernel_dir)/v_collection.c $(kernel_dir)/v_groupCache.c $(kernel_dir)/v_networkReaderStatistics.c $(kernel_dir)/v_spliced.c \
	$(kernel_dir)/v_configuration.c $(kernel_dir)/v_groupEntrySet.c $(kernel_dir)/v_networking.c $(kernel_dir)/v_statistics.c \
	$(kernel_dir)/v_crc.c $(kernel_dir)/v_groupInstance.c $(kernel_dir)/v_networkingStatistics.c $(kernel_dir)/v_status.c \
	$(kernel_dir)/v_dataReader.c $(kernel_dir)/v_groupQueue.c $(kernel_dir)/v_observable.c $(kernel_dir)/v_subscriber.c \
	$(kernel_dir)/v_dataReaderEntry.c $(kernel_dir)/v_groupSet.c $(kernel_dir)/v_observer.c $(kernel_dir)/v_subscriberQos.c \
	$(kernel_dir)/v_dataReaderInstance.c $(kernel_dir)/v_groupStream.c $(kernel_dir)/v_participant.c $(kernel_dir)/v_time.c \
	$(kernel_dir)/v_dataReaderQuery.c $(kernel_dir)/v_handle.c $(kernel_dir)/v_participantQos.c $(kernel_dir)/v_timedValue.c \
	$(kernel_dir)/v_dataReaderSample.c $(kernel_dir)/v_historicalDataRequest.c $(kernel_dir)/v_partition.c \
	$(kernel_dir)/v_topic.c $(kernel_dir)/v_dataView.c $(kernel_dir)/v_index.c $(kernel_dir)/v_partitionAdmin.c \
	$(kernel_dir)/v_topicQos.c $(kernel_dir)/v_dataViewInstance.c $(kernel_dir)/v_instance.c $(kernel_dir)/v_policy.c \
	$(kernel_dir)/v_waitset.c $(kernel_dir)/v_dataViewQos.c $(kernel_dir)/v_kernel.c $(kernel_dir)/v_projection.c \
	$(kernel_dir)/v_writer.c $(kernel_dir)/v_dataViewQuery.c $(kernel_dir)/v_kernelQos.c $(kernel_dir)/v_proxy.c \
	$(kernel_dir)/v_writerCache.c $(kernel_dir)/v_dataViewSample.c $(kernel_dir)/v_kernelStatistics.c $(kernel_dir)/v_public.c \
	$(kernel_dir)/v_writerInstance.c $(kernel_dir)/v_deadLineInstanceList.c $(kernel_dir)/v_lease.c $(kernel_dir)/v_publisher.c \
	$(kernel_dir)/v_writerQos.c $(kernel_dir)/v_deliveryGuard.c $(kernel_dir)/v_leaseManager.c $(kernel_dir)/v_publisherQos.c \
	$(kernel_dir)/v_writerSample.c $(kernel_dir)/v_deliveryService.c $(kernel_dir)/v_lifespanAdmin.c $(kernel_dir)/v_qos.c \
	$(kernel_dir)/v_writerStatistics.c

# TODO: Probably rewrite the bin/sppodl
kernelModule.h : odlpp

# .l.c :
#	asdfasdf $(am__skiplex) $(SHELL) $(YLWRAP) $< $(LEX_OUTPUT_ROOT).c $@ -- $(LEXCOMPILE)

# gcc -m32 -static-libgcc -L/home/xlange/code/OpenSplice/lib/x86.linux2.6-dev -shared -fpic c_avltree.o c_base.o c_collection.o c_extent.o c_field.o c_iterator.o c_laptime.o c_metabase.o c_metafactory.o c_misc.o c_mmbase.o c_mmCache.o c_querybase.o c_scope.o c_serialize.o c_stringSupport.o c_sync.o c_time.o c_typebase.o q_expr.o q_parser.o -lc -lm -lpthread  -lddsos -lddsutil -o libddsdatabase.so
