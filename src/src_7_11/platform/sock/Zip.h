// Zip.h: interface for the CZip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZIP_H__78E4280A_6A97_4F7F_9F1F_FFAE3BDFAD36__INCLUDED_)
#define AFX_ZIP_H__78E4280A_6A97_4F7F_9F1F_FFAE3BDFAD36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	PBITS		16
#define	BITS		PBITS
#if BITS == 16
#define	HSIZE	69001		// 95% occupancy
#endif
#if BITS == 15
#define HSIZE	35023		// 94% occupancy
#endif
#if BITS == 14
#define HSIZE	18013		// 91% occupancy
#endif
#if BITS == 13
#define HSIZE	9001		// 91% occupancy
#endif
#if BITS <= 12
#define HSIZE	5003		// 80% occupancy
#endif

#define BIT_MASK	0x1f
#define BLOCK_MASK	0x80
#define INIT_BITS	9
#define CHECK_GAP	10000		// ratio check interval
#define FIRST		257		// first free entry
#define	CLEAR		256		// table clear output code

#define MAXCODE(n_bits)	((1 << (n_bits)) - 1)
#define	htabof(i)	htab[i]
#define	codetabof(i)	codetab[i]
#define	tab_prefixof(i)	codetabof(i)
#define	tab_suffixof(i)	((BYTE *)(htab))[i]
#define	de_stack	((BYTE *)&tab_suffixof(1<<BITS))

class CZip  
{
public:
	CZip();
	virtual ~CZip();

public:
	BOOL	Compress(char* ofname, char* cfname);
	BOOL	Decompress(char* ofname, char* cfname);

protected:
	void	init();
	int	getcode();
	void	cl_hash(int size);
	void	cl_block();
	BOOL	output(int code);
	void	close(bool in, bool out, char* filename = NULL);
	int	file_size(char* filename);


private:
	int	n_bits;
	int	maxbits;
	int	maxcode;
	int	maxmaxcode;

	char	buf[BITS];
	int	htab[HSIZE];
	unsigned short codetab [HSIZE];
	int	hsize;
	int	free_ent;
	int	block_compress;
	int	clear_flg;
	int	ratio;
	int	checkpoint;

	int	offset;
	int	in_count;
	int	bytes_out;
	int	out_count;
};

#endif // !defined(AFX_ZIP_H__78E4280A_6A97_4F7F_9F1F_FFAE3BDFAD36__INCLUDED_)
