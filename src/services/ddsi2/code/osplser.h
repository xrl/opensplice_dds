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
#ifndef OSPLSER_H
#define OSPLSER_H

#include "os_abstract.h" /* big or little endianness */
#include "kernelModule.h"

#ifdef PA_BIG_ENDIAN
#define PLATFORM_IS_LITTLE_ENDIAN 0
#endif
#ifdef PA_LITTLE_ENDIAN
#ifdef PLATFORM_IS_LITTLE_ENDIAN
#error PA_BIG_ENDIAN and PA_LITTLE_ENDIAN both defined?
#endif
#define PLATFORM_IS_LITTLE_ENDIAN 1
#endif /* PA_LITTLE_ENDIAN */
#ifndef PLATFORM_IS_LITTLE_ENDIAN
#error PLATFORM_IS_LITTLE_ENDIAN undefined
#endif

typedef struct serstatepool *serstatepool_t;
typedef struct serstate *serstate_t;
typedef struct topic *topic_t;

struct cdrstring {
  unsigned length;
  char contents[1]; /* C90 does not support flex. array members */
};

struct CDRHeader {
  unsigned short identifier;
  unsigned short options;
};
#if PLATFORM_IS_LITTLE_ENDIAN
#define CDR_BE 0x0000
#define CDR_LE 0x0100
#else
#define CDR_BE 0x0000
#define CDR_LE 0x0001
#endif

typedef struct serdata {
  union {
    serstate_t st;	/* back pointer to (opaque) serstate so RTPS impl only needs serdata */
    long long align_i;
    double align_f;
  } u;
  char key[32];		/* @  0+X <= copies + 32-bit offsets (from &key[0] into data) to CDR strings */
  unsigned isstringref;	/* @ 32+X <= (isstringref & (1 << k)) iff key[k] is first of an offset */
  struct CDRHeader hdr;	/* @ 36+X */
  char data[1];		/* @ 40+X <= must be suitably aligned for all primitive types; C90 does not support flex. array members ... */
} *serdata_t;

extern topic_t osplser_topic4u;

int osplser_init (void *base);
void osplser_fini (void);

topic_t deftopic (C_STRUCT(v_topic) const * const ospl_topic, const char *keystr);
int topic_haskey (const struct topic *tp);
const char *topic_name (const struct topic *tp);
const char *topic_typename (const struct topic *tp);
void freetopic (topic_t tp);

serstatepool_t serstatepool_new (void);
void serstatepool_free (serstatepool_t pool);

serdata_t serialize_raw (serstatepool_t pool, const struct topic *tp, const void *data);
serdata_t serialize (serstatepool_t pool, const struct topic *tp, C_STRUCT (v_message) const *msg);
serdata_t serialize_key (serstatepool_t pool, const struct topic *tp, C_STRUCT (v_message) const *msg);
void serdata_release (serdata_t serdata);
void serdata_keyhash (const struct serdata *d, char keyhash[16]);
int serdata_cmp (const struct serdata *a, const struct serdata *b);
int serdata_size (const struct serdata *serdata);
int serdata_keyhash_exact_p (const struct serdata *serdata);
const struct topic *serdata_topic (const struct serdata *serdata);

v_message deserialize (const struct topic *topic, const void *vsrc, int vsrcsize);
v_message deserialize_from_key (const struct topic *topic, const void *vsrc, int vsrcsize);
v_message deserialize_from_keyhash (const struct topic *topic, const void *vsrc, int vsrcsize);

int prettyprint_serdata (char *dst, const int dstsize, const struct serdata *serdata);
int prettyprint_raw (char *dst, const int dstsize, const struct topic *topic, const void *vsrc, int vsrcsize);

int serdata_verify (serdata_t serdata, C_STRUCT (v_message) const *data);

#endif /* OSPLSER_H */
