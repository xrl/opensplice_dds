/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2010 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */

#ifndef NW_SOCKETMULTICAST_H
#define NW_SOCKETMULTICAST_H

#include "os_socket.h"
#include "nw_socket.h"

os_int
nw_socketGetDefaultMulticastInterface(
    const char *addressLookingFor,
    os_int sockfd,
    struct sockaddr_in *sockAddrPrimary,
    struct sockaddr_in *sockAddrBroadcasat);


void
nw_socketMulticastInitialize(
    nw_socket socket,
    sk_bool receiving,
    sk_address address);

void
nw_socketMulticastAddPartition(
    nw_socket sock,
    const char *addressString,
    sk_bool receiving,
    c_ulong mTTL);

os_int
nw_socketMulticastSetTTL(
    nw_socket socket,
    c_ulong timeToLive);



#endif /* NW_SOCKETMULTICAST_H */
