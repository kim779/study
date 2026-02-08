// KobaElwNotify.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "KobaElwNotify.h"
#include "MainFrm.h"
#include "../dll/axiscm/AxCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKobaElwNotify::CKobaElwNotify(CWnd* pParent /*=NULL*/)
	: CDialog(CKobaElwNotify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKobaElwNotify)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKobaElwNotify::~CKobaElwNotify()
{
	if (m_pGrid)
	{
		m_pGrid->DestroyWindow();
		delete m_pGrid;
	}
}

void CKobaElwNotify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKobaElwNotify)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKobaElwNotify, CDialog)
	//{{AFX_MSG_MAP(CKobaElwNotify)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_VIEW_KOBAELW, OnViewKobaelw)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKobaElwNotify message handlers
#pragma warning (disable : 26409)
BOOL CKobaElwNotify::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rc;
	CProfile profile(GetProfileFileName(pkSTInfo));

	GetClientRect(rc);

	rc.bottom -= 30;
	m_pGrid = new CContGrid();
	if (!m_pGrid->Create(IGRIDCLASS, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rc, this, IDC_KOBANOTIFY_GRID))
	{
		delete m_pGrid;
		m_pGrid = NULL;
		return FALSE;
	}

	m_pGrid->m_pFont = &Axis::font;
	m_pGrid->m_bFillEmpty = TRUE;
	m_pGrid->m_bSelection = TRUE;
	m_pGrid->m_bDataLine  = FALSE;
	m_pGrid->m_bColGubnLine = TRUE;
	m_pGrid->m_b3DHead    = FALSE;
	m_pGrid->m_bEnableTip = FALSE;
	m_pGrid->ClearColumn();

	CIColumn iCol;
	iCol.m_sEditFormat.Empty();

	iCol.m_dAttr = IGAT_CENTER|IGAT_USERCOLOR;
	iCol.m_lWidth = 60;
	iCol.m_sHeadCaption = "발생시간";
	m_pGrid->AddQColumn(iCol);

	iCol.m_dAttr = IGAT_CENTER|IGAT_USERCOLOR;
	iCol.m_lWidth = 60;
	iCol.m_sHeadCaption = "종목코드";
	m_pGrid->AddQColumn(iCol);

	iCol.m_dAttr = IGAT_LEFT|IGAT_USERCOLOR;
	iCol.m_lWidth = rc.Width() - 138;
	iCol.m_sHeadCaption = "종목명";
	m_pGrid->AddQColumn(iCol);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
#pragma warning (default : 26409)

void CKobaElwNotify::ChangePallete()
{
	if (m_pGrid)
	{
		m_pGrid->m_headBgColor  = GetColor(74);	// 헤더색
		m_pGrid->m_headFgColor  = GetColor(76);  //RGB(90, 90, 90);
		m_pGrid->m_dataBgColor1 = GetColor(68);	// 1st row color
		m_pGrid->m_dataBgColor2 = GetColor(77);	// 2st row color
		m_pGrid->m_lineColor    = GetColor(75);
		m_pGrid->m_dataFgColor  = GetColor(69);
		m_pGrid->m_selBgColor   = GetColor(78);	// select row color
		m_pGrid->Invalidate();
	}
}

void CKobaElwNotify::Init()
{
	ChangePallete();
}

COLORREF CKobaElwNotify::GetColor(int index)
{
	int		pos{};
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;
	
	CProfile profile(GetProfileFileName(pkPalette));
	section = profile.GetString(GENERALSN, "Palette", "Palette");
	
	key.Format("RGB%03d", index);
	dat = profile.GetString(section, key);
	if (dat.IsEmpty())
	{
		if (!section.Compare("Palette"))
			return 0;
		section = "Palette";
		dat = profile.GetString(section, key);
		if (dat.IsEmpty())
			return 0;
	}
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}
	
	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}

void CKobaElwNotify::AddNotify( LPCSTR data )
{
	if (m_pGrid)
	{
		CString code, time, hnam;
		CString tmp = data;

		int st=0, pos;
		pos = tmp.Find('|', st);  code = tmp.Mid(st, pos);  st = pos+1;
		pos = tmp.Find('|', st);  time = tmp.Mid(st, pos);  st = pos+1;
		hnam = tmp.Mid(st);

		time = time.Mid(0,2) + ":" + time.Mid(2, 2) + ":" + time.Mid(4, 2);
		tmp = time + "\t" + code + "\t" + hnam;

		m_pGrid->AddRow(tmp, true);
	}
}

void CKobaElwNotify::OnClear() 
{
	if (m_pGrid)
		m_pGrid->ResetContent();
}

void CKobaElwNotify::OnViewKobaelw() 
{
	const CWnd *pParent = GetParent();
	if (pParent)
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_KOBAELW_SCREEN, 0), (LPARAM)MAPN_KOBAELW);
}

BOOL CKobaElwNotify::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam==IDC_KOBANOTIFY_GRID)
	{
		const CWnd *pParent = GetParent();
		if (m_pGrid && pParent) {
			if (m_pGrid->m_nSelectedRow>=0)
			{
				CString mapstr, code;
				code = m_pGrid->GetData(m_pGrid->m_nSelectedRow, 1);
				if (code.GetAt(0)=='J')
				{
					mapstr.Format("%sE1301\t%s\n", MAPN_KOBAELW_SCREEN, (LPCSTR)code.Mid(1));
					pParent->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_KOBAELW_SCREEN, 0), (LPARAM)(LPCSTR)mapstr);
				}
			}
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CKobaElwNotify::OnConfig() 
{
	const CWnd *pParent = GetParent();
	if (pParent)
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(ID_APP_MNGSETUP, 0), 0);
}

void CKobaElwNotify::OnOK() 
{
	CProfile prop(pkManageSetup);

	if (prop.GetInt("ONETIME", "Q_KOBAELW", -1)==-1)
	{
		prop.Write("ONETIME", "Q_KOBAELW", 1);
		const int ret = MessageBox(
			"이 창을 계속 보시려면 [예]\r\n"
			"다시 보지 않으시려면  [아니오]\r\n"
			"를 눌러주시기 바랍니다.\r\n", 
			"IBK투자증권", MB_YESNO);

		prop.Write("Manage", MNG_INFO_KOBAELW, (ret==IDNO) ? "0" : "1");
	}

	CDialog::OnOK();
}
