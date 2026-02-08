// UpCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "UpCodeDlg.h"
#include "user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UPCODE_FILE	"upcode.dat" // 업종코드
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
// CUpCodeDlg dialog


CUpCodeDlg::CUpCodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CUpCodeDlg::IDD, pParent)
{
	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_root;
	m_aUPCode.RemoveAll();
	//{{AFX_DATA_INIT(CUpCodeDlg)
	//}}AFX_DATA_INIT
}


void CUpCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpCodeDlg)
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	DDX_Control(pDX, IDC_BTN_SECTOR, m_btn_sec);
	DDX_Control(pDX, IDC_BTN_KPU, m_btn_kpu);
	DDX_Control(pDX, IDC_BTN_KDU, m_btn_kdu);
	DDX_Control(pDX, IDC_BTN_FUS, m_btn_fus);
	DDX_Control(pDX, IDC_BTN_FREEBOARD, m_btn_free);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CUpCodeDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_KPU, IDC_BTN_FREEBOARD, OnClickBtnKind)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpCodeDlg message handlers

void CUpCodeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_CtrlFont.DeleteObject();
	m_aUPCode.RemoveAll();
}

BOOL CUpCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));

	InitListCtrl();
	SetBtn();

	GetExCode();

	loadUPCode();
	OnClickBtnKind(IDC_BTN_KPU);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpCodeDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem{};
	CString	code, name;
	POSITION pos = m_ListCtlCode.GetFirstSelectedItemPosition();
	if (pos != nullptr)
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

void CUpCodeDlg::GetExCode()
{
	const struct _infop {
		char* code;
		char* name;
	} infop[] = {
		{"101", "KOSPI200"},	{"401", "KOSPI100"},
		{"402", "KOSPI50"}
	};

/*
	struct _infod {
		char* code;
		char* name;
	} infod[] = {
		{"303", "KOSTAR"}
	};
*/

	_exCode	exCode;

	for (int ii = 0 ; ii < sizeof(infop) / sizeof(_infop) ; ii++)
	{
		exCode.code = infop[ii].code;
		exCode.name = infop[ii].name;

		m_exCode[EXKOSPI].Add(exCode);
	}

/*
	for (int jj = 0 ; jj < sizeof(infod) / sizeof(_infod) ; jj++)
	{
		exCode.code = infod[jj].code;
		exCode.name = infod[jj].name;

		m_exCode[EXKOSDAQ].Add(exCode);
	}
*/
}

void CUpCodeDlg::InitListCtrl()
{
	m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "코드", LVCFMT_LEFT, 60);
	m_ListCtlCode.InsertColumn(2, "종목명", LVCFMT_LEFT, 139);
}

void CUpCodeDlg::SetBtn()
{
	const short	shBtnColor = 245;

	for (int ii = IDC_BTN_KPU ; ii <= IDC_BTN_FREEBOARD ; ii++)
	{
		((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CButtonST*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
	}
}

LOGFONT CUpCodeDlg::SetFont(int Size)
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
	strcpy_s(Font.lfFaceName, "굴림");

	return Font;
}

BOOL CUpCodeDlg::loadUPCode()
{
	if (m_aUPCode.GetSize() > 0)
		return TRUE;

	TRY
	{
		CString		path = m_sRoot + "/tab/" + UPCODE_FILE;
		CFile		file(path, CFile::modeRead);
		upcode		upCode;
		UINT		nBytesRead;
		do
		{
			nBytesRead = file.Read(&upCode, sizeof(upCode));
			if (nBytesRead == sizeof(upCode))
			{
				m_aUPCode.Add(upCode);
			}
		}while((int)nBytesRead);
		file.Close();
	}CATCH(CFileException, e)
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
		#endif
	}
	END_CATCH
		
	return TRUE;
}

void CUpCodeDlg::OnClickBtnKind(UINT nID)
{
	ClearListCtlCode();
	m_btnSel = nID;
	
	switch (nID)
	{
	case IDC_BTN_KPU:
		{
			CString	code, name;
			upcode	upCode{};
			for (int ii = 0 ; ii < m_aUPCode.GetSize() ; ii++)
			{
				upCode = m_aUPCode.GetAt(ii);
				if ((int)upCode.jgub == upKOSPI)
				{
					name = CString(upCode.hnam, sizeof(upCode.hnam));
					code.Format("%s%02d", UPCODE_KOSPI,(int)upCode.ucod);

					AddListCtlCode(code, name);
				}
				else if ((int)upCode.jgub == upINTEGRATE)
				{
					name = CString(upCode.hnam, sizeof(upCode.hnam));
					code.Format("9%02d",(int)upCode.ucod);
					InsertListCtlCode(0, code, name);
				}
			}

			for (int ii = 0 ; ii < m_exCode[EXKOSPI].GetSize() ; ii++)
			{
				AddListCtlCode( m_exCode[EXKOSPI][ii].code, m_exCode[EXKOSPI][ii].name);
			}
		}
		break;
	case IDC_BTN_KDU:
		{
			CString	code, name;
			upcode	upCode{};
			for (int ii = 0; ii < m_aUPCode.GetSize(); ii++)
			{
				upCode = m_aUPCode.GetAt(ii);
				if ((int)upCode.jgub == upKOSDAQ)
				{
					name = CString(upCode.hnam, sizeof(upCode.hnam));
					code.Format("%s%02d", UPCODE_KOSDAQ,(int)upCode.ucod);

					AddListCtlCode(code, name);
				}
			}

			for (int ii = 0 ; ii < m_exCode[EXKOSDAQ].GetSize() ; ii++)
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
			upcode	upCode{};
			for (int ii = 0; ii < m_aUPCode.GetSize(); ii++)
			{
				upCode = m_aUPCode.GetAt(ii);
				if ((int)upCode.jgub == upSECTOR)
				{
					name = CString(upCode.hnam, sizeof(upCode.hnam));
					code.Format("%s%02d", UPCODE_SECTOR, (int)upCode.ucod);

					AddListCtlCode(code, name);
				}
			}
		}
		break;
	case IDC_BTN_FREEBOARD:
		{
			// 여기 수정...
			CString code, name;

			upcode	upCode{};
			for (int ii = 0; ii < m_aUPCode.GetSize(); ii++)
			{
				upCode = m_aUPCode.GetAt(ii);
				if ((int)upCode.jgub == upFREEBOARD)
				{
					name = CString(upCode.hnam, sizeof(upCode.hnam));
					code.Format("%s%02d", UPCODE_FREEBOARD, (int)upCode.ucod);

					AddListCtlCode(code, name);
				}
			}
		}
		break;
	}
}

void CUpCodeDlg::AddListCtlCode(CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	const int	nItem = m_ListCtlCode.GetItemCount() + 1;

	const int rItem = m_ListCtlCode.InsertItem(LVIF_TEXT, nItem, szCode, 0, 0, 0, 0);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
}

void CUpCodeDlg::ClearListCtlCode(int nItem /* = -1 */)
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

CString CUpCodeDlg::GetRtnData()
{
	CString rValue = _T("");

	switch (m_btnSel)
	{
	case IDC_BTN_KPU:
	case IDC_BTN_FUS:
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

int CUpCodeDlg::InsertListCtlCode(int nIdx, CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT, nIdx, szCode
		, 0, 0, 0, 0);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
	return rItem;
}
