// AcntDialogBar.cpp: implementation of the CAcntDialogBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "afxpriv.h"    // for CDockContext
#include "nbutton.h"

#include "basebar0.h"
#include "AcntDialogBar.h"
#include "fxIGrid2.h"
#include "MenuXP.h"
#include "../../axis/axmsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/ledger.h"
#include "../../h/group.h"




#define ACC_DEPT 3
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	LEN_USID	12
#define LEN_USPW	8
#define	LEN_GRPD	4
#define LEN_SYNF	1 
#define	LEN_ECOD	4
#define	LEN_EMSG	80
#define	LEN_NREC	4    //반복
#define	LEN_ACNO	11
#define	LEN_ACNM	40
#define	LEN_ACPW	8
//#define	LEN_ACPW	44
#define	LEN_GREC	4	 //이중반복
#define	LEN_GPID	4
#define	LEN_FLAG	1

#define LEN_GPNM	40

#define IDC_COMBO_GRP	20001
#define IDC_SEARCH_ACNT		20002
#define IDC_EDIT_ACNT		20003

#define LBUTTONBASE1	2000
#define LBUTTONBASE2	2001

#define VARWIDTH_ACNT 306


typedef struct _PIHOAGLS_MID
{
	char usid[12];
} PIHOAGLS_MID, *PPIHOAGLS_MID;

#define L_PIHOAGLS_MID	sizeof(_PIHOAGLS_MID)

typedef struct _PIHOACLS_MID
{
	char usid[12];
	char uspw[16];
	char gpid[ 4];
	char synf[ 1];
} PIHOACLS_MID, *PPIHOACLS_MID;

#define L_PIHOACLS_MID	sizeof(_PIHOACLS_MID)

typedef struct _IAccnm
{
	char szIn[5];
	char zEmpNo[9];
	char zEmpPwd[8];
	char zPrdtTp[1];

} IACCNM, *PIACCNM;

#define L_IAccnm	sizeof(_IAccnm)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CAcntDialogBar dialog

#pragma warning (disable : 26409)
#pragma warning (disable : 26408)
#pragma warning (disable : 26401)
#pragma warning (disable : 26400)

CAcntDialogBar::CAcntDialogBar(CWnd* parent, bool (*callback)(int, WPARAM, LPARAM), CWnd* wizard):
	m_clrBtnHilight(::GetSysColor(COLOR_BTNHILIGHT)),
	m_clrBtnShadow(::GetSysColor(COLOR_BTNSHADOW))
{
	m_parent	= parent;
	m_axiscall	= callback;
	m_wizard = wizard;
	m_cxGripper	= 0;//20;
	m_cxEdge	= 0;
	m_cxBorder	= 0;
	m_cyBorder	= 3;		
	m_cCaptionSize	= 0;//GetSystemMetrics(SM_CYSMCAPTION);
	m_cMinWidth	= VARWIDTH_ACNT + m_cxBorder*2;
	m_cMinHeight	= BANDHEIGHT*3 + CONTROLH + BANDGAP*2 + m_cCaptionSize + m_cyBorder*4;
	
	m_sizeMin	= CSize(m_cMinWidth, m_cMinHeight);
	m_sizeHorz	= CSize(m_cMinWidth, m_cMinHeight);
	m_sizeVert	= CSize(m_cMinWidth, m_cMinHeight);
	m_sizeFloat	= CSize(m_cMinWidth, m_cMinHeight*2);
	m_bTracking	= FALSE;
	m_bInRecalcNC	= FALSE;
	m_grid		= NULL;
	m_grp		= NULL;
	m_search	= NULL;
	//m_edit		= NULL;
	TotalRefresh = false;
	m_brushBkgd.CreateSolidBrush(RGB(242, 242, 242));
	//m_axis = new CAxisCommon(wizard);
	//wizard->SendMessage(
	
	long	rc = 0;
	m_data = _T("");
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), loadACC, 0);
	m_data = (rc ? (char *)rc : _T(""));

	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), getDEPT, 0);
	m_dept = rc ? (char *)rc : _T("");
	
}

CAcntDialogBar::~CAcntDialogBar()
{
	if (m_grid)		delete m_grid;
	if (m_grp)		delete m_grp;
	if (m_search)	delete m_search;
	//if (m_edit)		delete m_edit;
}

BEGIN_MESSAGE_MAP(CAcntDialogBar, CControlBar)
	//{{AFX_MSG_MAP(CAcntDialogBar)
	ON_WM_NCCALCSIZE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+13, OnGridDblClk)
	ON_MESSAGE(WM_XMSG, OnGrpEdit)
	ON_CBN_SELENDOK(IDC_COMBO_GRP, OnGroupSelEndOk)
//	ON_BN_CLICKED(IDC_SEARCH, OnGrpEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcntDialogBar message handlers

void CAcntDialogBar::OnUpdateCmdUI(class CFrameWnd *pTarget, int bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

BOOL CAcntDialogBar::Create(CWnd* pParentWnd/*, CDialog *pDialog, CString &pTitle*/, UINT nID, DWORD dwStyle) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(pParentWnd);   // must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	// save the style -- AMENDED by Holger Thiele - Thankyou
	m_dwStyle = dwStyle & CBRS_ALL;

	// create the base window
	CString wndclass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), m_brushBkgd, 0);
	if (!CWnd::Create(wndclass, "계좌 연동", dwStyle|WS_CLIPCHILDREN,
		CRect(0,0,m_cMinWidth,m_cMinHeight), pParentWnd, 0xE807))  //vc2019 계좌 다이아로그바가  안보여서 0->0xE807
		return FALSE;
	
//	m_sizeHorz = m_sizeVert = m_sizeFloat = rc.Size();
	//m_sizeHorz.cy += m_cxEdge + m_cxBorder;
	//m_sizeVert.cx += m_cxEdge + m_cxBorder;

	m_grp = new CComboBox();
	if (!m_grp->Create(WS_CHILD | WS_VISIBLE /*| WS_CLIPSIBLINGS */| CBS_DROPDOWNLIST ,CRect(28,20,158,1000), this, IDC_COMBO_GRP))
	{
		delete m_grp;
		m_grp = NULL;
		return false;
	}
	m_grp->SetFont(&Axis::font);
	m_grp->SetDroppedWidth(175);
	
	const DWORD dwStyle2 = WS_VISIBLE|WS_CHILD/*|WS_CLIPCHILDREN|WS_CLIPSIBLINGS*/;
	
	m_search = new CNButton(IDC_SEARCH_ACNT);
	if (!m_search->Create(NULL, "도구", dwStyle2, CRect(158, 19, 158+36, 39), this, -1))
	{
		delete m_search;
		m_search = NULL;
		return false;
	}
	else
	{
		m_search->set_ToolTipText("도구");
		m_search->set_Image("TK_2BTN2");
		
	}
	
	/*
	m_edit = new CNButton(IDC_EDIT);
	if (!m_edit->Create(NULL, "편집", dwStyle2, CRect(166, 19, 202, 39), this, -1))
	{
		delete m_edit;
		m_edit = NULL;
		return false;
	}
	else
	{
		m_edit->set_ToolTipText("그룹 편집");
		m_edit->set_Image("TK_2BTN2");
	}
	*/

	m_grid = new CfxIGrid2();
	if (!m_grid->Create(IGRIDCLASS, "Account list", 
		WS_VISIBLE|WS_CHILD/*|WS_CLIPSIBLINGS*/, CRect(0, 0, 0, 0), this, (int) m_grid))
	{
		delete m_grid;
		m_grid = NULL;
		return false;
	}
	m_grid->m_pFont = &Axis::font;
	m_grid->m_bFillEmpty = TRUE;
	m_grid->m_bSelection = TRUE;
	m_grid->m_bDataLine  = FALSE;
	m_grid->m_bColGubnLine = TRUE;
	m_grid->m_b3DHead    = FALSE;
	m_grid->m_bEnableTip = FALSE;
	m_grid->ClearColumn();
	
	
	CString	sAttr, value;
	long rowN{}, colN{};
	rowN = 400; colN = 2;
	CString keys;
	CRect rc;
	GetClientRect(rc);
	long lSumWidth = 0;
	const long lHeight = rc.Height();// rowN * 18;

	CIColumn iCol;
	CDC *pDC = GetDC();
	const CFont *pOldFont = pDC->SelectObject(&Axis::font);
	ReleaseDC(pDC);

	/*
	iCol.m_dAttr = IGAT_HIDDEN;
	iCol.m_lWidth = 0;
	m_grid->AddQColumn(iCol);
	*/
	iCol.m_sHeadCaption = "계좌번호";
	
	sAttr = "IGAT_CENTER|IGAT_EDITFORMAT|IGAT_HIDDEN";
	
	iCol.m_dAttr = GetAttrByStr(sAttr);
	iCol.m_dAttr |= IGAT_USERCOLOR;
	
	iCol.m_lWidth = 0;
	if ((iCol.m_dAttr & IGAT_HIDDEN) != IGAT_HIDDEN)
		iCol.m_lWidth += 0;
	iCol.m_sEditFormat = "999-99-999999";
	//iCol.m_sEditFormat = "99-999999";
	//iCol.m_sEditFormat.Empty();
	
	lSumWidth += iCol.m_lWidth;
	
	m_grid->AddQColumn(iCol);
//첫번째 컬럼
	iCol.m_sHeadCaption = "계좌번호";
	const CSize sz = 55;
	sAttr = "IGAT_CENTER|IGAT_EDITFORMAT";

	iCol.m_dAttr = GetAttrByStr(sAttr);
	iCol.m_dAttr |= IGAT_USERCOLOR;

	iCol.m_lWidth = sz.cx;
	if ((iCol.m_dAttr & IGAT_HIDDEN) != IGAT_HIDDEN)
		iCol.m_lWidth += 5;
	//iCol.m_sEditFormat = "999-99-999999";
	iCol.m_sEditFormat = "99-999999";
	//iCol.m_sEditFormat.Empty();

	lSumWidth += iCol.m_lWidth;
	
	m_grid->AddQColumn(iCol);
//두번째 컬럼
	iCol.m_sHeadCaption = "계좌명";
	
	sAttr = "IGAT_LEFT";

	iCol.m_dAttr = GetAttrByStr(sAttr);
	iCol.m_dAttr |= IGAT_USERCOLOR;

	iCol.m_lWidth = 220;
	if ((iCol.m_dAttr & IGAT_HIDDEN) != IGAT_HIDDEN)
		iCol.m_lWidth += 5;
	iCol.m_sEditFormat.Empty();

	lSumWidth += iCol.m_lWidth;
	m_grid->AddQColumn(iCol);
//세번째 컬럼
	iCol.m_sHeadCaption = "비밀번호";
	
	sAttr = "IGAT_LEFT|IGAT_HIDDEN";

	iCol.m_dAttr = GetAttrByStr(sAttr);
	iCol.m_dAttr |= IGAT_USERCOLOR;

	iCol.m_lWidth = 0;
	if ((iCol.m_dAttr & IGAT_HIDDEN) != IGAT_HIDDEN)
		iCol.m_lWidth += 0;
	iCol.m_sEditFormat.Empty();

	lSumWidth += iCol.m_lWidth;
	m_grid->AddQColumn(iCol);
	/*
	//네번째 컬럼
	iCol.m_sHeadCaption = "계좌번호";
	
	sAttr = "IGAT_CENTER|IGAT_EDITFORMAT|IGAT_HIDDEN";
	
	iCol.m_dAttr = GetAttrByStr(sAttr);
	iCol.m_dAttr |= IGAT_USERCOLOR;
	
	iCol.m_lWidth = 0;
	if ((iCol.m_dAttr & IGAT_HIDDEN) != IGAT_HIDDEN)
		iCol.m_lWidth += 0;
	//iCol.m_sEditFormat = "999-99-999999";
	iCol.m_sEditFormat = "99-999999";
	//iCol.m_sEditFormat.Empty();
	
	lSumWidth += iCol.m_lWidth;
	
	m_grid->AddQColumn(iCol);
	*/

	m_grid->SetWindowPos(NULL, 1, 40, lSumWidth + GetSystemMetrics(SM_CXVSCROLL), lHeight, SWP_NOZORDER|SWP_NOMOVE);
	
	m_grid->m_headBgColor = Axis::GetColor(74);
	m_grid->m_lineColor   = Axis::GetColor(75);
	m_grid->m_firstRowColor = Axis::GetColor(68);
	m_grid->m_b2RowColor    = Axis::GetColor(77);
	m_grid->m_selBgColor	= Axis::GetColor(78);
	//m_axis = new CAxisCommon(parent);
	//CString data = CAxisCommon::Variant(accountCC);
	//AfxMessageBox(data);
	//InitGridData();
	/*
	CString	Path;
	Path.Format("%s\\%s\\%s\\GrpList.dat", Axis::home, USRDIR, Axis::user);
	if (_access(Path,4)<0)
		QueryGroupList("");
	else
		ReadGroup();
	*/

	Refresh813(0);
	return TRUE;
}

CSize CAcntDialogBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect rc;

	m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP)->GetWindowRect(rc);
	const int nHorzDockBarWidth = bStretch ? 32767 : rc.Width() + 4;
	m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT)->GetWindowRect(rc);
	const int nVertDockBarHeight = bStretch ? 32767 : rc.Height() + 4;

	if(IsFloating())
		return CSize(m_cMinWidth, m_sizeFloat.cy);
	else if (bHorz)
		return CSize(m_cMinWidth/*nHorzDockBarWidth*/, m_sizeHorz.cy);
	else
		return CSize(m_cMinWidth/*m_sizeVert.cx*/, nVertDockBarHeight);
}

CSize CAcntDialogBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	// Rob Wolpov 10/15/98 Added support for diagonal sizing
	if (IsFloating())
	{
		// Enable diagonal arrow cursor for resizing
		GetParent()->GetParent()->ModifyStyle(MFS_4THICKFRAME,0);
	}    
	if (dwMode & (LM_HORZDOCK | LM_VERTDOCK))
	{
		SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
			SWP_NOACTIVATE | SWP_FRAMECHANGED);
		m_pDockSite->RecalcLayout();
	    return CControlBar::CalcDynamicLayout(nLength,dwMode);
	}

	if (dwMode & LM_MRUWIDTH || dwMode & LM_COMMIT)
		return CSize(m_cMinWidth, m_sizeFloat.cy);

//	if (dwMode & LM_COMMIT)
//	{
//		m_sizeFloat.cx = VARWIDTH;
//		return m_sizeFloat;
//	}

	// Rob Wolpov 10/15/98 Added support for diagonal sizing
	if (IsFloating())
	{
		RECT	window_rect;
		POINT	cursor_pt;

		GetCursorPos(&cursor_pt);
		GetParent()->GetParent()->GetWindowRect(&window_rect);

		switch (m_pDockContext->m_nHitTest)
		{
		case HTTOPLEFT:
			m_sizeFloat.cx = max(window_rect.right - cursor_pt.x,
				m_cMinWidth) - m_cxBorder;
			m_sizeFloat.cy = max(window_rect.bottom - m_cCaptionSize - 
				cursor_pt.y,m_cMinHeight) - 1;
			m_pDockContext->m_rectFrameDragHorz.top = min(cursor_pt.y,
				window_rect.bottom - m_cCaptionSize - m_cMinHeight) - 
				m_cyBorder;
			m_pDockContext->m_rectFrameDragHorz.left = min(cursor_pt.x,
				window_rect.right - m_cMinWidth) - 1;
			return m_sizeFloat;

		case HTTOPRIGHT:
			m_sizeFloat.cx = max(cursor_pt.x - window_rect.left,
				m_cMinWidth);
			m_sizeFloat.cy = max(window_rect.bottom - m_cCaptionSize - 
				cursor_pt.y,m_cMinHeight) - 1;
			m_pDockContext->m_rectFrameDragHorz.top = min(cursor_pt.y,
				window_rect.bottom - m_cCaptionSize - m_cMinHeight) - 
				m_cyBorder;
			return m_sizeFloat;
			
		case HTBOTTOMLEFT:
			m_sizeFloat.cx = max(window_rect.right - cursor_pt.x,
				m_cMinWidth) - m_cxBorder;
			m_sizeFloat.cy = max(cursor_pt.y - window_rect.top - 
				m_cCaptionSize,m_cMinHeight);
			m_pDockContext->m_rectFrameDragHorz.left = min(cursor_pt.x,
				window_rect.right - m_cMinWidth) - 1;
			return m_sizeFloat;
			
		case HTBOTTOMRIGHT:
			m_sizeFloat.cx = max(cursor_pt.x - window_rect.left,
				m_cMinWidth);
			m_sizeFloat.cy = max(cursor_pt.y - window_rect.top - 
				m_cCaptionSize,m_cMinHeight);
			return m_sizeFloat;
		}
	}
	
	if (dwMode & LM_LENGTHY)
		return CSize(m_cMinWidth, m_sizeFloat.cy = max(m_sizeMin.cy, nLength));
	else
		return CSize(m_cMinWidth, m_sizeFloat.cy);
}

void CAcntDialogBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	
		GetWindowRect(m_rectBorder);
	m_rectBorder = CRect(0, 0, m_rectBorder.Width(), m_rectBorder.Height());

	DWORD dwBorderStyle = m_dwStyle | CBRS_BORDER_LEFT;//ANY;

	switch(m_nDockBarID)
	{
	case AFX_IDW_DOCKBAR_TOP:
		dwBorderStyle &= ~CBRS_BORDER_BOTTOM;
		lpncsp->rgrc[0].left += m_cxGripper;
		lpncsp->rgrc[0].bottom += -m_cxEdge;
		lpncsp->rgrc[0].top += m_cxBorder;
		lpncsp->rgrc[0].right += -m_cxBorder;
		m_rectBorder.top = m_rectBorder.bottom - m_cxEdge;
		break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		dwBorderStyle &= ~CBRS_BORDER_TOP;
		lpncsp->rgrc[0].left += m_cxGripper;
		lpncsp->rgrc[0].top += m_cxEdge;
		lpncsp->rgrc[0].bottom += -m_cxBorder;
		lpncsp->rgrc[0].right += -m_cxBorder;
		m_rectBorder.bottom = m_rectBorder.top + m_cxEdge;
		break;
	case AFX_IDW_DOCKBAR_LEFT:
		dwBorderStyle &= ~CBRS_BORDER_RIGHT;
		lpncsp->rgrc[0].right += -m_cxEdge;
		lpncsp->rgrc[0].left += m_cxBorder;
		lpncsp->rgrc[0].bottom += -m_cxBorder;
		lpncsp->rgrc[0].top += m_cxGripper;
		m_rectBorder.left = m_rectBorder.right - m_cxEdge;
		break;
	case AFX_IDW_DOCKBAR_RIGHT:
		dwBorderStyle &= ~CBRS_BORDER_LEFT;
		lpncsp->rgrc[0].left += m_cxEdge;
		lpncsp->rgrc[0].right += -m_cxBorder;
		lpncsp->rgrc[0].bottom += -m_cxBorder;
		lpncsp->rgrc[0].top += m_cxGripper;
		m_rectBorder.right = m_rectBorder.left + m_cxEdge;
		break;
	default:
		m_rectBorder.SetRectEmpty();
		break;
	}

	SetBarStyle(dwBorderStyle);
}

void CAcntDialogBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SelectObject(&Axis::font);
	DrawGripper(dc);

	
	// Do not call CControlBar::OnPaint() for painting messages
}

DWORD CAcntDialogBar::GetAttrByStr(CString sAttr)
{
	sAttr.Replace("IGAT_COMMA", "1");
	sAttr.Replace("IGAT_CONTRAST", "2");
	sAttr.Replace("IGAT_CONDITION", "4");
	sAttr.Replace("IGAT_CONDITIONx", "8");
	sAttr.Replace("IGAT_ZEROSUP", "10");
	sAttr.Replace("IGAT_ZEROSUPx", "20");
	sAttr.Replace("IGAT_EDITFORMAT", "40");
	sAttr.Replace("IGAT_MOUSELINE", "1000");
	sAttr.Replace("IGAT_MOUSEHAND", "2000");
	sAttr.Replace("IGAT_USERIMAGE", "4000000");
	sAttr.Replace("IGAT_USERCOLOR", "8000000");
	sAttr.Replace("IGAT_LEFT", "10000000");
	sAttr.Replace("IGAT_CENTER", "20000000");
	sAttr.Replace("IGAT_RIGHT", "40000000");
	sAttr.Replace("IGAT_HIDDEN", "80000000");
	
	DWORD dwAttr = 0;
	while (!sAttr.IsEmpty())
	{
		CString sVal = parseX(sAttr, "|");
		dwAttr |= _httoi(sVal);
	}
	return dwAttr;
}

int CAcntDialogBar::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

CString CAcntDialogBar::parseX(CString &srcstr, CString substr)
{
	const int nFind = srcstr.Find(substr);
	if (nFind == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(nFind);
		srcstr = srcstr.Mid(nFind + substr.GetLength());
		return temp;
	}
}

void CAcntDialogBar::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);
	if (m_grid)
	{
		CRect rc;
		GetClientRect(rc);
		const long lSumWidth = rc.Width();
		const long lHeight = rc.Height() - 45;// rowN * 18;
		m_grid->SetWindowPos(NULL, 2, 40, lSumWidth-4 /*+ GetSystemMetrics(SM_CXVSCROLL)*/, lHeight, SWP_NOZORDER);
	}
	// TODO: Add your message handler code here
	
}

void CAcntDialogBar::DrawGripper(CDC & dc)
{
	// no gripper if floating
	//if( m_dwStyle & CBRS_FLOATING )
	//	return;

	const int	gH = 20, gW = 40;
	// -==HACK==-
	// in order to calculate the client area properly after docking,
	// the client area must be recalculated twice (I have no idea why)
	m_pDockSite->RecalcLayout();
	// -==END HACK==-

	CRect gripper;
	GetWindowRect( gripper );
	
	ScreenToClient( gripper );
	gripper.OffsetRect( -gripper.left, -gripper.top );

	if( m_dwStyle & CBRS_ORIENT_HORZ )
	{
		// gripper at left
		m_rectGripper.top	= gripper.top + 40;
		m_rectGripper.bottom	= gripper.bottom;
		m_rectGripper.left	= gripper.left;
		m_rectGripper.right	= gripper.left + gH;

		// draw close box
		m_rectClose.left = gripper.left + 7;
		m_rectClose.right = m_rectClose.left + 12;
		m_rectClose.top = gripper.top + 10;
		m_rectClose.bottom = m_rectClose.top + 12;
		dc.DrawFrameControl(m_rectClose, DFC_CAPTION, DFCS_CAPTIONCLOSE);

		// draw docking toggle box
		m_rectUndock = m_rectClose;
		m_rectUndock.OffsetRect(0,13);
		dc.DrawFrameControl(m_rectUndock, DFC_CAPTION, DFCS_CAPTIONMAX);

		gripper.top += 38;
		gripper.bottom -= 10;
		gripper.left += 10;
		gripper.right = gripper.left+3;
		gripper.OffsetRect(0, -4);	
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );

		gripper.OffsetRect(4, 0);
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );
	}
	else
	{
		// gripper at top
		m_rectGripper.top	= gripper.top;
		m_rectGripper.bottom	= gripper.top + gH;
		m_rectGripper.left	= gripper.left;
		m_rectGripper.right	= gripper.right - 40;

		// draw close box
		m_rectClose.right = gripper.right - 10;
		m_rectClose.left = m_rectClose.right - 11;
		m_rectClose.top = gripper.top + 7;
		m_rectClose.bottom = m_rectClose.top + 11;
		m_rectClose.OffsetRect(2, -2);
		dc.DrawFrameControl(m_rectClose, DFC_CAPTION, DFCS_CAPTIONCLOSE);


		// draw docking toggle box
		m_rectUndock = m_rectClose;
		m_rectUndock.OffsetRect(-13,0);
		dc.DrawFrameControl(m_rectUndock, DFC_CAPTION, DFCS_CAPTIONMAX);

		gripper.right -= 38;
		gripper.left += 10;
		gripper.top += 10;
		gripper.bottom = gripper.top+3;
		gripper.OffsetRect(0, -4);
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );

		gripper.OffsetRect(0, 4);
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );
	}
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(3,24, "그룹");
	dc.SetBkMode(OPAQUE);

}

void CAcntDialogBar::InitGridData()
{
	int		pos{};
	CString data = m_data;
	CString		tmps, tmpx;
	CString	m_account, m_pass, m_name;
	
	//for (int j = 1; j<=m_grid->GetRowCount();j++)
	//	m_grid->DeleteRow(j);
	for ( ; !data.IsEmpty(); )
	{
		pos = data.Find('\n');
		if (pos < 0)
			break;

		tmps = data.Left(pos);
		data = (data.GetLength() > pos+1) ? data.Mid(pos+1) : _T("");

		for (int ii = 0; ii < 3; ii++)
		{
			pos = tmps.Find('\t');
			if (pos >= 0)
			{
				tmpx = tmps.Left(pos);
				tmps = (tmps.GetLength() > pos+1) ? tmps.Mid(pos+1) : _T("");
			}
			else
			{
				tmpx = tmps;
				tmps = _T("");
			}

			switch (ii)
			{
			case 0:	m_account = tmpx;	break;
			case 1:	m_pass = tmpx;	break;
			case 2:
				{
					tmpx = tmpx.Mid(0,tmpx.Find('|'));
					m_name = tmpx;	
					
				}
				break;
			}
		}

		if (m_account.GetLength() != 11)
		{
			continue;
		}
		m_grid->AddRow("0\t"+m_account+"\t"+m_name);
	}
	
}

void CAcntDialogBar::ProcessGroup(char* data, int len)
{
	CString sdata(_T(""));
	sdata = (char*) data;
	data = data+88;
	CString	Path;
	if (sdata[0] == '1') AfxMessageBox(sdata);

	Path.Format("%s\\%s\\%s\\GrpList.dat", Axis::home, USRDIR, Axis::user);
	
	TRY
	{
		CFile	file;
		if (!file.Open(Path, CFile::modeCreate|CFile::modeWrite))
		{
			TRACE("Create Failed\n");
			return;
		}
		file.Write(data, len-88);
		file.Close();
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
	if (TotalRefresh) 
	{
		TotalRefresh = false;
		ReadGroup();
	}
}

LRESULT CAcntDialogBar::OnGridDblClk(WPARAM wparam, LPARAM lparam)  //vc2019 void->LRESULT
{
	//QueryAccntList("00110000026");
	//ProcessAccount("");
	//return;
	
	CString string(_T(""));
	if (m_grid->m_nSelectedRow>=0)
	string = m_grid->GetData(m_grid->m_nSelectedRow, 0);

	CString s;
	s.Format("ACCOUNT SETACCOUNT : [%s]\n",string);
	OutputDebugString(s);
	
	(*m_axiscall)(AXI_SETACCOUNT, 0, (LPARAM) (const char *)string);
	
	
	//m_bHover = TRUE; 
	//Invalidate();
	//AfxMessageBox("OnMouseHover");
	return 0;
}

LRESULT CAcntDialogBar::OnNcHitTest(CPoint point)   //vc2019 UINT->LRESULT
{
	// TODO: Add your message handler code here and/or call default
	if (IsFloating())
		return CControlBar::OnNcHitTest(point);

	CRect rc;
	GetWindowRect(rc);
	point.Offset(-rc.left, -rc.top);
	if(m_rectClose.PtInRect(point))
		return HTSYSMENU;
	else if (m_rectUndock.PtInRect(point))
		return HTMINBUTTON;
	else if (m_rectGripper.PtInRect(point))
		return HTCAPTION;
	else if (m_rectBorder.PtInRect(point))
		return HTSIZE;
	else
		return CControlBar::OnNcHitTest(point);
}

void CAcntDialogBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// only start dragging if clicked in "void" space
	if (m_pDockBar != NULL)
	{
		// start the drag
		ASSERT(m_pDockContext != NULL);
		ClientToScreen(&point);
		m_pDockContext->StartDrag(point);
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

void CAcntDialogBar::StartTracking()
{
	SetCapture();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
	m_pDockSite->LockWindowUpdate();

	m_ptOld = m_rectBorder.CenterPoint();
	m_bTracking = TRUE;

	m_rectTracker = m_rectBorder;
	if (!IsHorz()) m_rectTracker.bottom -= 4;

	OnInvertTracker(m_rectTracker);
}

void CAcntDialogBar::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_bTracking && pWnd != this)
		StopTracking(FALSE); // cancel tracking

	CControlBar::OnCaptureChanged(pWnd);
}

void CAcntDialogBar::StopTracking(BOOL bAccept)
{
	OnInvertTracker(m_rectTracker);
	m_pDockSite->UnlockWindowUpdate();
	m_bTracking = FALSE;
	ReleaseCapture();

	if (!bAccept) return;

	int maxsize{}, minsize{}, newsize{};
	CRect rcc;
	m_pDockSite->GetWindowRect(rcc);

	newsize = IsHorz() ? m_sizeHorz.cy : m_sizeVert.cx;
	maxsize = newsize + (IsHorz() ? rcc.Height() : rcc.Width());
	minsize = IsHorz() ? m_sizeMin.cy : m_sizeMin.cx;

	const CPoint point = m_rectTracker.CenterPoint();
	switch (m_nDockBarID)
	{
	case AFX_IDW_DOCKBAR_TOP:	newsize += point.y - m_ptOld.y; break;
	case AFX_IDW_DOCKBAR_BOTTOM:	newsize += -point.y + m_ptOld.y; break;
	case AFX_IDW_DOCKBAR_LEFT:	newsize += point.x - m_ptOld.x; break;
	case AFX_IDW_DOCKBAR_RIGHT:	newsize += -point.x + m_ptOld.x; break;
	}

	newsize = max(minsize, min(maxsize, newsize));

	if (IsHorz())
		m_sizeHorz.cy = newsize;
	else	m_sizeVert.cx = newsize;

	m_pDockSite->RecalcLayout();
}

void CAcntDialogBar::OnInvertTracker(const CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT(m_bTracking);

	CRect rct = rect, rcc, rcf;
	GetWindowRect(rcc);
	m_pDockSite->GetWindowRect(rcf);

	rct.OffsetRect(rcc.left - rcf.left, rcc.top - rcf.top);
	rct.DeflateRect(1, 1);

	CDC *pDC = m_pDockSite->GetDCEx(NULL,
	DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);

	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);

	pDC->PatBlt(rct.left, rct.top, rct.Width(), rct.Height(), PATINVERT);

	if (hOldBrush != NULL)	SelectObject(pDC->m_hDC, hOldBrush);

	m_pDockSite->ReleaseDC(pDC);
}

BOOL CAcntDialogBar::IsHorz() const
{
	return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP || m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

void CAcntDialogBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bTracking) return;
	//if (IsFloating())	AfxMessageBox("asdf");
	if((nHitTest == HTSYSMENU) && !IsFloating())
	{		
		GetDockingFrame()->ShowControlBar(this, FALSE, FALSE);
		(*m_axiscall)(AXI_TOOLTIPCHANGE, 0, 0);
	}
	else if ((nHitTest == HTMINBUTTON) && !IsFloating())
		m_pDockContext->ToggleDocking();
	else if ((nHitTest == HTCAPTION) && !IsFloating() && (m_pDockBar != NULL))
	{
		// start the drag
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->StartDrag(point);
	}
	else if ((nHitTest == HTSIZE) && !IsFloating())
		StartTracking();
	else    
		CControlBar::OnNcLButtonDown(nHitTest, point);
}

void CAcntDialogBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if (!m_bTracking)
        CControlBar::OnLButtonUp(nFlags, point);
    else
    {
        ClientToWnd(point);
        StopTracking(TRUE);
    }
}


void CAcntDialogBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (IsFloating() || !m_bTracking)
	{
		CControlBar::OnMouseMove(nFlags, point);
		return;
	}

	const CPoint cpt = m_rectTracker.CenterPoint();
	ClientToWnd(point);

	if (IsHorz())
	{
		if (cpt.y != point.y)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(0, point.y - cpt.y);
			OnInvertTracker(m_rectTracker);
		}
	}
	else 
	{
		if (cpt.x != point.x)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(point.x - cpt.x, 0);
			OnInvertTracker(m_rectTracker);
		}
	}
	/*
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

	//CDialogBar::OnMouseMove(nFlags, point);
	CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		//if (!m_mouseIN)
		{
			//m_mouseIN = true;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}

		//SetInfo(lpoint);
		
	}
	*/
	
}

CPoint& CAcntDialogBar::ClientToWnd(CPoint& point)
{
	if (m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM)
		point.y += m_cxEdge;
	else if (m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
		point.x += m_cxEdge;

	return point;
}

bool CAcntDialogBar::sendTR(CString trN, char* datB, int datL, BYTE stat, int key)
{
	if (datL <= 0)	return false;

	char* wb{};
	BOOL	rc  = FALSE;
	struct	_userTH* userTH{};

	wb = new char[L_userTH+datL+1];
	ZeroMemory(wb, L_userTH+datL+1);
	userTH = (struct _userTH *)wb;
	CopyMemory(userTH->trc, (char *)(const char*)trN, trN.GetLength());
	
	userTH->stat = stat;	
	userTH->key  = key;
	CopyMemory(&wb[L_userTH], datB, datL);

	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), (long)wb, datL);
	delete[] wb;

	return (rc ? true : false);
}
void CAcntDialogBar::QueryGroupList(CString strGrp)
{
	CString strData(_T(""));
	PIHOAGLS_MID iagls;

	FillMemory(&iagls, L_PIHOAGLS_MID, ' ');

	CString	Path;
	Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","COMMONNM","01",readB,sizeof(readB),Path);
	
	CString sCommonNm(readB,readL);
	sCommonNm.TrimLeft();sCommonNm.TrimRight();

	CString usid;

	CString strCommonNm;
	//strCommonNm = "9" + m_dept + "00";
	strCommonNm = "9" + m_dept + sCommonNm;

	CopyMemory(&iagls.usid, (LPCSTR)strCommonNm, strCommonNm.GetLength());
	
	strData += CString((char *)&iagls, L_PIHOAGLS_MID);
	
	sendTR("PIHOAGLS", (char*)(const char*)strData, L_PIHOAGLS_MID, US_PASS, 253);

}

void CAcntDialogBar::QueryAccntList(CString strAccount)
{
	CString strData(_T(""));
	PIHOACLS_MID iacls;
	FillMemory(&iacls, L_PIHOACLS_MID, ' ');

	if(m_dept == "")
		m_dept = "000";

	CString	Path;
	Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","COMMONNM","01",readB,sizeof(readB),Path);
	
	CString sCommonNm(readB,readL);
	sCommonNm.TrimLeft();sCommonNm.TrimRight();

	CString strCommonNm;
	strCommonNm = "9" + m_dept + sCommonNm;

	CopyMemory(&iacls.usid, (LPCSTR)strCommonNm, strCommonNm.GetLength());
	CopyMemory(&iacls.uspw, (LPCSTR)m_uspw, m_uspw.GetLength());

	CopyMemory(&iacls.synf, _T("Y"), 1);
	strData += CString((char *)&iacls, L_PIHOACLS_MID);
	
	//sendTR("PIBOACLS", (char*)(const char*)strData, L_PIHOACLS_MID, US_PASS, 254);
	sendTR("PIBFACLS", (char*)(const char*)strData, L_PIHOACLS_MID, US_PASS, 254);
}

bool CAcntDialogBar::sendTR(char* datB, int datL)
{
	BOOL	rc = FALSE;
	m_wizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4), (long)datB, datL);
	return (rc ? true : false);
}

void CAcntDialogBar::ProcessAccount(char* data, int len)
{
	//data = "00110000026정문일                                          00010001\n00110000035박영태                                          000200020001";
	CString sdata(_T(""));
	
	
	sdata = (char*) data;
	data = data+88;
	if (sdata[0] == '1') 
		AfxMessageBox(sdata);
	CString	Path;
	
	Path.Format("%s\\%s\\%s\\AccList.dat", Axis::home, USRDIR, Axis::user);
CString s;
s.Format("!!!SCRAP ProcessAccount [%s]\n",Path);
OutputDebugString(s);
	TRY
	{
		CFile	file;
		if (!file.Open(Path, CFile::modeCreate|CFile::modeWrite))
		{
			TRACE("Create Failed\n");
			return;
		}
		file.Write(data, len-88);
		file.Close();
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
	if (TotalRefresh) 
	{
		QueryGroupList("");
	}
	
}

void CAcntDialogBar::ReadGroup()
{
	CString	Path;
	Path.Format("%s\\%s\\%s\\grpList.dat", Axis::home, USRDIR, Axis::user);
	int nrec{}, grec; nrec = grec = 0;
	int pos  = 0;
	CString gpid, gpnm;

	bool find = false;
	
	const int row = m_grp->GetCount();
	for (int jj = 1; jj <= row ; jj++) m_grp->DeleteString(0);
	m_grp->AddString("전체");
	TRY
	{
		CFile file;
		if (file.Open(Path, CFile::modeRead)) 
		{
			const int len = gsl::narrow_cast<int>(file.GetLength());
			if (len > 0)	
			{
				char	*pBuf = new char[len];
				file.Read(pBuf, len);
							
				while (true)
				{
					find = false;
					gpid = CString(pBuf+pos, LEN_GPID);  pos+=LEN_GPID;
					gpnm = CString(pBuf+pos, LEN_GPNM);  pos+=LEN_GPNM;
		
					//pos+=1;			//서비스에서 리드해올땐 필요없음
					//AfxMessageBox(acno);
					//if (find) m_grid->AddRow("0\t"+acno+"\t"+acnm+"\t"+acpw);

					m_grp->AddString("["+gpid+"]"+gpnm);
					if (pos >= len) break;
				}
				delete[] pBuf;
			}
			file.Close();
		}
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
	if (m_grp->GetCount()>0)
	{
		m_grp->SetCurSel(0);
		OnGroupSelEndOk();
	}
}

void CAcntDialogBar::ReadAccount(CString grp)
{
	CString	Path;
	Path.Format("%s\\%s\\%s\\AccList.dat", Axis::home, USRDIR, Axis::user);
	int nrec{}, grec; nrec = grec = 0;
	int pos  = 0;
	CString ecod, emsg, acno, acnm, acpw, gpid, tacno;
	CStringArray gary;
CString s;
s.Format("!!!SCRAP ReadAccount [%s]\n",Path);
OutputDebugString(s);	
	bool find = false;
	const int row = m_grid->GetRowCount();
	for (int jj = 1; jj <= row ; jj++) m_grid->DeleteRow(0);
	m_grid->ResetContent();
	TRY
	{
		CFile file;
		if (file.Open(Path, CFile::modeRead)) 
		{
			const int len = (int)file.GetLength();
			if (len > 0)	
			{
				char	*pBuf = new char[len];
				file.Read(pBuf, len);
							
				while (true)
				{
					find = false;
					acno = CString(pBuf+pos, LEN_ACNO);  pos+=LEN_ACNO;
					acnm = CString(pBuf+pos, LEN_ACNM);  pos+=LEN_ACNM;
					acpw = CString(pBuf+pos, LEN_ACPW);  pos+=LEN_ACPW;
					grec = atoi(CString(pBuf+pos, LEN_GREC));  pos+=LEN_GREC;
					if (grp.IsEmpty()) find = true;
					gary.RemoveAll();
					for (int ii = 1; ii <=grec; ii++)
					{
						gpid = CString(pBuf+pos, LEN_GPID);  pos+=LEN_GPID;
						gary.Add(gpid);
						if (gpid == grp) find = true;
					}
					tacno = acno;
					acno = acno.Mid(3,8);
					//pos+=1;			//서비스에서 리드해올땐 필요없음
					//AfxMessageBox(acno);
					
					if (find) m_grid->AddRow(tacno+"\t"+acno+"\t"+acnm+"\t"+acpw);
					if (pos >= len) break;					
				}
				delete[] pBuf;
			}
			file.Close();
		}
	}
	CATCH (CFileException, e) 
	{
	}
	END_CATCH;
	
	//AfxMessageBox("Finished");

}


void CAcntDialogBar::OnGroupSelEndOk()
{
	if (m_grp->GetCount()>0)
	{
		CString gpid;
		m_grp->GetWindowText(gpid);
		if (gpid.Compare("전체")<0)
		{
			gpid = gpid.Mid(1,4);
			//AfxMessageBox(gpid);
			ReadAccount(gpid);
		}
		else ReadAccount("");

	}
}

LRESULT CAcntDialogBar::OnGrpEdit(WPARAM wparam, LPARAM lparam)  //vc2019 void->LRESULT
{
	/*
	CString string; string = "9002";
	(*m_axiscall)(AXI_INPUTSCREENNO, 0, (LPARAM) (const char *)string);
	*/
	CMenuXP		menu;
	menu.CreatePopupMenu();
	menu.AppendMenuX(MF_STRING, 1, "갱신");
	menu.AppendMenuX(MF_STRING, 2, "편집");
	CPoint	curPt;
	GetCursorPos(&curPt);
	const int nRtn = menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, curPt.x, curPt.y, this);
	MenuCommand(nRtn);
	return 0;
}

void CAcntDialogBar::Refresh813(int tp)
{
	CString dept = m_dept.Mid(0, 3);

	CString	Path;
	Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","DEPT","811",readB,sizeof(readB),Path);
	
	CString tDept(readB,readL);
	tDept.TrimLeft();tDept.TrimRight();

CString s;
s.Format("ACCTEST CAcntDialogBar Refresh813 로그인부서=[%s] 파일부서=[%s] \n",dept, tDept);
OutputDebugString(s);
	//if (dept!="813" && dept!="828" && dept!="812" && dept != tDept)
//if(dept != tDept)
	if(dept != tDept && Axis::userID != "221116")
	{
		return;
	}
	
	TotalRefresh = true;
	QueryAccntList("PIBFACLS send " + dept + "   " + tDept);
	
}

void CAcntDialogBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CControlBar::OnWindowPosChanged(lpwndpos);
	if (m_bInRecalcNC) 
	{
		CRect rc;
		GetClientRect(rc);
		return;
	}

	// Find on which side are we docked
	const UINT nDockBarID = GetParent()->GetDlgCtrlID();

	// Return if dropped at same location
	if (nDockBarID == m_nDockBarID // no docking side change
		&& (lpwndpos->flags & SWP_NOSIZE) // no size change
		&& ((m_dwStyle & CBRS_BORDER_ANY) != CBRS_BORDER_LEFT))//ANY))
		return; 

	m_nDockBarID = nDockBarID;

	// Force recalc the non-client area
	m_bInRecalcNC = TRUE;
	SetWindowPos(NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
	m_bInRecalcNC = FALSE;	
	// TODO: Add your message handler code here
	
}

void CAcntDialogBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_pDockBar != NULL)
	{
		// toggle docking
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->ToggleDocking();
	}
	else
	{
		CWnd::OnLButtonDblClk(nFlags, point);
	}
}

void CAcntDialogBar::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if ((m_pDockBar != NULL) && (nHitTest == HTCAPTION))
	{
		// toggle docking
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->ToggleDocking();
	}
	else
	{
		CWnd::OnNcLButtonDblClk(nHitTest, point);
	}
}

BOOL CAcntDialogBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ((nHitTest != HTSIZE) || m_bTracking)
		return CControlBar::OnSetCursor(pWnd, nHitTest, message);

	if (IsHorz())
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	else
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	return TRUE;
}

BOOL CAcntDialogBar::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CControlBar::OnCommand(wParam, lParam);
}

void CAcntDialogBar::MenuCommand(int tp)
{
	switch (tp)
	{
		case 1:	// 갱신
			{
				Refresh813(0);
			}
			break;
		case 2:	// 편집
			{
				CString string; string = "9002";
				(*m_axiscall)(AXI_INPUTSCREENNO, 0, (LPARAM) (const char *)string);
			}
			break;			
	}
}

CString CAcntDialogBar::Variant(int nComm, CString strData)
{
	//위저드는 현재 쓸수없는 컨트롤임 만약 쓰려면 위닉스 플랫폼 담당자와 협의 해야함
	CString strRet(_T(""));
	
	long rc = 0;
	
	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
		(BYTE *)(VTS_I4 VTS_I4), nComm, 0);
	strRet = rc ? (char *)rc : _T("");
	
	return strRet;
}

#pragma warning (default : 26409)
#pragma warning (default : 26408)
#pragma warning (default : 26401)
#pragma warning (default : 26400)
