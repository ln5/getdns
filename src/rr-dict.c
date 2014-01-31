/**
 *
 * /brief getdns support functions for DNS Resource Records
 *
 * This file contains the tables with the information needed by getdns about
 * individual RRs, such as their name and rdata fields and types.
 * This information is provided via the response dict.
 *
 */
/*
 * Copyright (c) 2013, Versign, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the <organization> nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Verisign, Inc. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "rr-dict.h"
#include "types-internal.h"
#include "context.h"

#define ALEN(a) (sizeof(a)/sizeof(a[0]))

struct rdata_def {
	const char *name;
	int type;
};

struct rr_def { 
	const char *name;
	const struct rdata_def *rdata;
	int n_rdata_fields;
};

static struct rdata_def          a_rdata[] = {
	{ "ipv4_address"                , t_bindata }};
static struct rdata_def         ns_rdata[] = {
	{ "nsdname"                     , t_bindata }};
static struct rdata_def         md_rdata[] = {
	{ "madname"                     , t_bindata }};
static struct rdata_def         mf_rdata[] = {
	{ "madname"                     , t_bindata }};
static struct rdata_def      cname_rdata[] = {
	{ "cname"                       , t_bindata }};
static struct rdata_def        soa_rdata[] = {
	{ "mname"                       , t_bindata },
	{ "rname"                       , t_bindata },
	{ "serial"                      , t_int     },
	{ "refresh"                     , t_int     },
	{ "refresh"                     , t_int     },
	{ "retry"                       , t_int     },
	{ "expire"                      , t_int     }};
static struct rdata_def         mb_rdata[] = {
	{ "madname"                     , t_bindata }};
static struct rdata_def         mg_rdata[] = {
	{ "mgmname"                     , t_bindata }};
static struct rdata_def         mr_rdata[] = {
	{ "newname"                     , t_bindata }};
static struct rdata_def       null_rdata[] = {
	{ "anything"                    , t_bindata }};
static struct rdata_def        wks_rdata[] = {
	{ "address"                     , t_bindata },
	{ "protocol"                    , t_int     },
	{ "bitmap"                      , t_bindata }};
static struct rdata_def        ptr_rdata[] = {
	{ "ptrdname"                    , t_bindata }};
static struct rdata_def      hinfo_rdata[] = {
	{ "cpu"                         , t_bindata }};
static struct rdata_def      minfo_rdata[] = {
	{ "rmailbx"                     , t_bindata }};
static struct rdata_def         mx_rdata[] = {
	{ "preference"                  , t_bindata }};
static struct rdata_def        txt_rdata[] = {
	{ "txt_strings"                 , t_list    }};
static struct rdata_def         rp_rdata[] = {
	{ "mbox_dname"                  , t_bindata }};
static struct rdata_def      afsdb_rdata[] = {
	{ "subtype"                     , t_bindata }};
static struct rdata_def        x25_rdata[] = {
	{ "psdn_address"                , t_bindata }};
static struct rdata_def       isdn_rdata[] = {
	{ "isdn_address"                , t_bindata }};
static struct rdata_def         rt_rdata[] = {
	{ "preference"                  , t_bindata }};
static struct rdata_def       nsap_rdata[] = {
	{ "nsap"                        , t_bindata }};
static struct rdata_def        sig_rdata[] = {
	{ "sig_obsolete"                , t_bindata }};
static struct rdata_def        key_rdata[] = {
	{ "key_obsolete"                , t_bindata }};
static struct rdata_def         px_rdata[] = {
	{ "preference"                  , t_int     },
	{ "map822"                      , t_bindata },
	{ "mapx400"                     , t_bindata }};
static struct rdata_def       gpos_rdata[] = {
	{ "longitude"                   , t_bindata },
	{ "latitude"                    , t_bindata },
	{ "altitude"                    , t_bindata }};
static struct rdata_def       aaaa_rdata[] = {
	{ "ipv6_address"                , t_bindata }};
static struct rdata_def        loc_rdata[] = {
	{ "loc_obsolete"                , t_bindata }};
static struct rdata_def        nxt_rdata[] = {
	{ "nxt_obsolete"                , t_bindata }};
static struct rdata_def        eid_rdata[] = {
	{ "eid_unknown"                 , t_bindata }};
static struct rdata_def     nimloc_rdata[] = {
	{ "nimloc_unknown"              , t_bindata }};
static struct rdata_def        srv_rdata[] = {
	{ "priority"                    , t_int     },
	{ "weight"                      , t_int     },
	{ "port"                        , t_int     },
	{ "target"                      , t_bindata }};
static struct rdata_def       atma_rdata[] = {
	{ "format"                      , t_bindata }};
static struct rdata_def      naptr_rdata[] = {
	{ "order"                       , t_int     },
	{ "preference"                  , t_int     },
	{ "flags"                       , t_bindata },
	{ "service"                     , t_bindata },
	{ "regexp"                      , t_bindata },
	{ "replacement"                 , t_bindata }};
static struct rdata_def         kx_rdata[] = {
	{ "preference"                  , t_bindata }};
static struct rdata_def       cert_rdata[] = {
	{ "type"                        , t_int     },
	{ "key_tag"                     , t_int     },
	{ "algorithm"                   , t_int     },
	{ "certificate_or_crl"          , t_bindata }};
static struct rdata_def         a6_rdata[] = {
	{ "a6_obsolete"                 , t_bindata }};
static struct rdata_def      dname_rdata[] = {
	{ "target"                      , t_bindata }};
static struct rdata_def       sink_rdata[] = {
	{ "sink_unknown"                , t_bindata }};
static struct rdata_def        opt_rdata[] = {
	{ "options"                     , t_dict    },
	{ "option_code"                 , t_int     },
	{ "option_data"                 , t_bindata }};
static struct rdata_def        apl_rdata[] = {
	{ "apitems"                     , t_dict    },
	{ "address_family"              , t_int     },
	{ "prefix"                      , t_int     },
	{ "n"                           , t_int     },
	{ "afdpart"                     , t_bindata }};
static struct rdata_def         ds_rdata[] = {
	{ "key_tag"                     , t_int     },
	{ "algorithm"                   , t_int     },
	{ "digest_type"                 , t_int     },
	{ "digest"                      , t_bindata }};
static struct rdata_def      sshfp_rdata[] = {
	{ "algorithm"                   , t_int     },
	{ "fp_type"                     , t_int     },
	{ "fingerprint"                 , t_bindata }};
static struct rdata_def   ipseckey_rdata[] = {
	{ "algorithm"                   , t_int     },
	{ "gateway_type"                , t_int     },
	{ "precedence"                  , t_int     },
	{ "gateway"                     , t_bindata },
	{ "public_key"                  , t_bindata }};
static struct rdata_def      rrsig_rdata[] = {
	{ "type_covered"                , t_int     },
	{ "algorithm"                   , t_int     },
	{ "labels"                      , t_int     },
	{ "original_ttl"                , t_int     },
	{ "signature_expiration"        , t_int     },
	{ "signature_inception"         , t_int     },
	{ "key_tag"                     , t_int     },
	{ "signers_name"                , t_bindata },
	{ "signature"                   , t_bindata }};
static struct rdata_def       nsec_rdata[] = {
	{ "next_domain_name"            , t_bindata }};
static struct rdata_def     dnskey_rdata[] = {
	{ "flags"                       , t_int     },
	{ "protocol"                    , t_int     },
	{ "algorithm"                   , t_int     },
	{ "public_key"                  , t_bindata }};
static struct rdata_def      dhcid_rdata[] = {
	{ "dhcid_opaque"                , t_bindata }};
static struct rdata_def      nsec3_rdata[] = {
	{ "hash_algorithm"              , t_int     },
	{ "flags"                       , t_int     },
	{ "iterations"                  , t_int     },
	{ "salt"                        , t_bindata },
	{ "next_hashed_owner_name"      , t_bindata },
	{ "type_bit_maps"               , t_bindata }};
static struct rdata_def nsec3param_rdata[] = {
	{ "hash_algorithm"              , t_int     },
	{ "flags"                       , t_int     },
	{ "iterations"                  , t_int     },
	{ "salt"                        , t_bindata }};
static struct rdata_def       tlsa_rdata[] = {
	{ "certificate_usage"           , t_int     },
	{ "selector"                    , t_int     },
	{ "matching_type"               , t_int     },
	{ "certificate_association_data", t_bindata }};
static struct rdata_def        hip_rdata[] = {
	{ "pk_algorithm"                , t_int     },
	{ "hit"                         , t_bindata },
	{ "public_key"                  , t_bindata },
	{ "rendezvous_servers"          , t_list    }};
static struct rdata_def      ninfo_rdata[] = {
	{ "ninfo_unknown"               , t_bindata }};
static struct rdata_def       rkey_rdata[] = {
	{ "rkey_unknown"                , t_bindata }};
static struct rdata_def     talink_rdata[] = {
	{ "talink_unknown"              , t_bindata }};
static struct rdata_def        cds_rdata[] = {
	{ "cds_unknown"                 , t_bindata }};
static struct rdata_def        spf_rdata[] = {
	{ "text"                        , t_bindata }};
static struct rdata_def      uinfo_rdata[] = {
	{ "uinfo_unknown"               , t_bindata }};
static struct rdata_def        uid_rdata[] = {
	{ "uid_unknown"                 , t_bindata }};
static struct rdata_def        gid_rdata[] = {
	{ "gid_unknown"                 , t_bindata }};
static struct rdata_def     unspec_rdata[] = {
	{ "unspec_unknown"              , t_bindata }};
static struct rdata_def        nid_rdata[] = {
	{ "preference"                  , t_int     },
	{ "node_id"                     , t_bindata }};
static struct rdata_def        l32_rdata[] = {
	{ "preference"                  , t_int     },
	{ "locator32"                   , t_bindata }};
static struct rdata_def        l64_rdata[] = {
	{ "preference"                  , t_int     },
	{ "locator64"                   , t_bindata }};
static struct rdata_def         lp_rdata[] = {
	{ "preference"                  , t_int     },
	{ "fqdn"                        , t_bindata }};
static struct rdata_def      eui48_rdata[] = {
	{ "eui48_address"               , t_bindata }};
static struct rdata_def      eui64_rdata[] = {
	{ "eui64_address"               , t_bindata }};
static struct rdata_def       tkey_rdata[] = {
	{ "algorithm"                   , t_bindata },
	{ "inception"                   , t_int     },
	{ "expiration"                  , t_int     },
	{ "mode"                        , t_int     },
	{ "error"                       , t_int     },
	{ "key_data"                    , t_bindata },
	{ "other_data"                  , t_bindata }};
static struct rdata_def       tsig_rdata[] = {
	{ "algorithm"                   , t_bindata },
	{ "time_signed"                 , t_bindata },
	{ "fudge"                       , t_int     },
	{ "mac"                         , t_bindata },
	{ "original_id"                 , t_int     },
	{ "error"                       , t_int     },
	{ "other_data"                  , t_bindata }};
static struct rdata_def      mailb_rdata[] = {
	{ "mailb_unknown"               , t_bindata }};
static struct rdata_def      maila_rdata[] = {
	{ "maila_unknown"               , t_bindata }};
static struct rdata_def        uri_rdata[] = {
	{ "priority"                    , t_int     },
	{ "weight"                      , t_int     },
	{ "target"                      , t_bindata }};
static struct rdata_def        caa_rdata[] = {
	{ "flags"                       , t_int     },
	{ "tag"                         , t_bindata },
	{ "value"                       , t_bindata }};
static struct rdata_def         ta_rdata[] = {
	{ "ta_unknown"                  , t_bindata }};
static struct rdata_def        dlv_rdata[] = {
	{ "key_tag"                     , t_int     },
	{ "algorithm"                   , t_int     },
	{ "digest_type"                 , t_int     },
	{ "digest"                      , t_bindata }};

static struct rr_def rr_defs[] = {
	{         NULL,             NULL, 0                      },
	{          "A",          a_rdata, ALEN(         a_rdata) }, /* 1 - */
	{         "NS",         ns_rdata, ALEN(        ns_rdata) },
	{         "MD",         md_rdata, ALEN(        md_rdata) },
	{         "MF",         mf_rdata, ALEN(        mf_rdata) },
	{      "CNAME",      cname_rdata, ALEN(     cname_rdata) },
	{        "SOA",        soa_rdata, ALEN(       soa_rdata) },
	{         "MB",         mb_rdata, ALEN(        mb_rdata) },
	{         "MG",         mg_rdata, ALEN(        mg_rdata) },
	{         "MR",         mr_rdata, ALEN(        mr_rdata) },
	{       "NULL",       null_rdata, ALEN(      null_rdata) },
	{        "WKS",        wks_rdata, ALEN(       wks_rdata) },
	{        "PTR",        ptr_rdata, ALEN(       ptr_rdata) },
	{      "HINFO",      hinfo_rdata, ALEN(     hinfo_rdata) },
	{      "MINFO",      minfo_rdata, ALEN(     minfo_rdata) },
	{         "MX",         mx_rdata, ALEN(        mx_rdata) },
	{        "TXT",        txt_rdata, ALEN(       txt_rdata) },
	{         "RP",         rp_rdata, ALEN(        rp_rdata) },
	{      "AFSDB",      afsdb_rdata, ALEN(     afsdb_rdata) },
	{        "X25",        x25_rdata, ALEN(       x25_rdata) },
	{       "ISDN",       isdn_rdata, ALEN(      isdn_rdata) },
	{         "RT",         rt_rdata, ALEN(        rt_rdata) },
	{       "NSAP",       nsap_rdata, ALEN(      nsap_rdata) }, /* - 22 */
	{         NULL,             NULL, 0                      },
	{        "SIG",        sig_rdata, ALEN(       sig_rdata) }, /* 24 - */
	{        "KEY",        key_rdata, ALEN(       key_rdata) },
	{         "PX",         px_rdata, ALEN(        px_rdata) },
	{       "GPOS",       gpos_rdata, ALEN(      gpos_rdata) },
	{       "AAAA",       aaaa_rdata, ALEN(      aaaa_rdata) },
	{        "LOC",        loc_rdata, ALEN(       loc_rdata) },
	{        "NXT",        nxt_rdata, ALEN(       nxt_rdata) },
	{        "EID",        eid_rdata, ALEN(       eid_rdata) },
	{     "NIMLOC",     nimloc_rdata, ALEN(    nimloc_rdata) },
	{        "SRV",        srv_rdata, ALEN(       srv_rdata) },
	{       "ATMA",       atma_rdata, ALEN(      atma_rdata) },
	{      "NAPTR",      naptr_rdata, ALEN(     naptr_rdata) },
	{         "KX",         kx_rdata, ALEN(        kx_rdata) },
	{       "CERT",       cert_rdata, ALEN(      cert_rdata) },
	{         "A6",         a6_rdata, ALEN(        a6_rdata) },
	{      "DNAME",      dname_rdata, ALEN(     dname_rdata) },
	{       "SINK",       sink_rdata, ALEN(      sink_rdata) },
	{        "OPT",        opt_rdata, ALEN(       opt_rdata) },
	{        "APL",        apl_rdata, ALEN(       apl_rdata) },
	{         "DS",         ds_rdata, ALEN(        ds_rdata) },
	{      "SSHFP",      sshfp_rdata, ALEN(     sshfp_rdata) },
	{   "IPSECKEY",   ipseckey_rdata, ALEN(  ipseckey_rdata) },
	{      "RRSIG",      rrsig_rdata, ALEN(     rrsig_rdata) },
	{       "NSEC",       nsec_rdata, ALEN(      nsec_rdata) },
	{     "DNSKEY",     dnskey_rdata, ALEN(    dnskey_rdata) },
	{      "DHCID",      dhcid_rdata, ALEN(     dhcid_rdata) },
	{      "NSEC3",      nsec3_rdata, ALEN(     nsec3_rdata) },
	{ "NSEC3PARAM", nsec3param_rdata, ALEN(nsec3param_rdata) },
	{       "TLSA",       tlsa_rdata, ALEN(      tlsa_rdata) }, /* - 52 */
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{        "HIP",        hip_rdata, ALEN(       hip_rdata) }, /* 55 - */
	{      "NINFO",      ninfo_rdata, ALEN(     ninfo_rdata) },
	{       "RKEY",       rkey_rdata, ALEN(      rkey_rdata) },
	{     "TALINK",     talink_rdata, ALEN(    talink_rdata) },
	{        "CDS",        cds_rdata, ALEN(       cds_rdata) }, /* - 59 */
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{        "SPF",        spf_rdata, ALEN(       spf_rdata) }, /* 99 - */
	{      "UINFO",      uinfo_rdata, ALEN(     uinfo_rdata) },
	{        "UID",        uid_rdata, ALEN(       uid_rdata) },
	{        "GID",        gid_rdata, ALEN(       gid_rdata) },
	{     "UNSPEC",     unspec_rdata, ALEN(    unspec_rdata) },
	{        "NID",        nid_rdata, ALEN(       nid_rdata) },
	{        "L32",        l32_rdata, ALEN(       l32_rdata) },
	{        "L64",        l64_rdata, ALEN(       l64_rdata) },
	{         "LP",         lp_rdata, ALEN(        lp_rdata) },
	{      "EUI48",      eui48_rdata, ALEN(     eui48_rdata) },
	{      "EUI64",      eui64_rdata, ALEN(     eui64_rdata) }, /* - 109 */
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{       "TKEY",       tkey_rdata, ALEN(      tkey_rdata) }, /* 249 - */
	{       "TSIG",       tsig_rdata, ALEN(      tsig_rdata) }, /* - 250 */
	{         NULL,             NULL, 0                      },
	{         NULL,             NULL, 0                      },
	{      "MAILB",      mailb_rdata, ALEN(     mailb_rdata) }, /* 253 - */
	{      "MAILA",      maila_rdata, ALEN(     maila_rdata) }, /* - 254 */
	{         NULL,             NULL, 0                      },
	{        "URI",        uri_rdata, ALEN(       uri_rdata) }, /* 256 - */
	{        "CAA",        caa_rdata, ALEN(       caa_rdata) }, /* - 257 */
	{         "TA",         ta_rdata, ALEN(        ta_rdata) }, /* 32768 */
	{        "DLV",        dlv_rdata, ALEN(       dlv_rdata) }  /* 32769 */
};

static const struct rr_def *
rr_def_lookup(uint16_t rr_type)
{
	if (rr_type <= 257)
		return &rr_defs[rr_type];
	else if (rr_type == 32768)
		return &rr_defs[258];
	else if (rr_type == 32769)
		return &rr_defs[259];
	return rr_defs;
}

const char *
priv_getdns_rr_type_name(int rr_type)
{
	return rr_def_lookup(rr_type)->name;
}

static getdns_return_t
priv_getdns_equip_dict_with_rdfs(struct getdns_dict *rdata, ldns_rr *rr)
{
	getdns_return_t r = GETDNS_RETURN_GOOD;
	const struct rr_def *def;
	struct getdns_bindata bindata;
	size_t i;
	int intval;

	assert(rdata);
	assert(rr);

	def = rr_def_lookup(ldns_rr_get_type(rr));
	for (i = 0; i < ldns_rr_rd_count(rr) && r == GETDNS_RETURN_GOOD; i++) {
		if (i >= def->n_rdata_fields)
			break;

		switch (def->rdata[i].type) {
		case t_bindata: bindata.size = ldns_rdf_size(ldns_rr_rdf(rr, i));
		                bindata.data = ldns_rdf_data(ldns_rr_rdf(rr, i));
		                r = getdns_dict_set_bindata(
				    rdata, (char *)def->rdata[i].name, &bindata);
				break;
		case t_int    : switch (ldns_rdf_size(ldns_rr_rdf(rr, i))) {
				case  1: intval = (uint8_t)*ldns_rdf_data(
				             ldns_rr_rdf(rr, i));
					 break;
				case  2: intval = ldns_read_uint16(
				             ldns_rdf_data(ldns_rr_rdf(rr, i)));
				         break;
				case  4: intval = ldns_read_uint32(
				             ldns_rdf_data(ldns_rr_rdf(rr, i)));
					 break;
				default: intval = -1;
				         /* TODO Compare with LDNS rdf types */
					 break;
		                }
		                r = getdns_dict_set_int(
				    rdata, (char *)def->rdata[i].name, intval);
				break;
		default       : break;
		}
	}
	return r;
}

static getdns_return_t
priv_getdns_create_dict_from_rdfs(
    struct getdns_context *context, ldns_rr *rr, struct getdns_dict** rdata)
{
	getdns_return_t r = GETDNS_RETURN_GOOD;
	struct getdns_bindata rdata_raw;
	uint8_t *data_ptr;
	size_t i;

	assert(context);
	assert(rr);
	assert(rdata);

	*rdata = getdns_dict_create_with_context(context);
	if (! *rdata)
		return GETDNS_RETURN_MEMORY_ERROR;
	do { /* break on error (to cleanup *rdata) */

		/* Count and reserve "raw" rdata space */
		rdata_raw.size = 0;
		for (i = 0; i < ldns_rr_rd_count(rr); i++)
			rdata_raw.size += ldns_rdf_size(ldns_rr_rdf(rr, i));
		rdata_raw.data = GETDNS_XMALLOC(
		    context->mf, uint8_t, rdata_raw.size);
		if (! rdata_raw.data) {
			r = GETDNS_RETURN_MEMORY_ERROR;
			break;
		}
		/* Copy rdata fields to rdata space */
		data_ptr = rdata_raw.data;
		for (i = 0; i < ldns_rr_rd_count(rr); i++) {
			(void) memcpy(data_ptr,
			    ldns_rdf_data(ldns_rr_rdf(rr, i)),
			    ldns_rdf_size(ldns_rr_rdf(rr, i)));
			data_ptr += ldns_rdf_size(ldns_rr_rdf(rr, i));
		}

		/* Set "rdata_raw" attribute" */
		r = getdns_dict_set_bindata(*rdata, "rdata_raw", &rdata_raw);
		GETDNS_FREE(context->mf, rdata_raw.data);
		if (r != GETDNS_RETURN_GOOD)
			break;

		/* Now set the RR type specific attributes */
		r = priv_getdns_equip_dict_with_rdfs(*rdata, rr);
		if (r == GETDNS_RETURN_GOOD)
			return r;
	} while(0);
	getdns_dict_destroy(*rdata);
	return r;
}

getdns_return_t
priv_getdns_create_dict_from_rr(
    struct getdns_context *context, ldns_rr *rr, struct getdns_dict** rr_dict)
{
	getdns_return_t r = GETDNS_RETURN_GOOD;
	struct getdns_bindata name;
	struct getdns_dict *rdata;

	assert(context);
	assert(rr);
	assert(rr_dict);

	*rr_dict = getdns_dict_create_with_context(context);
	if (! *rr_dict)
		return GETDNS_RETURN_MEMORY_ERROR;
	do { /* break on error (to cleanup *rr_dict) */
		r = getdns_dict_set_int(*rr_dict,
		    "type", ldns_rr_get_type(rr));
		if (r != GETDNS_RETURN_GOOD)
			break;
		r = getdns_dict_set_int(*rr_dict,
		    "class", ldns_rr_get_class(rr));
		if (r != GETDNS_RETURN_GOOD)
			break;
		r = getdns_dict_set_int(*rr_dict, "ttl", ldns_rr_ttl(rr));
		if (r != GETDNS_RETURN_GOOD)
			break;

		/* "name" attribute.
		 * ldns_rr_owner(rr) is already uncompressed!
		 */
		name.size = ldns_rdf_size(ldns_rr_owner(rr));
		name.data = ldns_rdf_data(ldns_rr_owner(rr));
		r = getdns_dict_set_bindata(*rr_dict, "name", &name);
		if (r != GETDNS_RETURN_GOOD)
			break;

		/* The "rdata" dict... copies of copies of copies :( */
		r = priv_getdns_create_dict_from_rdfs(context, rr, &rdata);
		if (r != GETDNS_RETURN_GOOD)
			break;
		r = getdns_dict_set_dict(*rr_dict, "rdata", rdata);
		if (r == GETDNS_RETURN_GOOD)
			return r;

		getdns_dict_destroy(rdata);
	} while (0);
	getdns_dict_destroy(*rr_dict);
	return r;
}

getdns_return_t
priv_getdns_create_reply_question_dict(
    struct getdns_context *context, ldns_pkt *pkt, struct getdns_dict** q_dict)
{
	getdns_return_t r = GETDNS_RETURN_GOOD;
	ldns_rr *rr;
	struct getdns_bindata qname;

	assert(context);
	assert(pkt);
	assert(q_dict);

       	rr = ldns_rr_list_rr(ldns_pkt_question(pkt), 0);
	if (! rr)
		return GETDNS_RETURN_GENERIC_ERROR;

	*q_dict = getdns_dict_create_with_context(context);
	if (! *q_dict)
		return GETDNS_RETURN_MEMORY_ERROR;
	do { /* break on error (to cleanup *q_dict) */
		r = getdns_dict_set_int(*q_dict,
		    "qtype", ldns_rr_get_type(rr));
		if (r != GETDNS_RETURN_GOOD)
			break;
		r = getdns_dict_set_int(*q_dict,
		    "qclass", ldns_rr_get_class(rr));
		if (r != GETDNS_RETURN_GOOD)
			break;

		/* "qname" attribute.
		 * ldns_rr_owner(rr) is already uncompressed!
		 */
		qname.size = ldns_rdf_size(ldns_rr_owner(rr));
		qname.data = ldns_rdf_data(ldns_rr_owner(rr));
		r = getdns_dict_set_bindata(*q_dict, "qname", &qname);
		if (r == GETDNS_RETURN_GOOD)
			return r;
	} while (0);
	getdns_dict_destroy(*q_dict);
	return r;
}
