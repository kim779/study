#pragma once


#pragma pack(1)
	typedef struct _STRUCT_LBDATA
	{
		DWORD_PTR	dwItemData;	// 64-bit value associated with an item in the list box
		LPVOID		pData;		// Pointer associated with an item in the list box
		int		nImage;		// Image associated with an item in the list box. -1 if no image.
		UINT		nFormat;	// Text alignment
		DWORD		dwFlags;	// Bit 0 - Item is disabled

		_STRUCT_LBDATA::_STRUCT_LBDATA()
		{::ZeroMemory(this, sizeof(_STRUCT_LBDATA));}
	} STRUCT_LBDATA;
#pragma pack()

// CScListBox

class CScListBox : public CListBox
{
	DECLARE_DYNAMIC(CScListBox)

public:
	CScListBox();
	virtual ~CScListBox();

	int	AddString(LPCTSTR lpszItem, int nImage = -1L);
	void	ResetContent();
	void	SetImageList(CImageList* pImageList);

protected:
	enum { ST_FULLROWSELECT = 0,	// Hilight full list box item (Default)
		ST_FULLTEXTSELECT,	// Hilight half list box item (Part containing text)
		ST_TEXTSELECT };	// Hilight only list box text

	BYTE		m_byRowSelect;
	CImageList*	m_pImageList;
	SIZE		m_szImage;		// Size of each image in image list
	CFont		m_font;

	void	setFont(LOGFONT& lf, int fsize, CString fname = "", BOOL bBold = FALSE, BOOL bItalic = FALSE);
	void	enableItem(int nIndex, BOOL bEnable = TRUE, BOOL bRepaint = TRUE);
	BOOL	isItemEnabled(int nIndex);
	void	setRowSelect(BYTE byRowSelect = ST_FULLROWSELECT, BOOL bRepaint = TRUE);

private:
	void	freeResources();
	int	replaceItemData(int nIndex, DWORD dwItemData, LPVOID pData, int nImage, DWORD dwFlags, BYTE byMask);
	void	deleteItemData(int nIndex);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMS);

	virtual DWORD OnDrawTextBackground(int nIndex, CDC* pDC, CRect* prcItem, CRect* prcText, BOOL bIsDisabled, BOOL bIsSelected, COLORREF crSuggestedColor);
	virtual DWORD OnDrawIconBackground(int nIndex, CDC* pDC, CRect* prcItem, CRect* prcIcon, BOOL bIsDisabled, BOOL bIsSelected, COLORREF crSuggestedColor);
	virtual DWORD OnDrawIcon(int nIndex, CDC* pDC, CRect* prcItem, CRect* prcIcon, int nImage, BOOL bIsDisabled, BOOL bIsSelected);

	afx_msg void OnDestroy();
	afx_msg BOOL OnReflectedDblclk();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};