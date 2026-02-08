#pragma once

#include <afxtempl.h>

#define	DA_RIGHT	0	// align right
#define	DA_LEFT		1	// align left
#define	DA_CENTER	1<<1	// align center

#define	DA_SUPPRESSPM	1<<2	// suppress plus minus
#define	DA_SUPPRESSP	1<<3	// suppress 

#define	DA_CORGB	1<<4
#define	DA_CONTRAST	1<<5

#define	DA_ZERO		1<<6

#define	DA_COMMA	1<<7
#define	DA_BLINK	1<<8

#define	DA_WRAP		1<<9
#define	DA_ONLYPOSITIVE	1<<10

#define	DA_BOLD		1<<11


class CAxStatic : public CStatic
{
public:
	CAxStatic();

public:
	bool		m_bVertical;
	int		m_nHeadWidth;
	COLORREF	m_bkClr, m_frClr, m_bxClr;
	COLORREF	m_hbkClr, m_hfrClr;
	COLORREF	m_upRGB, m_dnRGB, m_blkRGB;

private:
	CString		m_text;

	CStringArray	m_headArr, m_dataArr;
	CArray		<DWORD, DWORD>	m_attrArr;

	bool		m_bInitialize;

public:

// Overrides
	//{{AFX_VIRTUAL(CAxStatic)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	void		BeginBlink(int nRow);
	void		SetAttr(int nRow, DWORD Attr);
	void		SetHead(int nRow, CString sDat);
	void		SetData(int nRow, CString sDat);
	void		setHeadColor(COLORREF bkClr, COLORREF frClr);
	void		setColor(COLORREF bkClr, COLORREF frClr, COLORREF bxClr);
	
	CString		SetComma(CString src);
	CString		GetHead(int nRow);
	CString		GetData(int nRow, bool bModified = false);
	CString		Parser(CString &srcstr, CString substr);

	CRect		getRect(int nRow);
	
	DWORD		GetAttr(int nRow);
	
	virtual		~CAxStatic();
private:
	void		drawContrast(CDC *pDC, CRect cellRc, char cStyle, COLORREF clr);

protected:
	//{{AFX_MSG(CAxStatic)
	afx_msg long HandleGetText(WPARAM wParam, LPARAM lParam);
	afx_msg void HandleSetText(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetTextLength(WPARAM , LPARAM );
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

