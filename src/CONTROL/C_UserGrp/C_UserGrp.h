// C_UserGrp.h : main header file for the C_UserGrp DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#define UNUSED_VALUE	-99999999.999

/////////////////////////////////////////////////////////////////////////////
// CC_UserGrpApp
// See C_UserGrp.cpp for the implementation of this class
//

class CC_UserGrpApp : public CWinApp
{
public:
	CC_UserGrpApp();
	~CC_UserGrpApp();

	char*	GetSampleData(int pInfo1, int pInfo2, int *pLen);
	void	SetView(CWnd *pView)	{ m_pView = pView; }
	bool	CreateImage();
	CImageList* GetCtrlImage() { return m_pImgCtrl.get(); }
	std::shared_ptr<CImageList> getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);
	CFont*	GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd* pView, COLORREF rColor);
	COLORREF GetColor(CWnd *pView, int index);

	HINSTANCE m_hDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_UserGrpApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_UserGrpApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd		*m_pView;
	std::shared_ptr<CImageList>	m_pImgCtrl;
};