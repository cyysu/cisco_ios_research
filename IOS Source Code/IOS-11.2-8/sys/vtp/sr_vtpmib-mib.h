/* $Id: sr_vtpmib-mib.h,v 1.1.2.3 1996/07/10 20:24:47 cyoung Exp $
 * $Source: /release/112/cvs/Xsys/vtp/sr_vtpmib-mib.h,v $
 *------------------------------------------------------------------
 * SNMP/SNMPv2 bi-lingual agent code.
 *
 * May 1996, Jeffrey T. Johnson (via the SNMP Research MIB Compiler)
 *
 * Copyright (c) 1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: sr_vtpmib-mib.h,v $
 * Revision 1.1.2.3  1996/07/10  20:24:47  cyoung
 * MIB_OID struct must end with 0, 0
 * CSCdi62442:  Loading VTP MIB can cause write term to crash
 * Branch: California_branch
 *
 * Revision 1.1.2.2  1996/07/02  02:51:05  cyoung
 * load the VTP OID table into the hash list so MakeOIDFragFromDot
 * doesn't fail.
 * CSCdi61363:  vtp notifications (traps) are not sent to management
 * station.
 * Branch: California_branch
 *
 * Revision 1.1.2.1  1996/05/27  17:12:36  jjohnson
 * CSCdi58765:  sr_vtpmib-mib.h missing from source code repository
 *
 * Revision 1.1  1996/05/27  16:41:22  jjohnson
 * placeholder
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 *
 * Copyright (C) 1992-1996 by SNMP Research, Incorporated.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by SNMP Research, Incorporated.
 *
 * Restricted Rights Legend:
 *  Use, duplication, or disclosure by the Government is subject to
 *  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
 *  in Technical Data and Computer Software clause at DFARS 52.227-7013
 *  and in similar clauses in the FAR and NASA FAR Supplement.
 *
 */


/*
 * automatically generated by the mib compiler -- do not edit
 */

#ifndef SR_VTPMIBMIB_H
#define SR_VTPMIBMIB_H


#ifdef SR_MIB_TABLE_HASHING
#define HASHNEXT(x) x, NULL
#else /* SR_MIB_TABLE_HASHING */
#define HASHNEXT(x) x
#endif /* SR_MIB_TABLE_HASHING */

/* SNMP-MIB translation table.  Subitem on left gets
 * translated into subitem on right.
 */

static const struct MIB_OID vtpmib_oid_table[] =
{
{    "ciscoVtpMIB",                     HASHNEXT("1.3.6.1.4.1.9.9.46"),},
{    "vtpMIBObjects",                   HASHNEXT("1.3.6.1.4.1.9.9.46.1"),},
{    "vtpStatus",                       HASHNEXT("1.3.6.1.4.1.9.9.46.1.1"),},
{    "vlanManagementDomains",           HASHNEXT("1.3.6.1.4.1.9.9.46.1.2"),},
{    "managementDomainEntry",           HASHNEXT("1.3.6.1.4.1.9.9.46.1.2.1.1"),},
{    "vlanInfo",                        HASHNEXT("1.3.6.1.4.1.9.9.46.1.3"),},
{    "vtpVlanEntry",                    HASHNEXT("1.3.6.1.4.1.9.9.46.1.3.1.1"),},
{    "vlanEdit",                        HASHNEXT("1.3.6.1.4.1.9.9.46.1.4"),},
{    "vtpEditControlEntry",             HASHNEXT("1.3.6.1.4.1.9.9.46.1.4.1.1"),},
{    "vtpVlanEditEntry",                HASHNEXT("1.3.6.1.4.1.9.9.46.1.4.2.1"),},
{    "vtpStats",                        HASHNEXT("1.3.6.1.4.1.9.9.46.1.5"),},
{    "vtpStatsEntry",                   HASHNEXT("1.3.6.1.4.1.9.9.46.1.5.1.1"),},
{    "vlanTrunkPorts",                  HASHNEXT("1.3.6.1.4.1.9.9.46.1.6"),},
{    "vlanTrunkPortEntry",
	 HASHNEXT("1.3.6.1.4.1.9.9.46.1.6.1.1"),},
{    "vtpNotificationsPrefix",
	 HASHNEXT("1.3.6.1.4.1.9.9.46.2.0"),},
{    0, 0},
};



/* DO NOT PUT ANYTHING AFTER THIS #endif */
#endif /* SR_VTPMIBMIB_H */
