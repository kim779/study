#if !defined(AFX_FILEBITMAP_H__C2760129_8E23_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_FILEBITMAP_H__C2760129_8E23_11D4_8D2E_00C026A220A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileBitmap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileBitmap window

class CFileBitmap : public CBitmap
{
// Construction
public:
	CFileBitmap();
	~CFileBitmap();

// Attributes
public:
	HGLOBAL		m_hDIB;
//	CPalette	m_Pal;
//	CPalette	m_pPal;
	int			m_nWidth,
				m_nHeight;
	CBitmap		m_bitmap;

// Operations
public:
	void ClearAll();
	BOOL LoadBMP( LPCTSTR sBMPFile );
	BOOL LoadBMP( LPCTSTR sBMPFile, HGLOBAL *phDIB/*, CPalette *pPal*/);
	BOOL LoadBMPImage( LPCTSTR sBMPFile, CBitmap& bitmap/*, CPalette *pPal*/);
	BOOL LoadBMPImage( LPCTSTR sBMPFile);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEBITMAP_H__C2760129_8E23_11D4_8D2E_00C026A220A8__INCLUDED_)
