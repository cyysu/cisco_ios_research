/* $Id: radix.c,v 3.3.56.5 1996/09/11 23:07:12 snyder Exp $
 * $Source: /release/112/cvs/Xsys/util/radix.c,v $
 *------------------------------------------------------------------
 * Radix Trie manipulation package
 *
 * January 1993, Paul Traina
 *
 * A small portion of this code is
 * Copyright (c) 1993-1996,1997 by cisco Systems, Inc.
 * All rights reserved.
 *
 * See below for additional copyright notices.
 *
 * This file contains the radix trie manipulation code from
 * 4.4BSD with additions and a few modifications for cisco.
 *
 *------------------------------------------------------------------
 * $Log: radix.c,v $
 * Revision 3.3.56.5  1996/09/11  23:07:12  snyder
 * CSCdi68880:  more const stuff
 *              120 out of image, 144 data
 * Branch: California_branch
 *
 * Revision 3.3.56.4  1996/08/28  13:19:02  thille
 * CSCdi67083: Message externs need a cleanup.
 * Branch: California_branch
 * Get rid of all extern msgsym()s in .c files, rework logger.h and
 * msg_foo.c files so they are included and provide the externs.  Clean up
 * other random externs in .c files.
 *
 * Revision 3.3.56.3  1996/08/06  08:28:23  gchristy
 * CSCdi64974:  IP Cache: delayed invalidation can create inconsistancies
 * Branch: California_branch
 *  - Back out changes made for CSCdi55725.
 *
 * Revision 3.3.56.2  1996/06/24  16:05:45  myeung
 * CSCdi55725:  OSPF CPU HOG
 * Branch: California_branch
 * - Modify IP cache invalidation code to allow delayed invalidation
 *
 * Revision 3.3.56.1  1996/03/18  22:30:48  gstovall
 * Branch: California_branch
 * Elvis has left the building.  He headed out to California, and took the
 * port ready changes with him.
 *
 * Revision 3.3.22.2  1996/03/07  11:00:00  mdb
 * Branch: DeadKingOnAThrone_branch
 * cisco and ANSI/POSIX libraries.
 *
 * Revision 3.3.22.1  1996/02/20  21:45:11  dstine
 * Branch: DeadKingOnAThrone_branch
 *           Sync from DeadKingOnAThrone_baseline_960122 to
 *                     DeadKingOnAThrone_baseline_960213
 *
 * Revision 3.3  1995/11/25  12:09:54  rchandra
 * CSCdi34541:  BGP: bgp out of sync with ip routing table
 * - during version walk use the child's version number to pick the
 *   correct sub-tree to walk (fix from Paul)
 *
 * Revision 3.2  1995/11/17  17:56:22  hampton
 * Remove old entries from the RCS header logs.
 *
 * Revision 3.1  1995/11/09  13:43:28  shaker
 * Bump version numbers from 2.x to 3.x.
 *
 * Revision 2.6  1995/11/08  21:34:04  shaker
 * Merge Arkansas_branch into 11.1 mainline.
 *
 * Revision 2.5  1995/08/24  22:34:47  bcole
 * CSCdi39279:  NHRP: Aggregation needed to provide purge notification
 *
 * Provide aggregated NHRP responses.
 * Cache now consists of radix tree(s) instead of hash table.
 * Allow NHRP resolution during encapsulation of NHRP purge messages.
 * For IP, NHRP now a routing process so as to receive callbacks.
 * Allow for radix tree walk where varargs have already been pre-collected.
 *
 * Revision 2.4  1995/08/04  06:44:16  hampton
 * sched_private.h included too many places  [CSCdi38246]
 *
 * Revision 2.3  1995/06/23 18:48:46  ahh
 * CSCdi36037:  Tons of memory wasted on no memory messages
 * Phase III -- remove unneeded NOMEMORY message externs.
 *
 * Revision 2.2  1995/06/09  13:22:29  hampton
 * Rename old scheduler calls to fit the new naming convention.
 * [CSCdi35514]
 *
 * Revision 2.1  1995/06/07 23:13:04  hampton
 * Bump version numbers from 1.x to 2.x.
 *
 *------------------------------------------------------------------
 * $Endlog$
 */

/*
 * Copyright (c) 1988, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)radix.c	8.4 (Berkeley) 11/2/94
 */

/*
 * Routines to build and maintain radix trees for routing lookups.
 */
#include "master.h"
#include <stdarg.h>
#include <string.h>
#include <ciscolib.h>
#include "logger.h"
#include "../util/msg_radix.c"		/* Not a typo, see logger.h */
#include "radix.h"

static int max_keylen = 0;
static struct radix_mask *rn_mkfreelist;
static struct radix_node_head *mask_rnhead;
static caddr_t addmask_key;
static const u_char normal_chars[] =
	{0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, -1};
static char *rn_zeros, *rn_ones;

static int rn_refines(void *, void *);
static struct radix_node
	*rn_addmask(void *, int, int),
	*rn_newpair(void *, int, struct radix_node[2]),
       	*rn_insert(void *, struct radix_node_head *, int *,
                      struct radix_node [2]);
static int rn_init(void);

static const int rn_debug = FALSE;	/* Never set, init to skip dead code */

#define rn_masktop (mask_rnhead->rnh_treetop)
#undef Bcmp
#define Bcmp(a, b, l) (l == 0 ? 0 : bcmp((caddr_t)(a), (caddr_t)(b), (u_long)l))
/*
 * The data structure for the keys is a radix tree with one way
 * branching removed.  The index rn_b at an internal node n represents a bit
 * position to be tested.  The tree is arranged so that all descendants
 * of a node n have keys whose bits all agree up to position rn_b - 1.
 * (We say the index of n is rn_b.)
 *
 * There is at least one descendant which has a one bit at position rn_b,
 * and at least one with a zero there.
 *
 * A route is determined by a pair of key and mask.  We require that the
 * bit-wise logical and of the key and mask to be the key.
 * We define the index of a route to associated with the mask to be
 * the first bit number in the mask where 0 occurs (with bit number 0
 * representing the highest order bit).
 * 
 * We say a mask is normal if every bit is 0, past the index of the mask.
 * If a node n has a descendant (k, m) with index(m) == index(n) == rn_b,
 * and m is a normal mask, then the route applies to every descendant of n.
 * If the index(m) < rn_b, this implies the trailing last few bits of k
 * before bit b are all 0, (and hence consequently true of every descendant
 * of n), so the route applies to all descendants of the node as well.
 * 
 * Similar logic shows that a non-normal mask m such that
 * index(m) <= index(n) could potentially apply to many children of n.
 * Thus, for each non-host route, we attach its mask to a list at an internal
 * node as high in the tree as we can go. 
 *
 * The present version of the code makes use of normal routes in short-
 * circuiting an explict mask and compare operation when testing whether
 * a key satisfies a normal route, and also in remembering the unique leaf
 * that governs a subtree.
 */

inline struct radix_node *
rn_search (void *v_arg, struct radix_node *head)
{
	register struct radix_node *x;
	register caddr_t v;

	for (x = head, v = v_arg; x->rn_b >= 0;) {
		if (x->rn_bmask & v[x->rn_off])
			x = x->rn_r;
		else
			x = x->rn_l;
	}
	return (x);
}

inline struct radix_node *
rn_search_m (void *v_arg, struct radix_node *head, void *m_arg)
{
	register struct radix_node *x;
	register caddr_t v = v_arg, m = m_arg;

	for (x = head; x->rn_b >= 0;) {
		if ((x->rn_bmask & m[x->rn_off]) &&
		    (x->rn_bmask & v[x->rn_off]))
			x = x->rn_r;
		else
			x = x->rn_l;
	}
	return x;
}

static inline int
rn_refines (void *m_arg, void *n_arg)
{
	register caddr_t m = m_arg, n = n_arg;
	register caddr_t lim, lim2 = lim = n + *(u_char *)n;
	int longer = (*(u_char *)n++) - (int)(*(u_char *)m++);
	int masks_are_equal = 1;

	if (longer > 0)
		lim -= longer;
	while (n < lim) {
		if (*n & ~(*m))
			return 0;
		if (*n++ != *m++)
			masks_are_equal = 0;
	}
	while (n < lim2)
		if (*n++)
			return 0;
	if (masks_are_equal && (longer < 0))
		for (lim2 = m - longer; m < lim2; )
			if (*m++)
				return 1;
	return (!masks_are_equal);
}

struct radix_node *
rn_lookup (void *v_arg, void *m_arg, struct radix_node_head *head)
{
	register struct radix_node *x;
	caddr_t netmask = 0;

	if (m_arg) {
		if ((x = rn_addmask(m_arg, 1, head->rnh_treetop->rn_off)) == 0)
			return (0);
		netmask = x->rn_key;
	}
	x = rn_match(v_arg, head);
	if (x && netmask) {
		while (x && x->rn_mask != netmask)
			x = x->rn_dupedkey;
	}
	return x;
}

static inline int
rn_satsifies_leaf (char *trial, register struct radix_node *leaf, int skip)
{
	register char *cp = trial, *cp2 = leaf->rn_key, *cp3 = leaf->rn_mask;
	char *cplim;
	int length = min(*(u_char *)cp, *(u_char *)cp2);

	if (cp3 == 0)
		cp3 = rn_ones;
	else
		length = min(length, *(u_char *)cp3);
	cplim = cp + length; cp3 += skip; cp2 += skip;
	for (cp += skip; cp < cplim; cp++, cp2++, cp3++)
		if ((*cp ^ *cp2) & *cp3)
			return 0;
	return 1;
}

inline struct radix_node *
rn_match_unsafe (void *v_arg, struct radix_node_head *head)
{
	caddr_t v = v_arg;
	register struct radix_node *t = head->rnh_treetop, *x;
	register caddr_t cp = v, cp2;
	caddr_t cplim;
	struct radix_node *saved_t, *top = t;
	int off = t->rn_off, vlen = *(u_char *)cp, matched_off;
	register int test, b, rn_b;

	/*
	 * Open code rn_search(v, top) to avoid overhead of extra
	 * subroutine call.
	 */
	for (; t->rn_b >= 0; ) {
		if (t->rn_bmask & cp[t->rn_off])
			t = t->rn_r;
		else
			t = t->rn_l;
	}
	/*
	 * See if we match exactly as a host destination
	 * or at least learn how many bits match, for normal mask finesse.
	 *
	 * It doesn't hurt us to limit how many bytes to check
	 * to the length of the mask, since if it matches we had a genuine
	 * match and the leaf we have is the most specific one anyway;
	 * if it didn't match with a shorter length it would fail
	 * with a long one.  This wins big for class B&C netmasks which
	 * are probably the most common case...
	 */
	if (t->rn_mask)
		vlen = *(u_char *)t->rn_mask;
	cp += off; cp2 = t->rn_key + off; cplim = v + vlen;
	for (; cp < cplim; cp++, cp2++)
		if (*cp != *cp2)
			goto on1;
	/*
	 * This extra grot is in case we are explicitly asked
	 * to look up the default.  Ugh!
	 */
	if ((t->rn_flags & RNF_ROOT) && t->rn_dupedkey)
		t = t->rn_dupedkey;
	return t;
on1:
	test = (*cp ^ *cp2) & 0xff; /* find first bit that differs */
	for (b = 7; (test >>= 1) > 0;)
		b--;
	matched_off = cp - v;
	b += matched_off << 3;
	rn_b = -1 - b;
	/*
	 * If there is a host route in a duped-key chain, it will be first.
	 */
	if ((saved_t = t)->rn_mask == 0)
		t = t->rn_dupedkey;
	for (; t; t = t->rn_dupedkey)
		/*
		 * Even if we don't match exactly as a host,
		 * we may match if the leaf we wound up at is
		 * a route to a net.
		 */
		if (t->rn_flags & RNF_NORMAL) {
			if (rn_b <= t->rn_b)
				return t;
		} else if (rn_satsifies_leaf(v, t, matched_off))
				return t;
	t = saved_t;
	/* start searching up the tree */
	do {
		register struct radix_mask *m;
		t = t->rn_p;
		if ((m = t->rn_mklist)) {
			/*
			 * If non-contiguous masks ever become important
			 * we can restore the masking and open coding of
			 * the search and satisfaction test and put the
			 * calculation of "off" back before the "do".
			 */
			do {
				if (m->rm_flags & RNF_NORMAL) {
					if (rn_b <= m->rm_b)
						return (m->rm_leaf);
				} else {
					off = min(t->rn_off, matched_off);
					x = rn_search_m(v, t, m->rm_mask);
					while (x && x->rn_mask != m->rm_mask)
						x = x->rn_dupedkey;
					if (x && rn_satsifies_leaf(v, x, off))
						    return x;
				}
			} while ((m = m->rm_mklist));
		}
	} while (t != top);
	return 0;
}

/*
 * Don't ever return a root node
 * cisco added code
 */
struct radix_node *
rn_match (void *v_arg, struct radix_node_head *head)
{
	struct radix_node *rn;

	rn = rn_match_unsafe(v_arg, head);

	if (rn && (rn->rn_flags & RNF_ROOT))
	    return 0;

	return rn;
}

#ifdef RN_DEBUG
int	rn_nodenum;
struct	radix_node *rn_clist;
int	rn_saveinfo;
#endif

static inline struct radix_node *
rn_newpair (void *v, int b, struct radix_node *nodes)
{
	register struct radix_node *tt = nodes, *t = tt + 1;
	t->rn_b = b; t->rn_bmask = 0x80 >> (b & 7);
	t->rn_l = tt; t->rn_off = b >> 3;
	tt->rn_b = -1; tt->rn_key = (caddr_t)v; tt->rn_p = t;
	tt->rn_flags = t->rn_flags = RNF_ACTIVE; 
	/* make sure internal nodes get marked so they are always traversed
	   even in race cases */
	tt->rn_version = t->rn_version = MAXULONG;	/* cisco */
#ifdef RN_DEBUG
	tt->rn_info = rn_nodenum++; t->rn_info = rn_nodenum++;
	tt->rn_twin = t; tt->rn_ybro = rn_clist; rn_clist = tt;
#endif
	return t;
}

static struct radix_node *
rn_insert (void *v_arg, struct radix_node_head *head, int *dupentry,
	   struct radix_node *nodes)
{
	caddr_t v = v_arg;
	struct radix_node *top = head->rnh_treetop;
	int head_off = top->rn_off, vlen = (int)*((u_char *)v);
	register struct radix_node *t = rn_search(v_arg, top);
	register caddr_t cp = v + head_off;
	register int b;
	struct radix_node *tt;


	/*
	 * Find first bit at which v and t->rn_key differ
	 */
    {
	register caddr_t cp2 = t->rn_key + head_off;
	register int cmp_res;
	caddr_t cplim = v + vlen;

	while (cp < cplim)
		if (*cp2++ != *cp++)
			goto on1;
	*dupentry = 1;
	return t;
on1:
	*dupentry = 0;
	cmp_res = (cp[-1] ^ cp2[-1]) & 0xff;
	for (b = (cp - v) << 3; cmp_res; b--)
		cmp_res >>= 1;
    }
    {
	register struct radix_node *p, *x = top;
	cp = v;
	do {
		p = x;
		if (cp[x->rn_off] & x->rn_bmask) 
			x = x->rn_r;
		else x = x->rn_l;
	} while (b > (unsigned) x->rn_b); /* x->rn_b < b && x->rn_b >= 0 */
#ifdef RN_DEBUG
	if (rn_debug)
		log(LOG_DEBUG, "rn_insert: Going In:\n"), traverse(p);
#endif
	t = rn_newpair(v_arg, b, nodes); tt = t->rn_l;
	if ((cp[p->rn_off] & p->rn_bmask) == 0)
		p->rn_l = t;
	else
		p->rn_r = t;
	x->rn_p = t; t->rn_p = p; /* frees x, p as temp vars below */
	if ((cp[t->rn_off] & t->rn_bmask) == 0) {
		t->rn_r = x;
	} else {
		t->rn_r = tt; t->rn_l = x;
	}
#ifdef RN_DEBUG
	if (rn_debug)
		log(LOG_DEBUG, "rn_insert: Coming Out:\n"), traverse(p);
#endif
    }
	return (tt);
}

static struct radix_node *
rn_addmask (void *n_arg, int search, int skip)
{
	caddr_t netmask = (caddr_t)n_arg;
	register struct radix_node *x;
	register caddr_t cp, cplim;
	register int b = 0, mlen, j;
	int maskduplicated, m0, isnormal;
	struct radix_node *saved_x;
	static int last_zeroed = 0;

	if ((mlen = *(u_char *)netmask) > max_keylen)
		mlen = max_keylen;
	if (skip == 0)
		skip = 1;
	if (mlen <= skip)
		return (mask_rnhead->rnh_nodes);
	if (skip > 1)
		Bcopy(rn_ones + 1, addmask_key + 1, skip - 1);
	if ((m0 = mlen) > skip)
		Bcopy(netmask + skip, addmask_key + skip, mlen - skip);
	/*
	 * Trim trailing zeroes.
	 */
	for (cp = addmask_key + mlen; (cp > addmask_key) && cp[-1] == 0;)
		cp--;
	mlen = cp - addmask_key;
	if (mlen <= skip) {
		if (m0 >= last_zeroed)
			last_zeroed = mlen;
		return (mask_rnhead->rnh_nodes);
	}
	if (m0 < last_zeroed)
		Bzero(addmask_key + m0, last_zeroed - m0);
	*addmask_key = last_zeroed = mlen;
	x = rn_search(addmask_key, rn_masktop);
	if (Bcmp(addmask_key, x->rn_key, mlen) != 0)
		x = 0;
	if (x || search)
		return (x);
	R_Malloc(x, struct radix_node *, max_keylen + 2 * sizeof (*x));
	if ((saved_x = x) == 0)
		return (0);
	Bzero(x, max_keylen + 2 * sizeof (*x));
	netmask = cp = (caddr_t)(x + 2);
	Bcopy(addmask_key, cp, mlen);
	x = rn_insert(cp, mask_rnhead, &maskduplicated, x);
	if (maskduplicated) {
		errmsg(&msgsym(ADDMASK, RADIX), "mask already in tree");
		Free(saved_x);
		return (x);
	}
	/*
	 * Calculate index of mask, and check for normalcy.
	 */
	cplim = netmask + mlen; isnormal = 1;
	for (cp = netmask + skip; (cp < cplim) && *(u_char *)cp == 0xff;)
		cp++;
	if (cp != cplim) {
		for (j = 0x80; (j & *cp) != 0; j >>= 1)  
			b++;
		if (*cp != normal_chars[b] || cp != (cplim - 1))
			isnormal = 0;
	}
	b += (cp - netmask) << 3;
	x->rn_b = -1 - b;
	if (isnormal)
		x->rn_flags |= RNF_NORMAL;
	return (x);
}

static inline int	/* XXX: arbitrary ordering for non-contiguous masks */
rn_lexobetter(void *m_arg, void *n_arg)
{
	register u_char *mp = m_arg, *np = n_arg, *lim;

	if (*mp > *np)
		return 1;  /* not really, but need to check longer one first */
	if (*mp == *np) 
		for (lim = mp + *mp; mp < lim;)
			if (*mp++ > *np++)
				return 1;
	return 0;
}

static inline struct radix_mask *
rn_new_radix_mask(register struct radix_node *tt,
		  register struct radix_mask *next)
{
	register struct radix_mask *m;

	MKGet(m);
	if (m == 0) {
		errmsg(&msgsym(ADDMASK, RADIX), "mask for route not entered");
		return (0);
	}
	Bzero(m, sizeof *m);
	m->rm_b = tt->rn_b;
	m->rm_flags = tt->rn_flags;
	if (tt->rn_flags & RNF_NORMAL)
		m->rm_leaf = tt;
	else
		m->rm_mask = tt->rn_mask;
	m->rm_mklist = next;
	tt->rn_mklist = m;
	return m;
}

struct radix_node *
rn_addroute (void *v_arg, void *n_arg, struct radix_node_head *head,
	     struct radix_node *treenodes)
{
	caddr_t v = (caddr_t) v_arg, netmask = (caddr_t) n_arg;
	register struct radix_node *t, *x = NULL, *tt;
	struct radix_node *saved_tt, *top = head->rnh_treetop;
	short b = 0, b_leaf = 0;
	int keyduplicated;
	caddr_t mmask;
	struct radix_mask *m, **mp;

	/*
	 * In dealing with non-contiguous masks, there may be
	 * many different routes which have the same mask.
	 * We will find it useful to have a unique pointer to
	 * the mask to speed avoiding duplicate references at
	 * nodes and possibly save time in calculating indices.
	 */
	if (netmask)  {
		if ((x = rn_addmask(netmask, 0, top->rn_off)) == 0)
			return (0);
		b_leaf = x->rn_b;
		b = -1 - x->rn_b;
		netmask = x->rn_key;
	}
	/*
	 * Deal with duplicated keys: attach node to previous instance
	 */
	saved_tt = tt = rn_insert(v, head, &keyduplicated, treenodes);
	if (keyduplicated) {
		for (t = tt; tt; t = tt, tt = tt->rn_dupedkey) {
			if (tt->rn_mask == netmask)
				return (0);
			if (netmask == 0 ||
			    (tt->rn_mask &&
			     ((b_leaf < tt->rn_b) || /* index(netmask) > node */
			       rn_refines(netmask, tt->rn_mask) ||
			       rn_lexobetter(netmask, tt->rn_mask))))
				break;
		}
		/*
		 * If the mask is not duplicated, we wouldn't
		 * find it among possible duplicate key entries
		 * anyway, so the above test doesn't hurt.
		 *
		 * We sort the masks for a duplicated key the same way as
		 * in a masklist -- most specific to least specific.
		 * This may require the unfortunate nuisance of relocating
		 * the head of the list.
		 */
		if (tt == saved_tt) {
			struct	radix_node *xx = x;
			/* link in at head of list */
			(tt = treenodes)->rn_dupedkey = t;
			tt->rn_flags = t->rn_flags;
			tt->rn_p = x = t->rn_p;
			t->rn_p = tt;				   /* cisco */
			if (x->rn_l == t) x->rn_l = tt; else x->rn_r = tt;
			saved_tt = tt; x = xx;
		} else {
			(tt = treenodes)->rn_dupedkey = t->rn_dupedkey;
			t->rn_dupedkey = tt;
			tt->rn_p = t;				   /* cisco */
			if (tt->rn_dupedkey)			   /* cisco */
			    tt->rn_dupedkey->rn_p = tt;		   /* cisco */
		}
#ifdef RN_DEBUG
		t=tt+1; tt->rn_info = rn_nodenum++; t->rn_info = rn_nodenum++;
		tt->rn_twin = t; tt->rn_ybro = rn_clist; rn_clist = tt;
#endif
		tt->rn_key = (caddr_t) v;
		tt->rn_b = -1;
		tt->rn_flags = RNF_ACTIVE;
	}
	/*
	 * Put mask in tree.
	 */
	if (netmask) {
		tt->rn_mask = netmask;
		tt->rn_b = x->rn_b;
		tt->rn_flags |= x->rn_flags & RNF_NORMAL;
	}
	t = saved_tt->rn_p;
	if (keyduplicated)
		goto on2;
	b_leaf = -1 - t->rn_b;
	if (t->rn_r == saved_tt) x = t->rn_l; else x = t->rn_r;
	/* Promote general routes from below */
	if (x->rn_b < 0) { 
	    for (mp = &t->rn_mklist; x; x = x->rn_dupedkey)
		if (x->rn_mask && (x->rn_b >= b_leaf) && x->rn_mklist == 0) {
			if ((*mp = m = rn_new_radix_mask(x, 0)))
				mp = &m->rm_mklist;
		}
	} else if (x->rn_mklist) {
		/*
		 * Skip over masks whose index is > that of new node
		 */
		for (mp = &x->rn_mklist; (m = *mp); mp = &m->rm_mklist)
			if (m->rm_b >= b_leaf)
				break;
		t->rn_mklist = m; *mp = 0;
	}
on2:
	/* Add new route to highest possible ancestor's list */
	if ((netmask == 0) || (b > t->rn_b ))
		return tt; /* can't lift at all */
	b_leaf = tt->rn_b;
	do {
		x = t;
		t = t->rn_p;
	} while (b <= t->rn_b && x != top);
	/*
	 * Search through routes associated with node to
	 * insert new route according to index.
	 * Need same criteria as when sorting dupedkeys to avoid
	 * double loop on deletion.
	 */
	for (mp = &x->rn_mklist; (m = *mp); mp = &m->rm_mklist) {
		if (m->rm_b < b_leaf)
			continue;
		if (m->rm_b > b_leaf)
			break;
		if (m->rm_flags & RNF_NORMAL) {
			mmask = m->rm_leaf->rn_mask;
			if (tt->rn_flags & RNF_NORMAL) {
				errmsg(&msgsym(ADDMASK, RADIX),
				   "non-unique normal route, mask not entered");
				return tt;
			}
		} else
			mmask = m->rm_mask;
		if (mmask == netmask) {
			m->rm_refs++;
			tt->rn_mklist = m;
			return tt;
		}
		if (rn_refines(netmask, mmask) || rn_lexobetter(netmask, mmask))
			break;
	}
	*mp = rn_new_radix_mask(tt, *mp);
	return tt;
}

struct radix_node *
rn_delete (void *v_arg, void *netmask_arg, struct radix_node_head *head)
{
	register struct radix_node *t, *p, *x, *tt;
	struct radix_mask *m, *saved_m, **mp;
	struct radix_node *dupedkey, *saved_tt, *top;
	caddr_t v, netmask;
	int b, head_off, vlen;

	v = v_arg;
	vlen =  *(u_char *)v;
	netmask = netmask_arg;
	x = head->rnh_treetop;
	tt = rn_search(v, x);
	head_off = x->rn_off;
	saved_tt = tt;
	top = x;
	if (tt == 0 ||
	    Bcmp(v + head_off, tt->rn_key + head_off, vlen - head_off))
		return (0);
	/*
	 * Delete our route from mask lists.
	 */
	if (netmask) {
		if ((x = rn_addmask(netmask, 1, head_off)) == 0)
			return (0);
		netmask = x->rn_key;
		while (tt->rn_mask != netmask)
			if ((tt = tt->rn_dupedkey) == 0)
				return (0);
	}
	if (tt->rn_mask == 0 || (saved_m = m = tt->rn_mklist) == 0)
		goto on1;
	if (tt->rn_flags & RNF_NORMAL) {
		if (m->rm_leaf != tt || m->rm_refs > 0) {
			errmsg(&msgsym(DELETE, RADIX), "inconsistent annotation");
			return 0;  /* dangling ref could cause disaster */
		}
	} else { 
		if (m->rm_mask != tt->rn_mask) {
			errmsg(&msgsym(DELETE, RADIX), "inconsistent annotation");
			goto on1;
		}
		if (--m->rm_refs >= 0)
			goto on1;
	}
	b = -1 - tt->rn_b;
	t = saved_tt->rn_p;
	if (b > t->rn_b)
		goto on1; /* Wasn't lifted at all */
	do {
		x = t;
		t = t->rn_p;
	} while (b <= t->rn_b && x != top);
	for (mp = &x->rn_mklist; (m = *mp); mp = &m->rm_mklist)
		if (m == saved_m) {
			*mp = m->rm_mklist;
			MKFree(m);
			break;
		}
	if (m == 0) {
		errmsg(&msgsym(DELETE, RADIX), "couldn't find our annotation");
		if (tt->rn_flags & RNF_NORMAL)
			return (0); /* Dangling ref to us */
	}
on1:
	/*
	 * Eliminate us from tree
	 */
	if (tt->rn_flags & RNF_ROOT)
		return (0);
#ifdef RN_DEBUG
	/* Get us out of the creation list */
	for (t = rn_clist; t && t->rn_ybro != tt; t = t->rn_ybro) {}
	if (t) t->rn_ybro = tt->rn_ybro;
#endif
	t = tt->rn_p;
	if ((dupedkey = saved_tt->rn_dupedkey)) {
		/*
		 * at this point, tt is the deletion target and
		 * saved_tt is the head of the dupekey chain
		 */
		if (tt == saved_tt) {
			/* remove from head of chain */
			x = dupedkey; x->rn_p = t;
			if (t->rn_l == tt) t->rn_l = x; else t->rn_r = x;
		} else {
			/* find node in front of tt on the chain */
			for (x = p = saved_tt; p && p->rn_dupedkey != tt;)
				p = p->rn_dupedkey;
			if (p) {
			    p->rn_dupedkey = tt->rn_dupedkey;
			    if (tt->rn_dupedkey)		/* cisco */
				tt->rn_dupedkey->rn_p = p;	/* cisco */
			} else errmsg(&msgsym(DELETE, RADIX),
				      "couldn't find deletion target");
		}
		t = tt + 1;
		if  (t->rn_flags & RNF_ACTIVE) {
#ifndef RN_DEBUG
			*++x = *t; p = t->rn_p;
#else
			b = t->rn_info; *++x = *t; t->rn_info = b; p = t->rn_p;
#endif
			if (p->rn_l == t) p->rn_l = x; else p->rn_r = x;
			x->rn_l->rn_p = x; x->rn_r->rn_p = x;
		}
		goto out;
	}
	if (t->rn_l == tt) x = t->rn_r; else x = t->rn_l;
	p = t->rn_p;
	if (p->rn_r == t) p->rn_r = x; else p->rn_l = x;
	x->rn_p = p;
	/*
	 * Demote routes attached to us.
	 */
	if (t->rn_mklist) {
		if (x->rn_b >= 0) {
			for (mp = &x->rn_mklist; (m = *mp);)
				mp = &m->rm_mklist;
			*mp = t->rn_mklist;
		} else {
			/* If there are any key,mask pairs in a sibling
			   duped-key chain, some subset will appear sorted
			   in the same order attached to our mklist */
			for (m = t->rn_mklist; m && x; x = x->rn_dupedkey)
				if (m == x->rn_mklist) {
					struct radix_mask *mm = m->rm_mklist;
					x->rn_mklist = 0;
					if (--(m->rm_refs) < 0)
						MKFree(m);
					m = mm;
				}
			if (m)
				errmsg(&msgsym(ORPHAN, RADIX),
					m, m ? m->rm_refs+1 : 0,
					x, x ? x->rn_mklist : 0);
		}
	}
	/*
	 * We may be holding an active internal node in the tree.
	 */
	x = tt + 1;
	if (t != x) {
#ifndef RN_DEBUG
		*t = *x;
#else
		b = t->rn_info; *t = *x; t->rn_info = b;
#endif
		t->rn_l->rn_p = t; t->rn_r->rn_p = t;
		p = x->rn_p;
		if (p->rn_l == x) p->rn_l = t; else p->rn_r = t;
	}
out:
	tt->rn_flags &= ~RNF_ACTIVE;
	tt[1].rn_flags &= ~RNF_ACTIVE;
	return (tt);
}

int
rn_walktree(register struct radix_node *rn, rn_walk_function f, ...)
{
	int error;
	struct radix_node *base, *next;
	va_list ap;

	va_start(ap, f);

	if (!rn) {
	    errmsg(&msgsym(BADTREE, RADIX), rn);
	    return 0;
	}

	/*
	 * This gets complicated because we may delete the node
	 * while applying the function f to it, so we need to calculate
	 * the successor node in advance.
	 */
	/* First time through node, go left */
	while (rn->rn_b >= 0)
		rn = rn->rn_l;
	for (;;) {
		base = rn;
		/* If at right child go back up, otherwise, go right */
		while (rn->rn_p->rn_r == rn && (rn->rn_flags & RNF_ROOT) == 0)
			rn = rn->rn_p;
		/* Find the next *leaf* since next node might vanish, too */
		for (rn = rn->rn_p->rn_r; rn->rn_b >= 0;)
			rn = rn->rn_l;
		next = rn;
		/* Process leaves */
		while ((rn = base)) {
			base = rn->rn_dupedkey;
			if (!(rn->rn_flags & RNF_ROOT) &&
			    (error = (*f)(rn, ap))) {
			        va_end(ap);
				return (error);
			}
		}
		rn = next;
		if (rn->rn_flags & RNF_ROOT) {
		        va_end(ap);
			return (0);
		}
	}
	/* NOTREACHED */
}

int
rn_inithead (void **head, int off)
{
	register struct radix_node_head *rnh;
	register struct radix_node *t, *tt, *ttt;

	if (!max_keylen)
		if (rn_init())
			return (1);

	if (*head)
		return (1);
	R_Malloc(rnh, struct radix_node_head *, sizeof (*rnh));
	if (rnh == 0)
		return (0);
	Bzero(rnh, sizeof (*rnh));
	*head = rnh;
	t = rn_newpair(rn_zeros, off, rnh->rnh_nodes);
	ttt = rnh->rnh_nodes + 2;
	t->rn_r = ttt;
	t->rn_p = t;
	tt = t->rn_l;
	tt->rn_flags = t->rn_flags = RNF_ROOT | RNF_ACTIVE;
	tt->rn_b = -1 - off;
	*ttt = *tt;
	ttt->rn_key = rn_ones;
#ifdef	RADIX_HEAD_VECTORS
	rnh->rnh_addaddr = rn_addroute;
	rnh->rnh_deladdr = rn_delete;
	rnh->rnh_matchaddr = rn_match;
	rnh->rnh_lookup = rn_lookup;
	rnh->rnh_walktree = rn_walktree;
#endif	RADIX_HEAD_VECTORS
	rnh->rnh_treetop = t;
	return (1);
}

static int
rn_init(void)
{
	char *cp, *cplim;

	max_keylen = RN_MAX_KEYLEN;

	R_Malloc(rn_zeros, char *, 3 * max_keylen);
	if (rn_zeros == NULL) {
		errmsg(&msgsym(INIT, RADIX), "initializing leaf nodes");
		return 1;			/* cisco */
	}
	Bzero(rn_zeros, 3 * max_keylen);
	rn_ones = cp = rn_zeros + max_keylen;
	addmask_key = cplim = rn_ones + max_keylen;
	while (cp < cplim)
		*cp++ = -1;

	if (rn_inithead((void **)&mask_rnhead, 0) == 0) {
		errmsg(&msgsym(INIT, RADIX), "mask housekeeping");
		return 1;
	}

	return 0;
}

/*
 * all code after this point is cisco code
 */

/*
 * Mark all parents up to the root with the specified verion number
 *
 * NOTE: don't be tempted to use a MAX here, we explicitly want to set
 * the version number because in a number of cases, we use MAXULONG to
 * avoid race conditions between version marking and tree traversal.
 *
 */
void
rn_mark_parents (register struct radix_node *rn, register u_long version)
{
    while (rn && (rn->rn_p != rn)) {
	rn->rn_version = version;
	rn = rn->rn_p;
    }
    if (rn)
	rn->rn_version = version;
}

/*
 * Helper functions for rn_walktree_blocking to lock memory as appropriate.
 * We never lock root nodes because they're allocated out of a different
 * structure.
 */

static inline void
rn_lock (struct radix_node *rn)
{
    if (rn && !(rn->rn_flags & RNF_ROOT))
	mem_lock(rn);
}

static inline void
rn_free (struct radix_node *rn)
{
    if (rn && !(rn->rn_flags & RNF_ROOT))
	free(rn);
}

/*
 * rn_walktree_blocking_inline
 *
 * This version of rn_walktree will lock down all data structures that
 * we are using and check to see that they are still active before
 * using them.
 */

static inline int
rn_walktree_blocking_inline (struct radix_node *rn, rn_walk_function f,
			   va_list ap)
{
    int error;
    struct radix_node *next_leaf, *next_dupekey;

    if (!rn) {
	errmsg(&msgsym(BADTREE, RADIX), rn);
	return 0;
    }

    /*
     * First time through tree, go left
     */
    while (rn->rn_b >= 0)
	rn = rn->rn_l;

    do {
	next_leaf = rn_nextleaf(rn);
	rn_lock(next_leaf);

	/* Process nodes in dupedkey linked list */
	while (rn) {
	    next_dupekey = rn->rn_dupedkey;

	    if (!(rn->rn_flags & RNF_ROOT)) {
		rn_lock(next_dupekey);

		rn_lock(rn);
		error = (*f)(rn, ap);
		rn_free(rn);
		
		if (error) {
		    rn_free(next_dupekey);
		    rn_free(next_leaf);
		    return (error);
		}

		/*
		 * check and see if the link to next dupekey is still valid
		 * if not, abort processing
		 */
		if (next_dupekey) {
		    if (!(next_dupekey->rn_flags & RNF_ACTIVE)) {
			rn_free(next_dupekey);
			rn_free(next_leaf);
			return (-1);
		    }
		    rn_free(next_dupekey);
		}
	    }

	    rn = next_dupekey;
	}

	/*
	 * check and see if next leaf is still valid
	 * if not, abort processing 
	 */
	if (!(next_leaf->rn_flags & RNF_ACTIVE)) {
	    rn_free(next_leaf);
	    return (-1);
	}
	rn_free(next_leaf);
	rn = next_leaf;

    } while (!(rn->rn_flags & RNF_ROOT));

    return(0);
}
/*
 * rn_walktree_blocking_list
 * wrapper for rn_walktree_blocking_inline, where the varargs have already
 * been assembled into a single argument by the calling routine.
 */

int
rn_walktree_blocking_list (struct radix_node *rn, rn_walk_function f,
			   va_list ap)
{
    return(rn_walktree_blocking_inline(rn, f, ap));
}
/*
 * rn_walktree_blocking
 * varargs wrapper for rn_walktree_blocking_inline
 */

int
rn_walktree_blocking (struct radix_node *rn, rn_walk_function f, ...)
{
    int status;
    va_list ap;

    va_start(ap, f);
    status = rn_walktree_blocking_inline(rn, f, ap);
    va_end(ap);
    return(status);
}
/*
 * rn_walktree_timed
 *
 * This version of rn_walktree will run without locking until we realize
 * that our time is up,  at which point we'll lock down the structures
 * we need for dealing with the rest of the tree and suspend.
 *
 * WARNING: the called function MAY NOT BLOCK
 * WARNING: this overall walker does block.
 *
 */
int
rn_walktree_timed (struct radix_node_head *head,
		   rn_walk_function walker,
		   rn_succ_function nextnode, ...)
{
    int error;
    struct radix_node *rn, *next_leaf, *next_dupekey;
    va_list ap;

    va_start(ap, nextnode);

    if (!head) {
	errmsg(&msgsym(BADTREE, RADIX), head);
	return 0;
    }

    rn = head->rnh_treetop;

    /*
     * First time through tree, go left
     */
    while (rn->rn_b >= 0)
	rn = rn->rn_l;

    for (;;) {
	next_leaf = rn_nextleaf(rn);

	/* Process nodes in dupedkey linked list */
	for (; rn; rn = next_dupekey) {
	    next_dupekey = rn->rn_dupedkey;
	    if (!(rn->rn_flags & RNF_ROOT)) {
		error = (*walker)(rn, ap);
		if (error) {
		    va_end(ap);
		    return (error);
		}
	    }
	}

	/*
	 * rn may be no longer valid, overwrite it right now so no one is
	 * tempted to use it.
	 */

	rn = next_leaf;

	if (rn->rn_flags & RNF_ROOT) {
	    va_end(ap);
	    return 0;
	}

	/*
	 * If we've run more than our allocated period, stop traversing
	 * the tree... save what would have been our next node, and give
	 * up the CPU.
	 *
	 * If, while we were gone, that next node was deleted, we need to
	 * find the successor to our current node (which already was the
	 * next leaf).
	 *
	 * First try to search down the tree towards a match.  If there's
	 * nothing there, then start at the left most node in the entire
	 * tree and linearly traverse until we get to the right spot (ugly).
	 */
	if (process_time_exceeded()) {
	    mem_lock(rn);
	    process_suspend();
	    if (rn->rn_flags & RNF_ACTIVE)
		free(rn);
	    else {
		/*
		 * Start at top of tree, search down to left-most node,
		 * skip over it, since it will be the all zero's root,
		 * and call the successor function.
		 */
		next_leaf = head->rnh_treetop;

		while (next_leaf->rn_b >= 0)
		    next_leaf = next_leaf->rn_l;

		next_leaf = rn_nextleaf(next_leaf);

		next_leaf = (*nextnode)(next_leaf, rn->rn_key);
		free(rn);
		rn = next_leaf;
	    }
	}
    }
}

/*
 * rn_walktree_version
 *
 * This version of rn_walktree will run without locking until we realize
 * that our time is up,  at which point we'll lock down the structures
 * we need for dealing with the rest of the tree and suspend.
 *
 * WARNING: the called function MAY NOT BLOCK
 * WARNING: this overall walker does block.
 *
 */
int
rn_walktree_version (struct radix_node_head *head,
		     u_long version,
		     rn_walk_function walker,
		     rn_succ_ver_function nextnode, ...)
{
    int error;
    struct radix_node *rn, *next_leaf, *next_dupekey;
    va_list ap;

    va_start(ap, nextnode);

    if (!head) {
	errmsg(&msgsym(BADTREE, RADIX), head);
	return 0;
    }

    rn = head->rnh_treetop;

    /*
     * First time through tree, go left
     */
    while (rn->rn_b >= 0) {
	if (rn->rn_l->rn_version < version)
	    rn = rn->rn_r;
	else
	    rn = rn->rn_l;
    }

    for (;;) {
	next_leaf = rn_nextleaf_version(rn, version);

	/* Process nodes in dupedkey linked list */
	for (; rn; rn = next_dupekey) {
	    next_dupekey = rn->rn_dupedkey;
	    if (!(rn->rn_flags & RNF_ROOT)) {
		error = (*walker)(rn, ap);
		if (error) {
		    va_end(ap);
		    return (error);
		}
	    }
	}

	/*
	 * rn may be no longer valid, overwrite it right now so no one is
	 * tempted to use it.
	 */

	rn = next_leaf;

	if (rn->rn_flags & RNF_ROOT) {
	    va_end(ap);
	    return 0;
	}

	/*
	 * If we've run more than our allocated period, stop traversing
	 * the tree... save what would have been our next node, and give
	 * up the CPU.
	 *
	 * If, while we were gone, that next node was deleted, we need to
	 * find the successor to our current node (which already was the
	 * next leaf).
	 *
	 * First try to search down the tree towards a match.  If there's
	 * nothing there, then start at the left most node in the entire
	 * tree and linearly traverse until we get to the right spot (ugly).
	 */
	if (process_time_exceeded()) {
	    mem_lock(rn);
	    process_suspend();
	    if (rn->rn_flags & RNF_ACTIVE)
		free(rn);
	    else {
		/*
		 * Start at top of tree, search down to left-most node,
		 * skip over it, since it will be the all zero's root,
		 * and call the successor function.
		 */
		next_leaf = head->rnh_treetop;

		while (next_leaf->rn_b >= 0)
		    if (next_leaf->rn_l->rn_version < version)
			next_leaf = next_leaf->rn_r;
		    else
			next_leaf = next_leaf->rn_l;

		/*
		 * If we're at the left-most root node, walk one leaf
		 * forward before starting our search since the nextnode
		 * function terminates processing when it reaches a root.
		 */
		if ((next_leaf->rn_flags & RNF_ROOT) &&
		    (next_leaf->rn_p->rn_l == next_leaf))
		    next_leaf = rn_nextleaf(next_leaf);

		next_leaf = (*nextnode)(next_leaf, version, rn->rn_key);
		free(rn);
		rn = next_leaf;
	    }
	}
    }
}

#ifdef	RADIX_DEBUG

void radix_stats (struct radix_node_head *head, int *retleaves,
		  int *retmaxlevel, int *retavglevel,
		  int *retdupes, int *retmaxdupe)
{
	int level, dupelen, leaves, dupes;
	int maxlevel, maxdupe, avglevel;
	struct radix_node *base, *next, *rn;

	rn = head->rnh_treetop;
	level = leaves = dupes = maxlevel = maxdupe = avglevel = 0;

	/*
	 * This gets complicated because we may delete the node
	 * while applying the function f to it, so we need to calculate
	 * the successor node in advance.
	 */
	/* First time through node, go left */
	while (rn->rn_b >= 0) {
		rn = rn->rn_l;
		level++;
	}
	for (;;) {
		base = rn;

		leaves++;
		avglevel += level;
		if (level > maxlevel) maxlevel = level;

		/* If at right child go back up, otherwise, go right */
		while (rn->rn_p->rn_r == rn && (rn->rn_flags & RNF_ROOT) == 0) {
			rn = rn->rn_p;
			level--;
		}
		/* Find the next *leaf* since next node might vanish, too */
		for (rn = rn->rn_p->rn_r; rn->rn_b >= 0;) {
			rn = rn->rn_l;
			level++;
		}
		next = rn;
		dupelen = -1;
		/* Process leaves */
		while ((rn = base)) {
			base = rn->rn_dupedkey;
			dupelen++;
		}
		dupes += dupelen;
		if (dupelen > maxdupe) maxdupe = dupelen;
		rn = next;
		if (rn->rn_flags & RNF_ROOT) {
			*retleaves   = leaves;
			*retmaxlevel = maxlevel;
			*retavglevel = avglevel / leaves;
			*retdupes    = dupes;
			*retmaxdupe  = maxdupe;
			return;
		}
	}
	/* NOTREACHED */
}

#endif	RADIX_DEBUG
