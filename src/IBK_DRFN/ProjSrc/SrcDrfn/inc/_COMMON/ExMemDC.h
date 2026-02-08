#if !defined(__EXMEMDC_H__)
#define __EXMEMDC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExMemDC : public CDC  
{
public:
    DECLARE_DYNAMIC(CExMemDC);
    
    CExMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor);
    virtual ~CExMemDC();

	void Discard();
	void FromDC();
	CBitmap& GetBitmap();

	void SetStretchMode(BOOL bMode, CSize sizeImage);

protected:
    
	CDC*	m_pDC;			// Saves the CDC passed in constructor.
	BOOL	m_bValid;		// Flag used for autodraw in destructor.
	CRect	m_rc;			// Rectangle of the drawing area.
	CBitmap m_bitmap;		// Offscreen bitmap.
	HBITMAP m_hOldBitmap;	// Original GDI object.

	CSize	m_sizeImage;
	BOOL	m_bStretch;
	
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CBitmap& CExMemDC::GetBitmap() {
	return m_bitmap;
}
AFX_INLINE void CExMemDC::Discard() {
	m_bValid = FALSE;
}
AFX_INLINE void CExMemDC::FromDC() {
	BitBlt(0, 0, m_rc.Width(), m_rc.Height(), m_pDC, m_rc.left, m_rc.top, SRCCOPY);            
}

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__EXMEMDC_H__)
