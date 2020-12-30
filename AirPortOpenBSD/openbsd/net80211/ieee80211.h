/*    $OpenBSD: ieee80211.h,v 1.62 2019/02/19 08:12:30 stsp Exp $    */
/*    $NetBSD: ieee80211.h,v 1.6 2004/04/30 23:51:53 dyoung Exp $    */

/*-
 * Copyright (c) 2001 Atsushi Onoe
 * Copyright (c) 2002, 2003 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _NET80211_IEEE80211_H_
#define _NET80211_IEEE80211_H_

/*
 * 802.11 protocol definitions.
 */

#define IEEE80211_ADDR_LEN    6    /* size of 802.11 address */
/* is 802.11 address multicast/broadcast? */
#define IEEE80211_IS_MULTICAST(_a)    (*(_a) & 0x01)

/*
 * Generic definitions for IEEE 802.11 frames.
 */
struct ieee80211_frame {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr3[IEEE80211_ADDR_LEN];
    u_int8_t    i_seq[2];
} __packed;

struct ieee80211_qosframe {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr3[IEEE80211_ADDR_LEN];
    u_int8_t    i_seq[2];
    u_int8_t    i_qos[2];
} __packed;

struct ieee80211_htframe {        /* 11n */
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr3[IEEE80211_ADDR_LEN];
    u_int8_t    i_seq[2];
    u_int8_t    i_qos[2];
    u_int8_t    i_ht[4];
} __packed;

struct ieee80211_frame_addr4 {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr3[IEEE80211_ADDR_LEN];
    u_int8_t    i_seq[2];
    u_int8_t    i_addr4[IEEE80211_ADDR_LEN];
} __packed;

struct ieee80211_qosframe_addr4 {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr3[IEEE80211_ADDR_LEN];
    u_int8_t    i_seq[2];
    u_int8_t    i_addr4[IEEE80211_ADDR_LEN];
    u_int8_t    i_qos[2];
} __packed;

struct ieee80211_htframe_addr4 {    /* 11n */
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr3[IEEE80211_ADDR_LEN];
    u_int8_t    i_seq[2];
    u_int8_t    i_addr4[IEEE80211_ADDR_LEN];
    u_int8_t    i_qos[2];
    u_int8_t    i_ht[4];
} __packed;

#define    IEEE80211_FC0_VERSION_MASK        0x03
#define    IEEE80211_FC0_VERSION_SHIFT        0
#define    IEEE80211_FC0_VERSION_0            0x00
#define    IEEE80211_FC0_TYPE_MASK            0x0c
#define    IEEE80211_FC0_TYPE_SHIFT        2
#define    IEEE80211_FC0_TYPE_MGT            0x00
#define    IEEE80211_FC0_TYPE_CTL            0x04
#define    IEEE80211_FC0_TYPE_DATA            0x08

#define    IEEE80211_FC0_SUBTYPE_MASK        0xf0
#define    IEEE80211_FC0_SUBTYPE_SHIFT        4
/* for TYPE_MGT */
#define    IEEE80211_FC0_SUBTYPE_ASSOC_REQ        0x00
#define    IEEE80211_FC0_SUBTYPE_ASSOC_RESP    0x10
#define    IEEE80211_FC0_SUBTYPE_REASSOC_REQ    0x20
#define    IEEE80211_FC0_SUBTYPE_REASSOC_RESP    0x30
#define    IEEE80211_FC0_SUBTYPE_PROBE_REQ        0x40
#define    IEEE80211_FC0_SUBTYPE_PROBE_RESP    0x50
#define    IEEE80211_FC0_SUBTYPE_BEACON        0x80
#define    IEEE80211_FC0_SUBTYPE_ATIM        0x90
#define    IEEE80211_FC0_SUBTYPE_DISASSOC        0xa0
#define    IEEE80211_FC0_SUBTYPE_AUTH        0xb0
#define    IEEE80211_FC0_SUBTYPE_DEAUTH        0xc0
#define IEEE80211_FC0_SUBTYPE_ACTION        0xd0
#define IEEE80211_FC0_SUBTYPE_ACTION_NOACK    0xe0    /* 11n */
/* for TYPE_CTL */
#define IEEE80211_FC0_SUBTYPE_WRAPPER        0x70    /* 11n */
#define IEEE80211_FC0_SUBTYPE_BAR        0x80
#define IEEE80211_FC0_SUBTYPE_BA        0x90
#define    IEEE80211_FC0_SUBTYPE_PS_POLL        0xa0
#define    IEEE80211_FC0_SUBTYPE_RTS        0xb0
#define    IEEE80211_FC0_SUBTYPE_CTS        0xc0
#define    IEEE80211_FC0_SUBTYPE_ACK        0xd0
#define    IEEE80211_FC0_SUBTYPE_CF_END        0xe0
#define    IEEE80211_FC0_SUBTYPE_CF_END_ACK    0xf0
/* for TYPE_DATA (bit combination) */
#define    IEEE80211_FC0_SUBTYPE_DATA        0x00
#define    IEEE80211_FC0_SUBTYPE_DATA_CF_ACK    0x10
#define    IEEE80211_FC0_SUBTYPE_DATA_CF_POLL    0x20
#define    IEEE80211_FC0_SUBTYPE_DATA_CF_ACKPOLL    0x30
#define    IEEE80211_FC0_SUBTYPE_NODATA        0x40
#define    IEEE80211_FC0_SUBTYPE_NODATA_CF_ACK    0x50
#define    IEEE80211_FC0_SUBTYPE_NODATA_CF_POLL    0x60
#define    IEEE80211_FC0_SUBTYPE_NODATA_CF_ACKPOLL    0x70
#define    IEEE80211_FC0_SUBTYPE_QOS        0x80

#define    IEEE80211_FC1_DIR_MASK            0x03
#define    IEEE80211_FC1_DIR_NODS            0x00    /* STA->STA */
#define    IEEE80211_FC1_DIR_TODS            0x01    /* STA->AP  */
#define    IEEE80211_FC1_DIR_FROMDS        0x02    /* AP ->STA */
#define    IEEE80211_FC1_DIR_DSTODS        0x03    /* AP ->AP  */

#define    IEEE80211_FC1_MORE_FRAG            0x04
#define    IEEE80211_FC1_RETRY            0x08
#define    IEEE80211_FC1_PWR_MGT            0x10
#define    IEEE80211_FC1_MORE_DATA            0x20
#define    IEEE80211_FC1_PROTECTED            0x40
#define    IEEE80211_FC1_WEP            0x40    /* pre-RSNA compat */
#define    IEEE80211_FC1_ORDER            0x80
#define IEEE80211_FC1_BITS                    \
    "\20\03MORE_FRAG\04RETRY\05PWR_MGT\06MORE_DATA"        \
    "\07PROTECTED\08ORDER"

/*
 * Sequence Control field (see 802.11-2012 8.2.4.4).
 */
#define    IEEE80211_SEQ_FRAG_MASK            0x000f
#define    IEEE80211_SEQ_FRAG_SHIFT        0
#define    IEEE80211_SEQ_SEQ_MASK            0xfff0
#define    IEEE80211_SEQ_SEQ_SHIFT            4

#define    IEEE80211_NWID_LEN            32
#define IEEE80211_MMIE_LEN            18    /* 11w */

/*
 * QoS Control field (see 802.11-2012 8.2.4.5).
 */
#define IEEE80211_QOS_TXOP            0xff00
#define IEEE80211_QOS_AMSDU            0x0080    /* 11n */
#define IEEE80211_QOS_ACK_POLICY_NORMAL        0x0000
#define IEEE80211_QOS_ACK_POLICY_NOACK        0x0020
#define IEEE80211_QOS_ACK_POLICY_NOEXPLACK    0x0040
#define IEEE80211_QOS_ACK_POLICY_BA        0x0060
#define IEEE80211_QOS_ACK_POLICY_MASK        0x0060
#define IEEE80211_QOS_ACK_POLICY_SHIFT        5
#define IEEE80211_QOS_EOSP            0x0010
#define IEEE80211_QOS_TID            0x000f

/*
 * Control frames.
 */
struct ieee80211_frame_min {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_addr1[IEEE80211_ADDR_LEN];
    u_int8_t    i_addr2[IEEE80211_ADDR_LEN];
    /* FCS */
} __packed;

struct ieee80211_frame_rts {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_ra[IEEE80211_ADDR_LEN];
    u_int8_t    i_ta[IEEE80211_ADDR_LEN];
    /* FCS */
} __packed;

struct ieee80211_frame_cts {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_ra[IEEE80211_ADDR_LEN];
    /* FCS */
} __packed;

struct ieee80211_frame_ack {
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];
    u_int8_t    i_ra[IEEE80211_ADDR_LEN];
    /* FCS */
} __packed;

struct ieee80211_frame_pspoll {
    u_int8_t    i_fc[2];
    u_int8_t    i_aid[2];
    u_int8_t    i_bssid[IEEE80211_ADDR_LEN];
    u_int8_t    i_ta[IEEE80211_ADDR_LEN];
    /* FCS */
} __packed;

struct ieee80211_frame_cfend {        /* NB: also CF-End+CF-Ack */
    u_int8_t    i_fc[2];
    u_int8_t    i_dur[2];    /* should be zero */
    u_int8_t    i_ra[IEEE80211_ADDR_LEN];
    u_int8_t    i_bssid[IEEE80211_ADDR_LEN];
    /* FCS */
} __packed;

#ifdef _KERNEL
static __inline int
ieee80211_has_seq(const struct ieee80211_frame *wh)
{
    return (wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) !=
        IEEE80211_FC0_TYPE_CTL;
}

static __inline int
ieee80211_has_addr4(const struct ieee80211_frame *wh)
{
    return (wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) ==
        IEEE80211_FC1_DIR_DSTODS;
}

static __inline int
ieee80211_has_qos(const struct ieee80211_frame *wh)
{
    return (wh->i_fc[0] &
        (IEEE80211_FC0_TYPE_MASK | IEEE80211_FC0_SUBTYPE_QOS)) ==
        (IEEE80211_FC0_TYPE_DATA | IEEE80211_FC0_SUBTYPE_QOS);
}

static __inline int
ieee80211_has_htc(const struct ieee80211_frame *wh)
{
    return (wh->i_fc[1] & IEEE80211_FC1_ORDER) &&
        (ieee80211_has_qos(wh) ||
         (wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) ==
         IEEE80211_FC0_TYPE_MGT);
}

static __inline u_int16_t
ieee80211_get_qos(const struct ieee80211_frame *wh)
{
    const u_int8_t *frm;

    if (ieee80211_has_addr4(wh))
        frm = ((const struct ieee80211_qosframe_addr4 *)wh)->i_qos;
    else
        frm = ((const struct ieee80211_qosframe *)wh)->i_qos;

    return letoh16(*(const u_int16_t *)frm);
}
#endif    /* _KERNEL */

/*
 * Capability Information field (see 802.11-2012 8.4.1.4).
 */
#define    IEEE80211_CAPINFO_ESS            0x0001
#define    IEEE80211_CAPINFO_IBSS            0x0002
#define    IEEE80211_CAPINFO_CF_POLLABLE        0x0004
#define    IEEE80211_CAPINFO_CF_POLLREQ        0x0008
#define    IEEE80211_CAPINFO_PRIVACY        0x0010
#define    IEEE80211_CAPINFO_SHORT_PREAMBLE    0x0020
#define    IEEE80211_CAPINFO_PBCC            0x0040
#define    IEEE80211_CAPINFO_CHNL_AGILITY        0x0080
#define IEEE80211_CAPINFO_SPECTRUM_MGMT        0x0100
#define IEEE80211_CAPINFO_QOS            0x0200
#define    IEEE80211_CAPINFO_SHORT_SLOTTIME    0x0400
#define    IEEE80211_CAPINFO_APSD            0x0800
#define    IEEE80211_CAPINFO_RADIO_MEASUREMENT    0x1000
#define    IEEE80211_CAPINFO_DSSSOFDM        0x2000
#define IEEE80211_CAPINFO_DELAYED_B_ACK        0x4000
#define IEEE80211_CAPINFO_IMMEDIATE_B_ACK    0x8000
#define IEEE80211_CAPINFO_BITS                    \
    "\10\01ESS\02IBSS\03CF_POLLABLE\04CF_POLLREQ"        \
    "\05PRIVACY\06SHORT_PREAMBLE\07PBCC\10CHNL_AGILITY"    \
    "\11SPECTRUM_MGMT\12QOS\13SHORT_SLOTTIME\14APSD"    \
    "\15RADIO_MEASUREMENT\16DSSSOFDM\17DELAYED_B_ACK\20IMMEDIATE_B_ACK"

/*
 * Information element IDs (see 802.11-2012 Table 8.4.2).
 */
enum {
    IEEE80211_ELEMID_SSID            = 0,
    IEEE80211_ELEMID_RATES            = 1,
    IEEE80211_ELEMID_FHPARMS        = 2,
    IEEE80211_ELEMID_DSPARMS        = 3,
    IEEE80211_ELEMID_CFPARMS        = 4,
    IEEE80211_ELEMID_TIM            = 5,
    IEEE80211_ELEMID_IBSSPARMS        = 6,
    IEEE80211_ELEMID_COUNTRY        = 7,
    IEEE80211_ELEMID_HOPPING_PARMS        = 8,
    IEEE80211_ELEMID_HOPPING_PATTERN    = 9,
    IEEE80211_ELEMID_REQUEST        = 10,
    IEEE80211_ELEMID_QBSS_LOAD        = 11,
    IEEE80211_ELEMID_EDCAPARMS        = 12,
    IEEE80211_ELEMID_TSPEC            = 13,
    IEEE80211_ELEMID_TCLASS            = 14,
    IEEE80211_ELEMID_SCHEDULE        = 15,
    IEEE80211_ELEMID_CHALLENGE        = 16,
    /* 17-31 reserved for challenge text extension */
    IEEE80211_ELEMID_POWER_CONSTRAINT    = 32,
    IEEE80211_ELEMID_POWER_CAP        = 33,
    IEEE80211_ELEMID_TPC_REQUEST        = 34,
    IEEE80211_ELEMID_TPC_REPORT        = 35,
    IEEE80211_ELEMID_SUPP_CHNLS        = 36,
    IEEE80211_ELEMID_CSA            = 37,    /* 11h */
    IEEE80211_ELEMID_MEASUREMENT_REQUEST    = 38,    /* DFS */
    IEEE80211_ELEMID_MEASUREMENT_REPORT    = 39,    /* DFS */
    IEEE80211_ELEMID_QUIET            = 40,
    IEEE80211_ELEMID_IBSS_DFS        = 41,
    IEEE80211_ELEMID_ERP            = 42,
    IEEE80211_ELEMID_TS_DELAY        = 43,
    IEEE80211_ELEMID_TCLAS            = 44,
    IEEE80211_ELEMID_HTCAPS            = 45,    /* 11n */
    IEEE80211_ELEMID_QOS_CAP        = 46,
    /* 47 reserved */
    IEEE80211_ELEMID_RSN            = 48,
    /* 49 reserved */
    IEEE80211_ELEMID_XRATES            = 50,
    IEEE80211_ELEMID_AP_CHNL_REPORT        = 51,
    IEEE80211_ELEMID_NBR_REPORT        = 52,
    IEEE80211_ELEMID_RCPI            = 53,
    IEEE80211_ELEMID_MDE            = 54,
    IEEE80211_ELEMID_FTE            = 55,
    IEEE80211_ELEMID_TIE            = 56,    /* 11r */
    IEEE80211_ELEMID_RDE            = 57,
    IEEE80211_ELEMID_DSE            = 58,
    IEEE80211_ELEMID_SUPP_OPCLASS        = 59,
    IEEE80211_ELEMID_XCSA            = 60,
    IEEE80211_ELEMID_HTOP            = 61,    /* 11n */
    IEEE80211_ELEMID_SECONDARY_CHANL_OFFSET    = 62,    /* 11n */
    IEEE80211_ELEMID_AVG_ACCESS_DELAY    = 63,
    IEEE80211_ELEMID_ANTENNA        = 64,
    IEEE80211_ELEMID_RSNI            = 65,
    IEEE80211_ELEMID_MEASUREMENT_PILOT_TX    = 66,
    IEEE80211_ELEMID_AVAIL_CAPACITY        = 67,
    IEEE80211_ELEMID_AC_ACCESS_DELAY    = 68,
    IEEE80211_ELEMID_TIME_ADVERT        = 69,
    IEEE80211_ELEMID_RM            = 70,
    IEEE80211_ELEMID_MULTI_BSSID        = 71,
    IEEE80211_ELEMID_20_40_CBW_COEX        = 72,    /* 11n */
    IEEE80211_ELEMID_20_40_CBW_INTOLERANT    = 73,    /* 11n */
    IEEE80211_ELEMID_SCAN_PARAM_OVERLAP    = 74,
    IEEE80211_ELEMID_RIC            = 75,
    IEEE80211_ELEMID_MMIE            = 76,    /* 11w */
    IEEE80211_ELEMID_EVENT_REQUEST        = 78,
    IEEE80211_ELEMID_EVENT_REPORT        = 79,
    IEEE80211_ELEMID_DIAG_REQUEST        = 80,
    IEEE80211_ELEMID_DIAG_REPORT        = 81,
    IEEE80211_ELEMID_LOCATION_PARMS        = 82,
    IEEE80211_ELEMID_NONTX_BSSID        = 83,
    IEEE80211_ELEMID_SSID_LIST        = 84,
    IEEE80211_ELEMID_MULTI_BSSID_IDX    = 85,
    IEEE80211_ELEMID_FMS_DESC        = 86,
    IEEE80211_ELEMID_FMS_REQUEST        = 87,
    IEEE80211_ELEMID_FMS_RESPONSE        = 88,
    IEEE80211_ELEMID_QOS_TRAFFIC_CAP    = 89,
    IEEE80211_ELEMID_MAX_IDLE_PERIOD    = 90,
    IEEE80211_ELEMID_TFS_REQUEST        = 91,
    IEEE80211_ELEMID_TFS_RESPONSE        = 92,
    IEEE80211_ELEMID_WNM_SLEEP        = 93,
    IEEE80211_ELEMID_TIM_BCAST_REQUEST    = 94,
    IEEE80211_ELEMID_TIM_BCAST_RESPONSE    = 95,
    IEEE80211_ELEMID_INTERFERENCE_REPORT    = 96,
    IEEE80211_ELEMID_CHNL_USAGE        = 97,
    IEEE80211_ELEMID_TIME_ZONE        = 98,
    IEEE80211_ELEMID_DMS_REQUEST        = 99,
    IEEE80211_ELEMID_DMS_RESPONSE        = 100,
    IEEE80211_ELEMID_LINK_ID        = 101,
    IEEE80211_ELEMID_WAKE_SCHED        = 102,
    /* 103 undefined */
    IEEE80211_ELEMID_CHNL_SWITCH_TIMING    = 104,
    IEEE80211_ELEMID_PTI_CTRL        = 105,
    IEEE80211_ELEMID_TPU_BUF_STATUS        = 106,
    IEEE80211_ELEMID_INTERWORKING        = 107,
    IEEE80211_ELEMID_ADVERT_PROTOCOL    = 108,
    IEEE80211_ELEMID_EXPEDITED_BW_REQUEST    = 109,
    IEEE80211_ELEMID_QOS_MAP_SET        = 110,
    IEEE80211_ELEMID_ROAMING_CONSORTIUM    = 111,
    IEEE80211_ELEMID_EMERGENCY_ALERT_ID    = 112,
    IEEE80211_ELEMID_MESHCONF        = 113,
    IEEE80211_ELEMID_MESHID            = 114,
    IEEE80211_ELEMID_MESHLINK        = 115,
    IEEE80211_ELEMID_MESHCNGST        = 116,
    IEEE80211_ELEMID_MESHPEER        = 117,
    IEEE80211_ELEMID_MESHCSA        = 118,
    IEEE80211_ELEMID_MESHAWAKEW        = 119,
    IEEE80211_ELEMID_MESHBEACONT        = 120,
    IEEE80211_ELEMID_MCCAOP_SETUP_REQUEST    = 121,
    IEEE80211_ELEMID_MCCAOP_SETUP_REPLY    = 122,
    IEEE80211_ELEMID_MCCAOP_ADVERT        = 123,
    IEEE80211_ELEMID_MCCAOP_TEARDOWN    = 124,
    IEEE80211_ELEMID_MESHGANN        = 125,
    IEEE80211_ELEMID_MESHRANN        = 126,
    IEEE80211_ELEMID_XCAPS            = 127,
    /* 128-129 reserved */
    IEEE80211_ELEMID_MESHPREQ        = 130,
    IEEE80211_ELEMID_MESHPREP        = 131,
    IEEE80211_ELEMID_MESHPERR        = 132,
    /* 133-136 reserved */
    IEEE80211_ELEMID_MESHPXU        = 137,
    IEEE80211_ELEMID_MESHPXUC        = 138,
    IEEE80211_ELEMID_AUTH_MESH_PEERING_XCHG    = 139,
    IEEE80211_ELEMID_MIC            = 140,
    IEEE80211_ELEMID_DEST_URI        = 141,
    IEEE80211_ELEMID_U_APSD_COEX        = 142,
    /* 143-174 reserved */
    IEEE80211_ELEMID_MCCAOP_ADVERT_OVIEW    = 174,
    /* 175-220 reserved */
    IEEE80211_ELEMID_VENDOR            = 221    /* vendor private */
    /* 222-255 reserved */
};

/*
 * Action field category values (see 802.11-2012 8.4.1.11 Table 8-38).
 */
enum {
    IEEE80211_CATEG_SPECTRUM        = 0,
    IEEE80211_CATEG_QOS            = 1,
    IEEE80211_CATEG_DLS            = 2,
    IEEE80211_CATEG_BA            = 3,
    IEEE80211_CATEG_PUB            = 4,
    IEEE80211_CATEG_RADIO_MSRMNT        = 5,
    IEEE80211_CATEG_FAST_BSS_TRANS        = 6,
    IEEE80211_CATEG_HT            = 7,    /* 11n */
    IEEE80211_CATEG_SA_QUERY        = 8,    /* 11w */
    IEEE80211_CATEG_PROT_DUAL_PUBLIC_ACTION    = 9,
    IEEE80211_CATEG_WNM            = 10,
    IEEE80211_CATEG_UNPROT_WNM        = 11,
    IEEE80211_CATEG_TDLS            = 12,
    IEEE80211_CATEG_MESH            = 13,
    IEEE80211_CATEG_MULTIHOP        = 14,
    IEEE80211_CATEG_SELF_PROT        = 15,
    /* 16-125 reserved */
    IEEE80211_CATEG_PROT_VENDOR        = 126,
    IEEE80211_CATEG_VENDOR            = 127
    /* 128-255 error */
};

/*
 * Block Ack Action field values (see 802.11-2012 8.5.5 Table 8-202).
 */
#define IEEE80211_ACTION_ADDBA_REQ    0
#define IEEE80211_ACTION_ADDBA_RESP    1
#define IEEE80211_ACTION_DELBA        2
/* 3-255 reserved */

/*
 * SA Query Action field values (see 802.11-2012 8.5.10 Table 8-227).
 */
#define IEEE80211_ACTION_SA_QUERY_REQ    0
#define IEEE80211_ACTION_SA_QUERY_RESP    1

/*
 * HT Action field values (see 802.11-2012 8.5.12 Table 8-229).
 */
#define IEEE80211_ACTION_NOTIFYCW        0
#define IEEE80211_ACTION_SM_PWRSAVE        1
#define IEEE80211_ACTION_PSMP            2
#define IEEE80211_ACTION_SET_PCO_PHASE        3
#define IEEE80211_ACTION_CSI            4
#define IEEE80211_ACTION_NONCOMPRESSED_BF    5
#define IEEE80211_ACTION_COMPRESSED_BF        6
#define IEEE80211_ACTION_ASEL_IDX_FEEDBACK    7
/* 8-255 reserved */

#define    IEEE80211_RATE_BASIC            0x80
#define    IEEE80211_RATE_VAL            0x7f
#define    IEEE80211_RATE_SIZE            8    /* 802.11 standard */
#define    IEEE80211_RATE_MAXSIZE            15    /* max rates we'll handle */

#define    IEEE80211_HT_NUM_MCS            77
#define    IEEE80211_VHT_NUM_MCS            10

/*
 * BlockAck/BlockAckReq Control field (see 802.11-2012 8.3.1.9 Figure 8-25).
 */
#define IEEE80211_BA_ACK_POLICY        0x0001
#define IEEE80211_BA_MULTI_TID        0x0002
#define IEEE80211_BA_COMPRESSED        0x0004
#define IEEE80211_BA_TID_INFO_MASK    0xf000
#define IEEE80211_BA_TID_INFO_SHIFT    12

/*
 * ADDBA Parameter Set field (see 802.11-2012 8.4.1.14 Figure 8-48).
 */
#define IEEE80211_ADDBA_AMSDU        0x0001 /* A-MSDU in A-MPDU supported */
#define IEEE80211_ADDBA_BA_POLICY    0x0002 /* 1=immediate BA 0=delayed BA */
#define IEEE80211_ADDBA_TID_MASK    0x003c
#define IEEE80211_ADDBA_TID_SHIFT    2
#define IEEE80211_ADDBA_BUFSZ_MASK    0xffc0
#define IEEE80211_ADDBA_BUFSZ_SHIFT    6

/*
 * DELBA Parameter Set field (see 802.11-2012 8.4.1.16 Figure 8-50).
 */
#define IEEE80211_DELBA_INITIATOR    0x0800
#define IEEE80211_DELBA_TID_INFO_MASK    0xf000
#define IEEE80211_DELBA_TID_INFO_SHIFT    12

/*
 * ERP information element parameters (see 802.11-2012 8.4.2.14 Figure 8-95).
 */
#define    IEEE80211_ERP_NON_ERP_PRESENT        0x01
#define    IEEE80211_ERP_USE_PROTECTION        0x02
#define    IEEE80211_ERP_BARKER_MODE        0x04

/*
 * RSN capabilities (see 802.11-2012 8.4.2.27.4).
 */
#define IEEE80211_RSNCAP_PREAUTH        0x0001
#define IEEE80211_RSNCAP_NOPAIRWISE        0x0002
#define IEEE80211_RSNCAP_PTKSA_RCNT_MASK    0x000c
#define IEEE80211_RSNCAP_PTKSA_RCNT_SHIFT    2
#define IEEE80211_RSNCAP_GTKSA_RCNT_MASK    0x0030
#define IEEE80211_RSNCAP_GTKSA_RCNT_SHIFT    4
#define IEEE80211_RSNCAP_RCNT1            0
#define IEEE80211_RSNCAP_RCNT2            1
#define IEEE80211_RSNCAP_RCNT4            2
#define IEEE80211_RSNCAP_RCNT16            3
#define IEEE80211_RSNCAP_MFPR            0x0040    /* 11w */
#define IEEE80211_RSNCAP_MFPC            0x0080    /* 11w */
#define IEEE80211_RSNCAP_PEERKEYENA        0x0200
#define IEEE80211_RSNCAP_SPPAMSDUC        0x0400    /* 11n */
#define IEEE80211_RSNCAP_SPPAMSDUR        0x0800    /* 11n */
#define IEEE80211_RSNCAP_PBAC            0x1000    /* 11n */
#define IEEE80211_RSNCAP_EXTENDED_KEYID        0x2000

/*
 * HT Capabilities Info (see 802.11-2012 8.4.2.58.2).
 */
#define IEEE80211_HTCAP_LDPC        0x00000001
#define IEEE80211_HTCAP_CBW20_40    0x00000002
#define IEEE80211_HTCAP_SMPS_MASK    0x0000000c
#define IEEE80211_HTCAP_SMPS_SHIFT    2
#define IEEE80211_HTCAP_SMPS_STA    0
#define IEEE80211_HTCAP_SMPS_DYN    1
#define IEEE80211_HTCAP_SMPS_DIS    3
#define IEEE80211_HTCAP_GF        0x00000010
#define IEEE80211_HTCAP_SGI20        0x00000020
#define IEEE80211_HTCAP_SGI40        0x00000040
#define IEEE80211_HTCAP_TXSTBC        0x00000080
#define IEEE80211_HTCAP_RXSTBC_MASK    0x00000300
#define IEEE80211_HTCAP_RXSTBC_SHIFT    8
#define IEEE80211_HTCAP_DELAYEDBA    0x00000400
#define IEEE80211_HTCAP_AMSDU7935    0x00000800
#define IEEE80211_HTCAP_DSSSCCK40    0x00001000
#define IEEE80211_HTCAP_PSMP        0x00002000
#define IEEE80211_HTCAP_40INTOLERANT    0x00004000
#define IEEE80211_HTCAP_LSIGTXOPPROT    0x00008000

/*
 * HT A-MPDU parameters (see 802.11-2012 8.4.2.58.3).
 */
#define IEEE80211_AMPDU_PARAM_LE    0x03
#define IEEE80211_AMPDU_PARAM_SS    0x1c
#define IEEE80211_AMPDU_PARAM_SS_NONE    (0 << 2)
#define IEEE80211_AMPDU_PARAM_SS_0_25    (1 << 2)
#define IEEE80211_AMPDU_PARAM_SS_0_5    (2 << 2)
#define IEEE80211_AMPDU_PARAM_SS_1    (3 << 2)
#define IEEE80211_AMPDU_PARAM_SS_2    (4 << 2)
#define IEEE80211_AMPDU_PARAM_SS_4    (5 << 2)
#define IEEE80211_AMPDU_PARAM_SS_8    (6 << 2)
#define IEEE80211_AMPDU_PARAM_SS_16    (7 << 2)
/* bits 5-7 reserved */

/*
 * HT Supported MCS Set (see 802.11-2012 8.4.2.58.4).
 * This field is 16 bytes in size. Bitmasks given below
 * operate on 8 or 16 bit integer subsets of this field
 * for use with ieee80211com and ieee80211_node.
 */
/* Bits 0-76: Supported Rx MCS bitmask */
/* Bits 77-79: Reserved */
/* Bits 80-89: Highest Rx rate in units of 1MB/s */
#define IEEE80211_MCS_RX_RATE_HIGH    0x03ff
/* Bits 90-95: Reserved */
/* Bits 96-100: Tx MCS set */
#define IEEE80211_TX_MCS_SET_DEFINED        0x01
#define IEEE80211_TX_RX_MCS_NOT_EQUAL        0x02
#define IEEE80211_TX_SPATIAL_STREAMS        0x0c
#define IEEE80211_TX_UNEQUAL_MODULATION        0x10
/* Bits 101-127: Reserved */

/*
 * HT Extended Capabilities (see 802.11-2012 8.4.2.58.5).
 */
#define IEEE80211_HTXCAP_PCO        0x0001
#define IEEE80211_HTXCAP_PCOTT_MASK    0x0006
#define IEEE80211_HTXCAP_PCOTT_SHIFT    1
#define IEEE80211_HTXCAP_PCOTT_400    1
#define IEEE80211_HTXCAP_PCOTT_1500    2
#define IEEE80211_HTXCAP_PCOTT_5000    3
/* Bits 3-7 are reserved. */
#define IEEE80211_HTXCAP_MFB_MASK    0x0300
#define IEEE80211_HTXCAP_MFB_SHIFT    8
#define IEEE80211_HTXCAP_MFB_NONE    0
#define IEEE80211_HTXCAP_MFB_UNSOL    2
#define IEEE80211_HTXCAP_MFB_BOTH    3
#define IEEE80211_HTXCAP_HTC        0x0400
#define IEEE80211_HTXCAP_RDRESP        0x0800
/* Bits 12-15 are reserved. */

/*
 * Transmit Beamforming (TxBF) Capabilities (see 802.11-2012 8.4.2.58.6).
 */
#define IEEE80211_TXBFCAP_IMPLICIT_RX            0x00000001
#define IEEE80211_TXBFCAP_RSSC                0x00000002
#define IEEE80211_TXBFCAP_TSSC                0x00000004
#define IEEE80211_TXBFCAP_RNDP                0x00000008
#define IEEE80211_TXBFCAP_TNDP                0x00000010
#define IEEE80211_TXBFCAP_IMPLICIT_TX            0x00000020
#define IEEE80211_TXBFCAP_CALIB_MASK            0x000000c0
#define IEEE80211_TXBFCAP_CALIB_SHIFT            6
#define IEEE80211_TXBFCAP_TX_CSI            0x00000100
#define IEEE80211_TXBFCAP_EXPLICIT_NSC            0x00000200
#define IEEE80211_TXBFCAP_EXPLICIT_CSC            0x00000400
#define IEEE80211_TXBFCAP_CSI_FB_DELAYED        0x00000800
#define IEEE80211_TXBFCAP_CSI_FB_IMMEDIATE        0x00001000
#define IEEE80211_TXBFCAP_EXPLICIT_NB_FB_DELAYED    0x00002000
#define IEEE80211_TXBFCAP_EXPLICIT_NB_FB_IMMEDIATE    0x00004000
#define IEEE80211_TXBFCAP_EXPLICIT_CB_FB_DELAYED    0x00008000
#define IEEE80211_TXBFCAP_EXPLICIT_CB_FB_IMMEDIATE    0x00010000
#define IEEE80211_TXBFCAP_MINIMAL_GROUPING_1_2        0x00020000
#define IEEE80211_TXBFCAP_MINIMAL_GROUPING_1_4        0x00040000
#define IEEE80211_TXBFCAP_CSI_NUM_ANT_MASK        0x00180000
#define IEEE80211_TXBFCAP_CSI_NUM_ANT_SHIFT        19
#define IEEE80211_TXBFCAP_NS_NUM_ANT_MASK        0x00600000
#define IEEE80211_TXBFCAP_NS_NUM_ANT_SHIFT        21
#define IEEE80211_TXBFCAP_CS_NUM_ANT_MASK        0x01800000
#define IEEE80211_TXBFCAP_CS_NUM_ANT_SHIFT        23
#define IEEE80211_TXBFCAP_CSI_NUM_ROWS_MASK        0x06000000
#define IEEE80211_TXBFCAP_CSI_NUM_ROWS_SHIFT        25
#define IEEE80211_TXBFCAP_CHANL_ESTIMATE_MASK        0x18000000
#define IEEE80211_TXBFCAP_CHANL_ESTIMATE_SHIFT        27

/*
 * Antenna Selection (ASEL) Capability (see 802.11-2012 8.4.2.58.7).
 */
#define IEEE80211_ASELCAP_ASEL        0x01
#define IEEE80211_ASELCAP_CSIFB_TX    0x02
#define IEEE80211_ASELCAP_ANT_IDX_FB_TX    0x04
#define IEEE80211_ASELCAP_CSIFB        0x08
#define IEEE80211_ASELCAP_ANT_IDX_FB    0x10
#define IEEE80211_ASELCAP_ASEL_RX    0x20
#define IEEE80211_ASELCAP_TX_SOUND_PPDU    0x20
/* Bit 7 is reserved. */

/*
 * HT Operation element (see 802.11-2012 8.4.2.59).
 */
/* Byte 0 contains primary channel number. */
/* Byte 1. */
#define IEEE80211_HTOP0_SCO_MASK    0x03
#define IEEE80211_HTOP0_SCO_SHIFT    0
#define IEEE80211_HTOP0_SCO_SCN        0
#define IEEE80211_HTOP0_SCO_SCA        1
#define IEEE80211_HTOP0_SCO_SCB        3
#define IEEE80211_HTOP0_CHW        0x04
#define IEEE80211_HTOP0_RIFS        0x08
/* bits 4-7 reserved */
/* Bytes 2-3. */
#define IEEE80211_HTOP1_PROT_MASK    0x0003
#define IEEE80211_HTOP1_PROT_SHIFT    0
#define IEEE80211_HTOP1_NONGF_STA    0x0004
/* Bit 3 is reserved. */
#define IEEE80211_HTOP1_OBSS_NONHT_STA    0x0010
/* Bits 5-15 are reserved. */
/* Bytes 4-5. */
/* Bits 0-5 are reserved. */
#define IEEE80211_HTOP2_DUALBEACON    0x0040
#define IEEE80211_HTOP2_DUALCTSPROT    0x0080
#define IEEE80211_HTOP2_STBCBEACON    0x0100
#define IEEE80211_HTOP2_LSIGTXOP    0x0200
#define IEEE80211_HTOP2_PCOACTIVE    0x0400
#define IEEE80211_HTOP2_PCOPHASE40    0x0800
/* Bits 12-15 are reserved. */

/*
 * EDCA Access Categories.
 */
enum ieee80211_edca_ac {
    EDCA_AC_BK  = 1,    /* Background */
    EDCA_AC_BE  = 0,    /* Best Effort */
    EDCA_AC_VI  = 2,    /* Video */
    EDCA_AC_VO  = 3        /* Voice */
};
#define EDCA_NUM_AC    4

/* number of TID values (traffic identifier) */
#define IEEE80211_NUM_TID    16

/* Atheros private advanced capabilities info */
#define    ATHEROS_CAP_TURBO_PRIME            0x01
#define    ATHEROS_CAP_COMPRESSION            0x02
#define    ATHEROS_CAP_FAST_FRAME            0x04
/* bits 3-6 reserved */
#define    ATHEROS_CAP_BOOST            0x80

/*-
 * Organizationally Unique Identifiers.
 * See http://standards.ieee.org/regauth/oui/oui.txt for a list.
 */
#define ATHEROS_OUI    ((const u_int8_t[]){ 0x00, 0x03, 0x7f })
#define BROADCOM_OUI    ((const u_int8_t[]){ 0x00, 0x90, 0x4c })
#define IEEE80211_OUI    ((const u_int8_t[]){ 0x00, 0x0f, 0xac })
#define MICROSOFT_OUI    ((const u_int8_t[]){ 0x00, 0x50, 0xf2 })

#define    IEEE80211_AUTH_ALGORITHM(auth) \
    ((auth)[0] | ((auth)[1] << 8))
#define    IEEE80211_AUTH_TRANSACTION(auth) \
    ((auth)[2] | ((auth)[3] << 8))
#define    IEEE80211_AUTH_STATUS(auth) \
    ((auth)[4] | ((auth)[5] << 8))

/*
 * Authentication Algorithm Number field (see 7.3.1.1).
 */
#define IEEE80211_AUTH_ALG_OPEN            0x0000
#define IEEE80211_AUTH_ALG_SHARED        0x0001
#define IEEE80211_AUTH_ALG_LEAP            0x0080

/*
 * Authentication Transaction Sequence Number field (see 7.3.1.2).
 */
enum {
    IEEE80211_AUTH_OPEN_REQUEST        = 1,
    IEEE80211_AUTH_OPEN_RESPONSE        = 2
};
enum {
    IEEE80211_AUTH_SHARED_REQUEST        = 1,
    IEEE80211_AUTH_SHARED_CHALLENGE        = 2,
    IEEE80211_AUTH_SHARED_RESPONSE        = 3,
    IEEE80211_AUTH_SHARED_PASS        = 4
};

/*
 * Reason codes (see Table 22).
 */
enum {
    IEEE80211_REASON_UNSPECIFIED        = 1,
    IEEE80211_REASON_AUTH_EXPIRE        = 2,
    IEEE80211_REASON_AUTH_LEAVE        = 3,
    IEEE80211_REASON_ASSOC_EXPIRE        = 4,
    IEEE80211_REASON_ASSOC_TOOMANY        = 5,
    IEEE80211_REASON_NOT_AUTHED        = 6,
    IEEE80211_REASON_NOT_ASSOCED        = 7,
    IEEE80211_REASON_ASSOC_LEAVE        = 8,
    IEEE80211_REASON_ASSOC_NOT_AUTHED    = 9,

    /* XXX the following two reason codes are not correct */
    IEEE80211_REASON_RSN_REQUIRED        = 11,
    IEEE80211_REASON_RSN_INCONSISTENT    = 12,

    IEEE80211_REASON_IE_INVALID        = 13,
    IEEE80211_REASON_MIC_FAILURE        = 14,
    IEEE80211_REASON_4WAY_TIMEOUT        = 15,
    IEEE80211_REASON_GROUP_TIMEOUT        = 16,
    IEEE80211_REASON_RSN_DIFFERENT_IE    = 17,
    IEEE80211_REASON_BAD_GROUP_CIPHER    = 18,
    IEEE80211_REASON_BAD_PAIRWISE_CIPHER    = 19,
    IEEE80211_REASON_BAD_AKMP        = 20,
    IEEE80211_REASON_RSN_IE_VER_UNSUP    = 21,
    IEEE80211_REASON_RSN_IE_BAD_CAP        = 22,

    IEEE80211_REASON_CIPHER_REJ_POLICY    = 24,

    IEEE80211_REASON_SETUP_REQUIRED        = 38,
    IEEE80211_REASON_TIMEOUT        = 39
};

/*
 * Status codes (see Table 23).
 */
enum {
    IEEE80211_STATUS_SUCCESS        = 0,
    IEEE80211_STATUS_UNSPECIFIED        = 1,
    IEEE80211_STATUS_CAPINFO        = 10,
    IEEE80211_STATUS_NOT_ASSOCED        = 11,
    IEEE80211_STATUS_OTHER            = 12,
    IEEE80211_STATUS_ALG            = 13,
    IEEE80211_STATUS_SEQUENCE        = 14,
    IEEE80211_STATUS_CHALLENGE        = 15,
    IEEE80211_STATUS_TIMEOUT        = 16,
    IEEE80211_STATUS_TOOMANY        = 17,
    IEEE80211_STATUS_BASIC_RATE        = 18,
    IEEE80211_STATUS_SP_REQUIRED        = 19,
    IEEE80211_STATUS_PBCC_REQUIRED        = 20,
    IEEE80211_STATUS_CA_REQUIRED        = 21,
    IEEE80211_STATUS_TOO_MANY_STATIONS    = 22,
    IEEE80211_STATUS_RATES            = 23,
    IEEE80211_STATUS_SHORTSLOT_REQUIRED    = 25,
    IEEE80211_STATUS_DSSSOFDM_REQUIRED    = 26,

    IEEE80211_STATUS_TRY_AGAIN_LATER    = 30,
    IEEE80211_STATUS_MFP_POLICY        = 31,

    IEEE80211_STATUS_REFUSED        = 37,
    IEEE80211_STATUS_INVALID_PARAM        = 38,

    IEEE80211_STATUS_IE_INVALID        = 40,
    IEEE80211_STATUS_BAD_GROUP_CIPHER    = 41,
    IEEE80211_STATUS_BAD_PAIRWISE_CIPHER    = 42,
    IEEE80211_STATUS_BAD_AKMP        = 43,
    IEEE80211_STATUS_RSN_IE_VER_UNSUP    = 44,

    IEEE80211_STATUS_CIPHER_REJ_POLICY    = 46
};

#define    IEEE80211_WEP_KEYLEN            5    /* 40bit */
#define    IEEE80211_WEP_NKID            4    /* number of key ids */
#define IEEE80211_CHALLENGE_LEN            128

/* WEP header constants */
#define    IEEE80211_WEP_IVLEN            3    /* 24bit */
#define    IEEE80211_WEP_KIDLEN            1    /* 1 octet */
#define    IEEE80211_WEP_CRCLEN            4    /* CRC-32 */
#define    IEEE80211_CRC_LEN            4
#define    IEEE80211_WEP_TOTLEN        (IEEE80211_WEP_IVLEN + \
                     IEEE80211_WEP_KIDLEN + \
                     IEEE80211_WEP_CRCLEN)

/*
 * 802.11i defines an extended IV for use with non-WEP ciphers.
 * When the EXTIV bit is set in the key id byte an additional
 * 4 bytes immediately follow the IV for TKIP.  For CCMP the
 * EXTIV bit is likewise set but the 8 bytes represent the
 * CCMP header rather than IV+extended-IV.
 */
#define    IEEE80211_WEP_EXTIV        0x20
#define    IEEE80211_WEP_EXTIVLEN        4    /* extended IV length */
#define    IEEE80211_WEP_MICLEN        8    /* trailing MIC */

/*
 * Maximum acceptable MTU is:
 *    IEEE80211_MAX_LEN - WEP overhead - CRC -
 *        QoS overhead - RSN/WPA overhead
 * Min is arbitrarily chosen > IEEE80211_MIN_LEN.  The default
 * mtu is Ethernet-compatible; it's set by ether_ifattach.
 */
#define    IEEE80211_MTU_MAX            2290
#define    IEEE80211_MTU_MIN            32

#define    IEEE80211_MAX_LEN            (2300 + IEEE80211_CRC_LEN + \
    (IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN + IEEE80211_WEP_CRCLEN))
#define    IEEE80211_ACK_LEN \
    (sizeof(struct ieee80211_frame_ack) + IEEE80211_CRC_LEN)
#define    IEEE80211_MIN_LEN \
    (sizeof(struct ieee80211_frame_min) + IEEE80211_CRC_LEN)

/*
 * The 802.11 spec says at most 2007 stations may be
 * associated at once.  For most AP's this is way more
 * than is feasible so we use a default of 1800. This
 * number may be overridden by the driver and/or by
 * user configuration.
 */
#define    IEEE80211_AID_MAX    2007
#define    IEEE80211_AID_DEF    1800
#define IEEE80211_AID(b)    ((b) &~ 0xc000)

/*
 * RTS frame length parameters.  The default is specified in
 * the 802.11 spec.  The max may be wrong for jumbo frames.
 */
#define    IEEE80211_RTS_DEFAULT            512
#define    IEEE80211_RTS_MIN            1
#define    IEEE80211_RTS_MAX            IEEE80211_MAX_LEN

#define IEEE80211_PLCP_SERVICE        0x00
#define IEEE80211_PLCP_SERVICE_PBCC    0x08    /* PBCC encoded */
#define IEEE80211_PLCP_SERVICE_LENEXT    0x80    /* length extension bit */

/* One Time Unit (TU) is 1Kus = 1024 microseconds. */
#define IEEE80211_DUR_TU        1024

/* IEEE 802.11b durations for DSSS PHY in microseconds */
#define IEEE80211_DUR_DS_LONG_PREAMBLE    144
#define IEEE80211_DUR_DS_SHORT_PREAMBLE    72
#define    IEEE80211_DUR_DS_PREAMBLE_DIFFERENCE    \
    (IEEE80211_DUR_DS_LONG_PREAMBLE - IEEE80211_DUR_DS_SHORT_PREAMBLE)
#define IEEE80211_DUR_DS_FAST_PLCPHDR    24
#define IEEE80211_DUR_DS_SLOW_PLCPHDR    48
#define    IEEE80211_DUR_DS_PLCPHDR_DIFFERENCE    \
    (IEEE80211_DUR_DS_SLOW_PLCPHDR - IEEE80211_DUR_DS_FAST_PLCPHDR)
#define IEEE80211_DUR_DS_SLOW_ACK    112
#define IEEE80211_DUR_DS_FAST_ACK    56
#define IEEE80211_DUR_DS_SLOW_CTS    112
#define IEEE80211_DUR_DS_FAST_CTS    56
#define IEEE80211_DUR_DS_SLOT        20
#define IEEE80211_DUR_DS_SHSLOT        9
#define IEEE80211_DUR_DS_SIFS        10
#define IEEE80211_DUR_DS_PIFS    (IEEE80211_DUR_DS_SIFS + IEEE80211_DUR_DS_SLOT)
#define IEEE80211_DUR_DS_DIFS    (IEEE80211_DUR_DS_SIFS + \
                 2 * IEEE80211_DUR_DS_SLOT)
#define IEEE80211_DUR_DS_EIFS    (IEEE80211_DUR_DS_SIFS + \
                 IEEE80211_DUR_DS_SLOW_ACK + \
                 IEEE80211_DUR_DS_LONG_PREAMBLE + \
                 IEEE80211_DUR_DS_SLOW_PLCPHDR + \
                 IEEE80211_DUR_DIFS)

/*
 * The RSNA key descriptor used by IEEE 802.11 does not use the IEEE 802.1X
 * key descriptor.  Instead, it uses the key descriptor described in 8.5.2.
 */
#define EAPOL_KEY_NONCE_LEN    32
#define EAPOL_KEY_IV_LEN    16
#define EAPOL_KEY_MIC_LEN    16

struct ieee80211_eapol_key {
    u_int8_t    version;
#define EAPOL_VERSION    1

    u_int8_t    type;
/* IEEE Std 802.1X-2004, 7.5.4 (only type EAPOL-Key is used here) */
#define EAP_PACKET    0
#define EAPOL_START    1
#define EAPOL_LOGOFF    2
#define EAPOL_KEY    3
#define EAPOL_ASF_ALERT    4

    u_int8_t    len[2];
    u_int8_t    desc;
/* IEEE Std 802.1X-2004, 7.6.1 */
#define EAPOL_KEY_DESC_RC4          1    /* deprecated */
#define EAPOL_KEY_DESC_IEEE80211      2
#define EAPOL_KEY_DESC_WPA        254    /* non-standard WPA */

    u_int8_t    info[2];
#define EAPOL_KEY_VERSION_MASK    0x7
#define EAPOL_KEY_DESC_V1    1
#define EAPOL_KEY_DESC_V2    2
#define EAPOL_KEY_DESC_V3    3        /* 11r */
#define EAPOL_KEY_PAIRWISE    (1 <<  3)
#define EAPOL_KEY_INSTALL    (1 <<  6)    /* I */
#define EAPOL_KEY_KEYACK    (1 <<  7)    /* A */
#define EAPOL_KEY_KEYMIC    (1 <<  8)    /* M */
#define EAPOL_KEY_SECURE    (1 <<  9)    /* S */
#define EAPOL_KEY_ERROR        (1 << 10)
#define EAPOL_KEY_REQUEST    (1 << 11)
#define EAPOL_KEY_ENCRYPTED    (1 << 12)
#define EAPOL_KEY_SMK        (1 << 13)
/* WPA compatibility */
#define EAPOL_KEY_WPA_KID_MASK    0x3
#define EAPOL_KEY_WPA_KID_SHIFT    4
#define EAPOL_KEY_WPA_TX    EAPOL_KEY_INSTALL

    u_int8_t    keylen[2];
    u_int8_t    replaycnt[8];
    u_int8_t    nonce[EAPOL_KEY_NONCE_LEN];
    u_int8_t    iv[EAPOL_KEY_IV_LEN];
    u_int8_t    rsc[8];
    u_int8_t    reserved[8];
    u_int8_t    mic[EAPOL_KEY_MIC_LEN];
    u_int8_t    paylen[2];
} __packed;

/* Pairwise Transient Key (see 8.5.1.2) */
struct ieee80211_ptk {
    u_int8_t    kck[16];    /* Key Confirmation Key */
    u_int8_t    kek[16];    /* Key Encryption Key */
    u_int8_t    tk[32];        /* Temporal Key */
} __packed;

#define IEEE80211_PMKID_LEN    16
#define IEEE80211_SMKID_LEN    16

/*
 * Key Data Encapsulation (see Table 62).
 */
enum {
    IEEE80211_KDE_GTK    = 1,
    IEEE80211_KDE_MACADDR    = 3,
    IEEE80211_KDE_PMKID    = 4,
    IEEE80211_KDE_SMK    = 5,
    IEEE80211_KDE_NONCE    = 6,
    IEEE80211_KDE_LIFETIME    = 7,
    IEEE80211_KDE_ERROR    = 8,
    IEEE80211_KDE_IGTK    = 9    /* 11w */
};

/*
 * HT protection modes (see 802.11-2012 8.4.2.59)
 */
enum ieee80211_htprot {
    IEEE80211_HTPROT_NONE = 0,    /* only 20/40MHz HT STAs exist */
    IEEE80211_HTPROT_NONMEMBER,    /* non-HT STA overlaps our channel */
    IEEE80211_HTPROT_20MHZ,        /* 20MHz HT STA on a 40MHz channel */
    IEEE80211_HTPROT_NONHT_MIXED    /* non-HT STA associated to our BSS */
};

#endif /* _NET80211_IEEE80211_H_ */
