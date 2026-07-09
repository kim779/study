// K50Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "K50Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CK50Dlg dialog


CK50Dlg::CK50Dlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CK50Dlg::IDD, pParent)
{
	m_Point = pt;
}


void CK50Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CK50Dlg)
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CK50Dlg, CDialog)
	//{{AFX_MSG_MAP(CK50Dlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CK50Dlg message handlers

BOOL CK50Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetDlgPos(m_Point.x, m_Point.y);

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;

	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));
	
	InitListCtrl();

	loadJCode();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CK50Dlg::InitListCtrl()
{
	m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "코드", LVCFMT_LEFT, 60);
	m_ListCtlCode.InsertColumn(2, "종목명", LVCFMT_LEFT, 139);
}

void CK50Dlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_CtrlFont.DeleteObject();	
}

void CK50Dlg::AddListCtlCode(CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	int	nItem = m_ListCtlCode.GetItemCount() + 1;

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT, nItem, szCode
		, NULL, NULL, 0, 0);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
}

void CK50Dlg::ClearListCtlCode(int nItem /* = -1 */)
{
	if (nItem < 0)
	{
		m_ListCtlCode.DeleteAllItems();
	}
	else
	{
		m_ListCtlCode.DeleteItem(nItem);
	}
}

void CK50Dlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem;
	CString	code, name;
	POSITION pos = m_ListCtlCode.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = m_ListCtlCode.GetNextSelectedItem(pos);

			code = m_ListCtlCode.GetItemText(nItem, 0);
			name = m_ListCtlCode.GetItemText(nItem, 1);

			m_rtnData.code = code;
			m_rtnData.name = name;
		}
	}
	
	OnOK();

	*pResult = 0;
}

int CK50Dlg::InsertListCtlCode(int nIdx, CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT, nIdx, szCode
		, NULL, NULL, 0, 0);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
	return rItem;
}

LOGFONT CK50Dlg::SetFont(int Size)
{
	LOGFONT		Font;

	ZeroMemory(&Font, sizeof(LOGFONT));
	Font.lfHeight		= Size * 10;
	Font.lfCharSet		= DEFAULT_CHARSET;
	Font.lfWeight		= FW_NORMAL;
	Font.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	Font.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	Font.lfQuality		= DEFAULT_QUALITY;
	Font.lfPitchAndFamily	= DEFAULT_PITCH; 
	Font.lfItalic		= false;
	strcpy(Font.lfFaceName, "굴림");

	return Font;
}

BOOL CK50Dlg::loadJCode()
{		
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int	codeN;		
	struct	hjcode*  HJCode;
	struct	JCode*	jCode;
	
	codeN = (int)pApp->m_arrayHJCode.GetSize();

	for (int ii = 0; ii < codeN; ii++)
	{
		jCode = new struct JCode;
		HJCode = pApp->m_arrayHJCode.GetAt(ii);

		jCode->Name = CString(HJCode->hnam, HNameLen);
		jCode->Name.TrimRight();
		jCode->fill = HJCode->jsiz;
		CopyMemory(jCode->stgb, HJCode->stgb, sizeof(HJCode->stgb));
		jCode->size = HJCode->size;
		jCode->ucdm = HJCode->ucdm;
		jCode->ucds = HJCode->ucds;
		jCode->jjug = HJCode->jjug;
		jCode->kpgb = HJCode->kpgb;
		jCode->ucmd = HJCode->ucmd;
		jCode->kosd = (char)HJCode->kosd;
		jCode->ssgb = HJCode->ssgb;
		jCode->itgb = HJCode->itgb;
		jCode->wsgb = HJCode->wsgb;
		jCode->Code = CString(HJCode->code, HCodeLen + 2);
		jCode->Code = jCode->Code.Mid(1);
		jCode->star = HJCode->star;
		jCode->unio = HJCode->unio;
		jCode->jsiz = HJCode->jsiz;
		jCode->pInf = NULL;

		if (jCode->kpgb == 3)
			InsertListCtlCode(0, jCode->Code, jCode->Name);

		delete jCode;
	}

	return TRUE;
}

void CK50Dlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth, scHeight;
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect	rc;
	GetClientRect(&rc);

	int	gabX = scWidth - (x + rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}