// FBMemDC.h: interface for the SMemDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMEMDC_H__280493D3_1551_4E6A_A85E_1C5A3A509F1A__INCLUDED_)
#define AFX_SMEMDC_H__280493D3_1551_4E6A_A85E_1C5A3A509F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SMemDC : public CDC  
{
public:
	SMemDC(CDC* pDC, BOOL bMap);
	BOOL	IsValid();
	BOOL	SetSize(int iWidth, int iHeight);
	void	SBitBlt(CDC *pDC);

	void	FreeDC();
	virtual ~SMemDC();

public:
	int			m_Width, m_Height;
	CRect		m_Rect;			// Rectangle of drawing area.
	CBitmap*	m_pBitmap;		// Offscreen bitmap
	CBitmap*	m_pOldBitmap;	// Original GDI object
	bool		m_bValid;
};

#endif // !defined(AFX_SMEMDC_H__280493D3_1551_4E6A_A85E_1C5A3A509F1A__INCLUDED_)
