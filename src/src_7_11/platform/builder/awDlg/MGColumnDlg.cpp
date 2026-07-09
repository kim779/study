// MGColumnDlg.cpp : implementation file
//

#include "stdafx.h"

#include "MGColumnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMGColumnDlg dialog


CMGColumnDlg::CMGColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMGColumnDlg::IDD, pParent)
{
	m_sCellInfo = "Name|1||Name|name\tHead|1||Caption|head\tWidth|1||80|width\tAlign|3|1-Left;2-Center;3-Right;|Left|align\tComma|3|$bool|False|comma\tCoRGB|3|$bool|False|corgb\tSupress|3|$bool|False|supress\tcontrast|3|$bool|False|contrast\tseperator|3|$bool|False|seperator";
	//{{AFX_DATA_INIT(CMGColumnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMGColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMGColumnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMGColumnDlg, CDialog)
	//{{AFX_MSG_MAP(CMGColumnDlg)
	ON_BN_CLICKED(IDC_ADDBTN, OnAddbtn)
	ON_BN_CLICKED(IDC_DELBTN, OnDelbtn)
	ON_BN_CLICKED(IDC_INSERTBTN, OnInsertbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMGColumnDlg message handlers

BOOL CMGColumnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_property.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
		CRect(0, 30, 634, 214), this, 110))
		PostMessage(WM_CLOSE);
	
	SetWindowPos(NULL, 0, 0, 643, 242, SWP_NOZORDER|SWP_NOMOVE);

	m_property.EraseCtrls();
	m_property.ResetGrid();
	m_property.Initialize("");
	m_property.SetMulti(true);
	m_property.setHeadInfo(0, m_sCellInfo);
	m_property.setHeadInfo(0);
	if (!m_sData.IsEmpty())
		ParseData();
	m_property.m_bInvalidate = true;
	m_property.Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMGColumnDlg::OnAddbtn() 
{
	m_property.AddCol("");
	m_property.m_bInvalidate = true;
	m_property.Invalidate();
}

void CMGColumnDlg::OnDelbtn() 
{
	m_property.deleteCol(-1);
}

void CMGColumnDlg::OnInsertbtn() 
{
	m_property.insertCol(-2);
}

BOOL CMGColumnDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMGColumnDlg::OnOK() 
{
	m_property.EraseCtrls();
	m_sData.Empty();
	for (int col = 0; col < m_property.m_GridAttr.cols; col++)
	{
		m_sData += '{';
		m_sData += m_property.getData2(col, 1);	// caption
		m_sData += "|2|";
		m_sData += m_property.getData2(col, 4);	// comma
		m_sData += m_property.getData2(col, 5);	// corgb
		m_sData += m_property.getData2(col, 6);	// supress
		m_sData += m_property.getData2(col, 3);	// align
		m_sData += m_property.getData2(col, 7);	// contrast
		m_sData += "1";//m_property.getData2(col, 8);	// seperator
		m_sData += m_property.getData2(col, 8);	// seperator
		m_sData += "||";
		m_sData += m_property.getData2(col, 0);	// symbol
		m_sData += "|";
		m_sData += m_property.getData2(col, 2);	// width
		m_sData += '}';
	}
	m_sData.Replace("/", "*sl");
	CDialog::OnOK();
}

void CMGColumnDlg::ParseData()
{
	CString str = m_sData;

	str.Replace("{", "");
	while(!str.IsEmpty())
	{
		CString tmp = Parser(str, "}"), tArr[10];
		if (tmp.IsEmpty())
			break;
		tArr[1] = Parser(tmp, "|2|");
		tArr[4] = Parser(tmp, "||");
		if (tArr[4].GetLength() < 7)
			break;
		tArr[5] = (tArr[4].GetAt(1)=='0'?"False":"True");
		tArr[6] = (tArr[4].GetAt(2)=='0'?"False":"True");
		tArr[3] = (tArr[4].GetAt(3)=='1'?"Left":(tArr[4].GetAt(3)=='2'?"Center":"Right"));
		tArr[7] = (tArr[4].GetAt(4)=='0'?"False":"True");
		tArr[8] = (tArr[4].GetAt(6)=='0'?"False":"True");
		//tArr[8] = (tArr[4].GetAt(6)=='0'?"False":"True");
		tArr[4] = (tArr[4].GetAt(0)=='0'?"False":"True");
		
		tArr[0] = Parser(tmp, "|");
		tArr[2] = tmp;

		tmp.Empty();
		for (int ii = 0; ii <= 8; ii++)
		{
			tmp += tArr[ii];
			tmp += '\t';
		}

		m_property.AddCol(tmp);
	}

}

CString CMGColumnDlg::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}
