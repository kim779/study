// C_TickEx.h : main header file for the C_TICKEX DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
//#include "../../h/axisgdef.h"

struct _tickinfo
{
	char	request[7];
	char	display[7];
	char	reserved[18];
};

#define	sz_TICKINFO	sizeof(_tickinfo)

/////////////////////////////////////////////////////////////////////////////
// CC_TickExApp
// See C_TickEx.cpp for the implementation of this class
//

class CC_TickExApp : public CWinApp
{
public:
	CC_TickExApp();
	virtual	~CC_TickExApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_TickExApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_TickExApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool		CreateImage(CWnd *pView);
	CImageList*	GetCtrlImage() { return m_pImgCtrl.get(); }
	std::shared_ptr<CImageList> getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);
	//CImageList*	GetImage(CWnd *pView, CString path, int Width, int Height, UINT flag, COLORREF crMask);

	COLORREF	GetColor(CWnd *pView, int color);
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd *pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd *pView, COLORREF rColor);

	void	Init(CWnd *pView);
	bool	ReadCondition(CString path, struct _tickinfo *tickinfo);
	void	SaveCondition(CString path, struct _tickinfo *tickinfo);
	bool	readFile(CString path, char *pBytes, int nBytes);
	bool	writeFile(CString path, char *pBytes, int nBytes);

	CString	m_Path;

private:
	std::shared_ptr<CImageList> m_pImgCtrl;

};
