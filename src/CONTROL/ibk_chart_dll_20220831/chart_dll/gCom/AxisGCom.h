// AxisGCom.h : main header file for the AXISGCOM DLL
//

#if !defined(AFX_AXISGCOM_H__D48195E1_0507_49DD_82B5_234788B42F9F__INCLUDED_)
#define AFX_AXISGCOM_H__D48195E1_0507_49DD_82B5_234788B42F9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "definfo.h"
#include "../../h/axisgwin.h"
#include "../../h/axisgenv.h"
#include "../../h/axisvar.h"

enum { imgDate = 1, imgTime, imgNTime, imgNBtn, imgCode, imgIEtc, imgIOpt, imgICfg, imgTool, imgTool2, imgCtrl, imgCtrl2, 
		/** macho add **/imgIUnit};

#define COLOR_PAL_BG	PALETTERGB(27, 79, 129)
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2)
#define	COLOR_WHITE	PALETTERGB(255, 255, 255)	

/////////////////////////////////////////////////////////////////////////////
// CAxisGComApp
// See AxisGCom.cpp for the implementation of this class
//

class CAxisGComApp : public CWinApp
{
public:
	CAxisGComApp();
	virtual ~CAxisGComApp();

private:
	CImageList	*m_pImgDate;
	CImageList	*m_pImgTime;
	CImageList	*m_pImgNTime;
	CImageList	*m_pImgNBtn;
	CImageList	*m_pImgCode;
	CImageList	*m_pImgIEtc;
	CImageList	*m_pImgIOpt;
	CImageList	*m_pImgICfg;
	CImageList	*m_pImgCtrl;
	CImageList	*m_pImgCtrl2;
	CImageList	*m_pImgTool;
	CImageList	*m_pImgTool2;
	//** macho add
	CImageList	*m_pImgUnit;

	CString		m_root;
	CString		m_user;

public:
	char*		GetHome(CWnd* pwndView);
	char*		GetName(CWnd* pwndView);

	CFont*		GetFont(CWnd* pwndView, int iPoint, CString strName, int iBold = 0, bool bItalic = false);
	CPen*		GetPen(CWnd* pwndView, int iStyle, int iWidth, COLORREF rgbLine);
	CBrush*		GetBrush(CWnd* pwndView, COLORREF rgbBrush);
	CBitmap*	GetBitmap(CWnd* pwndView, CString strPath);

	CString		GetImagePath(CWnd* pwndView, CString imgName);
	CImageList*	GetImage(int imgType, CString path);

	CString		GetUserPath(CWnd* pwndView);

/*
	CString		GetRoot(CWnd* pView);
	CString		GetUser(CWnd* pView);
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd *pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd *pView, COLORREF rColor);
*/

private:
	CImageList*	GetImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisGComApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisGComApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISGCOM_H__D48195E1_0507_49DD_82B5_234788B42F9F__INCLUDED_)
