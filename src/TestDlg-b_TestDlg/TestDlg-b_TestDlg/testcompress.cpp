#include "pch.h"
#include "testcompress.h"

static BYTE	lmask[9] = { 0xff,0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00 };
static BYTE	rmask[9] = { 0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff };

cl_testcompress::cl_testcompress()
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
	memset(buf, 0, sizeof(buf));  //buf[13]

m_slog.Format( "\r\n  \
						  maxbits = [%d]	\r\n  \
						  maxmaxcode=[%d]			 \r\n \
						  block_compress=[%x]			\r\n \
						  checkpoint=[%d]			\r\n \
						  hsize =[%d]", maxbits, maxmaxcode, block_compress, checkpoint, hsize);
LOG_OUTP(3, "compress", __FUNCTION__, m_slog);
}

cl_testcompress::~cl_testcompress()
{

}

int cl_testcompress::compress(BYTE* rbuf, int rlen, BYTE* cbuf)
{//rbuf 원본    //cbuf 압축한 결과 , 이거 메모리버터 nullptr 세팅 했는데 그 길이를 모르내...?

	m_slog.Format("\r\n  \
						  rbuf = [%s]	\r\n  \
						  rlen=[%d]			 \r\n \
						  cbuf=[%s]			\r\n \
						", rbuf, rlen, cbuf);

	LOG_OUTP(3, "compress", __FUNCTION__, m_slog);
	long fcode;
	int i = 0;
	int c, ent, disp, hsize_reg, hshift;

	varinit();

	hsize = HSIZE;
	if (rlen < (1 << 12))
		hsize = min(5003, HSIZE);
	else if (rlen < (1 << 13))
		hsize = min(9001, HSIZE);
	else if (rlen < (1 << 14))
		hsize = min(18013, HSIZE);
	else if (rlen < (1 << 15))
		hsize = min(35023, HSIZE);
	else if (rlen < 47000)
		hsize = min(50021, HSIZE);

	proc_b = (BYTE*)cbuf;  //
	offset = 0;
	bytes_out = 3;			// includes 3-byte header mojo
	out_count = 0;
	clear_flg = 0;
	in_count = 1;
	checkpoint = CHECK_GAP;
	maxcode = MAXCODE(n_bits = INIT_BITS);
	free_ent = ((block_compress) ? FIRST : 256);

	ent = rbuf[0];   //rbuf 압축할 원본

	hshift = 0;
	for (fcode = (long)hsize; fcode < 65536L; fcode *= 2L)  //곱하기 연산이다
		hshift++; 
	hshift = 8 - hshift;		// set hash code range bound

	hsize_reg = hsize;

//--------------------------------------------------------------------
	m_slog.Format("\r\n  \
						  hsize = [%d]	\r\n  \
						  proc_b=[%s]			 \r\n \
						  maxcode = [%d] \r\n \
						  free_ent = [%d] \r\n \
						  ent = [%d] \r\n \
						  hshift = [%d] \r\n \
						", hsize, proc_b, maxcode, free_ent, ent, hshift);
	LOG_OUTP(3, "compress", __FUNCTION__, m_slog);
	//--------------------------------------------------------------------

	cl_hash((int)hsize_reg);	// clear hash table  htap 를 -1 로 초기화 
/*  Lempel-Zi 압축 알고리즘
in_count: 현재까지 읽은 입력 데이터의 바이트 수를 나타냅니다.
rlen: 입력 데이터의 총 길이를 나타냅니다.
rbuf: 입력 버퍼입니다.
maxbits: 최대 비트 수입니다.
ent: 현재 해시 테이블 항목을 가리키는 인덱스입니다.
hshift: 해시 테이블을 구성하는 데 사용되는 쉬프트 값입니다.
htabof(i): 해시 테이블에서 인덱스 i에 해당하는 항목의 값을 가져옵니다.
codetabof(i): 코드 테이블에서 인덱스 i에 해당하는 항목의 값을 가져옵니다.
fcode: 현재 입력 데이터의 코드를 나타냅니다.
disp: 충돌 시 이동할 거리를 나타냅니다.
probe: 충돌 시 다음 위치를 조사하기 위해 사용되는 레이블입니다.
out_count: 현재까지 출력한 데이터의 바이트 수를 나타냅니다.
proc_l: 처리된 데이터의 길이를 나타냅니다.
free_ent: 다음에 사용될 빈 항목의 인덱스입니다.
maxmaxcode: 코드의 최대값입니다.
checkpoint: 체크포인트로 사용되는 값입니다.
block_compress: 블록 압축 모드인지 여부를 나타냅니다.


입력 데이터의 바이트를 하나씩 읽어와서 c에 저장합니다.
fcode를 계산하여 해시 테이블에서 값을 찾습니다. 만약 찾았다면 해당 항목으로 이동합니다.
해시 충돌을 처리합니다. 만약 충돌이 발생했다면 다음 위치로 이동하여 검색합니다.
빈 항목이 발견되면 출력합니다.
새로운 코드를 해시 테이블에 추가합니다. 만약 해시 테이블이 가득 찼다면 블록을 압축합니다.
모든 입력 데이터를 처리할 때까지 반복합니다.
	*/
	while (in_count < rlen)
	{
		c = rbuf[in_count++];  //rbuf 압축할 원본
		fcode = (long)(((long)c << maxbits) + ent);   //maxbits 는 define 값이다.. 13   ent = rbuf[0];
		i = ((c << hshift) ^ ent);

		if (htabof(i) == fcode)
		{
			ent = codetabof(i);
			continue;
		}
		else if ((long)htabof(i) < 0) //empty slot
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

		if ((long)htabof(i) > 0)
			goto probe;
	nomatch:
		output((int)ent);
		out_count++;
		if (out_count > rlen | proc_l > rlen)
			return (0);
		ent = c;
		if (free_ent < maxmaxcode)
		{
			codetabof(i) = free_ent++;
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
	if(out_count > rlen)
		return 0;

	return(proc_l);
}

int cl_testcompress::decompress(BYTE* rbuf, int rlen, BYTE* cbuf)
{
	BYTE* stackp;
	int finchar, code, oldcode, incode;

	varinit();
	proc_b = (BYTE*)cbuf;
	raw_b = (BYTE*)rbuf;
	raw_l = 0;
	max_l = rlen;
	offset = 0;
	size = 0;

	maxcode = MAXCODE(n_bits = INIT_BITS);
	for (code = 255; code >= 0; code--)
	{
		tab_prefixof(code) = 0;
		tab_suffixof(code) = (BYTE)code;
	}
	free_ent = ((block_compress) ? FIRST : 256);

	finchar = oldcode = getcode();
	if (oldcode == -1)			// EOF already?
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
			if ((code = getcode()) == -1)
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
		while (stackp > de_stack);

		if ((code = free_ent) < maxmaxcode)
		{
			tab_prefixof(code) = (unsigned short)oldcode;
			tab_suffixof(code) = finchar;
			free_ent = code + 1;
		}
		oldcode = incode;
	}
	return (proc_l);
}

void cl_testcompress::varinit()
{
	free_ent = 0;
	maxmaxcode = (1 << BITS);
	memset(htab, 0, sizeof(htab));
	memset(codetab, 0, sizeof(codetab));
	maxbits = BITS;
	maxmaxcode = (1 << maxbits);
	clear_flg = 0;
	in_count = 1;
	bytes_out = 0;
	ratio = 0;
	proc_l = 0;
	size = 0;
	hsize = HSIZE;

	m_slog.Format("\r\n  \
						  maxmaxcode=[%d]			 \r\n \
						  hsize =[%d]", maxmaxcode,  hsize);
	LOG_OUTP(3, "compress", __FUNCTION__, m_slog);
}

void cl_testcompress::output(int code)
{
	int r_off = offset, bits = n_bits;
	BYTE* bp = buf;

	if (code >= 0)
	{
		bp += (r_off >> 3);
		r_off &= 7;
		*bp = (*bp & rmask[r_off]) | (code << r_off) & lmask[r_off];
		bp++;
		bits -= (8 - r_off);
		code >>= (8 - r_off);
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
				proc_b[proc_l++] = *bp++;
			while (--bits);
			offset = 0;
		}

		if (free_ent > maxcode || (clear_flg > 0))
		{
			if (offset > 0)
			{
				memcpy(&proc_b[proc_l], buf, n_bits);
				proc_l += n_bits;
				bytes_out += n_bits;
			}
			offset = 0;

			if (clear_flg)
			{
				maxcode = MAXCODE(n_bits = INIT_BITS);
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
		{
			memcpy(&proc_b[proc_l], buf, (offset + 7) / 8);
			proc_l += (offset + 7) / 8;
			bytes_out += (offset + 7) / 8;
		}
		offset = 0;
	}
}

int cl_testcompress::getcode()
{
	int code, r_off, bits;
	BYTE* bp = (BYTE*)buf;

	if (clear_flg > 0 || offset >= size || free_ent > maxcode)
	{
		if (free_ent > maxcode)
		{
			n_bits++;
			if (n_bits == maxbits)
				maxcode = maxmaxcode;	// won't get any bigger now
			else
				maxcode = MAXCODE(n_bits);
		}
		if (clear_flg > 0)
		{
			maxcode = MAXCODE(n_bits = INIT_BITS);
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

void cl_testcompress::cl_block()
{
	long int rat;

	checkpoint = in_count + CHECK_GAP;

	if (in_count > 0x007fffff)
	{
		rat = bytes_out >> 8;
		if (rat == 0) 			// Don't divide by zero
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
		cl_hash((int)hsize);
		free_ent = FIRST;
		clear_flg = 1;
		output((int)CLEAR);
	}
}

void cl_testcompress::cl_hash(int hsize)
{ //int	htab[HSIZE];  //HSIZE 압축률에 따라 다르다 
// 결국 htab메모리를 -1 로 초기화 하겠다는 거다
	int* htab_p = htab + hsize;
	long i, m1 = -1;

	i = hsize - 16;  //
	do {				// might use Sys V memset(3) here
		*(htab_p - 16) = m1;
		*(htab_p - 15) = m1;
		*(htab_p - 14) = m1;
		*(htab_p - 13) = m1;
		*(htab_p - 12) = m1;
		*(htab_p - 11) = m1;
		*(htab_p - 10) = m1;
		*(htab_p - 9) = m1;
		*(htab_p - 8) = m1;
		*(htab_p - 7) = m1;
		*(htab_p - 6) = m1;
		*(htab_p - 5) = m1;
		*(htab_p - 4) = m1;
		*(htab_p - 3) = m1;
		*(htab_p - 2) = m1;
		*(htab_p - 1) = m1;
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for (i += 16; i > 0; i--)
		*--htab_p = m1;

}

int cl_testcompress::getrawcode(BYTE* rbuf, int rlen)
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

int cl_testcompress::axpack(BYTE* ib, int il, BYTE* ob)
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
			  bits |= prev;
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
		  bits |= prev;
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
		bits |= _NC;
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

int cl_testcompress::axunpack(BYTE* ib, int il, BYTE* ob)
{
	int	nibb[2], nibble;
	int	flow, ol, chax;
	int	ii, jj, kk;

	flow = 0;
	ol = 0;
	for (ii = 0; ii < il; ii++)
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
				chax = ob[ol - 1];
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


int qmatch(int length)
{
	

	return 0;
}

std::string cl_testcompress::string_compress(const std::string& input)
{
	return "";
}

std::string cl_testcompress::string_decompress(const std::string& input)
{


	return "";
}