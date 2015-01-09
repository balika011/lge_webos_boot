/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-
 * Copyright (c) 1986, 1988, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)subr_prf.c	8.3 (Berkeley) 1/21/94
 * $FreeBSD: src/lib/libstand/printf.c,v 1.2.2.1 1999/08/29 14:57:34 peter Exp $
 */

/*
 * Standaloneified version of the FreeBSD kernel printf family.
 */

#include "x_lint.h"

LINT_SAVE_AND_DISABLE   // Do not lint this file

#include "x_serial.h"
#include "x_assert.h"
#include "ddr.h"
#include "u_common.h"

#ifndef CC_MTK_LOADER
#ifdef DRV_DBG_LOG_TO_FILE 
// #include "u_common.h"
#endif
#endif

#define NBBY        (8)

#if defined(DRAM_DEBUG) || !defined(CC_ROM_BOOT) || !defined(CC_MTK_PRELOADER)

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned char u_char;

static char	*ksprintn (u_long num, int base, int *len, char *buf);
static int	kvprintf(char const *fmt, VOID (*func)(CHAR), void *arg, int radix, VA_LIST ap);

void vprintf(const char *fmt, VA_LIST ap);
void vsprintf(char *buf, const char *cfmt, VA_LIST ap);

EXTERN VOID outbyte(CHAR c);
/* assuming sizeof(void *) == sizeof(int) */
#ifndef CC_MTK_LOADER
#ifdef DRV_DBG_LOG_TO_FILE 
EXTERN INT32 x_log_to_drv_file_query(CHAR** ppBuffer,UINT32 *p_ui4Len);
EXTERN VOID x_log_to_drv_file_output_fct(CONST CHAR* ps_stmt);
EXTERN VOID *x_memset(VOID *s, INT32 c, UINT32 n);
EXTERN INT32 x_vsnprintf (CHAR*        ps_str,
                          SIZE_T       z_size,
                          CONST CHAR*  ps_format,
                          VA_LIST      VA_LIST);
#endif
#endif

const CHAR hex2ascii_array[] = {    '0', '1', '2', '3', '4', '5', '6', '7', 
                                    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

#define hex2ascii(h)    (hex2ascii_array[h])

INT32
Printf(const CHAR *fmt, ...)
{
	VA_LIST t_ap;
	int retval;

    #ifndef CC_MTK_LOADER
    #ifdef DRV_DBG_LOG_TO_FILE 
    register INT32 *varg = (INT32 *)(&fmt);
    CHAR *p = NULL;
    UINT32 ui4_len;

    varg = varg;
    if (x_log_to_drv_file_query(&p, &ui4_len))
    {
        VA_START(t_ap, fmt);
        x_memset(p, 0, ui4_len);
        retval = x_vsnprintf(p, ui4_len-1, fmt, t_ap);
        //nRet = print(&p, varg);
        x_log_to_drv_file_output_fct(p);
        VA_END(t_ap);    
    }
    else
    #endif
    #endif
    {
        VA_START(t_ap, fmt);
	    retval = kvprintf(fmt, outbyte, NULL, 10, t_ap);
        VA_END(t_ap);
    }
	return retval;
}

void
vprintf(const char *fmt, VA_LIST ap)
{
	kvprintf(fmt, outbyte, NULL, 10, ap);
}

INT32
SPrintf(CHAR *buf, const CHAR *cfmt, ...)
{
	int retval;
	VA_LIST ap;

	VA_START(ap, cfmt);
	retval = kvprintf(cfmt, NULL, (void *)buf, 10, ap);
	buf[retval] = '\0';
	VA_END(ap);
	return retval;
}

void
vsprintf(char *buf, const char *cfmt, VA_LIST ap)
{
	int	retval;
	
	retval = kvprintf(cfmt, NULL, (void *)buf, 10, ap);
	buf[retval] = '\0';
}

/*
 * Put a number (base <= 16) in a buffer in reverse order; return an
 * optional length and a pointer to the NULL terminated (preceded?)
 * buffer.
 */
static char *
ksprintn(u_long ul, int base, int *lenp, char *buf)
{					/* A long in base 8, plus NULL. */
/*	char buf[sizeof(long) * NBBY / 3 + 2]; */
	register char *p;

	p = buf;
	do {
		*++p = hex2ascii(ul % base);
	} while (ul /= base);
	if (lenp)
		*lenp = p - buf;
	return (p);
}

/*
 * Scaled down version of printf(3).
 *
 * Two additional formats:
 *
 * The format %b is supported to decode error registers.
 * Its usage is:
 *
 *	printf("reg=%b\n", regval, "<base><arg>*");
 *
 * where <base> is the output base expressed as a control character, e.g.
 * \10 gives octal; \20 gives hex.  Each arg is a sequence of characters,
 * the first of which gives the bit number to be inspected (origin 1), and
 * the next characters (up to a control character, i.e. a character <= 32),
 * give the name of the register.  Thus:
 *
 *	kvprintf("reg=%b\n", 3, "\10\2BITTWO\1BITONE\n");
 *
 * would produce output:
 *
 *	reg=3<BITTWO,BITONE>
 *
 * XXX:  %D  -- Hexdump, takes pointer and separator string:
 *		("%6D", ptr, ":")   -> XX:XX:XX:XX:XX:XX
 *		("%*D", len, ptr, " " -> XX XX XX XX ...
 */
static int
kvprintf(char const *fmt, VOID (*func)(CHAR), void *arg, int radix, VA_LIST ap)
{
#define PCHAR(c) { if (func) (*func)(c); else *d++ = c; retval++; }
	char buf[sizeof(long) * NBBY / 3 + 2] = {0};
	char *p, *q, *d;
	u_char *up;
	int ch, n;
	u_long ul;
	int base, lflag, tmp, width, ladjust, sharpflag, neg, sign, dot;
	int dwidth;
	char padc;
	int retval = 0;

	if (!func)
		d = (char *) arg;
	else
		d = NULL;

	if (fmt == NULL)
		fmt = "(fmt null)\n";

	if (radix < 2 || radix > 36)
		radix = 10;

	for (;;) {
		padc = ' ';
		width = 0;
		ch = (u_char)*fmt++;
		while ((ch) != '%') {
			if (ch == '\0') 
				return retval;
			PCHAR(ch);
		    ch = (u_char)*fmt++;
		}
		lflag = 0; ladjust = 0; sharpflag = 0; neg = 0;
		sign = 0; dot = 0; dwidth = 0;
reswitch:
        ch = (u_char)*fmt++;
        switch (ch) {
		case '.':
			dot = 1;
			goto reswitch;
		case '#':
			sharpflag = 1;
			goto reswitch;
		case '+':
			sign = 1;
			goto reswitch;
		case '-':
			ladjust = 1;
			goto reswitch;
		case '%':
			PCHAR(ch);
			break;
		case '*':
			if (!dot) {
				width = VA_ARG(ap, int);
				if (width < 0) {
					ladjust = !ladjust;
					width = -width;
				}
			} else {
				dwidth = VA_ARG(ap, int);
			}
			goto reswitch;
		case '0':
			if (!dot) {
				padc = '0';
				goto reswitch;
			}
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
				for (n = 0;; ++fmt) {
					n = n * 10 + ch - '0';
					ch = *fmt;
					if (ch < '0' || ch > '9')
						break;
				}
			if (dot)
				dwidth = n;
			else
				width = n;
			goto reswitch;
		case 'b':
			ul = VA_ARG(ap, int);
			p = VA_ARG(ap, char *);
			for (q = ksprintn(ul, *p++, NULL, &buf[0]); *q;)
				PCHAR(*q--);

			if (!ul)
				break;

			for (tmp = 0; *p;) {
				n = *p++;
				if (ul & (1 << (n - 1))) {
					PCHAR(tmp ? ',' : '<');
					for (; (n = *p) > ' '; ++p)
						PCHAR(n);
					tmp = 1;
				} else
					for (; *p > ' '; ++p)
						continue;
			}
			if (tmp)
				PCHAR('>');
			break;
		case 'c':
			PCHAR(VA_ARG(ap, int));
			break;
		case 'D':
			up = VA_ARG(ap, u_char *);
			p = VA_ARG(ap, char *);
			if (!width)
				width = 16;
			while(width--) {
				PCHAR(hex2ascii(*up >> 4));
				PCHAR(hex2ascii(*up & 0x0f));
				up++;
				if (width)
					for (q=p;*q;q++)
						PCHAR(*q);
			}
			break;
		case 'd':
			ul = lflag ? VA_ARG(ap, long) : VA_ARG(ap, int);
			sign = 1;
			base = 10;
			goto number;
		case 'l':
			lflag = 1;
			goto reswitch;
		case 'n':
			ul = lflag ? VA_ARG(ap, u_long) : VA_ARG(ap, u_int);
			base = radix;
			goto number;
		case 'o':
			ul = lflag ? VA_ARG(ap, u_long) : VA_ARG(ap, u_int);
			base = 8;
			goto number;
		case 'p':
			ul = (u_long)VA_ARG(ap, void *);
			base = 16;
			sharpflag = 1;
			goto number;
		case 's':
			p = VA_ARG(ap, char *);
			if (p == NULL)
				p = "(null)";
			if (!dot) {
                register INT32 len = 0;
                register const CHAR *ptr;
                for (ptr = p; *ptr; ++ptr) ++len;
				// n = x_strlen (p);
                n = len;
			} else
				for (n = 0; n < dwidth && p[n]; n++)
					continue;

			width -= n;

			if (!ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			while (n--)
				PCHAR(*p++);
			if (ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			break;
		case 'u':
			ul = lflag ? VA_ARG(ap, u_long) : VA_ARG(ap, u_int);
			base = 10;
			goto number;
		case 'x':
		case 'X':
			ul = lflag ? VA_ARG(ap, u_long) : VA_ARG(ap, u_int);
			base = 16;
number:			if (sign && (long)ul < 0L) {
				neg = 1;
				ul = -(long)ul;
			}
			p = ksprintn(ul, base, &tmp, &buf[0]);
			if (sharpflag && ul != 0) {
				if (base == 8)
					tmp++;
				else if (base == 16)
					tmp += 2;
			}
			if (neg)
				tmp++;

			if (!ladjust && width && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);
			if (neg)
				PCHAR('-');
			if (sharpflag && ul != 0) {
				if (base == 8) {
					PCHAR('0');
				} else if (base == 16) {
					PCHAR('0');
					PCHAR('x');
				}
			}

			while (*p)
				PCHAR(*p--);

			if (ladjust && width && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);

			break;
		default:
			PCHAR('%');
			if (lflag)
				PCHAR('l');
			PCHAR(ch);
			break;
		}
	}
#undef PCHAR
}

LINT_RESTORE

#endif //#if !defined(CC_ROM_BOOT) && !defined(CC_MTK_PRELOADER)

