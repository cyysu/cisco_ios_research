/* $Id: exec_show_refuse.h,v 3.3.20.1 1996/08/12 16:08:05 widmer Exp $
 * $Source: /release/112/cvs/Xsys/tcp/exec_show_refuse.h,v $
 *------------------------------------------------------------------
 * E X E C _ S H O W _ R E F U S E . H
 *
 * Copyright (c) 1992-1996 by cisco Systems, Inc.
 * All Rights Reserved
 *------------------------------------------------------------------
 * $Log: exec_show_refuse.h,v $
 * Revision 3.3.20.1  1996/08/12  16:08:05  widmer
 * CSCdi65725:  Create Eolns macro that defaults csb->which to 0
 * Branch: California_branch
 * Add Eolns and Nvgenns macro that default csb->which to 0
 * Remove hidden keyword help strings
 *
 * Revision 3.3  1996/01/29  07:09:54  hampton
 * Remove command number arguments passed to parse routines that only
 * perform a single function.  [CSCdi47717]
 *
 * Revision 3.2  1995/11/17  18:47:43  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:35:08  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.1  1995/07/01  12:56:32  tli
 * CSCdi36508:  Rationalize parser file placement, part I
 * Rationalize ip, ipmulticast, tcp
 *
 * Revision 2.1  1995/06/07  22:25:27  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */


/******************************************************************
 * show refuse-message
 *
 */
EOLNS	(show_refuse_eol, show_ttybusy);
KEYWORD (show_refuse, show_refuse_eol, ALTERNATE,
	 "refuse-message", NULL,
	 PRIV_USER | PRIV_HIDDEN);


#undef	ALTERNATE
#define	ALTERNATE	show_refuse
