// NDib.h: interface for the CNDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDIB_H__ADDF5CF0_0213_46C9_8376_3AFCA63BFD8D__INCLUDED_)
#define AFX_NDIB_H__ADDF5CF0_0213_46C9_8376_3AFCA63BFD8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* flags for _lseek */
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0


#define MAXREAD  32768             /* Number of bytes to be read during */

/* Header signatutes for various resources */
#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* macro to determine if resource is a DIB */
#define ISDIB(bft) ((bft) == BFT_BITMAP)

/* Macro to align given value to the closest DWORD (unsigned long ) */
#define ALIGNULONG(i)   ((i+3)/4*4)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((i+31)/32*4)

class CNDib  
{
public:
	CNDib();
	virtual ~CNDib();
protected:
	HANDLE	m_dib;         /* Handle to current memory DIB         */
	HANDLE	m_bi;          /* Handle to current bitmap info struct */

	BOOL	m_bMemoryDIB;
	BOOL	m_bLegitDraw;
	DWORD	m_dwOffset{};

public:
	BOOL	SetNDIB(CString file);
	WORD	PaletteSize(VOID FAR *pv);
	HANDLE	GetNDIB();
	HANDLE	GetNBI();
protected:
	BOOL	InitNDIB(CString file);
	void	FreeNDib();
	HANDLE	ReadDibBitmapInfo(int fh);
	BOOL	DibInfo (HANDLE hbi,LPBITMAPINFOHEADER lpbi);
	HANDLE	OpenDIB (LPCSTR szFile);

	WORD	DibNumColors (VOID FAR *pv);
	DWORD	lread (int fh,VOID far * pv,DWORD  ul);

};

#endif // !defined(AFX_NDIB_H__ADDF5CF0_0213_46C9_8376_3AFCA63BFD8D__INCLUDED_)
