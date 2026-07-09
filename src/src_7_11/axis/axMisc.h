// axMisc.h: interface for the CAxMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXMISC_H__1E712BA4_0BF3_11D6_8AC5_00001CD7F9BE__INCLUDED_)
#define AFX_AXMISC_H__1E712BA4_0BF3_11D6_8AC5_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define	WM_CHILDMSG	WM_USER+666
#define WM_GETACC	WM_USER+667
#define WM_GETPINCH	WM_USER+668
#define WM_SETPINCH	WM_USER+669
#define WM_TRACE		WM_USER+670

#define NORMALVIEW	1
#define SCROLLVIEW	2
#define TABLISTVIEW	3

#define	verUPDATE	0
#define	verUSERID	1
#define	verRETRY	2

#define	BMP_CNT		22
#define GROUP_CNT	7

#define IDX_COPY	0
#define IDX_SINGLE	1
#define IDX_PIN		2
#define	IDX_DESIZE	3
#define	IDX_CAPTURE	4
#define	IDX_MIN		5
#define	IDX_MAX		6
#define	IDX_CLOSE		7
#define	IDX_RESTORE	8



#define	IDX_GROUP	9
#define	IDX_FONT	3
/*
#define IDX_PIN		0
#define IDX_SINGLE	1
#define	IDX_GROUP	2
#define	IDX_FONT	3

#define	IDX_COPY	4
#define	IDX_DESIZE	4

#define	IDX_CAPTURE	5
#define	IDX_HELP	5

#define	IDX_MIN		6
#define	IDX_MAX		7
#define	IDX_CLOSE		8
#define	IDX_RESTORE	9
*/

#define IDX_ICON	10
#define IDX_V1		11
#define IDX_V2		12
#define IDX_V3		13
#define IDX_V4		14
#define IDX_CHILD	15
#define IDX_TOOLBARBG 16
#define IDX_DUAL	17
//childframe 용 이미지 추가
#define IDX_CHILD_MIN	18
#define IDX_CHILD_MAX	19
#define IDX_CHILD_CLOSE	20
#define IDX_CHILD_RESTORE	21

#define MBMP_CNT	16
#define MIDX_ICON	0
#define MIDX_MIN	1
#define MIDX_MAX	2
#define MIDX_CLOSE	3
#define MIDX_RESTORE	4
#define MIDX_V1		5
#define MIDX_V2		6
#define MIDX_V3		7
#define MIDX_V4		8
#define MIDX_CHILD	9
#define MIDX_DUAL	10


#define UB_WIDTH	16
#define ALL_GROUP	99

//	skin color index define
#define FLINE_CNT	3
#define ACTTITLE_VSBK	166
#define ACTTITLE_VEBK	167
#define ACTTITLE_HSBK	168
#define ACTTITLE_HEBK	169
#define ACTFLINE_1	111
#define ACTFLINE_2	112
#define ACTFLINE_3	113
#define ACTFLINE_4	114
#define ACTFLINE_5	115

#define INACTTITLE_VSBK	166
#define INACTTITLE_VEBK	167
#define INACTTITLE_HSBK	172
#define INACTTITLE_HEBK	169
#define INACTFLINE_1	111
#define INACTFLINE_2	112
#define INACTFLINE_3	113
#define INACTFLINE_4	114
#define INACTFLINE_5	115

#define TAB_CNT		7
#define TAB_LEFT	0
#define TAB_CENTER	1
#define TAB_RIGHT	2
#define TAB_SLEFT	3
#define TAB_SCENTER	4
#define TAB_SRIGHT	5
#define TAB_BKGROUND	6

class _axisRes
{
public:
	_axisRes(CString home);
	~_axisRes();

	HICON			m_hIcon;
	HICON			m_hTray;
	CString			m_home;
	class CNPalette*	m_palette;

	CFont*		GetFont();
	CFont*		GetBoldFont();
	CBitmap*	GetIcon();
	CBitmap*	GetBmp(int index);
	CBitmap*	GetTabImg(int index);
	COLORREF	GetColor(int index);
	CString		GetHome() { return m_home; }

	void		ChangeRES(CString skinName = _T(""));
protected:
	CFont	m_font;
	CFont	m_fontB;
	CBitmap	m_bmp[BMP_CNT];
	CBitmap	m_tab[TAB_CNT];
	CBitmap	m_icon;
};

class CAxMisc  
{
public:
	CAxMisc(CString root, CString regkey);
	virtual ~CAxMisc();

public:
	CString	m_regkey;

protected:
	CString	m_root;
	CMap	< int, int, CString, CString& > m_guide;

public:
	int	MsgBox(CString msg, CString title = _T(""), UINT type = MB_OK);
	void	LoadGuide();
	void	GetGuide(int key, CString& string);
	BOOL	RunVers(int type, CString user = _T(""), CString pass = _T(""), CString cpass = _T(""));

protected:
};

#endif // !defined(AFX_AXMISC_H__1E712BA4_0BF3_11D6_8AC5_00001CD7F9BE__INCLUDED_)
