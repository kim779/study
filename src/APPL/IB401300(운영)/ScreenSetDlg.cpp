// ScreenSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB401300.h"
#include "ScreenSetDlg.h"
#include "Common.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDC_PANEL2	10205

#define clContents 181
#define clGuide	26
#define	clBack	64
#define clBox	65
#define	clText	69

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		1
#define GAP_BLOCK		5
#define GAP_PANEL		4
#define PANAL_HEIGHT	26
/////////////////////////////////////////////////////////////////////////////
// CScreenSetDlg dialog


CScreenSetDlg::CScreenSetDlg(CWnd* pParent /*=NULL*/, CString root, CString user, CWnd* m_Wizard)
	: CDialog(CScreenSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_sRoot = Variant(homeCC, "");
	m_pWizard	= m_Wizard;
	m_pParent	= pParent;
	m_sRoot		= root;
	m_sUser		= user;

	m_sScreenNo.Empty();		// 옵션 연결화면 번호
	m_sFScreenNo.Empty();		// 선물 연결화면 번호
	m_bDbClick	= TRUE;		// Double Click 유무
	m_bCurScreen	= TRUE;		// 연결화면 현재가 유무
	m_bFCurScreen	= TRUE;
	m_panel2  = NULL;
	m_sFScreenName = DEFAULT_FSCR_NAME;
	m_sScreenName	= DEFAULT_SCR_NAME;

	
}


void CScreenSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreenSetDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenSetDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenSetDlg message handlers

bool CScreenSetDlg::loadConfig()
{
	CString sFile;
	sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, USER_CONFIG);

	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;

	// 클릭 이벤트
	readL = GetPrivateProfileString(USER_SECTION, USER_EVENT_KEY, "2", readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		CButton* btn = NULL;

		if (readB[0] == '1')		// 원클릭
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO_ONE);	btn->SetCheck(TRUE);
			btn = (CButton *) GetDlgItem(IDC_RADIO_DOUBLE);	btn->SetCheck(FALSE);
			m_bDbClick = FALSE;
		}
		else if (readB[0] == '2')	// 더블클릭
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO_ONE);	btn->SetCheck(FALSE);
			btn = (CButton *) GetDlgItem(IDC_RADIO_DOUBLE); btn->SetCheck(TRUE);
			m_bDbClick = TRUE;
		}
	}

	// 옵션 연결 화면 번호
	readL = GetPrivateProfileString(USER_SECTION, USER_SCREEN_NO_KEY, DEFAULT_SCREEN, readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_sScreenNo = CString(readB, readL);
	}

	// 선물 연결 화면 번호
	readL = GetPrivateProfileString(USER_SECTION, USER_FSCREEN_NO_KEY, DEFAULT_FSCREEN, readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_sFScreenNo = CString(readB, readL);
	}

	// 옵션 연결 화면 이름
	readL = GetPrivateProfileString(USER_SECTION, USER_SCR_NAME_KEY, DEFAULT_SCR_NAME, readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_sScreenName = CString(readB, readL);
		
		CStatic*	lable;
		lable = (CStatic *) GetDlgItem(IDC_STATIC_SCRNAME);	lable->SetWindowText(m_sScreenName);
	}

	// 선물 연결 화면 이름
	readL = GetPrivateProfileString(USER_SECTION, USER_FSCR_NAME_KEY, DEFAULT_FSCR_NAME, readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_sFScreenName = CString(readB, readL);
		
		CStatic*	lable;
		lable = (CStatic *) GetDlgItem(IDC_STATIC_FSCRNAME);	lable->SetWindowText(m_sFScreenName);
	}
	 
	// 옵션 연결 화면
	readL = GetPrivateProfileString(USER_SECTION, USER_SCREEN_KEY, "0", readB, sizeof(readB), sFile);
	if (readL > 0) 
	{
		CButton*	btn = NULL;
		CEdit*		edit = NULL;
		
		if (readB[0] == '0')		// 현재가 화면
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO_CUR);	btn->SetCheck(TRUE);
			btn = (CButton *) GetDlgItem(IDC_RADIO_USER);	btn->SetCheck(FALSE);
			edit = (CEdit *) GetDlgItem(IDC_EDIT_SCREEN);	edit->SetWindowText("");
			btn = (CButton *) GetDlgItem(IDC_BUTTON_SEL);	btn->EnableWindow(FALSE);
			m_bCurScreen = TRUE;
		}
		else if (readB[0] == '1')	// 사용자 지정 화면
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO_CUR);	btn->SetCheck(FALSE);
			btn = (CButton *) GetDlgItem(IDC_RADIO_USER);	btn->SetCheck(TRUE);
			btn = (CButton *) GetDlgItem(IDC_BUTTON_SEL);	btn->EnableWindow(TRUE);
			edit = (CEdit *) GetDlgItem(IDC_EDIT_SCREEN);	edit->SetWindowText(m_sScreenNo);
			m_bCurScreen = FALSE;
		}
	}

	// 선물 연결 화면
	readL = GetPrivateProfileString(USER_SECTION, USER_FSCREEN_KEY, "0", readB, sizeof(readB), sFile);
	if (readL > 0) 
	{
		CButton*	btn = NULL;
		CEdit*		edit = NULL;
		
		if (readB[0] == '0')		// 현재가 화면
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO_FCUR);	btn->SetCheck(TRUE);
			btn = (CButton *) GetDlgItem(IDC_RADIO_FUSER);	btn->SetCheck(FALSE);
			edit = (CEdit *) GetDlgItem(IDC_EDIT_FSCREEN);	edit->SetWindowText("");
			btn = (CButton *) GetDlgItem(IDC_BUTTON_FSEL);	btn->EnableWindow(FALSE);
			m_bFCurScreen = TRUE;
		}
		else if (readB[0] == '1')	// 사용자 지정 화면
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO_FCUR);	btn->SetCheck(FALSE);
			btn = (CButton *) GetDlgItem(IDC_RADIO_FUSER);	btn->SetCheck(TRUE);
			btn = (CButton *) GetDlgItem(IDC_BUTTON_FSEL);	btn->EnableWindow(TRUE);
			edit = (CEdit *) GetDlgItem(IDC_EDIT_FSCREEN);	edit->SetWindowText(m_sFScreenNo);
			m_bFCurScreen = FALSE;
		}
	}

	return true;
}

void CScreenSetDlg::saveConfig()
{
	CString sFile, string;
	sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, USER_CONFIG);

	// 클릭 이벤트
	CButton* btn; btn = (CButton *) GetDlgItem(IDC_RADIO_ONE);
	string = _T("2"); if (btn->GetCheck()) string = _T("1");
	WritePrivateProfileString(USER_SECTION, USER_EVENT_KEY, string, sFile);

	// 옵션 연결 화면
	btn = (CButton *) GetDlgItem(IDC_RADIO_CUR);
	string = _T("1"); if (btn->GetCheck()) string = _T("0");
	WritePrivateProfileString(USER_SECTION, USER_SCREEN_KEY, string, sFile);

	// 선물 연결 화면
	btn = (CButton *) GetDlgItem(IDC_RADIO_FCUR);
	string = _T("1"); if (btn->GetCheck()) string = _T("0");
	WritePrivateProfileString(USER_SECTION, USER_FSCREEN_KEY, string, sFile);

	// 옵션 연결 화면 번호
	WritePrivateProfileString(USER_SECTION, USER_SCREEN_NO_KEY, m_sScreenNo, sFile);

	// 선물 연결 화면 번호
	WritePrivateProfileString(USER_SECTION, USER_FSCREEN_NO_KEY, m_sFScreenNo, sFile);

	// 옵션 연결 화면 이름
	WritePrivateProfileString(USER_SECTION, USER_SCR_NAME_KEY, m_sScreenName, sFile);

	// 선물 연결 화면 이름
	WritePrivateProfileString(USER_SECTION, USER_FSCR_NAME_KEY, m_sFScreenName, sFile);
}

BOOL CScreenSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString		userD, userR;

	if (!m_sRoot.IsEmpty() && !m_sUser.IsEmpty())
	{
		userR.Format("%s/%s", m_sRoot, USRDIR);
		userD.Format("%s/%s/%s", m_sRoot, USRDIR, m_sUser);

		CreateDirectory(userR, NULL);
		CreateDirectory(userD, NULL);

		loadConfig();
	}
	HBITMAP BitMap[4]{};
	CStringArray strImage;
	int nLineTick[4]{};
	CRect rcPanel2;
	GetClientRect(&rcPanel2);
	rcPanel2.DeflateRect(2,2);
	m_panel2 = std::make_unique<CfxStatic>();
	m_panel2->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rcPanel2, this, IDC_PANEL2);
	m_panel2->SetBkColor(GetIndexColor(clContents));
	
	strImage.RemoveAll();
	//contents
	strImage.Add("AXPANEL0_LT.BMP");
	strImage.Add("AXPANEL0_RT.BMP");
	strImage.Add("AXPANEL0_LB.BMP");
	strImage.Add("AXPANEL0_RB.bmp");
	
	nLineTick[0] = GAP_SIDE;
	nLineTick[1] = 0;
	nLineTick[2] = GAP_SIDE;
	nLineTick[3] = GAP_BOTTOM;

	for(int i = 0; i < strImage.GetSize(); i++)
	{	
		//CString sImageDir = m_sRoot + "\\image\\" + strImage.GetAt(i);
		
		BitMap[i] = getBitmap2(strImage.GetAt(i));
	}
	m_panel2->SetCornerRound(BitMap, 5, &nLineTick[0]);
	m_panel2->SetBorderColor(GetIndexColor(clBack));
	m_backBrush.CreateSolidBrush(GetIndexColor(clBack));
	m_dataBrush.CreateSolidBrush(GetIndexColor(clContents));
	m_clrInBack = GetIndexColor(clContents);
	m_clrBack = GetIndexColor(clBack);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScreenSetDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	if (m_panel2)	//delete m_panel2;
		m_panel2 = nullptr;
	saveConfig();
}

BOOL CScreenSetDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CButton*	button	= NULL;
	CEdit*		edit	= NULL;
	CStatic*	lable	= NULL;
	BOOL		bCheck	= FALSE;

	switch (LOWORD(wParam))
	{
	case IDC_RADIO_ONE:
		button = (CButton *)GetDlgItem(IDC_RADIO_ONE); button->SetCheck(TRUE);
		button = (CButton *)GetDlgItem(IDC_RADIO_DOUBLE); button->SetCheck(FALSE);
		break;

	case IDC_RADIO_DOUBLE:
		button = (CButton *)GetDlgItem(IDC_RADIO_ONE); button->SetCheck(FALSE);
		button = (CButton *)GetDlgItem(IDC_RADIO_DOUBLE); button->SetCheck(TRUE);
		break;

	case IDC_RADIO_CUR:
		button = (CButton *)GetDlgItem(IDC_RADIO_CUR); button->SetCheck(TRUE);
		button = (CButton *)GetDlgItem(IDC_RADIO_USER); button->SetCheck(FALSE);
		button = (CButton *)GetDlgItem(IDC_BUTTON_SEL); button->EnableWindow(FALSE);
		edit = (CEdit *) GetDlgItem(IDC_EDIT_SCREEN);	edit->SetWindowText("");edit->ModifyStyle(0, WS_DISABLED);
		lable = (CStatic *)GetDlgItem(IDC_STATIC_SCRNAME); lable->SetWindowText("옵션현재가");
		break;
	case IDC_RADIO_FCUR:
		button = (CButton *)GetDlgItem(IDC_RADIO_FCUR); button->SetCheck(TRUE);
		button = (CButton *)GetDlgItem(IDC_RADIO_FUSER); button->SetCheck(FALSE);
		button = (CButton *)GetDlgItem(IDC_BUTTON_FSEL); button->EnableWindow(FALSE);
		edit = (CEdit *) GetDlgItem(IDC_EDIT_FSCREEN);	edit->SetWindowText("");edit->ModifyStyle(0, WS_DISABLED);
		lable = (CStatic *)GetDlgItem(IDC_STATIC_FSCRNAME); lable->SetWindowText("선물현재가");
		break;
	case IDC_RADIO_USER:
		button = (CButton *)GetDlgItem(IDC_RADIO_CUR); button->SetCheck(FALSE);
		button = (CButton *)GetDlgItem(IDC_RADIO_USER); button->SetCheck(TRUE);
		button = (CButton *)GetDlgItem(IDC_BUTTON_SEL); button->EnableWindow(TRUE);
		edit = (CEdit *)GetDlgItem(IDC_EDIT_SCREEN);	edit->SetWindowText(m_sScreenNo);edit->ModifyStyle(WS_DISABLED, 0);
		button = (CButton *)GetDlgItem(IDC_BUTTON_SEL); button->SetFocus();
		lable = (CStatic *)GetDlgItem(IDC_STATIC_SCRNAME); lable->SetWindowText(m_sScreenName);
		break;
	case IDC_RADIO_FUSER:
		button = (CButton *)GetDlgItem(IDC_RADIO_FCUR); button->SetCheck(FALSE);
		button = (CButton *)GetDlgItem(IDC_RADIO_FUSER); button->SetCheck(TRUE);
		button = (CButton *)GetDlgItem(IDC_BUTTON_FSEL); button->EnableWindow(TRUE);
		edit = (CEdit *)GetDlgItem(IDC_EDIT_FSCREEN);	edit->SetWindowText(m_sFScreenNo);edit->ModifyStyle(WS_DISABLED, 0);
		button = (CButton *)GetDlgItem(IDC_BUTTON_FSEL); button->SetFocus();
		lable = (CStatic *)GetDlgItem(IDC_STATIC_FSCRNAME); lable->SetWindowText(m_sFScreenName);
		break;
	case IDC_BUTTON_SEL:
		{
			button = (CButton *)GetDlgItem(IDC_RADIO_USER); 
			edit = (CEdit *)GetDlgItem(IDC_EDIT_SCREEN);
			bCheck = button->GetCheck();
			CString userM;
			if (bCheck)
			{
				char readB[128];
				CString	string, stringx;	
				CString sFile;
				sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, USER_CONFIG);
				
				ZeroMemory(readB, sizeof(readB));
				m_pParent->GetParent()->GetParent()->SendMessage(WM_USER + 666, 0, (LPARAM) readB);
				string = CString(readB);
				if (string.GetLength() <= 0) break;
				
				stringx = parseX(string, "\t");
				
				edit->SetWindowText(stringx);				

				// 연결 화면 번호
				m_sScreenNo = stringx;
				WritePrivateProfileString(USER_SECTION, USER_SCREEN_NO_KEY, m_sScreenNo, sFile);

				// 연결 화면 이름
				m_sScreenName = string;
				lable = (CStatic *)GetDlgItem(IDC_STATIC_SCRNAME); 
				lable->SetWindowText(m_sScreenName);
				WritePrivateProfileString(USER_SECTION, USER_SCR_NAME_KEY, m_sScreenName, sFile);
			}
		}
		break;
	case IDC_BUTTON_FSEL:
		{
			button = (CButton *)GetDlgItem(IDC_RADIO_FUSER); 
			edit = (CEdit *)GetDlgItem(IDC_EDIT_FSCREEN);
			bCheck = button->GetCheck();
			CString userM;
			if (bCheck)
			{
				char readB[128];
				CString	string, stringx;	
				CString sFile;
				sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, USER_CONFIG);
				
				ZeroMemory(readB, sizeof(readB));
				m_pParent->GetParent()->GetParent()->SendMessage(WM_USER + 666, 0, (LPARAM) readB);
				string = CString(readB);
				if (string.GetLength() <= 0) break;
				
				stringx = parseX(string, "\t");
				
				edit->SetWindowText(stringx);				
				
				// 연결 화면 번호
				m_sFScreenNo = stringx;
				WritePrivateProfileString(USER_SECTION, USER_FSCREEN_NO_KEY, m_sFScreenNo, sFile);
				
				// 연결 화면 이름
				m_sFScreenName = string;
				lable = (CStatic *)GetDlgItem(IDC_STATIC_FSCRNAME); 
				lable->SetWindowText(m_sFScreenName);
				WritePrivateProfileString(USER_SECTION, USER_FSCR_NAME_KEY, m_sFScreenName, sFile);
			}
		}
		break;
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

CString CScreenSetDlg::parseX(CString &srcstr, CString substr)
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

void CScreenSetDlg::OnButtonInit() 
{
	CButton*	button	= NULL;
	CEdit*		edit	= NULL;
	CStatic*	lable	= NULL;

	CString		sFile;
	sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, USER_CONFIG);

	button = (CButton *)GetDlgItem(IDC_RADIO_ONE); button->SetCheck(FALSE);
	button = (CButton *)GetDlgItem(IDC_RADIO_DOUBLE); button->SetCheck(TRUE);

	// 옵션 ----------------------------------------------------------------------------
	button = (CButton *)GetDlgItem(IDC_RADIO_CUR); button->SetCheck(TRUE);
	button = (CButton *)GetDlgItem(IDC_RADIO_USER); button->SetCheck(FALSE);
	edit = (CEdit *) GetDlgItem(IDC_EDIT_SCREEN);	edit->SetWindowText("");
	button = (CButton *)GetDlgItem(IDC_BUTTON_SEL); button->EnableWindow(FALSE);
	lable = (CStatic *)GetDlgItem(IDC_STATIC_SCRNAME); lable->SetWindowText(DEFAULT_SCR_NAME);

	m_sScreenNo = DEFAULT_SCREEN;
	WritePrivateProfileString(USER_SECTION, USER_SCREEN_NO_KEY, m_sScreenNo, sFile);
	
	m_sScreenName = DEFAULT_SCR_NAME;
	WritePrivateProfileString(USER_SECTION, USER_SCR_NAME_KEY, m_sScreenName, sFile);
	// ----------------------------------------------------------------------------------
	
	// 선물 ----------------------------------------------------------------------------
	button = (CButton *)GetDlgItem(IDC_RADIO_FCUR); button->SetCheck(TRUE);
	button = (CButton *)GetDlgItem(IDC_RADIO_FUSER); button->SetCheck(FALSE);
	edit = (CEdit *) GetDlgItem(IDC_EDIT_FSCREEN);	edit->SetWindowText("");
	button = (CButton *)GetDlgItem(IDC_BUTTON_FSEL); button->EnableWindow(FALSE);
	lable = (CStatic *)GetDlgItem(IDC_STATIC_FSCRNAME); lable->SetWindowText(DEFAULT_FSCR_NAME);
	
	m_sFScreenNo = DEFAULT_FSCREEN;
	WritePrivateProfileString(USER_SECTION, USER_FSCREEN_NO_KEY, m_sFScreenNo, sFile);
	
	m_sFScreenName = DEFAULT_FSCR_NAME;
	WritePrivateProfileString(USER_SECTION, USER_FSCR_NAME_KEY, m_sFScreenName, sFile);
	// ----------------------------------------------------------------------------------
}

void CScreenSetDlg::OnButtonOk() 
{
	CDialog::OnOK();		
}

void CScreenSetDlg::OnButtonCancel() 
{
	CDialog::OnCancel();
}

BOOL CScreenSetDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class


	return CDialog::Create(IDD, pParentWnd);
}

COLORREF CScreenSetDlg::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

HBITMAP CScreenSetDlg::getBitmap2(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", m_sRoot, IMAGEDIR, bmps);
	return ((CBitmap *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();

}

int CScreenSetDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DoModal();
}

void CScreenSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, m_clrBack);
	rc.DeflateRect(3,3);
	dc.FillSolidRect(rc, m_clrInBack);
	//dc.SelectObject(&m_dataBrush);
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CScreenSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	const int nID = pWnd->GetDlgCtrlID();
	//pDC->SelectObject(&m_dataBrush);
	//if (nID == ID_STATIC)
	{
		pDC->SetBkColor(GetIndexColor(clContents));
		//pDC->SetBkMode(TRANSPARENT);
		return m_dataBrush;
	}
	

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
