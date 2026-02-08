// JisuConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "JisuConfigDlg.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJisuConfigDlg dialog

CJisuConfigDlg::CJisuConfigDlg(struct _minijisu *mj, CRect rcBtn)
	: CDialog(CJisuConfigDlg::IDD, nullptr)
{
	m_pMJ = mj;
	m_rcBtn = rcBtn;
	CopyMemory(&m_mj, mj, sz_MINIJISU);

	m_hReg = nullptr;
}


void CJisuConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJisuConfigDlg)
	DDX_Control(pDX, IDC_JWIDTH5, m_jwidth5);
	DDX_Control(pDX, IDC_JWIDTH4, m_jwidth4);
	DDX_Control(pDX, IDC_JWIDTH3, m_jwidth3);
	DDX_Control(pDX, IDC_JWIDTH2, m_jwidth2);
	DDX_Control(pDX, IDC_JWIDTH1, m_jwidth1);
	DDX_Control(pDX, IDC_JSTYLE5, m_jstyle5);
	DDX_Control(pDX, IDC_JSTYLE4, m_jstyle4);
	DDX_Control(pDX, IDC_JSTYLE3, m_jstyle3);
	DDX_Control(pDX, IDC_JSTYLE2, m_jstyle2);
	DDX_Control(pDX, IDC_JSTYLE1, m_jstyle1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJisuConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CJisuConfigDlg)
	ON_BN_CLICKED(IDC_JCOLOR1, OnJcolor1)
	ON_BN_CLICKED(IDC_JCOLOR2, OnJcolor2)
	ON_BN_CLICKED(IDC_JCOLOR3, OnJcolor3)
	ON_BN_CLICKED(IDC_JCOLOR4, OnJcolor4)
	ON_BN_CLICKED(IDC_JCOLOR5, OnJcolor5)
	ON_BN_CLICKED(IDC_JCODE1, OnJcode1)
	ON_BN_CLICKED(IDC_JCODE2, OnJcode2)
	ON_BN_CLICKED(IDC_JCODE3, OnJcode3)
	ON_BN_CLICKED(IDC_JCODE4, OnJcode4)
	ON_BN_CLICKED(IDC_JCODE5, OnJcode5)
	ON_BN_CLICKED(IDC_BASIC, OnBasic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJisuConfigDlg message handlers

BOOL CJisuConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	rc;

	GetWindowRect(&rc);
	MoveWindow(m_rcBtn.left - rc.Width(), m_rcBtn.top, rc.Width(), rc.Height());


	const int	pID[10] = 
	{
		IDC_JCHECK1, IDC_JCHECK2, IDC_JCHECK3,
		IDC_JCHECK4, /*IDC_JCHECK5, */
		IDC_CCMPRADIO, IDC_PCMPRADIO,
		IDC_LTCHECK, IDC_RTCHECK,
		IDC_FIXCHECK
	};

	CButton* pButton{};
	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		pButton = (CButton *)GetDlgItem(pID[ii]);
		pButton->SetWindowText(m_mj.jisu[ii].name);
		if (m_mj.jisu[ii].view)	pButton->SetCheck(1);
	}

	m_jstyle1.SetLine(m_mj.jisu[0].style);
	m_jstyle2.SetLine(m_mj.jisu[1].style);
	m_jstyle3.SetLine(m_mj.jisu[2].style);
	m_jstyle4.SetLine(m_mj.jisu[3].style);
//	m_jstyle5.SetLine(m_mj.jisu[4].style);

	m_jwidth1.SetLineWidth(m_mj.jisu[0].width);
	m_jwidth2.SetLineWidth(m_mj.jisu[1].width);
	m_jwidth3.SetLineWidth(m_mj.jisu[2].width);
	m_jwidth4.SetLineWidth(m_mj.jisu[3].width);
//	m_jwidth5.SetLineWidth(m_mj.jisu[4].width);

	if (m_mj.compare & CMP_CSIGA)
	{
		pButton = (CButton *)GetDlgItem(pID[MAX_JISU]);
		pButton->SetCheck(1);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(pID[MAX_JISU+1]);
		pButton->SetCheck(1);
	}

	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+2]);
	if (m_mj.tick & TK_LEFT)	pButton->SetCheck(1);
	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+3]);
	if (m_mj.tick & TK_RIGHT)	pButton->SetCheck(1);
	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+4]);
	if (m_mj.fixmode)		pButton->SetCheck(1); 

	InitColorBtn();

	if (!m_hReg)
	{
		m_hReg = LoadLibrary("axisCode.dll");
		if (!m_hReg)
		{
			TRACE("axisCode.dll load error !!\n");
			return FALSE;
		}
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJisuConfigDlg::InitColorBtn()
{
	const UINT	pColorID[MAX_JISU] = {IDC_JCOLOR1, IDC_JCOLOR2, IDC_JCOLOR3, IDC_JCOLOR4/*,
		IDC_JCOLOR5*/
		};

	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		m_pColorBtn[ii].Attach(pColorID[ii], this);
		m_pColorBtn[ii].SetNewColor(m_mj.jisu[ii].color);
	}
}

void CJisuConfigDlg::OnJcolor1() 
{
	CColorDialog dlg(m_mj.jisu[0].color);

	if(dlg.DoModal()==IDOK)
	{
		m_mj.jisu[0].color = dlg.GetColor();
		m_pColorBtn[0].SetNewColor(m_mj.jisu[0].color);
	}
}

void CJisuConfigDlg::OnJcolor2() 
{
	CColorDialog dlg(m_mj.jisu[1].color);

	if(dlg.DoModal()==IDOK)
	{
		m_mj.jisu[1].color = dlg.GetColor();
		m_pColorBtn[1].SetNewColor(m_mj.jisu[1].color);
	}
}

void CJisuConfigDlg::OnJcolor3() 
{
	CColorDialog dlg(m_mj.jisu[2].color);

	if(dlg.DoModal()==IDOK)
	{
		m_mj.jisu[2].color = dlg.GetColor();
		m_pColorBtn[2].SetNewColor(m_mj.jisu[2].color);
	}
}

void CJisuConfigDlg::OnJcolor4() 
{
	CColorDialog dlg(m_mj.jisu[3].color);

	if(dlg.DoModal()==IDOK)
	{
		m_mj.jisu[3].color = dlg.GetColor();
		m_pColorBtn[3].SetNewColor(m_mj.jisu[3].color);
	}
}

void CJisuConfigDlg::OnJcolor5() 
{
	//CColorDialog dlg(m_mj.jisu[4].color);

	//if(dlg.DoModal()==IDOK)
	//{
	//	m_mj.jisu[4].color = dlg.GetColor();
	//	m_pColorBtn[4].SetNewColor(m_mj.jisu[4].color);
	//}
}

void CJisuConfigDlg::OnJcode1() 
{
	//AfxMessageBox("START");
	CString	code;	GetCode(code);
	//AfxMessageBox("GETCODE");
	if (code.IsEmpty())	return;

	int	index = code.Find("\t");
	if (index <= 0)	return;
	
	sprintf(m_mj.jisu[0].code, "%s", code.Left(index).GetString());
	sprintf(m_mj.jisu[0].name, "%s", code.Mid(++index).GetString());
	
	CButton	*pButton = (CButton *)GetDlgItem(IDC_JCHECK1);
	pButton->SetWindowText(m_mj.jisu[0].name);
}

void CJisuConfigDlg::OnJcode2() 
{
	CString	code;	GetCode(code);

	if (code.IsEmpty())	return;


	int	index = code.Find("\t");
	if (index <= 0)	return;

	sprintf(m_mj.jisu[1].code, "%s", code.Left(index).GetString());
	sprintf(m_mj.jisu[1].name, "%s", code.Mid(++index).GetString());

	CButton	*pButton = (CButton *)GetDlgItem(IDC_JCHECK2);
	pButton->SetWindowText(m_mj.jisu[1].name);
}

void CJisuConfigDlg::OnJcode3() 
{
	CString	code;	GetCode(code);

	if (code.IsEmpty())	return;


	int	index = code.Find("\t");
	if (index <= 0)	return;

	sprintf(m_mj.jisu[2].code, "%s", code.Left(index).GetString());
	sprintf(m_mj.jisu[2].name, "%s", code.Mid(++index).GetString());

	CButton	*pButton = (CButton *)GetDlgItem(IDC_JCHECK3);
	pButton->SetWindowText(m_mj.jisu[2].name);
}

void CJisuConfigDlg::OnJcode4() 
{
	CString	code;	GetCode(code);

	if (code.IsEmpty())	return;


	int	index = code.Find("\t");
	if (index <= 0)	return;

	sprintf(m_mj.jisu[3].code, "%s", code.Left(index).GetString());
	sprintf(m_mj.jisu[3].name, "%s", code.Mid(++index).GetString());
	
	CButton	*pButton = (CButton *)GetDlgItem(IDC_JCHECK4);
	pButton->SetWindowText(m_mj.jisu[3].name);
}

void CJisuConfigDlg::OnJcode5() 
{
	//CString	code;	GetCode(code);

	//if (code.IsEmpty())	return;


	//int	index = code.Find("\t");
	//if (index <= 0)	return;

	//sprintf(m_mj.jisu[4].code, "%s", code.Left(index));
	//sprintf(m_mj.jisu[4].name, "%s", code.Mid(++index));
	//
	//CButton	*pButton = (CButton *)GetDlgItem(IDC_JCHECK5);
	//pButton->SetWindowText(m_mj.jisu[4].name);
}
#include "../../H/jmcode.h"
void CJisuConfigDlg::OnBasic() 
{
	const _jisu	ijisu[MAX_JISU] = {	// kospi, kospi200, kosadq, kosdaq50, kospi선물
		{"0001", "코스피종합", 1, RGB(255,  0,  0), PS_SOLID, 1},
		{"0101", "코스피200", 0, RGB(23,134,216), PS_SOLID, 1},
		{"0201", "코스닥종합", 1, RGB(0,  0,255), PS_SOLID, 1},
		/*{"0303", "(코)KOSTAR", 0, RGB(126, 26, 77), PS_SOLID, 1},*/
		{"1"  , "(거)선물", 1, RGB(  0,128,  0), PS_SOLID, 1}	 // 30303, 최근월물
		//{"0"  , "(코)선물", 0, RGB(165,111, 227), PS_SOLID, 1},	 // 30303, 최근월물
	};

	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);

	CString spath, stmp;
	spath.Format("%s", chfile);
	spath.TrimRight();

	int iFind = spath.Find("exe");
	spath = spath.Left(iFind);
	spath += "tab\\fjcode.dat";

	int	codeN{};
	CFile	file;
	struct  fjcode  FJCode {};
	CString strFName;
	if (!file.Open(spath, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}
	// 데이터의 갯수...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct fjcode));
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));
		if (ii == 0)
		{
			strFName = CString(FJCode.hnam, FNameLen);
			strFName.TrimRight();
			strFName = strFName.Right(6);
			strFName = "선물" + strFName;
			break;
		}
	}

	file.Close();

	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		if (ii == 3)
		{
			CopyMemory(&m_mj.jisu[ii], &ijisu[ii], sz_JISU);
			CopyMemory(&m_mj.jisu[ii].name, strFName.GetBuffer(0), strFName.GetLength());
		}
		else
			CopyMemory(&m_mj.jisu[ii], &ijisu[ii], sz_JISU);
	}

	m_mj.version = MJ_VERSION;
	m_mj.compare = CMP_CSIGA;
	m_mj.tick = TK_RIGHT;
	m_mj.fixmode = 0;

	const int	pID[10] = 
	{
		IDC_JCHECK1, IDC_JCHECK2, IDC_JCHECK3,
		IDC_JCHECK4, /*IDC_JCHECK5, */
		IDC_CCMPRADIO, IDC_PCMPRADIO,
		IDC_LTCHECK, IDC_RTCHECK,
		IDC_FIXCHECK
	};

	CButton* pButton{}, * pButton2{};
	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		pButton = (CButton *)GetDlgItem(pID[ii]);
		pButton->SetWindowText(m_mj.jisu[ii].name);
		if (m_mj.jisu[ii].view)	
			pButton->SetCheck(1);
		else
			pButton->SetCheck(0);
	}

	m_jstyle1.SetLine(m_mj.jisu[0].style);
	m_jstyle2.SetLine(m_mj.jisu[1].style);
	m_jstyle3.SetLine(m_mj.jisu[2].style);
	m_jstyle4.SetLine(m_mj.jisu[3].style);
//	m_jstyle5.SetLine(m_mj.jisu[4].style);

	m_jwidth1.SetLineWidth(m_mj.jisu[0].width);
	m_jwidth2.SetLineWidth(m_mj.jisu[1].width);
	m_jwidth3.SetLineWidth(m_mj.jisu[2].width);
	m_jwidth4.SetLineWidth(m_mj.jisu[3].width);
//	m_jwidth5.SetLineWidth(m_mj.jisu[4].width);

	if (m_mj.compare & CMP_CSIGA)
	{
		pButton = (CButton *)GetDlgItem(pID[MAX_JISU]);
		pButton2 = (CButton *)GetDlgItem(pID[MAX_JISU+1]);
		pButton->SetCheck(1);
		pButton2->SetCheck(0);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(pID[MAX_JISU+1]);
		pButton2 = (CButton *)GetDlgItem(pID[MAX_JISU]);
		pButton->SetCheck(1);
		pButton2->SetCheck(0);
	}

	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+2]);
	if (m_mj.tick & TK_LEFT)	pButton->SetCheck(1);
	else				pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+3]);
	if (m_mj.tick & TK_RIGHT)	pButton->SetCheck(1);
	else				pButton->SetCheck(0);
	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+4]);
	if (m_mj.fixmode)	pButton->SetCheck(1);
	else			pButton->SetCheck(0);

	for (int ii = 0; ii < MAX_JISU; ii++)
		m_pColorBtn[ii].SetNewColor(m_mj.jisu[ii].color);

	Invalidate();
}

BOOL CJisuConfigDlg::DestroyWindow() 
{
	if (m_hReg)	FreeLibrary(m_hReg);
	
	return CDialog::DestroyWindow();
}

void CJisuConfigDlg::OnOK() 
{
	const int	pID[10] = 
	{
		IDC_JCHECK1, IDC_JCHECK2, IDC_JCHECK3,
		IDC_JCHECK4, /*IDC_JCHECK5, */
		IDC_CCMPRADIO, IDC_PCMPRADIO,
		IDC_LTCHECK, IDC_RTCHECK,
		IDC_FIXCHECK
	};

	CButton* pButton{};
	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		pButton = (CButton *)GetDlgItem(pID[ii]);
		if (pButton->GetCheck())	m_mj.jisu[ii].view = 1;
		else				m_mj.jisu[ii].view = 0;
	}

	m_mj.jisu[0].style = m_jstyle1.GetLine();
	m_mj.jisu[1].style = m_jstyle2.GetLine();
	m_mj.jisu[2].style = m_jstyle3.GetLine();
	m_mj.jisu[3].style = m_jstyle4.GetLine();
//	m_mj.jisu[4].style = m_jstyle5.GetLine();

	m_mj.jisu[0].width = m_jwidth1.GetLineWidth();
	m_mj.jisu[1].width = m_jwidth2.GetLineWidth();
	m_mj.jisu[2].width = m_jwidth3.GetLineWidth();
	m_mj.jisu[3].width = m_jwidth4.GetLineWidth();
//	m_mj.jisu[4].width = m_jwidth5.GetLineWidth();

	pButton = (CButton *)GetDlgItem(pID[MAX_JISU]);
	if (pButton->GetCheck())	m_mj.compare = CMP_CSIGA;
	else				m_mj.compare = CMP_PJGGA;

	m_mj.tick = 0;
	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+2]);
	if (pButton->GetCheck())	m_mj.tick |= TK_LEFT;

	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+3]);
	if (pButton->GetCheck())	m_mj.tick |= TK_RIGHT;

	pButton = (CButton *)GetDlgItem(pID[MAX_JISU+4]);
	if (pButton->GetCheck())	m_mj.fixmode = 1;
	else				m_mj.fixmode = 0;
	
	CopyMemory(m_pMJ, &m_mj, sz_MINIJISU);

	CDialog::OnOK();
}

void CJisuConfigDlg::GetCode(CString &code)
{
	void (APIENTRY *axGetMiniJisu)(char *);
	axGetMiniJisu = (void (APIENTRY *)(char *)) GetProcAddress(m_hReg, "axGetMiniJisu");
	if (axGetMiniJisu == nullptr)
	{
		TRACE("axisCode.dll axGetMiniJisu error !!\n");
		return;
	}

	char	link[128] = "";
	axGetMiniJisu(link);
	
	code.Format("%s", link);
	code.Trim();
}



