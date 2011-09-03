# Copyright: Xavier Lange
# License: GPLv3

lib_LTLIBRARIES = libddsos.la libddsosnet.la libddsutil.la \
                  libddsdatabase.la libddskernel.la libddsconf.la \
                  libddsconfparser.la libddsuser.la \
                  libcmxml.la \
                  libdcpsgapi.la libdcpssac.la
EXTRA_DIST=src/database/database/code/q_parser.c src/database/database/include/q_parser.h
bin_PROGRAMS=cppgen


AUTOMAKE_OPTIONS = foreign 1.4
ACLOCAL_AMFLAGS = -I m4
noinst_PROGRAMS = odlpp

# TODO: Automagical platform discovery (with sweet override!)
ARCH=darwin
PA=x86_64
CFLAGS=-g -m32 -pipe\
       -Wall -W -pedantic -Wno-long-long \
       -DOSPL_STRICT_MEM #{CFLAGS} -DINCLUDE_PLUGGABLE_REPORTING -DOSPL_BUILD_OS -DVERSION="\\\"V5.4.1OSS\\\""\
       -D_TYPECHECK_ -DE_DEBUG -DDO_HOST_BY_NAME

##############################################################################
##### Grab bag of includes
##############################################################################

# There's a kooky convention that the abstract files should only require a #include "os_direct_name.h"
# but if you have a platform specific file you will need to do #include "include/os_direct_name.h"
# About weird entries:
# src/kernel/code was neede for a v__blah.h in libcmxml.la
# src/user/code was needed for a v__blah.h in libcmxml.la
INCLUDES := -Isrc/abstraction/os/code -Isrc/abstraction/os/include -Isrc/abstraction/os/$(ARCH) \
 -Isrc/abstraction/os-net/code -Isrc/abstraction/os-net/include -Isrc/abstraction/os-net/$(ARCH) \
 -Isrc/abstraction/pa/$(PA) \
 -Isrc/abstraction -I src/abstraction/os \
 -Isrc/utilities -Isrc/utilities/include \
 -Isrc/database/database/include \
 -Isrc/database/serialization/include \
 -Isrc/database/odlpp/code \
 -Isrc/kernel/include \
 -Isrc/kernel/code \
 -Isrc/configuration/config/include \
 -Isrc/configuration/parser/include \
 -Isrc/user/include \
 -Isrc/user/code \
 -Isrc/api/cm/xml/include \
 -Isrc/api/dcps/gapi/include \
 -Isrc/api/dcps/sac/include \
 -Isrc/tools/cppgen/include

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
$(kernel_dir)/v_avgValue.c : $(kernel_dir)/kernelModule.h
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
$(kernel_dir)/kernelModule.h : odlpp $(kernel_dir)/v_kernel.odl
	./.libs/odlpp $(kernel_dir)/v_kernel.odl

##############################################################################
##### Configuration
##############################################################################

config_dir=src/configuration/config/code
libddsconf_la_SOURCES = $(config_dir)/cf_attribute.c $(config_dir)/cf_data.c $(config_dir)/cf_element.c \
    $(config_dir)/cf_node.c $(config_dir)/cf_nodeList.c

##############################################################################
##### Configuration (parser)
##############################################################################

parser_dir=src/configuration/parser/code
libddsconfparser_la_SOURCES = $(parser_dir)/mcl.c $(parser_dir)/xml_tmplExp.c $(parser_dir)/cfg_parser.h \
	$(parser_dir)/cfg_parser.c

$(parser_dir)/../include/cfg_scanner.h : $(parser_dir)/cfg_scanner.l
	$(LEX) $(LFLAGS) $(AM_LFLAGS) -Pcfg_scanner_yy -o$(db_dir)/../include/cfg_scanner.h $<
$(parser_dir)/cfg_parser.c : $(parser_dir)/cfg_parser.y $(parser_dir)/../include/cfg_scanner.h
	$(YACC) $(YFLAGS) $< -o $@

##############################################################################
##### Configuration (parser)
##############################################################################

user_dir=src/user/code
libddsuser_la_SOURCES=$(user_dir)/u_cfAttribute.c $(user_dir)/u_cfData.c $(user_dir)/u_cfElement.c $(user_dir)/u_cfNode.c \
	$(user_dir)/u_cfValue.c $(user_dir)/u_dataReader.c $(user_dir)/u_dataView.c $(user_dir)/u_dataViewQos.c $(user_dir)/u_dispatcher.c \
	$(user_dir)/u_domain.c $(user_dir)/u_entity.c $(user_dir)/u_group.c $(user_dir)/u_groupQueue.c $(user_dir)/u_handle.c \
	$(user_dir)/u_instanceHandle.c $(user_dir)/u_listener.c $(user_dir)/u_networkReader.c $(user_dir)/u_participant.c \
	$(user_dir)/u_participantQos.c $(user_dir)/u_partition.c $(user_dir)/u_partitionQos.c $(user_dir)/u_publisher.c \
	$(user_dir)/u_publisherQos.c $(user_dir)/u_qos.c $(user_dir)/u_query.c $(user_dir)/u_reader.c $(user_dir)/u_readerQos.c \
	$(user_dir)/u_scheduler.c $(user_dir)/u_service.c $(user_dir)/u_serviceManager.c $(user_dir)/u_spliced.c $(user_dir)/u_subscriber.c\
	$(user_dir)/u_subscriberQos.c $(user_dir)/u_time.c $(user_dir)/u_topic.c $(user_dir)/u_topicQos.c $(user_dir)/u_types.c \
	$(user_dir)/u_user.c $(user_dir)/u_usrClock.c $(user_dir)/u_usrReportPlugin.c $(user_dir)/u_waitset.c $(user_dir)/u_waitsetEvent.c \
	$(user_dir)/u_writer.c $(user_dir)/u_writerQos.c

##############################################################################
##### Control and Monitoring API (AKA, CM)
##############################################################################

##############################################################################
##### CM (XML)
##############################################################################

cmxml_dir=src/api/cm/xml/code
libcmxml_la_SOURCES=$(cmxml_dir)/cmx_dataReader.c $(cmxml_dir)/cmx_domain.c $(cmxml_dir)/cmx_entity.c $(cmxml_dir)/cmx_factory.c $(cmxml_dir)/cmx_groupqueue.c $(cmxml_dir)/cmx_networkReader.c $(cmxml_dir)/cmx_participant.c $(cmxml_dir)/cmx_publisher.c $(cmxml_dir)/cmx_qos.c $(cmxml_dir)/cmx_query.c $(cmxml_dir)/cmx_reader.c $(cmxml_dir)/cmx_readerSnapshot.c $(cmxml_dir)/cmx_service.c $(cmxml_dir)/cmx_serviceState.c $(cmxml_dir)/cmx_snapshot.c $(cmxml_dir)/cmx_subscriber.c $(cmxml_dir)/cmx_topic.c $(cmxml_dir)/cmx_waitset.c $(cmxml_dir)/cmx_writer.c $(cmxml_dir)/cmx_writerSnapshot.c

##############################################################################
##### CM (Java/JNI)
##############################################################################

# TODO

##############################################################################
##### CM (DCPS/GAPI)
##############################################################################

dcpsgapi_dir=src/api/dcps/gapi/code
libdcpsgapi_la_SOURCES=$(dcpsgapi_dir)/gapi_builtin.c $(dcpsgapi_dir)/gapi_common.c $(dcpsgapi_dir)/gapi_condition.c $(dcpsgapi_dir)/gapi_contentFilteredTopic.c $(dcpsgapi_dir)/gapi_dataReader.c $(dcpsgapi_dir)/gapi_dataReaderView.c $(dcpsgapi_dir)/gapi_dataWriter.c $(dcpsgapi_dir)/gapi_domain.c $(dcpsgapi_dir)/gapi_domainParticipant.c $(dcpsgapi_dir)/gapi_domainParticipantFactory.c $(dcpsgapi_dir)/gapi_entity.c $(dcpsgapi_dir)/gapi_error.c $(dcpsgapi_dir)/gapi_errorInfo.c $(dcpsgapi_dir)/gapi_expression.c $(dcpsgapi_dir)/gapi_fooDataReader.c $(dcpsgapi_dir)/gapi_fooDataReaderView.c $(dcpsgapi_dir)/gapi_fooDataWriter.c $(dcpsgapi_dir)/gapi_fooTypeSupport.c $(dcpsgapi_dir)/gapi_genericCopyBuffer.c $(dcpsgapi_dir)/gapi_genericCopyCache.c $(dcpsgapi_dir)/gapi_genericCopyIn.c $(dcpsgapi_dir)/gapi_genericCopyOut.c $(dcpsgapi_dir)/gapi_kernel.c $(dcpsgapi_dir)/gapi_loanRegistry.c $(dcpsgapi_dir)/gapi_map.c $(dcpsgapi_dir)/gapi_metaDataInterface.c $(dcpsgapi_dir)/gapi_multiTopic.c $(dcpsgapi_dir)/gapi_objManag.c $(dcpsgapi_dir)/gapi_object.c $(dcpsgapi_dir)/gapi_publisher.c $(dcpsgapi_dir)/gapi_qos.c $(dcpsgapi_dir)/gapi_scheduler.c $(dcpsgapi_dir)/gapi_status.c $(dcpsgapi_dir)/gapi_structured.c $(dcpsgapi_dir)/gapi_subscriber.c $(dcpsgapi_dir)/gapi_topic.c $(dcpsgapi_dir)/gapi_topicDescription.c $(dcpsgapi_dir)/gapi_typeSupport.c $(dcpsgapi_dir)/gapi_vector.c $(dcpsgapi_dir)/gapi_waitSet.c $(dcpsgapi_dir)/gapi_waitSetDomainEntry.c

##############################################################################
##### CM (DCPS/SAC)
##############################################################################

dcpssac_dir=src/api/dcps/sac/code
libdcpssac_la_SOURCES=$(dcpssac_dir)/sac_builtinTopics.c $(dcpssac_dir)/sac_common.c $(dcpssac_dir)/sac_condition.c $(dcpssac_dir)/sac_contentFilteredTopic.c $(dcpssac_dir)/sac_dataReader.c $(dcpssac_dir)/sac_dataReaderView.c $(dcpssac_dir)/sac_dataWriter.c $(dcpssac_dir)/sac_dcpsSplDcps.c $(dcpssac_dir)/sac_domain.c $(dcpssac_dir)/sac_domainParticipant.c $(dcpssac_dir)/sac_domainParticipantFactory.c $(dcpssac_dir)/sac_entity.c $(dcpssac_dir)/sac_errorInfo.c $(dcpssac_dir)/sac_fooDataReader.c $(dcpssac_dir)/sac_fooDataReaderView.c $(dcpssac_dir)/sac_fooDataWriter.c $(dcpssac_dir)/sac_fooTypeSupport.c $(dcpssac_dir)/sac_guardCondition.c $(dcpssac_dir)/sac_multiTopic.c $(dcpssac_dir)/sac_objManag.c $(dcpssac_dir)/sac_publisher.c $(dcpssac_dir)/sac_queryCondition.c $(dcpssac_dir)/sac_readCondition.c $(dcpssac_dir)/sac_statusCondition.c $(dcpssac_dir)/sac_structured.c $(dcpssac_dir)/sac_subscriber.c $(dcpssac_dir)/sac_topic.c $(dcpssac_dir)/sac_topicDescription.c $(dcpssac_dir)/sac_typeSupport.c $(dcpssac_dir)/sac_waitSet.c

##############################################################################
##### Tools
##############################################################################

##############################################################################
##### Tool (cppgen)
##############################################################################

cppgen_dir=src/tools/cppgen/code
cppgen_SOURCES=$(cppgen_dir)/ast_argument.cpp $(cppgen_dir)/ast_array.cpp $(cppgen_dir)/ast_attribute.cpp $(cppgen_dir)/ast_boxed_value.cpp $(cppgen_dir)/ast_check.cpp $(cppgen_dir)/ast_concrete_type.cpp $(cppgen_dir)/ast_constant.cpp $(cppgen_dir)/ast_decl.cpp $(cppgen_dir)/ast_enum.cpp $(cppgen_dir)/ast_enum_val.cpp $(cppgen_dir)/ast_exception.cpp $(cppgen_dir)/ast_expression.cpp $(cppgen_dir)/ast_field.cpp $(cppgen_dir)/ast_generator.cpp $(cppgen_dir)/ast_hdrs.cpp $(cppgen_dir)/ast_initializer.cpp $(cppgen_dir)/ast_interface.cpp $(cppgen_dir)/ast_interface_fwd.cpp $(cppgen_dir)/ast_module.cpp $(cppgen_dir)/ast_opaque.cpp $(cppgen_dir)/ast_operation.cpp $(cppgen_dir)/ast_predefined_type.cpp $(cppgen_dir)/ast_recursive.cpp $(cppgen_dir)/ast_redef.cpp $(cppgen_dir)/ast_root.cpp $(cppgen_dir)/ast_sequence.cpp $(cppgen_dir)/ast_statemember.cpp $(cppgen_dir)/ast_string.cpp $(cppgen_dir)/ast_structure.cpp $(cppgen_dir)/ast_type.cpp $(cppgen_dir)/ast_typedef.cpp $(cppgen_dir)/ast_union.cpp $(cppgen_dir)/ast_union_branch.cpp $(cppgen_dir)/ast_union_label.cpp $(cppgen_dir)/ast_value.cpp $(cppgen_dir)/ast_value_fwd.cpp $(cppgen_dir)/cppgen.cpp $(cppgen_dir)/drv_args.cpp $(cppgen_dir)/drv_fork.cpp $(cppgen_dir)/drv_init.cpp $(cppgen_dir)/drv_link.cpp $(cppgen_dir)/drv_main.cpp $(cppgen_dir)/drv_param.cpp $(cppgen_dir)/drv_preproc.cpp $(cppgen_dir)/drv_private.cpp $(cppgen_dir)/fe_declarator.cpp $(cppgen_dir)/fe_extern.cpp $(cppgen_dir)/fe_init.cpp $(cppgen_dir)/fe_interface_header.cpp $(cppgen_dir)/fe_private.cpp $(cppgen_dir)/fe_value_header.cpp $(cppgen_dir)/fe_value_inheritance_spec.cpp $(cppgen_dir)/lex_yy.cpp $(cppgen_dir)/nr_narrow.cpp $(cppgen_dir)/std.cpp $(cppgen_dir)/utl_decllist.cpp $(cppgen_dir)/utl_error.cpp $(cppgen_dir)/utl_exceptlist.cpp $(cppgen_dir)/utl_exprlist.cpp $(cppgen_dir)/utl_global.cpp $(cppgen_dir)/utl_identifier.cpp $(cppgen_dir)/utl_idlist.cpp $(cppgen_dir)/utl_indenter.cpp $(cppgen_dir)/utl_labellist.cpp $(cppgen_dir)/utl_list.cpp $(cppgen_dir)/utl_namelist.cpp $(cppgen_dir)/utl_pragmas.cpp $(cppgen_dir)/utl_scope.cpp $(cppgen_dir)/utl_stack.cpp $(cppgen_dir)/utl_string.cpp $(cppgen_dir)/utl_strlist.cpp $(cppgen_dir)/utl_wstring.cpp $(cppgen_dir)/xbe_arglist.cpp $(cppgen_dir)/xbe_args.cpp $(cppgen_dir)/xbe_argument.cpp $(cppgen_dir)/xbe_argument2.cpp $(cppgen_dir)/xbe_array.cpp $(cppgen_dir)/xbe_attribute.cpp $(cppgen_dir)/xbe_classes.cpp $(cppgen_dir)/xbe_classgen.cpp $(cppgen_dir)/xbe_codegen.cpp $(cppgen_dir)/xbe_constant.cpp $(cppgen_dir)/xbe_cppfwd.cpp $(cppgen_dir)/xbe_cppname.cpp $(cppgen_dir)/xbe_cppscope.cpp $(cppgen_dir)/xbe_cpptype.cpp $(cppgen_dir)/xbe_direction.cpp $(cppgen_dir)/xbe_dispatchable.cpp $(cppgen_dir)/xbe_dispatcher.cpp $(cppgen_dir)/xbe_enum.cpp $(cppgen_dir)/xbe_exception.cpp $(cppgen_dir)/xbe_expr_val.cpp $(cppgen_dir)/xbe_field.cpp $(cppgen_dir)/xbe_generator.cpp $(cppgen_dir)/xbe_genlist.cpp $(cppgen_dir)/xbe_globals.cpp $(cppgen_dir)/xbe_hash.cpp $(cppgen_dir)/xbe_incl.cpp $(cppgen_dir)/xbe_init.cpp $(cppgen_dir)/xbe_interface.cpp $(cppgen_dir)/xbe_interface2.cpp $(cppgen_dir)/xbe_interface3.cpp $(cppgen_dir)/xbe_invoke.cpp $(cppgen_dir)/xbe_literals.cpp $(cppgen_dir)/xbe_module.cpp $(cppgen_dir)/xbe_operation.cpp $(cppgen_dir)/xbe_opnameset.cpp $(cppgen_dir)/xbe_predefined.cpp $(cppgen_dir)/xbe_produce.cpp $(cppgen_dir)/xbe_root.cpp $(cppgen_dir)/xbe_sequence.cpp $(cppgen_dir)/xbe_source.cpp $(cppgen_dir)/xbe_string.cpp $(cppgen_dir)/xbe_structure.cpp $(cppgen_dir)/xbe_time.cpp $(cppgen_dir)/xbe_type.cpp $(cppgen_dir)/xbe_typedef.cpp $(cppgen_dir)/xbe_union.cpp $(cppgen_dir)/xbe_utils.cpp $(cppgen_dir)/xbe_value.cpp $(cppgen_dir)/xps_string.cpp $(cppgen_dir)/xps_vector.cpp $(cppgen_dir)/y_tab.cpp