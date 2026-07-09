// PreViewBar.h: interface for the CPreViewBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREVIEWBAR_H__B8340139_0149_4D33_BFB4_FD6396355747__INCLUDED_)
#define AFX_PREVIEWBAR_H__B8340139_0149_4D33_BFB4_FD6396355747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdAfx.h"
/*#include "mapvar.h"
#include "awWcc/libwcc.h"*/
#include "LoadThread.h"

class CPreViewBar : public CSizingControlBarG
{
public:
	CPreViewBar();
	virtual ~CPreViewBar();
public:
	void EndThread();
	bool compile();
	bool open(CString fileN);
	void setShow(class CAxisPalette* palette, class CAxisDraw* draw, CRect rect, CString root, CString fileN, CString caption, CString backBrush);
protected:
	CBitmap		*m_pBitmap;
	bool		m_bInvalidate;
	CRect		m_ClientRt, m_rect;
	CString		m_filename, m_root, m_path, m_srcN, m_objN, m_mapName;

	class CShow*		m_pShow;
	class CAxisPalette*	m_palette;
	class CAxisDraw*	m_draw;
	CBrush		*m_pBackBrush;
	CLoadThread	*m_pLoadThread;

	CMapToken	m_token[MAX_TOKEN_N];
private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreViewBar)
	//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	LOGFONT setFont(int fsize, CString fname = "±¼¸²Ã¼", BOOL bBold = FALSE, BOOL bItalic = FALSE);
	void DrawBitmap(CDC *pDC);
	//{{AFX_MSG(CPreViewBar)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_PREVIEWBAR_H__B8340139_0149_4D33_BFB4_FD6396355747__INCLUDED_)
