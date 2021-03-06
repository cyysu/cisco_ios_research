/* $Id: pmd_eeprom.h,v 1.1.4.1 1996/05/09 14:55:44 rbadri Exp $
 * $Source: /release/112/cvs/Xwbu/cat5k/alc_include/pmd_eeprom.h,v $
 *------------------------------------------------------------------
 * pmd_eeprom.h -- Contains all desc for the serial eeprom on PMD
 *
 * ?
 *
 * Copyright (c) 1991-1996 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 * $Log: pmd_eeprom.h,v $
 * Revision 1.1.4.1  1996/05/09  14:55:44  rbadri
 * Branch: California_branch
 * Synergy features to the mainline
 *
 * Revision 1.1.40.1  1996/04/27  06:15:13  cakyol
 * non sync sync from V112_0_2 to ....
 * Branch: LE_Cal_ATM_FR_California_Postsync_960425_branch
 *
 * Revision 1.1.28.1  1996/04/08  01:40:11  bbenson
 * Branch: LE_Cal_V112_0_2_branch
 * Sync of LE_Cal to new V112_0_2 sync point (post Port Ready).
 *
 * Revision 1.1.16.1  1996/03/22  09:11:22  rlowe
 * Non-sync Synalc3_LE_Cal_V111_1_0_3_merge_branch to V111_1_3
 * yielding LE_Syn_Cal_V111_1_3_branch.
 * Branch: LE_Syn_Cal_V111_1_3_branch
 *
 * Revision 1.1.2.1  1996/03/13  07:42:15  rlowe
 * Add file content from Synergy repository.
 * Branch: Synalc3_LE_Cal_V111_1_0_3_merge_branch
 *
 * Revision 1.1  1996/03/11  07:40:43  rlowe
 * Placeholders for California.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*********************************************************************

        File:      	pmd_eeprom.h
	
	
        Last Modified:  8/28/91

	Description: Contains all desc for the serial eeprom on PMD	
 
$log:$
*********************************************************************/

#ifndef __PMD_EEPROM_H
#define __PMD_EEPROM_H

/***********
 * Defines *
 ***********/

#define E_ADDR_MASK     0x003F
#define E_RD_CODE       0x0180
#define E_WR_CODE       0x0140
#define E_MAX_LEN       64
#define E_WEN_CODE      0x0130
#define E_WDS_CODE		0x0100

/* Read Protected area return values */
#define SER_EEPROM_OK         	0
#define SER_EEPROM_UNINIT     	1
#define SER_EEPROM_CHKSUM_ERROR	2

#define FEATURE_LINK_GOOD	0x0001
#define FEATURE_10BASEF		0x0002

#define OEM_STR_SIZE			28
#define MODEL_STR_SIZE		10 

#define START_PROT_AREA 	0x0010 /* Start of protected area */
#define PROT_AREA_SIZE		0x0030 /* size of protected area */


/**************
 * Structures *
 **************/

/* define split uInt16 for indivdual byte access */
struct spint {
  uChar h;
  uChar l;
};


typedef union {
  uInt16        word;
  struct spint  byte;
} BYT_ACC;

/* define slit long word for individual word access */
struct splint {
  uInt16 h;
  uInt16 l;
};

typedef union {
  uInt32        lword;
  struct splint word;
} WRD_ACC;

typedef union
{
  uInt16	word[3];
  uChar		byte[6];
} MAC_ACC;

typedef union
{
  uInt16	word[(OEM_STR_SIZE >> 1)];
  uChar		str[OEM_STR_SIZE];
} STR_ACC;

typedef union
{
  uInt16	word[(MODEL_STR_SIZE >> 1)];
  uChar		str[MODEL_STR_SIZE];
} MOD_ACC;


/*-------------------------------------------------------------------*
 * pmd_data{}
 *		93c46 is 16-bit word access. make member element aligned on
 *		16-bit word boundary
 *-------------------------------------------------------------------*/
struct pmd_data
{
	uInt16   data_valid;                 /* valid signature: 0x55aa */
	uInt32	serial_number;
	uInt16   hw_version_high;
	uInt16   hw_version_low;
	char	   pmd_name[MODEL_STR_SIZE];               /* char string name */ 
	uInt16   feature;
	uInt16   pmd_type;						 /* use SNMP mib-defined type */
	/*
	 * Insert new stuff here and reduce the size
	 */
	uInt16  empty[46];   /* of "empty" by the size of inserted stuff. */

	uInt16   checksum;   /* checksum must always be in the last word */
};

#endif  /* __PMD_EEPROM_H  */
