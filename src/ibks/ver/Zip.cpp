// Zip.cpp: implementation of the CZip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Zip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BYTE lmask[9] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00};
BYTE rmask[9] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};
BYTE magic_header[] = { "\037\235" };	// 1F 9D
BYTE axis_mark[] = { "AXIS" };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZip::CZip()
{
	init();
}

CZip::~CZip()
{

}

BOOL CZip::Compress(char* ofname, char* cfname)
{
	int fcode;
	int i = 0;
	int c, fsize, ent, disp, hsize_reg, hshift;
	FILE	*istream, *ostream;
	errno_t	err;

	init();
	err = freopen_s(&istream, ofname, "rb", stdin);
	if (err != 0)
	{
		TRACE("compress file name err NO FILE [%s]",ofname);
		return FALSE;
	}

	fsize = file_size(ofname);
	hsize = HSIZE;
	if (fsize < (1 << 12))
		hsize = min(5003, HSIZE);
	else if (fsize < (1 << 13))
		hsize = min(9001, HSIZE);
	else if (fsize < (1 << 14))
		hsize = min(18013, HSIZE);
	else if (fsize < (1 << 15))
		hsize = min(35023, HSIZE);
	else if (fsize < 47000)
		hsize = min(50021, HSIZE);

	err = freopen_s(&ostream, cfname, "wb", stdout);
	if (err != 0)
	{
		TRACE("write file err[%d]",errno);
		close(true, false);
		return FALSE;
	}

	// AXIS identifier
	putchar(axis_mark[0]);
	putchar(axis_mark[1]);
	putchar(axis_mark[2]);
	putchar(axis_mark[3]);
	//
	putchar(magic_header[0]);
	putchar(magic_header[1]);
	putchar((char)(maxbits | block_compress));
	if (ferror(stdout))
	{
		close(true, true, cfname);
		return FALSE;
	}

	offset     = 0;
	bytes_out  = 3;
	out_count  = 0;
	clear_flg  = 0;
	ratio      = 0;
	in_count   = 1;
	checkpoint = CHECK_GAP;
	maxcode    = MAXCODE(n_bits = INIT_BITS);
	free_ent   = ((block_compress) ? FIRST : 256);
	
	ent = getchar();
	hshift = 0;
	for (fcode = (long)hsize;  fcode < 65536L; fcode *= 2L)
		hshift++;
	hshift = 8 - hshift;
	
	hsize_reg = hsize;
	cl_hash(hsize_reg);
	
	while ((c = getchar()) != EOF)
	{
		in_count++;
		fcode = (long) (((long) c << maxbits) + ent);
 		i = ((c << hshift) ^ ent);

		if (htabof(i) == fcode)
		{
			ent = codetabof(i);
			continue;
		}
		else if ((long)htabof(i) < 0)
			goto nomatch;
 		
		disp = hsize_reg - i;
		if (i == 0)
			disp = 1;
	probe:
		if ((i -= disp) < 0)
			i += hsize_reg;

		if (htabof(i) == fcode)
		{
			ent = codetabof(i);
			continue;
		}
		if ((long)htabof (i) > 0) 
			goto probe;
	nomatch:
		if (!output((int)ent))
		{
			close(true, true, cfname);
			return FALSE;
		}
		out_count++;
 		ent = c;
		if (free_ent < maxmaxcode)
		{
			codetabof(i) = free_ent++;
			htabof(i) = fcode;
		}
		else if (in_count >= checkpoint && block_compress)
			cl_block();
	}

	if (!output((int)ent))
	{
		close(true, true, cfname);
		return FALSE;
	}
	out_count++;
	if (!output((int)-1))
	{
		close(true, true, cfname);
		return FALSE;
	}

	if (bytes_out > in_count)
	{
		close(true, true, cfname);
		return FALSE;
	}

	close(true, true, ofname);
	return TRUE;
}

BOOL CZip::Decompress(char* ofname, char* cfname)
{
	BYTE *stackp;
	int finchar, code, oldcode, incode;
	FILE	*istream, *ostream;
	errno_t	err;

	init();
	err = freopen_s(&istream, ofname, "rb", stdin);
	if (err != 0)
	{
		TRACE("decompress file name err NO FILE [%s]",ofname);
		return FALSE;
	}

	// AXIS identifier
	if ((getchar() != (axis_mark[0] & 0xFF)) || (getchar() != (axis_mark[1] & 0xFF))
		|| (getchar() != (axis_mark[2] & 0xFF)) || (getchar() != (axis_mark[3] & 0xFF)) )
	{
		TRACE("%s: not in compressed format\n", ofname);
		close(true, false);
		return FALSE;
	}
	//
	if ((getchar() != (magic_header[0] & 0xFF)) || (getchar() != (magic_header[1] & 0xFF)))
	{
		TRACE("%s: not in compressed format\n", ofname);
		close(true, false);
		return FALSE;
	}
	maxbits = getchar();
	block_compress = maxbits & BLOCK_MASK;
	maxbits &= BIT_MASK;
	maxmaxcode = 1 << maxbits;
	if (maxbits > BITS)
	{
		TRACE("%s: compressed with %d bits, can only handle %d bits\n", ofname, maxbits, BITS);
		close(true, false);
		return FALSE;
	}

	err = freopen_s(&ostream, cfname, "wb", stdout);
	{
		TRACE("write file err[%d]",errno);
		close(true, false);
		return FALSE;
	}

	maxcode = MAXCODE(n_bits = INIT_BITS);
	for (code = 255; code >= 0; code--)
	{
		tab_prefixof(code) = 0;
		tab_suffixof(code) = (BYTE)code;
	}
	free_ent = ((block_compress) ? FIRST : 256 );
	
	finchar = oldcode = getcode();
	if (oldcode == -1)
	{
		close(true, true, cfname);
		return FALSE;
	}
	putchar((char)finchar);
	if (ferror(stdout))
	{
		close(true, true, cfname);
		return FALSE;
	}
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
			putchar ( *--stackp );
		while ( stackp > de_stack );

		if ((code=free_ent) < maxmaxcode)
		{
			tab_prefixof(code) = (unsigned short)oldcode;
			tab_suffixof(code) = finchar;
			free_ent = code+1;
		} 
		oldcode = incode;
	}
	fflush(stdout);
	if (ferror(stdout))
	{
		close(true, true, cfname);
		return FALSE;
	}

	close(true, true, ofname);
	return TRUE;
}

void CZip::init()
{
	maxbits = BITS;
	maxmaxcode = 1 << BITS;

	hsize     = HSIZE;
	in_count  = 1;
	out_count = 0;
	free_ent  = 0;
	clear_flg = 0;
	ratio     = 0;

	block_compress = BLOCK_MASK;
	checkpoint     = CHECK_GAP;
}

int CZip::getcode()
{
	int code;
	int offset = 0, size = 0;
	static BYTE buf[BITS];
	int r_off, bits;
	BYTE *bp = buf;

	if (clear_flg > 0 || offset >= size || free_ent > maxcode)
	{
		if (free_ent > maxcode)
		{
			n_bits++;
			if (n_bits == maxbits)
				maxcode = maxmaxcode;
			else
				maxcode = MAXCODE(n_bits);
		}
		if (clear_flg > 0)
		{
    			maxcode = MAXCODE (n_bits = INIT_BITS);
			clear_flg = 0;
		}
		size = fread(buf, 1, n_bits, stdin);
		if (size <= 0)
			return -1;
		offset = 0;
		size = (size << 3) - (n_bits - 1);
	}
	r_off = offset;
	bits = n_bits;

	bp += (r_off >> 3);
	r_off &= 7;
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

void CZip::cl_hash(int size)
{
	int *htab_p = htab+size;
	long i  = size - 16;

 	do {
		*(htab_p-16) = -1;
		*(htab_p-15) = -1;
		*(htab_p-14) = -1;
		*(htab_p-13) = -1;
		*(htab_p-12) = -1;
		*(htab_p-11) = -1;
		*(htab_p-10) = -1;
		*(htab_p-9) = -1;
		*(htab_p-8) = -1;
		*(htab_p-7) = -1;
		*(htab_p-6) = -1;
		*(htab_p-5) = -1;
		*(htab_p-4) = -1;
		*(htab_p-3) = -1;
		*(htab_p-2) = -1;
		*(htab_p-1) = -1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for (i += 16; i > 0; i--)
		*--htab_p = -1;
}

void CZip::cl_block()
{
	long int rat;

	checkpoint = in_count + CHECK_GAP;
	if (in_count > 0x007fffff)
	{
		rat = bytes_out >> 8;
		if (rat == 0)
			rat = 0x7fffffff;
		else
			rat = in_count / rat;
	}
	else
		rat = (in_count << 8) / bytes_out;

	if (rat > ratio)
		ratio = rat;
	else
	{
		ratio = 0;
		cl_hash(hsize);
		free_ent  = FIRST;
		clear_flg = 1;
		output(CLEAR);
	}
}

BOOL CZip::output(int code)
{
	int r_off = offset, bits= n_bits;
	char * bp = buf;

	if (code >= 0)
	{
		bp += (r_off >> 3);
		r_off &= 7;
		*bp = (*bp & rmask[r_off]) | (code << r_off) & lmask[r_off];
		bp++;
		bits -= (8 - r_off);
		code >>= 8 - r_off;

		if (bits >= 8)
		{
			*bp++ = code;
			code >>= 8;
			bits -= 8;
		}

		if (bits)
			*bp = code;

		offset += n_bits;
		if (offset == (n_bits << 3))
		{
			bp = buf;
			bits = n_bits;
			bytes_out += bits;
			do
				putchar(*bp++);
			while (--bits);
			offset = 0;
		}


		if (free_ent > maxcode || (clear_flg > 0))
		{
			if (offset > 0)
			{
				if ((int)fwrite(buf, 1, n_bits, stdout) != n_bits)
					return FALSE;

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
				if (n_bits == maxbits)
					maxcode = maxmaxcode;
				else
					maxcode = MAXCODE(n_bits);
			}
		}
	}
	else
	{
		if (offset > 0)
			fwrite(buf, 1, (offset + 7) / 8, stdout);
		bytes_out += (offset + 7) / 8;
		offset = 0;
		fflush(stdout);

		if (ferror(stdout))
			return FALSE;
	}
	return TRUE;
}

void CZip::close(bool in, bool out, char* filename)
{
	fclose(stdin);
	fclose(stdout);
	if (filename)
		_unlink(filename);
}

int CZip::file_size(char* filename)
{
	FILE*	fd;
	int	f_size;
	errno_t	err;

	err = fopen_s(&fd, filename, "rb");
	if (err != 0)
		return -5;
	fseek(fd,0,SEEK_END);
	f_size = ftell(fd);
	fclose(fd);
	return f_size;
}