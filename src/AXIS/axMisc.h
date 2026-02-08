// axMisc.h: interface for the CAxMisc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXMISC_H__1E712BA4_0BF3_11D6_8AC5_00001CD7F9BE__INCLUDED_)
#define AFX_AXMISC_H__1E712BA4_0BF3_11D6_8AC5_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	WM_CHILDMSG	WM_USER+666

#define NORMALVIEW	1
#define SCROLLVIEW	2
#define TABLISTVIEW	3

#define	verUPDATE	0
#define	verUSERID	1
#define	verRETRY	2

#ifdef DF_MK_CAPTION
#define  MK_KRX 1
#define  MK_NXT 2
#define  MK_TOT 3
#define  MK_NON 4
#define TITLE_TOT   _T("[통합]")
#define TITLE_KRX	 _T("[KRX]")
#define TITLE_NXT	 _T("[NXT]")

#define	BMP_CNT		12
#define	GROUP_CNT	9
#define IDX_LOCK      0
#define IDX_MARKET 1
#define IDX_PIN         2
#define IDX_SINGLE         3
#define IDX_GROUP		4
#define IDX_FONT			5
#define IDX_COPY			6
#define IDX_HELP			7
#define	IDX_MIN			8
#define	IDX_MAX			9
#define	IDX_CLOSE		10
#define	IDX_RESTORE	11
#define	IDX_ICON			12
#define	IDX_FONTX		13

enum MarketPermission
{
	MARKET_NONE = 0,       // 불가
	MARKET_KRX = 1 << 0,  // 1
	MARKET_NXT = 1 << 1,  // 2
	MARKET_TOT = 1 << 2,  // 4
};
#else
#define	BMP_CNT		10
#define GROUP_CNT	7
#define IDX_PIN		0
#define IDX_SINGLE	1
#define	IDX_GROUP	2
#define	IDX_FONT	3
#define	IDX_COPY	4
#define	IDX_HELP	5
#define	IDX_MIN		6
#define	IDX_MAX		7
#define	IDX_CLOSE	8
#define	IDX_RESTORE	9
#define IDX_ICON	10
#define	IDX_FONTX	11
#endif

#define MBMP_CNT	14
#define MIDX_ICON	0
#define MIDX_HOME	1
#define MIDX_FULL	2
#define MIDX_DUAL	3
#define MIDX_VS1	4
#define MIDX_VS2	5
#define MIDX_VS3	6
#define MIDX_VS4	7
#define MIDX_VS5	8
#define MIDX_VS6	9
#define MIDX_MIN	10
#define MIDX_MAX	11
#define MIDX_CLOSE	12
#define MIDX_RESTORE	13

//#define UB_WIDTH	17
#define UB_WIDTH 15

#define ALL_GROUP	99

//	skin color index define
#define FLINE_CNT	3
#define ACTTITLE_VSBK	166
#define ACTTITLE_VEBK	167
#define ACTTITLE_HSBK	168
#define ACTTITLE_HEBK	169
#define ACTFLINE_1	170
#define ACTFLINE_2	171
#define ACTFLINE_3	165
#define ACTFLINE_4	165
#define ACTFLINE_5	174

#define INACTTITLE_VSBK	166
#define INACTTITLE_VEBK	167
#define INACTTITLE_HSBK	172
#define INACTTITLE_HEBK	169
#define INACTFLINE_1	173
#define INACTFLINE_2	171
#define INACTFLINE_3	165
#define INACTFLINE_4	165
#define INACTFLINE_5	174

#define TAB_CNT		6

#define TAB_LEFT	0
#define TAB_CENTER	1
#define TAB_RIGHT	2

#define TAB_SLEFT	3
#define TAB_SCENTER	4
#define TAB_SRIGHT	5

class CAxMisc  
{
public:
	CAxMisc(CString root, CString regkey);
	virtual ~CAxMisc();

public:
	CString	m_regkey;
	CWnd* m_pMain{};
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
