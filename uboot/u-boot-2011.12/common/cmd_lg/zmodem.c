
#include <common.h>
#include <zmodem.h>

#define OK       0
#define FALSE    0
#ifndef TRUE
#define TRUE     1
#endif

struct zm_fileinfo {
	char  *fname;
	void  *addr;
	time_t modtime;
	mode_t mode;
	size_t bytes_total;
	size_t bytes_received;
	size_t bytes_skipped; // crash recovery
        char   *seek_adr;
	int    eof_seen;
};

extern const unsigned short crctab[256];
extern const long cr3tab[];
#define UPDC32(b, c) (cr3tab[((int)c ^ b) & 0xff] ^ ((c >> 8) & 0x00FFFFFF))

#define Zrwindow 1400	// RX window size (controls garbage count)

#define READLINE_PF(timeout)	getc()


#define RCDO (-3)
#define TIMEOUT (-2)
#define ERROR (-1)


#define XON ('q'&037)
#define XOFF ('s'&037)

#define DEFBYTL	2000000000L	// default rx file size

#define _CAN ('X'&037)


#define UNIXFILE 0xF000	// The S_IFMT file mask bit for stat


enum zm_type_enum {
	ZM_XMODEM,
	ZM_YMODEM,
	ZM_ZMODEM
};

//******************************************************************************
//
// 광역 변수 정의
//
//******************************************************************************
static int zmodem_requested = FALSE;
static int Zctlesc;	              // Encode control characters
static char Txhdr[4];	              // Transmitted header
static char Rxhdr[4];	              // Received header
const char *digits = "0123456789abcdef";
static int Crc32t;	              // Display flag indicating 32 bit CRC being sent

static int tryzhdrtype = ZRINIT;      // Header type to send corresponding to Last rx close

unsigned ZModem_Baudrate = 115200;

static int Rxframeind;	             // ZBIN ZBIN32, or ZHEX type of frame received
static int Rxtype;	             // Type of header received

static int Crc32;	             // Display flag indicating 32 bit CRC being received

static char zconv;	             // ZMODEM file conversion request
static char zmanag;	             // ZMODEM file management request
static char ztrans;	             // ZMODEM file transport request

static int skip_if_not_found;

#define MAX_BLOCK 8192
static char secbuf[MAX_BLOCK + 1];
static char Attn[ZATTNLEN+1];	     // Attention string rx sends to tx on err

//
//  Crc calculation stuff
//

// crctab calculated by Mark G. Mendel, Network Systems Corporation
const unsigned short crctab[256] = {
    0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
    0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
    0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
    0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
    0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
    0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
    0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
    0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
    0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
    0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
    0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
    0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
    0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
    0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
    0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
    0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
    0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
    0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
    0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
    0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
    0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
    0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
    0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
    0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
    0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
    0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
    0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
    0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
    0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
    0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
    0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
    0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

//
//  updcrc macro derived from article Copyright (C) 1986 Stephen Satchell.
//   NOTE: First srgument must be in range 0 to 255.
//         Second argument is referenced twice.
//
//  Programmers may incorporate any or all code into their programs,
//  giving proper credit within the source. Publication of the
//  source routines is permitted so long as proper credit is given
//  to Stephen Satchell, Satchell Evaluations and Chuck Forsberg,
//  Omen Technology.
//

#define updcrc(cp, crc) ( crctab[((crc >> 8) & 255)] ^ (crc << 8) ^ cp)
//
//  Copyright (C) 1986 Gary S. Brown.  You may use this program, or
//  code or tables extracted from it, as desired without restriction.
//

// First, the polynomial itself and its table of feedback terms.  The
// polynomial is
// X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
//  /* Note that we take it "backwards" and put the highest-order term in
// the lowest-order bit.  The X^32 term is "implied"; the LSB is the
// X^31 term, etc.  The X^0 term (usually shown as "+1") results in
// the MSB being 1.

// Note that the usual hardware shift register implementation, which
// is what we're using (we're merely optimizing it by doing eight-bit
// chunks at a time) shifts bits into the lowest-order term.  In our
// implementation, that means shifting towards the right.  Why do we
// do it this way?  Because the calculated CRC must be transmitted in
// order from highest-order term to lowest-order term.  UARTs transmit
// characters in order from LSB to MSB.  By storing the CRC this way,
// we hand it to the UART in the order low-byte to high-byte; the UART
// sends each low-bit to hight-bit; and the result is transmission bit
// by bit from highest- to lowest-order term without requiring any bit
// shuffling on our part.  Reception works similarly.

// The feedback terms table consists of 256, 32-bit entries.  Notes:
//
//     The table can be generated at runtime if desired; code to do so
//     is shown later.  It might not be obvious, but the feedback
//     terms simply represent the results of eight shift/xor opera-
//     tions for all combinations of data and CRC register values.
//
//     The values must be right-shifted by eight bits by the "updcrc"
//     logic; the shift must be unsigned (bring in zeroes).  On some
//     hardware you could probably optimize the shift in assembler by
//     using byte-swap instructions.

const long cr3tab[] = { // CRC polynomial 0xedb88320
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


// End of crctab.c
static int SerialOutStr( char *str, int size )
{
	int	lp ;

	for (lp=0;lp<size;lp++) serial_putc(str[lp]) ;

	return lp;
}

//------------------------------------------------------------------------------
// 설명 : Store pos in Txhdr
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static void stohdr(size_t pos)
{
	long lpos=(long) pos;
	Txhdr[ZP0] = lpos;
	Txhdr[ZP1] = lpos>>8;
	Txhdr[ZP2] = lpos>>16;
	Txhdr[ZP3] = lpos>>24;
}

//------------------------------------------------------------------------------
// 설명 : Write a byte as two hex digits
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline void zputhex(int c, char *pos)
{
	pos[0] = digits[(c&0xF0)>>4];
	pos[1] = digits[c&0x0F];
}


//------------------------------------------------------------------------------
// 설명 : Send ZMODEM HEX header hdr of type type
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static void zshhdr(int type, char *hdr)
{
	register int n;
	register unsigned short crc;
	char s[30];
	size_t len;

	s[0]=ZPAD;
	s[1]=ZPAD;
	s[2]=ZDLE;
	s[3]=ZHEX;
	zputhex(type & 0x7f, s+4);
	len=6;
	Crc32t = 0;

	crc = updcrc((type & 0x7f), 0);
	for (n = 4; --n >= 0; ++hdr) {
		zputhex(*hdr, s+len);
		len += 2;
		crc = updcrc((0377 & *hdr), crc);
	}
	crc = updcrc(0,updcrc(0,crc));
	zputhex(crc>>8,s+len);
	zputhex(crc,s+len+2);
	len+=4;

	// Make it printable on remote machine
	s[len++]=015;
	s[len++]=0212;
	//
	// Uncork the remote in case a fake XOFF has stopped data flow
	//
	if (type != ZFIN && type != ZACK)
	{
		s[len++]=021;
	}

        SerialOutStr(s, len);

}

//------------------------------------------------------------------------------
// 설명 : Read a character from the modem line with timeout.
//        Eat parity, XON and XOFF characters.
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static int noxrd7(void)
{
	register int c;

	for (;;) {
		if ((c = READLINE_PF(Rxtimeout)) < 0)
			return c;
		switch (c &= 0177) {
		case XON:
		case XOFF:
			continue;
		default:
			if (Zctlesc && !(c & 0140))
				continue;
		case '\r':
		case '\n':
		case ZDLE:
			return c;
		}
	}
}

//------------------------------------------------------------------------------
// 설명 : no, i don't like gotos. -- uwe
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int zdlread2(int c)
{
	goto jump_over; // bad style

again:
	// Quick check for non control characters
	if ((c = READLINE_PF(Rxtimeout)) & 0140)
		return c;
jump_over:
	switch (c) {
	case ZDLE:
		break;
	case XON:
	case (XON|0200):
	case XOFF:
	case (XOFF|0200):
		goto again;
	default:
		if (Zctlesc && !(c & 0140)) {
			goto again;
		}
		return c;
	}
again2:
	if ((c = READLINE_PF(Rxtimeout)) < 0)
		return c;
	if (c == _CAN && (c = READLINE_PF(Rxtimeout)) < 0)
		return c;
	if (c == _CAN && (c = READLINE_PF(Rxtimeout)) < 0)
		return c;
	if (c == _CAN && (c = READLINE_PF(Rxtimeout)) < 0)
		return c;
	switch (c) {
	case _CAN:
		return GOTCAN;
	case ZCRCE:
	case ZCRCG:
	case ZCRCQ:
	case ZCRCW:
		return (c | GOTOR);
	case ZRUB0:
		return 0177;
	case ZRUB1:
		return 0377;
	case XON:
	case (XON|0200):
	case XOFF:
	case (XOFF|0200):
		goto again2;
	default:
		if (Zctlesc && ! (c & 0140)) {
			goto again2;
		}
		if ((c & 0140) ==  0100)
			return (c ^ 0100);
		break;
	}
	return ERROR;
}

static int zdlread(void)
{
	int c;
	// Quick check for non control characters
	if ((c = READLINE_PF(Rxtimeout)) & 0140)
		return c;
	return zdlread2(c);
}

//------------------------------------------------------------------------------
// 설명 : Receive a binary style header (type and position)
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int zrbhdr(char *hdr)
{
	register int c, n;
	register unsigned short crc;

	if ((c = zdlread()) & ~0377)
		return c;
	Rxtype = c;
	crc = updcrc(c, 0);

	for (n=4; --n >= 0; ++hdr) {
		if ((c = zdlread()) & ~0377)
			return c;
		crc = updcrc(c, crc);
		*hdr = c;
	}
	if ((c = zdlread()) & ~0377)
		return c;
	crc = updcrc(c, crc);
	if ((c = zdlread()) & ~0377)
		return c;
	crc = updcrc(c, crc);
	if (crc & 0xFFFF) {
		return ERROR;
	}
	zmodem_requested = TRUE;
	return Rxtype;
}

//------------------------------------------------------------------------------
// 설명 : Receive a binary style header (type and position) with 32 bit FCS
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int zrbhdr32(char *hdr)
{
	register int c, n;
	register unsigned long crc;

	if ((c = zdlread()) & ~0377)
		return c;
	Rxtype = c;
	crc = 0xFFFFFFFFL; crc = UPDC32(c, crc);

	for (n=4; --n >= 0; ++hdr) {
		if ((c = zdlread()) & ~0377)
			return c;
		crc = UPDC32(c, crc);
		*hdr = c;
	}
	for (n=4; --n >= 0;) {
		if ((c = zdlread()) & ~0377)
			return c;
		crc = UPDC32(c, crc);
	}
	if (crc != 0xDEBB20E3) {
		return ERROR;
	}
	zmodem_requested = TRUE;
	return Rxtype;
}

//------------------------------------------------------------------------------
// 설명 : Decode two lower case hex digits into an 8 bit byte value
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int zgeth1(void)
{
	register int c, n;

	if ((c = noxrd7()) < 0)
		return c;
	n = c - '0';
	if (n > 9)
		n -= ('a' - ':');
	if (n & ~0xF)
		return ERROR;
	if ((c = noxrd7()) < 0)
		return c;
	c -= '0';
	if (c > 9)
		c -= ('a' - ':');
	if (c & ~0xF)
		return ERROR;
	c += (n<<4);
	return c;
}

static int zgethex(void)
{
	return zgeth1();
}
//------------------------------------------------------------------------------
// 설명 : Receive a hex style header (type and position)
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int zrhhdr(char *hdr)
{
	register int c;
	register unsigned short crc;
	register int n;

	if ((c = zgethex()) < 0)
		return c;
	Rxtype = c;
	crc = updcrc(c, 0);

	for (n=4; --n >= 0; ++hdr) {
		if ((c = zgethex()) < 0)
			return c;
		crc = updcrc(c, crc);
		*hdr = c;
	}
	if ((c = zgethex()) < 0)
		return c;
	crc = updcrc(c, crc);
	if ((c = zgethex()) < 0)
		return c;
	crc = updcrc(c, crc);
	if (crc & 0xFFFF) {
		return ERROR;
	}
	switch ( c = READLINE_PF(1)) {
	case 0215:
		// **** FALL THRU TO ****
	case 015:
	 	// Throw away possible cr/lf
		READLINE_PF(1);
		break;
	}
	zmodem_requested = TRUE;
	return Rxtype;
}




//------------------------------------------------------------------------------
// 설명 : Read a ZMODEM header to hdr, either binary or hex.
//         eflag controls local display of non zmodem characters:
//        0:  no display
//        1:  display printing characters only
//        2:  display all non ZMODEM characters
//         On success, set Zmodem to 1, set Rxpos and return type of header.
//          Otherwise return negative on error.
//          Return ERROR instantly if ZCRCW sequence, for fast error recovery.
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static int zgethdr(char *hdr, int eflag, size_t *Rxpos)
{
	register int c, cancount;
	unsigned int max_garbage; // Max bytes before start of frame
	size_t rxpos = 0; // keep gcc happy

	max_garbage = Zrwindow + ZModem_Baudrate;
	Rxframeind = Rxtype = 0;

startover:
	cancount = 5;
again:
	// Return immediate ERROR if ZCRCW sequence seen
	switch (c = READLINE_PF(Rxtimeout)) {
	case RCDO:
	case TIMEOUT:
		goto fifi;
	case _CAN:
gotcan:
		if (--cancount <= 0) {
			c = ZCAN; goto fifi;
		}
		switch (c = READLINE_PF(1)) {
		case TIMEOUT:
			goto again;
		case ZCRCW:
			c = ERROR;
		// **** FALL THRU TO ****
		case RCDO:
			goto fifi;
		default:
			break;
		case _CAN:
			if (--cancount <= 0) {
				c = ZCAN; goto fifi;
			}
			goto again;
		}
	// **** FALL THRU TO ****
	default:
agn2:
		if ( --max_garbage == 0) {
			return(ERROR);
		}
		goto startover;
	case ZPAD|0200:		// This is what we want.
	case ZPAD:		// This is what we want.
		break;
	}
	cancount = 5;
splat:
	switch (c = noxrd7()) {
	case ZPAD:
		goto splat;
	case RCDO:
	case TIMEOUT:
		goto fifi;
	default:
		goto agn2;
	case ZDLE:		// This is what we want.
		break;
	}

	switch (c = noxrd7()) {
	case RCDO:
	case TIMEOUT:
		goto fifi;
	case ZBIN:
		Rxframeind = ZBIN;  Crc32 = FALSE;
		c = zrbhdr(hdr);
		break;
	case ZBIN32:
		Crc32 = Rxframeind = ZBIN32;
		c = zrbhdr32(hdr);
		break;
	case ZHEX:
		Rxframeind = ZHEX;  Crc32 = FALSE;
		c = zrhhdr(hdr);
		break;
	case _CAN:
		goto gotcan;
	default:
		goto agn2;
	}
	rxpos = hdr[ZP3] & 0377;
	rxpos = (rxpos<<8) + (hdr[ZP2] & 0377);
	rxpos = (rxpos<<8) + (hdr[ZP1] & 0377);
	rxpos = (rxpos<<8) + (hdr[ZP0] & 0377);

fifi:

	switch (c)
	{
	case GOTCAN:
		c = ZCAN;
	// **** FALL THRU TO ****
	case ZNAK:
	case ZCAN:
	case ERROR:
	case TIMEOUT:
	case RCDO:
	// **** FALL THRU TO ****
	default: break;
	}

	if (Rxpos)
		*Rxpos=rxpos;
	return c;
}

//------------------------------------------------------------------------------
// 설명 :
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int zrdat32(char *buf, int length, size_t *bytes_received)
{
	register int c;
	register unsigned long crc;
	register char *end;
	register int d;

	crc = 0xFFFFFFFFL;  end = buf + length;
	while (buf <= end) {
		if ((c = zdlread()) & ~0377) {
crcfoo:
			switch (c) {
			case GOTCRCE:
			case GOTCRCG:
			case GOTCRCQ:
			case GOTCRCW:
				d = c;
				c &= 0377;
				crc = UPDC32(c, crc);
				if ((c = zdlread()) & ~0377)
					goto crcfoo;
				crc = UPDC32(c, crc);
				if ((c = zdlread()) & ~0377)
					goto crcfoo;
				crc = UPDC32(c, crc);
				if ((c = zdlread()) & ~0377)
					goto crcfoo;
				crc = UPDC32(c, crc);
				if ((c = zdlread()) & ~0377)
					goto crcfoo;
				crc = UPDC32(c, crc);
				if (crc != 0xDEBB20E3) {
				//	zperr(badcrc);
					return ERROR;
				}
				*bytes_received = length - (end - buf);
			//	COUNT_BLK(*bytes_received);
				return d;
			case GOTCAN:
			//	zperr(_("Sender Canceled"));
				return ZCAN;
			case TIMEOUT:
			//	zperr(_("TIMEOUT"));
				return c;
			default:
			//	zperr(_("Bad data subpacket"));
				return c;
			}
		}
		*buf++ = c;
		crc = UPDC32(c, crc);
	}
//	zperr(_("Data subpacket too long"));
	return ERROR;
}

//------------------------------------------------------------------------------
// 설명 : Receive array buf of max length with ending ZDLE sequence
//         and CRC.  Returns the ending character or error code.
//         NB: On errors may store length+1 bytes!
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static int zrdata(char *buf, int length, size_t *bytes_received)
{
	register int c;
	register unsigned short crc;
	register char *end;
	register int d;

	*bytes_received=0;
	if (Rxframeind == ZBIN32)
		return zrdat32(buf, length, bytes_received);

	crc = 0;  end = buf + length;
	while (buf <= end) {
		if ((c = zdlread()) & ~0377) {
crcfoo:
			switch (c) {
			case GOTCRCE:
			case GOTCRCG:
			case GOTCRCQ:
			case GOTCRCW:
			{
				d = c;
				c &= 0377;
				crc = updcrc(c, crc);
				if ((c = zdlread()) & ~0377)
					goto crcfoo;
				crc = updcrc(c, crc);
				if ((c = zdlread()) & ~0377)
					goto crcfoo;
				crc = updcrc(c, crc);
				if (crc & 0xFFFF) {
				//	zperr(badcrc);
					return ERROR;
				}
				*bytes_received = length - (end - buf);
			//	COUNT_BLK(*bytes_received);
				return d;
			}
			case GOTCAN:
			//	zperr(_("Sender Canceled"));
				return ZCAN;
			case TIMEOUT:
			//	zperr(_("TIMEOUT"));
				return c;
			default:
			//	zperr(_("Bad data subpacket"));
				return c;
			}
		}
		*buf++ = c;
		crc = updcrc(c, crc);
	}
//	zperr(_("Data subpacket too long"));
	return ERROR;
}


//------------------------------------------------------------------------------
// 설명 : Routine to calculate the free bytes on the current file system
//        ~0 means many free bytes (unknown)
// 매계 :
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static inline size_t getfree(void)
{
	return ((size_t) (~0L));	// many free bytes ...
}
//------------------------------------------------------------------------------
// 설명 : 시리얼에서 한문자를 받는다.
// 매계 :
// 반환 : 없음
// 주의 : 수신대기를 하지 않는다.
//------------------------------------------------------------------------------
int serial_trygetchar(int port)
{
	int ch = -1;

	if ( serial_tstc() ) ch = serial_getc();

	return ch;
}


//------------------------------------------------------------------------------
// 설명 : flush serial input FIFO
// 매계 :
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static void purgeline(int fd)
{
	while (serial_trygetchar(0) != -1);
}

//------------------------------------------------------------------------------
// 설명 : Ack a ZFIN packet, let byegones be byegones
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static void ackbibi(void)
{
	int n;

	stohdr(0L);
	for (n = 3; --n >= 0; ) {
		purgeline(0);
		zshhdr(ZFIN, Txhdr);
		switch (READLINE_PF(100)) {
		case 'O':
			READLINE_PF(1);	// Discard 2nd 'O'
			return;
		case RCDO:
			return;
		case TIMEOUT:
		default:
			break;
		}
	}
}


//------------------------------------------------------------------------------
// 설명 : Initialize for Zmodem receive attempt, try to activate Zmodem sender
//         Handles ZSINIT frame
//         Return ZFILE if Zmodem filename received, -1 on error,
//          ZCOMPL if transaction finished,  else 0
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static int tryz(void)
{
	register int c, n;
	register int cmdzack1flg;
	int          zrqinits_received = 0;
	size_t       bytes_in_block    = 0;

	for (n = zmodem_requested ? 15 : 5;  (--n + zrqinits_received) >= 0 && zrqinits_received < 10; )
	{
		// Set buffer length (0) and capability flags
		stohdr(0L);
		Txhdr[ZF0] = CANFC32|CANFDX|CANOVIO|CANBRK;
		Txhdr[ZF0] = CANFC32|CANFDX|CANBRK;
		if (Zctlesc)	Txhdr[ZF0] |= TESCCTL; // TESCCTL == ESCCTL
		zshhdr(tryzhdrtype, Txhdr);

		if (tryzhdrtype == ZSKIP)	// Don't skip too far
			tryzhdrtype = ZRINIT;	// CAF 8-21-87
again:
		switch (zgethdr(Rxhdr, 0, NULL)) {
		case ZRQINIT:
		// getting one ZRQINIT is totally ok. Normally a ZFILE follows
		// (and might be in our buffer, so don't purge it). But if we
		// get more ZRQINITs than the sender has started up before us
		// and sent ZRQINITs while waiting.
		//
			zrqinits_received++;
			continue;
		case ZEOF:
			continue;
		case TIMEOUT:
			continue;
		case ZFILE:
			zconv = Rxhdr[ZF0];
			if (!zconv)
		// resume with sz -r is impossible (at least with unix sz)
		// if this is not set
				zconv=ZCBIN;
			if (Rxhdr[ZF1] & ZF1_ZMSKNOLOC) {
				Rxhdr[ZF1] &= ~(ZF1_ZMSKNOLOC);
				skip_if_not_found = TRUE;
			}
			zmanag = Rxhdr[ZF1];
			ztrans = Rxhdr[ZF2];
			tryzhdrtype = ZRINIT;
			c = zrdata(secbuf, MAX_BLOCK, &bytes_in_block);
			if (c == GOTCRCW)
				return ZFILE;
			zshhdr(ZNAK, Txhdr);
			goto again;
		case ZSINIT:
			// this once was:
			// Zctlesc = TESCCTL & Rxhdr[ZF0];
			// trouble: if rz get --escape flag:
			// - it sends TESCCTL to sz,
			//   get a ZSINIT _without_ TESCCTL
			//   (yeah - sender didn't know),
			//   overwrites Zctlesc flag ...
			// - sender receives TESCCTL and uses "|=..."
			// so: sz escapes, but rz doesn't unescape ... not good.
			//
			Zctlesc |= TESCCTL & Rxhdr[ZF0];
			if (zrdata(Attn, ZATTNLEN, &bytes_in_block) == GOTCRCW)
			{
				stohdr(1L);
				zshhdr(ZACK, Txhdr);
				goto again;
			}
			zshhdr(ZNAK, Txhdr);
			goto again;
		case ZFREECNT:
			stohdr(getfree());
			zshhdr(ZACK, Txhdr);
			goto again;
		case ZCOMMAND:
			cmdzack1flg = Rxhdr[ZF0];
			if (zrdata(secbuf, MAX_BLOCK, &bytes_in_block)==GOTCRCW)
			{
				// We don't support remote commands
				zshhdr(ZCOMPL, Txhdr);
				return ZCOMPL;
			}
			zshhdr(ZNAK, Txhdr);
			goto again;
		case ZCOMPL:
			goto again;
		default:
			continue;
		case ZFIN:
			ackbibi();
			return ZCOMPL;
		case ZRINIT:
			return ERROR;
		case ZCAN:
			return ERROR;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// 설명 : Process incoming file information header
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------

extern int sscanf(char *s, const char *fmt, ...);
static int procheader(char *name, struct zm_fileinfo *zi)
{
	static char name_static[256];
	char *nameend;

	strcpy(name_static, name);
	if (!zi->fname)
		zi->fname = name_static;

	zi->bytes_total = DEFBYTL;
	zi->mode = 0;
	zi->eof_seen = 0;
	zi->modtime = 0;


	nameend = name + 1 + strlen(name);
	if (*nameend) {	// file coming from Unix or DOS system
		long modtime;
		long bytes_total;
		int mode;
		sscanf(nameend, "%ld%lo%o", &bytes_total, &modtime, &mode);
		zi->modtime=modtime;
		zi->bytes_total=bytes_total;
		zi->mode=mode;
		if (zi->mode & UNIXFILE)
			/*++Thisbinary*/;
	}
        zi->seek_adr = zi->addr;

// @	zi->file = mem_file_creat(zi->fname, zi->addr, zi->bytes_total);
// @	if (!zi->file)
// @	{
// @		return ERROR;
// @	}
	zi->bytes_received = zi->bytes_skipped;

	return OK;
}

//------------------------------------------------------------------------------
// 설명 : Recover a long integer from a header
// 매계 : 화일명
// 반환 : addr  : 받을 주소
// 주의 : 없음
//------------------------------------------------------------------------------
static long rclhdr(char *hdr)
{
	long l;

	l = (hdr[ZP3] & 0377);
	l = (l << 8) | (hdr[ZP2] & 0377);
	l = (l << 8) | (hdr[ZP1] & 0377);
	l = (l << 8) | (hdr[ZP0] & 0377);
	return l;
}

//------------------------------------------------------------------------------
// 설명 : Send a string to the modem, processing for \336 (sleep 1 sec)
// 매계 : and \335 (break signal)
// 반환 : addr  : 받을 주소
// 주의 : 없음
//------------------------------------------------------------------------------
static void zmputs(const char *s)
{
	const char *p;

	while (s && *s) {
		p = strpbrk(s, "\335\336");
		if (!p)
		{
			SerialOutStr( (char *) s, strlen(s) );
			return;
		}
		if (p != s)
		{
			SerialOutStr( (char *)s, (size_t)(p-s) );
			s=p;
		}
		p++;
	}
}

static inline int closeit(struct zm_fileinfo *zi)
{
	return OK;
}

static int putsec(struct zm_fileinfo *zi, char *buf, size_t n)
{
        int lp;

	if (n == 0) return OK;
	// Only allow binary transfer
// @	mem_file_write(zi->file, buf, n);
        for( lp = 0; lp < n; lp++ )
        {
           *(zi->seek_adr) = buf[lp];
           zi->seek_adr++;
        }
	return OK;
}


//------------------------------------------------------------------------------
// 설명 : Receive a file with ZMODEM protocol
//        Assumes file name frame is in secbuf
// 매계 : 화일명
// 반환 : addr  : 받을 주소
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int rzfile(struct zm_fileinfo *zi)
{
	register int c, n;
	size_t bytes_in_block = 0;

	zi->eof_seen = FALSE;

	n = 20;

	if (procheader(secbuf, zi) == ERROR) {
		return (tryzhdrtype = ZSKIP);
	}

	for (;;) {
		stohdr(zi->bytes_received);
		zshhdr(ZRPOS, Txhdr);
nxthdr:
		c = zgethdr(Rxhdr, 0, NULL);
		switch (c) {
		default:
			return ERROR;
		case ZNAK:
		case TIMEOUT:
			if (--n < 0) {
				return ERROR;
			}
		case ZFILE:
			zrdata(secbuf, MAX_BLOCK, &bytes_in_block);
			continue;
		case ZEOF:
			if (rclhdr(Rxhdr) != (long)zi->bytes_received) {
				//
				// Ignore eof if it's at wrong place - force
				//  a timeout because the eof might have gone
				//  out before we sent our zrpos.
				//
				goto nxthdr;
			}
			if (closeit(zi)) {
				tryzhdrtype = ZFERR;
				return ERROR;
			}
			return c;
		case ERROR:	// Too much garbage in header search error
			if ( --n < 0) {
				return ERROR;
			}
			zmputs(Attn);
			continue;
		case ZSKIP:
			closeit(zi);
			return c;
		case ZDATA:
			if (rclhdr(Rxhdr) != (long)zi->bytes_received) {
				if (--n < 0) {
					return ERROR;
				}
				zmputs(Attn);  continue;
			}
moredata:
			switch (c = zrdata(secbuf, MAX_BLOCK, &bytes_in_block))
			{
			case ZCAN:
				return ERROR;
			case ERROR:	// CRC error
				if (--n < 0) {
					return ERROR;
				}
				zmputs(Attn);
				continue;
			case TIMEOUT:
				if (--n < 0) {
					return ERROR;
				}
				continue;
			case GOTCRCW:
				n = 20;
				putsec(zi, secbuf, bytes_in_block);
				zi->bytes_received += bytes_in_block;
				stohdr(zi->bytes_received);
				zshhdr(ZACK | 0x80, Txhdr);
				goto nxthdr;
			case GOTCRCQ:
				n = 20;
				putsec(zi, secbuf, bytes_in_block);
				zi->bytes_received += bytes_in_block;
				stohdr(zi->bytes_received);
				zshhdr(ZACK, Txhdr);
				goto moredata;
			case GOTCRCG:
				n = 20;
				putsec(zi, secbuf, bytes_in_block);
				zi->bytes_received += bytes_in_block;
				goto moredata;
			case GOTCRCE:
				n = 20;
				putsec(zi, secbuf, bytes_in_block);
				zi->bytes_received += bytes_in_block;
				goto nxthdr;
			}
		}
	}
}

//------------------------------------------------------------------------------
// 설명 : ZMODEM 프로토콜을 이용하여 화일 내용을 가져 온다.
// 매계 : 화일명
// 반환 : addr  : 받을 주소
// 주의 : 없음
//------------------------------------------------------------------------------
static inline int rzfiles(struct zm_fileinfo *zi)
{
	int c;

	for (;;) {
		c = rzfile(zi);
		switch (c) {
		case ZEOF:
			// FALL THROUGH
		case ZSKIP:
			switch (tryz()) {
			case ZCOMPL:
				return OK;
			default:
				return ERROR;
			case ZFILE:
				break;
			}
			continue;
		case ERROR:
		default:
			return c;
		}
	}
}

//------------------------------------------------------------------------------
// 설명 : send cancel string to get the other end to shut up
// 매계 : 없음
// 반환 : 없음
// 주의 : 없음
//------------------------------------------------------------------------------
static void canit(int fd)
{
	static char canistr[] =
	{
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0
	};
	purgeline(fd);
        SerialOutStr( canistr, strlen(canistr) );
}


//------------------------------------------------------------------------------
// 설명 : ZMODEM 프로토콜을 이용하여 화일 내용을 가져 온다.
// 매계 : name : 화일명  ( 아직은 쓰이지 않고 있다. )
//        addr : 받아내릴 어드레스
// 반환 : 에러 : -1
//        정상 : 수신된 화일 크기
// 주의 : 없음
//------------------------------------------------------------------------------
int rz(const char *name, unsigned long addr)
{
	int c;
	struct zm_fileinfo zi;

	zi.fname=(char *)name;
	zi.addr=(void *)addr;
	zi.modtime=0;
	zi.mode=0;
	zi.bytes_total=0;
	zi.bytes_received=0;
	zi.bytes_skipped=0;
	zi.eof_seen=0;

	if ((c = tryz()) != 0)
    {
		if (c == ZCOMPL) 	goto ok;
		if (c == ERROR)     goto fubar;
		c = rzfiles(&zi);
		if (c) 	goto fubar;
	} else
	{
		goto fubar;
	}
ok:
	c = 1000000;
	while (--c);
	return zi.bytes_total;
fubar:
	canit(1);

	c = 1000000;
	while (--c);
	return ERROR;
}
