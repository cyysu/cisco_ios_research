/* $Id: sr_old_lnovellmib.c,v 3.5.10.3 1996/07/03 20:45:11 thille Exp $
 * $Source: /release/112/cvs/Xsys/xns/sr_old_lnovellmib.c,v $
 *------------------------------------------------------------------
 * SNMP/SNMPv2 bi-lingual agent code.
 *
 * March 1994, Kenny Roberts (via the SNMP Research MIB Compiler)
 *
 * Copyright (c) 1994-1997 by cisco Systems, Inc.
 * All rights reserved.
 *
 *------------------------------------------------------------------
 * $Log: sr_old_lnovellmib.c,v $
 * Revision 3.5.10.3  1996/07/03  20:45:11  thille
 * CSCdi61860: Implement Jeffs glass of slim-fast for snmp
 * Branch: California_branch
 * Take hunk of common code from many mib files, make it a procedure in
 * snmp_util.c and call it from the mibs.  Save 1640 bytes.
 *
 * Revision 3.5.10.2  1996/07/01  18:47:17  hampton
 * Remove gratuitous casting.  [CSCdi61655]
 * Branch: California_branch
 *
 * Revision 3.5.10.1  1996/04/19  17:55:21  jjohnson
 * CSCdi55117:  Need to support the Draft Standard SNMPv2 SMI
 * Branch: California_branch
 * Prototypes for the <foo>_free functions are auto-generated by the
 * SR 14.0 MIB Compiler.  Make the declarations match the prototypes.
 *
 * Revision 3.5  1996/02/21  16:21:39  bstewart
 * CSCdi48325:  SNMP MIBs need modularized translation tables
 *
 * Revision 3.4  1996/01/18  15:58:50  anke
 * CSCdi46969:  Change empty req and/or seq strings in SUBSYS_HEADERs to
 *              NULL
 *              Five bytes saved is five bytes earned
 *
 * Revision 3.3  1995/12/14  08:30:30  jjohnson
 * CSCdi44148:  rationalize snmp library services
 *
 * Revision 3.2  1995/11/17  19:25:08  gstovall
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  14:00:28  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.3  1995/07/01  03:39:35  hampton
 * Minor Novell IPX Cleanups.  Remove some unused variables and routines.
 * Eliminate direct references to the system clock.  [CSCdi36668]
 *
 * Revision 2.2  1995/06/28 09:34:13  smackie
 * Repair widespread subsystem header braindamage. (CSCdi23568)
 *
 *   o Fix subsystem entry points to be foo(subsystype *) not foo(void)
 *   o Repair nonsensical and redundant subsystem sequences
 *   o Use NULL where no property is required
 *
 * Revision 2.1  1995/06/07  23:30:18  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 *
 * Copyright (C) 1994 by SNMP Research, Incorporated.
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

#include "master.h"
#include "subsys.h"
#include "../snmp/snmp_api.h"
#include "sr_old_lnovellmib.h"
#include "sr_old_lnovellmib-mib.h"

#include "../h/packet.h"
#include "novell.h"


struct ipxaddrtype {
    ulong net;
    uchar host[IEEEBYTES];
};

#define MAXBUFFERSIZE 	sizeof(struct ipxaddrtype) + sizeof(struct ipxaddrtype)

static ipx_acctg_entry_type *snmp_get_ipx(int searchType, OctetString *ipxActSrc,
    OctetString *ipxActDst, boolean isacc);
static ipx_acctg_entry_type *snmp_ipx_acct (ipx_acctg_data_type *ipxadt,
     ulong srcnet, uchar *srchost, ulong dstnet,
     uchar *dsthost, boolean srcset, boolean dstset, boolean next);

static void show_oct_ipx(OctetString *ipxSrc, OctetString *ipxDst, int val);

/****************************************************************/


void init_old_lnovellmib (subsystype *subsys)
{
    load_mib(old_lnovellmib_OidList, old_lnovellmib_OidListNum);
    load_oid(old_lnovellmib_oid_table);
}

/*---------------------------------------------------------------------
 * Retrieve data from the tmpnovell family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
tmpnovell_get(
    OID            *incoming,
    ObjectInfo     *object,
    int             searchType,
    ContextInfo    *contextInfo,
    int             serialNum)
{
    int             arg;
    void           *dp;
    tmpnovell_t    *data=NULL;


    arg = snmp_scalar_instance(incoming, object, searchType);

    /*
     * Retrieve the data from the kernel-specific routine.
     */
    if ((arg == -1) || (data = k_tmpnovell_get(serialNum, contextInfo, arg)) == NULL) {
	arg = -1;
    }
    /*
     * Build the the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_novellInput
      case I_novellInput:
	dp = &data->novellInput;
	break;
#endif				       /* I_novellInput */

#ifdef I_novellBcastin
      case I_novellBcastin:
	dp = &data->novellBcastin;
	break;
#endif				       /* I_novellBcastin */

#ifdef I_novellForward
      case I_novellForward:
	dp = &data->novellForward;
	break;
#endif				       /* I_novellForward */

#ifdef I_novellBcastout
      case I_novellBcastout:
	dp = &data->novellBcastout;
	break;
#endif				       /* I_novellBcastout */

#ifdef I_novellFormerr
      case I_novellFormerr:
	dp = &data->novellFormerr;
	break;
#endif				       /* I_novellFormerr */

#ifdef I_novellChksum
      case I_novellChksum:
	dp = &data->novellChksum;
	break;
#endif				       /* I_novellChksum */

#ifdef I_novellHopcnt
      case I_novellHopcnt:
	dp = &data->novellHopcnt;
	break;
#endif				       /* I_novellHopcnt */

#ifdef I_novellNoroute
      case I_novellNoroute:
	dp = &data->novellNoroute;
	break;
#endif				       /* I_novellNoroute */

#ifdef I_novellNoencap
      case I_novellNoencap:
	dp = &data->novellNoencap;
	break;
#endif				       /* I_novellNoencap */

#ifdef I_novellOutput
      case I_novellOutput:
	dp = &data->novellOutput;
	break;
#endif				       /* I_novellOutput */

#ifdef I_novellInmult
      case I_novellInmult:
	dp = &data->novellInmult;
	break;
#endif				       /* I_novellInmult */

#ifdef I_novellLocal
      case I_novellLocal:
	dp = &data->novellLocal;
	break;
#endif				       /* I_novellLocal */

#ifdef I_novellUnknown
      case I_novellUnknown:
	dp = &data->novellUnknown;
	break;
#endif				       /* I_novellUnknown */

#ifdef I_novellSapreqin
      case I_novellSapreqin:
	dp = &data->novellSapreqin;
	break;
#endif				       /* I_novellSapreqin */

#ifdef I_novellSapresin
      case I_novellSapresin:
	dp = &data->novellSapresin;
	break;
#endif				       /* I_novellSapresin */

#ifdef I_novellSapout
      case I_novellSapout:
	dp = &data->novellSapout;
	break;
#endif				       /* I_novellSapout */

#ifdef I_novellSapreply
      case I_novellSapreply:
	dp = &data->novellSapreply;
	break;
#endif				       /* I_novellSapreply */

#ifdef I_ipxActThresh
      case I_ipxActThresh:
	dp = &data->ipxActThresh;
	break;
#endif				       /* I_ipxActThresh */

#ifdef I_ipxActLostPkts
      case I_ipxActLostPkts:
	dp = &data->ipxActLostPkts;
	break;
#endif				       /* I_ipxActLostPkts */

#ifdef I_ipxActLostByts
      case I_ipxActLostByts:
	dp = &data->ipxActLostByts;
	break;
#endif				       /* I_ipxActLostByts */

#ifdef I_ipxActAge
      case I_ipxActAge:
	dp = &data->ipxActAge;
	break;
#endif				       /* I_ipxActAge */

#ifdef I_ipxCkactAge
      case I_ipxCkactAge:
	dp = &data->ipxCkactAge;
	break;
#endif				       /* I_ipxCkactAge */

#ifdef I_ipxActCheckPoint
      case I_ipxActCheckPoint:
	dp = &data->ipxActCheckPoint;
	break;
#endif				       /* I_ipxActCheckPoint */

      default:
	return ((VarBind *) NULL);

    }				       /* switch */

    return (MakeVarBind(object, &ZeroOid, dp));

}

#ifdef SETS

/*----------------------------------------------------------------------
 * Free the tmpnovell data object.
 *---------------------------------------------------------------------*/
void
tmpnovell_free(tmpnovell_t *data)
{
    if (data != NULL) {
	free((char *) data);
    }
}

/*----------------------------------------------------------------------
 * cleanup after tmpnovell set/undo
 *---------------------------------------------------------------------*/
static int
tmpnovell_cleanup(doList_t *trash)
{
    tmpnovell_free(trash->data);
#ifdef SR_SNMPv2
    tmpnovell_free(trash->undodata);
#endif				       /* SR_SNMPv2 */
    return NO_ERROR;
}

/*---------------------------------------------------------------------
 * Determine if this SET request is valid. If so, add it to the do-list.
 * This operation is performed in 4 steps:
 *
 *   1) Validate the object instance.
 *   2) Locate a "related" do-list element, or create a new do-list
 *      element.
 *   3) Add the SET request to the selected do-list element.
 *   4) Finally, determine if the do-list element (all "related" set
 *      requests) is completely valid.
 *---------------------------------------------------------------------*/
int
tmpnovell_test(
    OID            *incoming,
    ObjectInfo     *object,
    ObjectSyntax   *value,
    doList_t       *doHead,
    doList_t       *doCur,
    ContextInfo    *contextInfo)
{
    int             instLength = incoming->length - object->oid.length;
    doList_t       *dp;
    int             found;

    /*
     * Validate the object instance: 1) It must be of length 1  2) and the
     * instance must be 0.
     */
    if (instLength != 1 || incoming->oid_ptr[incoming->length - 1] != 0) {
	return (NO_CREATION_ERROR);
    }
    found = 0;

    if (!found) {
	dp = doCur;
	if ((dp->data = malloc(sizeof(tmpnovell_t))) == NULL) {
	    DPRINTF((APALWAYS, "snmpd: Cannot allocate memory\n"));
	    return (RESOURCE_UNAVAILABLE_ERROR);
	}
	memset(dp->data, 0, sizeof(tmpnovell_t));

	dp->setMethod = tmpnovell_set;
	dp->cleanupMethod = tmpnovell_cleanup;
#ifdef SR_SNMPv2
	dp->undoMethod = NULL;
#endif				       /* SR_SNMPv2 */
	dp->state = UNKNOWN;

    }
    switch (object->nominator) {

#ifdef I_ipxActCheckPoint
      case I_ipxActCheckPoint:

	SET_VALID(I_ipxActCheckPoint, ((tmpnovell_t *) (dp->data))->valid);

	((tmpnovell_t *) (dp->data))->ipxActCheckPoint = value->sl_value;
	break;
#endif				       /* I_ipxActCheckPoint */

      default:
	DPRINTF((APALWAYS, "snmpd: Internal error (invalid nominator in tmpnovell_test)\n"));
	return (GEN_ERROR);

    }				       /* switch */
    dp->state = ADD_MODIFY;

    return (NO_ERROR);

}

/*---------------------------------------------------------------------
 * Perform the kernel-specific set function for this group of
 * related objects.
 *---------------------------------------------------------------------*/
int
tmpnovell_set(
    doList_t       *doHead,
    doList_t       *doCur,
    ContextInfo    *contextInfo)
{
    return (k_tmpnovell_set((tmpnovell_t *) (doCur->data),
			    contextInfo, doCur->state));
}

#endif				       /* SETS */


/*---------------------------------------------------------------------
 * Retrieve data from the lipxAccountingEntry family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
lipxAccountingEntry_get(
    OID            *incoming,
    ObjectInfo     *object,
    int             searchType,
    ContextInfo    *contextInfo,
    int             serialNum)
{
    int             instLength = incoming->length - object->oid.length;
    int             arg = object->nominator;
    void           *dp;
    lipxAccountingEntry_t *data=NULL;
    unsigned long   buffer[MAXBUFFERSIZE];
    OID             inst;
    int             carry;
    int             i;
    OctetString    *ipxActSrc = NULL;
    int             ipxActSrc_offset;
    OctetString    *ipxActDst = NULL;
    int             ipxActDst_offset;
    int             index;
    int             final_index;

    ipxActSrc_offset = object->oid.length;
    ipxActDst_offset = ipxActSrc_offset + sizeof(struct ipxaddrtype);
    final_index = ipxActDst_offset + sizeof(struct ipxaddrtype);

    if (searchType == EXACT) {
	if (instLength != final_index) {
	    return ((VarBind *) NULL);
	}
	carry = 0;
    } else {
	carry = 1;
    }

    if ((ipxActSrc != NULL) && (ipxActDst != NULL))
        show_oct_ipx(ipxActSrc, ipxActDst, 10);

    /* incoming has past history information in it.  Set up new from old */
    if ((InstToFixedOctetString(incoming, ipxActDst_offset, 
		&ipxActDst, searchType, &carry, sizeof(struct ipxaddrtype))) < 0) {
	arg = -1;
    }
    if ((InstToFixedOctetString(incoming, ipxActSrc_offset, 
		&ipxActSrc, searchType, &carry, sizeof(struct ipxaddrtype))) < 0) {
	arg = -1;
    }
    /* ipxActDst just got incremented by one from last value */

    if ((ipxActSrc != NULL) && (ipxActDst != NULL))
        show_oct_ipx(ipxActSrc, ipxActDst, 11);

    if (carry) {
	arg = -1;
    }
    /*
     * Retrieve the data from the kernel-specific routine.
     */
    if ((arg == -1) || (data = k_lipxAccountingEntry_get(serialNum, 
	    contextInfo, arg, searchType, ipxActSrc, ipxActDst)) == NULL) {
	arg = -1;
    } else {
	/*
	 * Build instance information
	 */
	inst.oid_ptr = buffer;
	index = 0;
	for (i = 0; i < data->ipxActSrc->length && (index < MAXBUFFERSIZE); i++) {
	    inst.oid_ptr[index++] = (unsigned long) data->ipxActSrc->octet_ptr[i];
	}

	for (i = 0; i < data->ipxActDst->length && (index < MAXBUFFERSIZE); i++) {
	    inst.oid_ptr[index++] = (unsigned long) data->ipxActDst->octet_ptr[i];
	}
	inst.length = index;
    }

    if ((ipxActSrc != NULL) && (ipxActDst != NULL))
        show_oct_ipx(ipxActSrc, ipxActDst, 7);
    if (data && (data->ipxActSrc != NULL) && (data->ipxActDst != NULL))
        show_oct_ipx(data->ipxActSrc, data->ipxActDst, 8);

    if (ipxActSrc != NULL) {
	FreeOctetString(ipxActSrc);
    }
    if (ipxActDst != NULL) {
	FreeOctetString(ipxActDst);
    }

    /*
     * Build the the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_ipxActSrc
      case I_ipxActSrc:
	dp = MakeOctetString(data->ipxActSrc->octet_ptr, data->ipxActSrc->length);
	break;
#endif				       /* I_ipxActSrc */

#ifdef I_ipxActDst
      case I_ipxActDst:
	dp = MakeOctetString(data->ipxActDst->octet_ptr, data->ipxActDst->length);
	break;
#endif				       /* I_ipxActDst */

#ifdef I_ipxActPkts
      case I_ipxActPkts:
	dp = &data->ipxActPkts;
	break;
#endif				       /* I_ipxActPkts */

#ifdef I_ipxActByts
      case I_ipxActByts:
	dp = &data->ipxActByts;
	break;
#endif				       /* I_ipxActByts */

      default:
	return ((VarBind *) NULL);

    }				       /* switch */

    return (MakeVarBind(object, &inst, dp));

}

/*---------------------------------------------------------------------
 * Retrieve data from the lipxCkAccountingEntry family. This is
 * performed in 3 steps:
 *
 *   1) Test the validity of the object instance.
 *   2) Retrieve the data.
 *   3) Build the variable binding (VarBind) that will be returned.
 *---------------------------------------------------------------------*/
VarBind        *
lipxCkAccountingEntry_get(
    OID            *incoming,
    ObjectInfo     *object,
    int             searchType,
    ContextInfo    *contextInfo,
    int             serialNum)
{
    int             instLength = incoming->length - object->oid.length;
    int             arg = object->nominator;
    void           *dp;
    lipxCkAccountingEntry_t *data=NULL;
    unsigned long   buffer[MAXBUFFERSIZE];
    OID             inst;
    int             carry;
    int             i;
    OctetString    *ipxCkactSrc;
    int             ipxCkactSrc_offset;
    OctetString    *ipxCkactDst;
    int             ipxCkactDst_offset;
    int             index;
    int             final_index;

    ipxCkactSrc_offset = object->oid.length;
    ipxCkactDst_offset = ipxCkactSrc_offset + sizeof(struct ipxaddrtype);
    final_index = ipxCkactDst_offset + sizeof(struct ipxaddrtype);

    if (searchType == EXACT) {
	if (instLength != final_index) {
	    return ((VarBind *) NULL);
	}
	carry = 0;
    } else {
	carry = 1;
    }

    if ((InstToFixedOctetString(incoming, ipxCkactDst_offset, 
		&ipxCkactDst, searchType, &carry, sizeof(struct ipxaddrtype))) < 0) {
	arg = -1;
    }
    if ((InstToFixedOctetString(incoming, ipxCkactSrc_offset, 
		&ipxCkactSrc, searchType, &carry, sizeof(struct ipxaddrtype))) < 0) {
	arg = -1;
    }
    /* ipxCkactDst just got incremented by one from last value */

    if (carry) {
	arg = -1;
    }
    /*
     * Retrieve the data from the kernel-specific routine.
     */
    if ((arg == -1) || (data = k_lipxCkAccountingEntry_get(serialNum, contextInfo, arg, searchType, ipxCkactSrc, ipxCkactDst)) == NULL) {
	arg = -1;
    } else {
	/*
	 * Build instance information
	 */
	inst.oid_ptr = buffer;
	index = 0;
	for (i = 0; i < data->ipxCkactSrc->length && (index < MAXBUFFERSIZE); i++) {
	    inst.oid_ptr[index++] = (unsigned long) data->ipxCkactSrc->octet_ptr[i];
	}

	for (i = 0; i < data->ipxCkactDst->length && (index < MAXBUFFERSIZE); i++) {
	    inst.oid_ptr[index++] = (unsigned long) data->ipxCkactDst->octet_ptr[i];
	}

	inst.length = index;
    }

    if (ipxCkactSrc != NULL) {
	FreeOctetString(ipxCkactSrc);
    }
    if (ipxCkactDst != NULL) {
	FreeOctetString(ipxCkactDst);
    }
    /*
     * Build the the variable binding for the variable that will be returned.
     */

    switch (arg) {

#ifdef I_ipxCkactSrc
      case I_ipxCkactSrc:
	dp = MakeOctetString(data->ipxCkactSrc->octet_ptr, data->ipxCkactSrc->length);
	break;
#endif				       /* I_ipxCkactSrc */

#ifdef I_ipxCkactDst
      case I_ipxCkactDst:
	dp = MakeOctetString(data->ipxCkactDst->octet_ptr, data->ipxCkactDst->length);
	break;
#endif				       /* I_ipxCkactDst */

#ifdef I_ipxCkactPkts
      case I_ipxCkactPkts:
	dp = &data->ipxCkactPkts;
	break;
#endif				       /* I_ipxCkactPkts */

#ifdef I_ipxCkactByts
      case I_ipxCkactByts:
	dp = &data->ipxCkactByts;
	break;
#endif				       /* I_ipxCkactByts */

      default:
	return ((VarBind *) NULL);

    }				       /* switch */

    return (MakeVarBind(object, &inst, dp));

}


tmpnovell_t    *
k_tmpnovell_get(
    int             serialNum,
    ContextInfo    *contextInfo,
    int             nominator)
{
    static tmpnovell_t tmpnovellData;

    tmpnovellData.novellInput = novell_stat[NOVELL_INPUT];
    tmpnovellData.novellBcastin = novell_stat[NOVELL_BCASTIN];
    tmpnovellData.novellForward = novell_stat[NOVELL_FORWARDED];
    tmpnovellData.novellBcastout = novell_stat[NOVELL_BCASTOUT];
    tmpnovellData.novellFormerr = novell_stat[NOVELL_FORMATERROR];
    tmpnovellData.novellChksum = novell_stat[NOVELL_CKSUMERR];
    tmpnovellData.novellHopcnt = novell_stat[NOVELL_HOPCOUNT];
    tmpnovellData.novellNoroute = novell_stat[NOVELL_NOROUTE];
    tmpnovellData.novellNoencap = novell_stat[NOVELL_ENCAPSFAILED];
    tmpnovellData.novellOutput = novell_stat[NOVELL_OUTPUT];
    tmpnovellData.novellInmult = novell_stat[NOVELL_INMULT];
    tmpnovellData.novellLocal = novell_stat[NOVELL_LOCALDEST];
    tmpnovellData.novellUnknown = novell_stat[NOVELL_UNKNOWN];
    tmpnovellData.novellSapreqin = novell_stat[NOVELL_SAPREQIN];
    tmpnovellData.novellSapresin = novell_stat[NOVELL_SAPRESIN];
    tmpnovellData.novellSapout = novell_stat[NOVELL_SAPOUT];
    tmpnovellData.novellSapreply = novell_stat[NOVELL_SAPREPLY];
    tmpnovellData.ipxActThresh = ipx_acctg_info.threshold;
    if (ipx_acctg_info.current) {
	tmpnovellData.ipxActLostPkts = ipx_acctg_info.current->packets_tossed;
	tmpnovellData.ipxActLostByts = ipx_acctg_info.current->bytes_tossed;
	tmpnovellData.ipxActAge = ELAPSED_TIME(ipx_acctg_info.current->when)/10;
    } else {
	tmpnovellData.ipxActLostPkts = 0;
	tmpnovellData.ipxActLostByts = 0;
	tmpnovellData.ipxActAge = 0;
    }
    if (ipx_acctg_info.backup) {
	tmpnovellData.ipxCkactAge = ELAPSED_TIME(ipx_acctg_info.backup->when)/10;
    } else {
	tmpnovellData.ipxCkactAge = 0;
    }
    tmpnovellData.ipxActCheckPoint = ipx_checkpointvalue;

    return (&tmpnovellData);
}

#ifdef SETS
int
k_tmpnovell_set(
    tmpnovell_t    *data,
    ContextInfo    *contextInfo,
    int             function)
{
    if (VALID(I_ipxActCheckPoint, data->valid)) {
	if (ipx_checkpointvalue == data->ipxActCheckPoint) {
	    ipx_checkpointvalue++;
	    clear_ipx_accounting_func(FALSE);
	}
    }
    return (NO_ERROR);
}

#endif				       /* SETS */

lipxAccountingEntry_t *
k_lipxAccountingEntry_get(
    int             serialNum,
    ContextInfo    *contextInfo,
    int             nominator,
    int             searchType,
    OctetString    *ipxActSrc,
    OctetString    *ipxActDst)
{
    static lipxAccountingEntry_t lipxAccountingEntryData;
    ipx_acctg_entry_type *p;
    static OctetString nipxActSrc, nipxActDst;
    static struct ipxaddrtype ipxaddrsrc, ipxaddrdst;

    show_oct_ipx(ipxActSrc, ipxActDst, 1);

    p = snmp_get_ipx(searchType, ipxActSrc, ipxActDst, TRUE);
    if (p == NULL)
	return(NULL);

    show_oct_ipx(ipxActSrc, ipxActDst, 2);  

    ipxaddrsrc.net = p->srcnet;
    ieee_copy(p->srchost, ipxaddrsrc.host);
    lipxAccountingEntryData.ipxActSrc = &nipxActSrc;
    lipxAccountingEntryData.ipxActSrc->octet_ptr = (uchar *) &(ipxaddrsrc.net);
    lipxAccountingEntryData.ipxActSrc->length = sizeof(ulong) + IEEEBYTES;
    
    ipxaddrdst.net = p->dstnet;
    ieee_copy(p->dsthost, ipxaddrdst.host);
    lipxAccountingEntryData.ipxActDst = &nipxActDst;
    lipxAccountingEntryData.ipxActDst->octet_ptr = (uchar *) &(ipxaddrdst.net);
    lipxAccountingEntryData.ipxActDst->length = sizeof(ulong) + IEEEBYTES;

    show_oct_ipx(&nipxActSrc, &nipxActDst, 3);
    show_oct_ipx(lipxAccountingEntryData.ipxActSrc, lipxAccountingEntryData.ipxActDst, 4);

    lipxAccountingEntryData.ipxActPkts = p->packets;
    lipxAccountingEntryData.ipxActByts = p->bytes;

    return (&lipxAccountingEntryData);
}


lipxCkAccountingEntry_t *
k_lipxCkAccountingEntry_get(
    int             serialNum,
    ContextInfo    *contextInfo,
    int             nominator,
    int             searchType,
    OctetString    *ipxCkactSrc,
    OctetString    *ipxCkactDst)
{
    static lipxCkAccountingEntry_t lipxCkAccountingEntryData;
    ipx_acctg_entry_type *p;
    static OctetString nipxCkactSrc, nipxCkactDst;
    static struct ipxaddrtype ipxaddrsrc, ipxaddrdst;

    p = snmp_get_ipx(searchType, ipxCkactSrc, ipxCkactDst, FALSE);
    if (p == NULL)
	return(NULL);

    ipxaddrsrc.net = p->srcnet;
    ieee_copy(p->srchost, ipxaddrsrc.host);
    lipxCkAccountingEntryData.ipxCkactSrc = &nipxCkactSrc;
    lipxCkAccountingEntryData.ipxCkactSrc->octet_ptr = (uchar *) &(ipxaddrsrc.net);
    lipxCkAccountingEntryData.ipxCkactSrc->length = sizeof(ulong) + IEEEBYTES;
    
    ipxaddrdst.net = p->dstnet;
    ieee_copy(p->dsthost, ipxaddrdst.host);
    lipxCkAccountingEntryData.ipxCkactDst = &nipxCkactDst;
    lipxCkAccountingEntryData.ipxCkactDst->octet_ptr = (uchar *) &(ipxaddrdst.net);
    lipxCkAccountingEntryData.ipxCkactDst->length = sizeof(ulong) + IEEEBYTES;

    lipxCkAccountingEntryData.ipxCkactPkts = p->packets;
    lipxCkAccountingEntryData.ipxCkactByts = p->bytes;

    return (&lipxCkAccountingEntryData);
}


static ipx_acctg_entry_type *snmp_get_ipx(int searchType, OctetString *ipxSrc,
    OctetString *ipxDst, boolean isacc)
{
    uint srclen, dstlen;
    ulong srcnet, dstnet;
    uchar srchost[IEEEBYTES];
    uchar dsthost[IEEEBYTES];
    ipx_acctg_entry_type *p;
    boolean next = FALSE;
    struct ipxaddrtype *sipxaddr, *dipxaddr;

    if (searchType == NEXT)
	next = TRUE;
    sipxaddr = (struct ipxaddrtype *)ipxSrc->octet_ptr;
    srclen = ipxSrc->length;
    srcnet = sipxaddr->net;
    ieee_copy(sipxaddr->host, srchost);

    dipxaddr = (struct ipxaddrtype *)ipxDst->octet_ptr;
    dstlen = ipxDst->length;
    dstnet = dipxaddr->net;
    ieee_copy(dipxaddr->host, dsthost);
    
    if (!ipx_acctg_on)
	return(NULL);

    p = NULL;
    if (isacc)					/* for like ipxActSrc */
	p = snmp_ipx_acct(ipx_acctg_info.current, srcnet, srchost, dstnet,
			  dsthost, srclen, dstlen, next);
    else if (ipx_acctg_info.backup != NULL) 	/* for like ipxCkactSrc */
	p = snmp_ipx_acct(ipx_acctg_info.backup, srcnet, srchost, dstnet,
			  dsthost, srclen, dstlen, next);
    if (p != NULL) {
	sipxaddr->net = p->srcnet;
	ieee_copy(p->srchost, sipxaddr->host);
	dipxaddr->net = p->dstnet;
	ieee_copy(p->dsthost, dipxaddr->host);
    }
    return(p);
}


static void show_oct_ipx(OctetString *ipxSrc, OctetString *ipxDst, int val)
{
#ifdef DEBUGGY
    uint srclen, dstlen;
    ulong srcnet, dstnet;
    uchar srchost[IEEEBYTES];
    uchar dsthost[IEEEBYTES];
    struct ipxaddrtype *sipxaddr, *dipxaddr;

    sipxaddr = (struct ipxaddrtype *)ipxSrc->octet_ptr;
    if (sipxaddr==NULL) 
	return;
    srclen = ipxSrc->length;
    srcnet = sipxaddr->net;
    ieee_copy(sipxaddr->host, srchost);

    dipxaddr = (struct ipxaddrtype *)ipxDst->octet_ptr;
    if (dipxaddr==NULL) 
	return;
    dstlen = ipxDst->length;
    dstnet = dipxaddr->net;
    ieee_copy(dipxaddr->host, dsthost);
    
    printf("\n%d OCTET %08x.%e %08x.%e %d %d)", 
       val,srcnet,srchost,dstnet,dsthost,srclen,dstlen);
#endif
}


/*
 * snmp_ipx_acct
 * Perform IPX accounting lookup function for SNMP
 */
static ipx_acctg_entry_type *snmp_ipx_acct (ipx_acctg_data_type *ipxadt,
     ulong srcnet, uchar *srchost, ulong dstnet,
     uchar *dsthost, boolean srcset, boolean dstset, boolean next)
{
    ipx_acctg_entry_type *p, *np;
    long i;

    if (!srcset || !dstset)
	next = TRUE;

    if (!next) {
	i = srcnet;
	i ^= dstnet;
	i ^= (i >> 16);
	i ^= (i >> 8);
	i &= 0xff;
    } else 
        i = 0;
    np = NULL;
    /* this sorts table by increasing OID / instance numbers */
    for ( ; i < IPXACCTGARRAYSIZE; i++) {
	p = ipxadt->store[i];
	while (p) {
	    if (!next) {
		 if (ipx_addr_eq(p->srcnet, p->srchost, srcnet, srchost) &&
		     ipx_addr_eq(p->dstnet, p->dsthost, dstnet, dsthost))
		   return(p);
	    } else {
		/*
		 * this ugly code is attempting to find the next
		 * address to return.
		 */
		if (np == NULL) {
		    if (ipx_addr_eq(p->srcnet, p->srchost, srcnet,
				       srchost) && 
			(ipx_addr_gt(p->dstnet, p->dsthost, dstnet, dsthost) ||
			ipx_addr_eq(p->dstnet, p->dsthost, dstnet, dsthost)) )
		      np = p;
		    else if (ipx_addr_gt(p->srcnet, p->srchost, srcnet,
					 srchost))
		      np = p;
		} else {
		  if (ipx_addr_gt(p->srcnet, p->srchost, srcnet,
				       srchost) &&
			   ipx_addr_gt(np->srcnet, np->srchost, p->srcnet,
				       p->srchost)) {
		  np = p;
		}
		else if (ipx_addr_eq(p->srcnet, p->srchost, srcnet,
				      srchost) &&
			  (ipx_addr_gt(p->dstnet, p->dsthost, dstnet, dsthost)  ||
			  ipx_addr_eq(p->dstnet, p->dsthost, dstnet, dsthost)) ) {
		    if (ipx_addr_eq(p->srcnet, p->srchost, np->srcnet,
				    np->srchost)) {
			if (ipx_addr_gt(np->dstnet, np->dsthost, p->dstnet,
					p->dsthost))
			  np = p;
			/* added below */
			if (ipx_addr_eq(np->dstnet, np->dsthost, p->dstnet,
					p->dsthost))
			  np = p;
		    } else
		      np = p;
		} else if (!ipx_addr_eq(srcnet, srchost, p->srcnet,
					p->srchost) &&
			   ipx_addr_eq(p->srcnet, p->srchost, np->srcnet,
				       np->srchost) &&
			   (ipx_addr_gt(np->dstnet, np->dsthost, p->dstnet, p->dsthost) ||
			   ipx_addr_eq(np->dstnet, np->dsthost, p->dstnet, p->dsthost)) )
		  np = p;
		}
	    }
	    p = p->next;
	}
    }
    return(np);
}


/*
 * novellmib subsystem header
 */

#define MAJVERSION_old_lnovellmib 1
#define MINVERSION_old_lnovellmib 0
#define EDITVERSION_old_lnovellmib 0

SUBSYS_HEADER(novellmib,
              MAJVERSION_old_lnovellmib,
              MINVERSION_old_lnovellmib,
              EDITVERSION_old_lnovellmib,
              init_old_lnovellmib,
              SUBSYS_CLASS_MANAGEMENT,
              "req: ipx",
              NULL);
      