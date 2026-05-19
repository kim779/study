// compress.cpp: implementation of the CCompress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "compress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static BYTE	lmask[9] = {0xff,0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00};
static BYTE	rmask[9] = {0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff};

CCompress::CCompress()
{
	maxbits = BITS;			// user settable max # bits/code
	maxmaxcode = 1 << BITS;		// should NEVER generate this code
	hsize = HSIZE;			// for dynamic table sizing
	free_ent = 0;			// first unused entry
	block_compress = BLOCK_MASK;
	clear_flg = 0;
	checkpoint = CHECK_GAP;
	in_count = 1;			// length of input
	out_count = 0;			// # of codes output (for debugging)
	memset(buf, 0, sizeof(buf));
}

CCompress::~CCompress()
{

}

int CCompress::compress(BYTE *rbuf, int rlen, BYTE *cbuf) 
{
	register long fcode;
	register int i = 0;
	register int c;
	register int ent;
	register int disp;
	register int hsize_reg;
	register int hshift;

	varinit(); 
	
	hsize = HSIZE;
	if (rlen < (1 << 12) )
		hsize = min(5003, HSIZE);
	else if (rlen < (1 << 13) )
		hsize = min(9001, HSIZE);
	else if (rlen < (1 << 14))
		hsize = min( 18013, HSIZE);
	else if (rlen < (1 << 15))
		hsize = min(35023, HSIZE);
	else if (rlen < 47000)
		hsize = min(50021, HSIZE);

	proc_b = (BYTE *)cbuf;
	offset = 0;
	bytes_out = 3;			// includes 3-byte header mojo
	out_count = 0;
	clear_flg = 0;
	in_count = 1;
	checkpoint = CHECK_GAP;
	maxcode = MAXCODE(n_bits = INIT_BITS);
	free_ent = ((block_compress) ? FIRST : 256 );

	ent = rbuf[0];

	hshift = 0;
	for ( fcode = (long) hsize;  fcode < 65536L; fcode *= 2L )
		hshift++;
	hshift = 8 - hshift;		// set hash code range bound

	hsize_reg = hsize;
	cl_hash( (int) hsize_reg);	// clear hash table

	while (in_count < rlen)
	{
		c = rbuf[in_count++];
		fcode = (long) (((long) c << maxbits) + ent);
 		i = ((c << hshift) ^ ent);	// xor hashing

		if (htabof (i) == fcode) 
		{
			ent = codetabof (i);
			continue;
		} else if ((long)htabof (i) < 0)// empty slot
			goto nomatch;
 		disp = hsize_reg - i;		// secondary hash (after G. Knott)
		if (i == 0)
			disp = 1;
	probe:
		if ((i -= disp) < 0)
			i += hsize_reg;

		if (htabof (i) == fcode) 
		{
			ent = codetabof(i);
			continue;
		}
		if ((long)htabof(i) > 0) 
			goto probe;
	nomatch:
		output((int)ent);
		out_count++;
		if (out_count > rlen || proc_l > rlen)
			return (0);
 		ent = c;
		if (free_ent < maxmaxcode) 
		{
 			codetabof(i) = free_ent++;	// code -> hashtable
			htabof(i) = fcode;
		}
		else if ((int)in_count >= checkpoint && block_compress)
			cl_block();
	}
	output((int)ent);
	out_count++;
	if (proc_l > rlen)
		return (0);
	output((int)-1);
	out_count++;
	if (out_count > rlen)
		return (0);

	return (proc_l);
}

int CCompress::decompress(BYTE *rbuf, int rlen, BYTE *cbuf) 
{
	register BYTE *stackp;
	register int finchar;
	register int code, oldcode, incode;

	varinit();
	proc_b = (BYTE *)cbuf;
	raw_b = (BYTE *)rbuf;
	raw_l = 0;
	max_l = rlen;
	offset = 0;
	size = 0;

	maxcode = MAXCODE(n_bits = INIT_BITS);
	for (code = 255; code >= 0; code-- ) 
	{
		tab_prefixof(code) = 0;
		tab_suffixof(code) = (BYTE)code;
	}
	free_ent = ((block_compress) ? FIRST : 256 );

	finchar = oldcode = getcode(); 
	if(oldcode == -1)			// EOF already?
		return (0);			// Get out of here
	proc_b[proc_l++] = finchar;
	stackp = de_stack;

	while ((code = getcode()) > -1) 
	{
		if ((code == CLEAR) && block_compress) 
		{
			for (code = 255; code >= 0; code--)
				tab_prefixof(code) = 0;
			clear_flg = 1;
			free_ent = FIRST - 1;
			if ((code = getcode ()) == -1)	
				break;
		}
		incode = code;

		if (code >= free_ent) 
		{
			*stackp++ = finchar;
			code = oldcode;
		}

		while (code >= 256) 
		{
			*stackp++ = tab_suffixof(code);
			code = tab_prefixof(code);
		}
		*stackp++ = finchar = tab_suffixof(code);

		do
			proc_b[proc_l++] = *--stackp;
		while ( stackp > de_stack );

		if ((code=free_ent) < maxmaxcode) 
		{
			tab_prefixof(code) = (unsigned short)oldcode;
			tab_suffixof(code) = finchar;
			free_ent = code+1;
		} 
		oldcode = incode;
	}
	return (proc_l);
}

void CCompress::varinit()
{
	free_ent = 0;
	maxmaxcode = (1 << BITS);
	memset(htab, 0, sizeof(htab));
	memset(codetab, 0, sizeof(codetab));
	maxbits = BITS;
	maxmaxcode = (1 << maxbits);
	clear_flg  = 0;
	in_count   = 1;
	bytes_out  = 0;
		ratio = 0;
	proc_l = 0;
	size   = 0;
	hsize  = HSIZE;
}

void CCompress::output(int code)
{
	register int r_off = offset, bits= n_bits;
	register BYTE * bp = buf;

	if (code >= 0) 
	{
		bp += (r_off >> 3);
		r_off &= 7;
		*bp = (*bp & rmask[r_off]) | (code << r_off) & lmask[r_off];
		bp++;
		bits  -= (8 - r_off);
		code >>= (8 - r_off);
		if (bits >= 8) 
		{
			*bp++ = code;
			code >>= 8;
			bits -= 8;
		}
		if(bits)
			*bp = code;
		offset += n_bits;
		if (offset == (n_bits << 3)) 
		{
			bp = buf;
			bits = n_bits;
			bytes_out += bits;
			do
				proc_b[proc_l++] = *bp++;
			while(--bits);
			offset = 0;
		}

		if (free_ent > maxcode || (clear_flg > 0))
		{
			if (offset > 0) 
			{
				memcpy(&proc_b[proc_l], buf, n_bits);
				proc_l	+= n_bits;
				bytes_out += n_bits;
			}
			offset = 0;

			if (clear_flg) 
			{
				maxcode = MAXCODE (n_bits = INIT_BITS);
				clear_flg = 0;
			}
			else 
			{
				n_bits++;
				if ( n_bits == maxbits )
					maxcode = maxmaxcode;
				else
					maxcode = MAXCODE(n_bits);
			}
		}
	} 
	else 
	{
		if (offset > 0)
		{
			memcpy(&proc_b[proc_l], buf, (offset+7)/8);
			proc_l	+= (offset + 7) / 8;
			bytes_out += (offset + 7) / 8;
		}
		offset = 0;
	}
}

int CCompress:: getcode() 
{
	register int code;
	register int r_off, bits;
	register BYTE *bp = (BYTE *)buf;

	if (clear_flg > 0 || offset >= size || free_ent > maxcode) 
	{
		if (free_ent > maxcode) 
		{
			n_bits++;
			if ( n_bits == maxbits )
				maxcode = maxmaxcode;	// won't get any bigger now
			else
				maxcode = MAXCODE(n_bits);
		}
		if (clear_flg > 0) 
		{
			maxcode = MAXCODE (n_bits = INIT_BITS);
			clear_flg = 0;
		}
		size = getrawcode(buf, n_bits);
		if (size <= 0)
			return (-1);
		offset = 0;
		size = (size << 3) - (n_bits - 1);
	}
	r_off = offset;
	bits = n_bits;

	bp += (r_off >> 3);
	r_off &= 0x07;

	code = (*bp++ >> r_off);
	bits -= (8 - r_off);
	r_off = 8 - r_off;	
	if (bits >= 8) 
	{
		code |= *bp++ << r_off;
		r_off += 8;
		bits -= 8;
	}
	code |= (*bp & rmask[bits]) << r_off;
	offset += n_bits;

	return code;
}

//
//	table clear for block compress
//
void CCompress::cl_block ()
{
	register long int rat;

	checkpoint = in_count + CHECK_GAP;

	if (in_count > 0x007fffff) 
	{	
		rat = bytes_out >> 8;
		if(rat == 0) 			// Don't divide by zero
			rat = 0x7fffffff;
		else 
			rat = in_count / rat;
	} 
	else 
		rat = (in_count << 8) / bytes_out;	// 8 fractional bits
	if (rat > ratio) 
		ratio = rat;
	else 
	{
		ratio = 0;
		cl_hash ((int)hsize);
		free_ent = FIRST;
		clear_flg = 1;
		output ((int)CLEAR);
	}
}

void CCompress::cl_hash(int hsize)
{
	register int *htab_p = htab+hsize;
	register long i;
	register long m1 = -1;

	i = hsize - 16;
 	do {				// might use Sys V memset(3) here
		*(htab_p-16) = m1;
		*(htab_p-15) = m1;
		*(htab_p-14) = m1;
		*(htab_p-13) = m1;
		*(htab_p-12) = m1;
		*(htab_p-11) = m1;
		*(htab_p-10) = m1;
		*(htab_p-9) = m1;
		*(htab_p-8) = m1;
		*(htab_p-7) = m1;
		*(htab_p-6) = m1;
		*(htab_p-5) = m1;
		*(htab_p-4) = m1;
		*(htab_p-3) = m1;
		*(htab_p-2) = m1;
		*(htab_p-1) = m1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for ( i += 16; i > 0; i-- )
		*--htab_p = m1;
}

int CCompress::getrawcode(BYTE *rbuf, int rlen)
{
	int	size;

	size = (max_l - raw_l);
	if (size > rlen)
		size = rlen;
	if (size <= 0)
		return (size);
	memcpy(rbuf, &raw_b[raw_l], size);
	raw_l += size;
	return (size);
}

#define	_Z2	0x0a
#define	_Z3	0x0b
#define	_SP	0x0c
#define	_RP	0x0d
#define	_ND	0x0e
#define	_NC	0x0f

int CCompress::axpack(BYTE* ib, int il, BYTE* ob)
{
	int	nibb, prev, rept;
	int	chck, pchk, bits, nbit;
	int	ii, ol;

	prev = -1;
	rept = 0;
	bits = 0;
	nbit = 0;
	pchk = 0;

	for (ii = 0, ol = 0; ii < il; ii++)
	{
		if (ol >= il)
			return(0);
		chck = 0;
		if (ib[ii] >= '0' && ib[ii] <= '9')
			nibb = ib[ii] & 0x0f;
		else if (ib[ii] == ' ')
			nibb = _SP;
		else
			chck = 1;
		if (chck == 0 && chck == pchk && nibb == prev && rept < 15)
		{
			rept++;
			continue;
		}
		switch (rept)
		{
		case 0: break;
		case 1: if (prev == 0)
			{
				bits &= ~0x0f; bits |= _Z2;  
				break;
			}
			bits <<= 4; 
			bits  |= prev;
			nbit += 4;
			break;
		case 2: if (prev == 0)
			{
				bits &= ~0x0f; bits |= _Z3;  
				break;
			}
		default:
			bits <<= 4; bits |= _RP;  nbit += 4;
			bits <<= 4; bits |= rept; nbit += 4;
			break;
		}
		rept = 0;
		prev = -1;
		if (pchk != chck)
		{
			bits <<= 4; bits |= _NC;  nbit += 4;
		}
		pchk = chck;
		switch (chck)
		{
		case 0:  /*  compress */
			bits <<= 4; bits |= nibb; nbit += 4; 
			prev = nibb;
			break;
		case 1:  /* !compress */
			if ((ib[ii] >> 4) == _NC)
			{
				bits <<= 4; bits |= _NC; nbit += 4; 
			}
			bits <<= 8; bits |= ib[ii]; nbit += 8;
			break;
		}
		if (nbit <= 8)
			continue;
		switch (nbit)
		{
		case 12: ob[ol++] = bits >> 4;  
			 nbit = 4; 
			 break;
		case 16: ob[ol++] = bits >> 8;  
			 nbit = 8; 
			 break;
		case 20: ob[ol++] = bits >> 12; 
			 ob[ol++] = bits >> 4; 
			 nbit = 4; 
			 break;
		case 24: ob[ol++] = bits >> 16;
			 ob[ol++] = bits >> 8;
			 nbit = 8;
			 break;
		case 28: ob[ol++] = bits >> 20;
			 ob[ol++] = bits >> 12;
			 ob[ol++] = bits >> 4;
			 nbit = 4;
			 break;
		case 32: ob[ol++] = bits >> 24;
			 ob[ol++] = bits >> 16;
			 ob[ol++] = bits >> 8;
			 nbit = 8;
			 break;
		}
	}

	switch (rept)
	{
	case 0: break;
	case 1: if (prev == 0)
		{
			bits &= ~0x0f; bits |= _Z2;  
			break;
		}
		bits <<= 4; 
		bits  |= prev;
		nbit += 4;
		break;
	case 2: if (prev == 0)
		{
			bits &= ~0x0f; bits |= _Z3;  
			break;
		}
	default:
		bits <<= 4; bits |= _RP;  nbit += 4;
		bits <<= 4; bits |= rept; nbit += 4;
		break;
	}
	if (pchk)
	{
		bits <<= 4;
		bits  |= _NC;
		nbit += 4;
	}
	switch (nbit)
	{
	case  8: ob[ol++] = bits;
		 break;
	case 12: ob[ol++] = bits >> 4;  
		 break;
	case 16: ob[ol++] = bits >> 8;  
	         ob[ol++] = bits;
		 break;
	case 20: ob[ol++] = bits >> 12; 
		 ob[ol++] = bits >> 4; 
		 break;
	case 24: ob[ol++] = bits >> 16;
		 ob[ol++] = bits >> 8;
		 ob[ol++] = bits;
		 break;
	case 28: ob[ol++] = bits >> 20;
		 ob[ol++] = bits >> 12;
		 ob[ol++] = bits >> 4;
		 break;
	case 32: ob[ol++] = bits >> 24;
		 ob[ol++] = bits >> 16;
		 ob[ol++] = bits >> 8;
		 ob[ol++] = bits;
		 break;
	}
	if (nbit % 8)
		ob[ol++] = (bits << 4) | _ND;
	if (ol >= il)
		return(0);
	return(ol);
}

int CCompress::axunpack(BYTE* ib, int il, BYTE* ob)
{
	int	nibb[2], nibble;
	int	flow, ol, chax;
	int	ii,   jj, kk;

	flow = 0;
	ol   = 0;
	for (ii = 0;  ii < il; ii++)
	{
	    nibb[0] = ib[ii] >> 4;
	    nibb[1] = ib[ii] & 0x0f;
	    for (jj = 0; jj < 2; jj++)
	    {
		nibble = nibb[jj];
		switch (flow)
		{
		case 0: /*  compressed */
			break;
		case 1: /* !compressed : follwoing _NC */
			ob[ol] = nibble << 4;
			flow = 2;
			continue;
		case 2: /* 2nd nibble */
			ob[ol++] |= nibble;
			flow = 3;
			continue;
		case 3: /* 1st nibble : please check */
			if (nibble == _NC)
			{
				flow = 4;
				continue;
			}
			ob[ol] = nibble << 4;
			flow = 2;
			continue;
		case 4: /* following _NC */
			if (nibble == _NC)
			{
				ob[ol] = nibble << 4;
				flow = 2;
				continue;
			}
			break;
		case 10:
			chax = ob[ol-1];
			for (kk = 0; kk < nibble; kk++)
				ob[ol++] = chax;
			flow = 0;
			continue;
		}
		flow = 0;
		switch (nibble)
		{
		case _Z2: 
			ob[ol++] = '0'; 
			ob[ol++] = '0';
			break;
		case _Z3:
			ob[ol++] = '0'; 
			ob[ol++] = '0';
			ob[ol++] = '0';
			break;
		case _SP:
			ob[ol++] = ' '; 
			break;
		case _RP:
			flow = 10;
			break;
		case _ND:
			break;
		case _NC:
			flow = 1;
			break;
		default:
			ob[ol++] = nibble | 0x30;
			break;
		}
	    }
	}
	return(ol);
}

