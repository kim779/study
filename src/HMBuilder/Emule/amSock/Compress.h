// compress.h: interface for the CCompress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACK_H__5B089A64_22D3_11D2_ABD3_006097977DA0__INCLUDED_)
#define AFX_PACK_H__5B089A64_22D3_11D2_ABD3_006097977DA0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define MAXCODE(n_bits)	((1 << (n_bits)) - 1)

#define PBITS		13
#define	BITS		PBITS
#if BITS == 16
# define HSIZE	69001		/* 95% occupancy */
#endif
#if BITS == 15
# define HSIZE	35023		/* 94% occupancy */
#endif
#if BITS == 14
# define HSIZE	18013		/* 91% occupancy */
#endif
#if BITS == 13
# define HSIZE	9001		/* 91% occupancy */
#endif
#if BITS <= 12
# define HSIZE	5003		/* 80% occupancy */
#endif

#define BIT_MASK	0x1f
#define BLOCK_MASK	0x80
#define INIT_BITS 	9		/* initial number of bits/code */
#define CHECK_GAP 	10000		/* ratio check interval */
#define FIRST		257		/* first free entry */
#define	CLEAR		256		/* table clear output code */
#define htabof(i)	htab[i]
#define codetabof(i)	codetab[i]
#define tab_prefixof(i)	codetabof(i)
#define tab_suffixof(i)	((BYTE *)(htab))[i]
#define de_stack	((BYTE *)&tab_suffixof(1<<BITS))

class CCompress  
{
public:
	CCompress();
	virtual ~CCompress();

public:
	int	compress(BYTE *rbuf, int rlen, BYTE *cbuf);
	int	decompress(BYTE *rbuf, int rlen, BYTE *cbuf);
	int	axpack(BYTE* ib, int il, BYTE* ob);
	int	axunpack(BYTE* ib, int il, BYTE* ob);

private:
	void	varinit();
	void	output(int code);
	int	getcode();
	void	cl_block();
	void	cl_hash(int hsize);
	int	getrawcode(BYTE *rbuf, int rlen);

private:
	BYTE	buf[BITS];

	int	n_bits;			// number of bits/code
	int	maxbits;		// user settable max # bits/code
	int	maxcode;		// maximum code, given n_bits
	int	maxmaxcode;		// should NEVER generate this code
	int	htab[HSIZE];
	unsigned short codetab[HSIZE+4096];
	int	hsize;			// for dynamic table sizing
	int	free_ent;		// first unused entry
	int	block_compress;
	int	clear_flg;
	int	checkpoint;
	int	offset;
	int	size;
	int	in_count;		// length of input
	int	bytes_out;		// length of compressed output
	int	out_count;		// # of codes output (for debugging)
	int	ratio;

	int	proc_l;
	int	raw_l, max_l;
	BYTE *proc_b;		
	BYTE *raw_b;
};

#endif // !defined(AFX_PACK_H__5B089A64_22D3_11D2_ABD3_006097977DA0__INCLUDED_)
