// UPCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "UPCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define UPCODE_KOSPI	"0"
#define UPCODE_KOSDAQ	"2"
#define	UPCODE_SECTOR	"5"
#define UPCODE_FREEBOARD "6"

#define EXKOSPI		0
#define EXKOSDAQ	1

#define STR_KOSPI	_T("(거)")
#define STR_KOSDAQ	_T("(코)")
#define	STR_SECTOR	_T("(섹)")

#define cTAB		'\t'
/////////////////////////////////////////////////////////////////////////////
// CUPCodeDlg dialog


CUPCodeDlg::CUPCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUPCodeDlg::IDD, pParent)
{
	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;

	//{{AFX_DATA_INIT(CUPCodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUPCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUPCodeDlg)
	DDX_Control(pDX, IDC_BTN_FREEBOARD, m_btn_free);
	DDX_Control(pDX, IDC_BTN_SECTOR, m_btn_sec);
	DDX_Control(pDX, IDC_BTN_KPU, m_btn_kpu);
	DDX_Control(pDX, IDC_BTN_KDU, m_btn_kdu);
	DDX_Control(pDX, IDC_BTN_FUS, m_btn_fus);
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUPCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CUPCodeDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_KPU, IDC_BTN_FREEBOARD, OnClickBtnKind)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUPCodeDlg message handlers

void CUPCodeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_CtrlFont.DeleteObject();

}

BOOL CUPCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));

	InitListCtrl();
	SetBtn();

	GetExCode();
	OnClickBtnKind(IDC_BTN_KPU);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUPCodeDlg::GetExCode()
{
	struct _infop {
		char* code;
		char* name;
	} infop[] = {
		{"101", "KOSPI200"},	{"401", "KOSPI100"},
		{"402", "KOSPI50"}
	};

	struct _infod {
		char* code;
		char* name;
	} infod[] = {
		{"303", "KOSTAR"}
	};

	_exCode	exCode;

	for (int ii = 0 ; ii < sizeof(infop) / sizeof(_infop) ; ii++)
	{
		exCode.code = infop[ii].code;
		exCode.name = infop[ii].name;

		m_exCode[EXKOSPI].Add(exCode);
	}

	for (int jj = 0 ; jj < sizeof(infod) / sizeof(_infod) ; jj++)
	{
		exCode.code = infod[jj].code;
		exCode.name = infod[jj].name;

		m_exCode[EXKOSDAQ].Add(exCode);
	}
}

void CUPCodeDlg::InitListCtrl()
{
	m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "코드", LVCFMT_LEFT, 60);
	m_ListCtlCode.InsertColumn(2, "종목명", LVCFMT_LEFT, 139);
}

void CUPCodeDlg::SetBtn()
{
	short	shBtnColor = 245;
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	for (int ii = IDC_BTN_KPU ; ii <= IDC_BTN_FREEBOARD ; ii++)
	{
		//((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		//((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		//((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		//((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CfxButton*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
		((CfxButton*)GetDlgItem(ii))->LoadImg(strImg);
	}
}

LOGFONT CUPCodeDlg::SetFont(int Size)
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

void CUPCodeDlg::OnClickBtnKind(UINT nID)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	ClearListCtlCode();
	m_btnSel = nID;
	
	for (int ii = IDC_BTN_KPU ; ii <= IDC_BTN_FREEBOARD ; ii++)
	{
		((CfxButton*)GetDlgItem(ii))->SetSelected(false);
	}
	((CfxButton*)GetDlgItem(nID))->SetSelected(true);
	switch (nID)
	{
	case IDC_BTN_KPU:
		{
			CString	code, name;
			UPCODE	upCode;
			int ii = 0;
			for (ii = 0 ; ii < pApp->m_arrayUpcode.GetSize() ; ii++)
			{
				upCode = pApp->m_arrayUpcode.GetAt(ii);
				if (upCode.kind == upKOSPI)
				{
					name = upCode.name;
					code.Format("%s%02d", UPCODE_KOSPI, atoi(upCode.ucod));

					AddListCtlCode(code, name);
				}
				else if (upCode.kind == upINTEGRATE)
				{
					name = upCode.name;
					code.Format("9%02d", atoi(upCode.ucod));
					InsertListCtlCode(0, code, name);
				}
			}

			for (ii = 0 ; ii < m_exCode[EXKOSPI].GetSize() ; ii++)
			{
				AddListCtlCode( m_exCode[EXKOSPI][ii].code, m_exCode[EXKOSPI][ii].name);
			}
		}
		break;
	case IDC_BTN_KDU:
		{
			CString	code, name;
			UPCODE	upCode;
			int ii = 0;
			for (ii = 0; ii < pApp->m_arrayUpcode.GetSize(); ii++)
			{
				upCode = pApp->m_arrayUpcode.GetAt(ii);
				if (upCode.kind == upKOSDAQ)
				{
					name = upCode.name;
					code.Format("%s%02d", UPCODE_KOSDAQ, atoi(upCode.ucod));

					AddListCtlCode(code, name);
				}
			}

			for (ii = 0 ; ii < m_exCode[EXKOSDAQ].GetSize() ; ii++)
			{
				AddListCtlCode( m_exCode[EXKOSDAQ][ii].code, m_exCode[EXKOSDAQ][ii].name);
			}
		}
		break;
	case IDC_BTN_FUS:
		{
			m_rtnData.code = _T("1");
			m_rtnData.name = _T("선물");
			OnOK();
		}
		break;
	case IDC_BTN_SECTOR:
		{
			CString	code, name;
			UPCODE	upCode;
			for (int ii = 0; ii < pApp->m_arrayUpcode.GetSize(); ii++)
			{
				upCode = pApp->m_arrayUpcode.GetAt(ii);
				if ((int)upCode.kind == upSECTOR)
				{
					name = upCode.name;
					code.Format("%s%02d", UPCODE_SECTOR, atoi(upCode.ucod));

					AddListCtlCode(code, name);
				}
			}
		}
		break;
	case IDC_BTN_FREEBOARD:
		{
			// 여기 수정...
			CString code, name;

			UPCODE	upCode;
			for (int ii = 0; ii < pApp->m_arrayUpcode.GetSize(); ii++)
			{
				upCode = pApp->m_arrayUpcode.GetAt(ii);
				if ((int)upCode.kind == upFREEBOARD)
				{
					name = upCode.name;
					code.Format("%s%02d", UPCODE_FREEBOARD, atoi(upCode.ucod));

					AddListCtlCode(code, name);
				}
			}
		}
		break;
	}
}

void CUPCodeDlg::AddListCtlCode(CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	int	nItem = m_ListCtlCode.GetItemCount() + 1;

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT, nItem, szCode
		, NULL, NULL, 0, 0);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
}

void CUPCodeDlg::ClearListCtlCode(int nItem /* = -1 */)
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

void CUPCodeDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
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

CString CUPCodeDlg::GetRtnData()
{
	CString rValue = _T("");

	switch (m_btnSel)
	{
	case IDC_BTN_KPU:
		rValue.Format("%s%c%s%s", m_rtnData.code, cTAB, STR_KOSPI, m_rtnData.name);
		break;
	case IDC_BTN_KDU:
		rValue.Format("%s%c%s%s", m_rtnData.code, cTAB, STR_KOSDAQ, m_rtnData.name);
		break;
	case IDC_BTN_SECTOR:
		rValue.Format("%s%c%s%s", m_rtnData.code, cTAB, STR_SECTOR, m_rtnData.name);
		break;
	default:
		rValue.Format("%s%c%s", m_rtnData.code, cTAB, m_rtnData.name);
		break;
	}

	return rValue;
}

int CUPCodeDlg::InsertListCtlCode(int nIdx, CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT, nIdx, szCode
		, NULL, NULL, 0, 0);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
	return rItem;
}

/*

HBRUSH CUPCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here	
	DeleteObject(m_hBrush);

	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_hBrush = CreateSolidBrush(USER_BACKCOLOR);
		
		return m_hBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
*/
