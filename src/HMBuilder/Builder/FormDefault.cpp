// FormDefault.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "FormDefault.h"
#include "afxdialogex.h"

#include "h/mainvar.h"

// CFormDefault 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFormDefault, CDialogEx)

CFormDefault::CFormDefault(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFormDefault::IDD, pParent)
{
	m_workpath = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	m_workpath += _T("\\workshop.ini");

	m_formname = _T("formName");
	m_desc = _T("Untitled");
	m_Title = _T("form");
	m_width = _T("");
	m_height = _T("");
	m_fontname = _T("");
	m_fontsize = _T("");
	m_taborder = NONE;
	m_sLayOut.Empty();
}

CFormDefault::~CFormDefault()
{
}

void CFormDefault::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_Listlayout);
}


BEGIN_MESSAGE_MAP(CFormDefault, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &CFormDefault::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CFormDefault::OnBnClickedButtonFont)
	ON_BN_CLICKED(IDC_LIST_DELETE, &CFormDefault::OnBnClickedListDelete)
END_MESSAGE_MAP()


// CFormDefault 메시지 처리기입니다.


BOOL CFormDefault::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CEdit*)GetDlgItem(IDC_EDIT_FORM))->SetFocus();

	m_taborder = NAME;

	char	wb[256];
	CString	tmp;

	((CEdit*)GetDlgItem(IDC_EDIT_FORM))->LimitText(8);
	((CEdit*)GetDlgItem(IDC_EDIT_DESC))->LimitText(20);
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->LimitText(20);
	((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->LimitText(4);
	((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->LimitText(4);
	((CEdit*)GetDlgItem(IDC_EDIT_SIZE))->LimitText(2);
	
	((CEdit*)GetDlgItem(IDC_EDIT_FORM))->SetWindowText(m_formname);
	((CEdit*)GetDlgItem(IDC_EDIT_DESC))->SetWindowText(m_desc);
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetWindowText(m_Title);

	GetPrivateProfileString(_T("Environment"), _T("Form_Width"), _T("380"), wb, sizeof(wb), m_workpath);
	m_width = CString(wb);
	((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->SetWindowText(m_width);

	GetPrivateProfileString(_T("Environment"), _T("Form_Height"), _T("450"), wb, sizeof(wb), m_workpath);
	m_height = CString(wb);
	((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->SetWindowText(m_height);

	GetPrivateProfileString(_T("Environment"), _T("FONTNAME"), _T("굴림체"), wb, sizeof(wb), m_workpath);
	m_fontname = CString(wb);
	((CEdit*)GetDlgItem(IDC_EDIT_FONT))->SetWindowText(m_fontname);

	GetPrivateProfileString(_T("Environment"), _T("FONTPNT"), _T("10"), wb, sizeof(wb), m_workpath);
	m_fontsize = CString(wb);
	((CEdit*)GetDlgItem(IDC_EDIT_SIZE))->SetWindowText(m_fontsize);


	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFormDefault::OnOK()
{
	OnBnClickedOk();
}

void CFormDefault::OnBnClickedOk()
{
	((CEdit*)GetDlgItem(IDC_EDIT_FORM))->GetWindowText(m_formname);
	((CEdit*)GetDlgItem(IDC_EDIT_DESC))->GetWindowText(m_desc);
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->GetWindowText(m_Title);
	((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->GetWindowText(m_width);
	((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->GetWindowText(m_height);
	((CEdit*)GetDlgItem(IDC_EDIT_FONT))->GetWindowText(m_fontname);
	((CEdit*)GetDlgItem(IDC_EDIT_SIZE))->GetWindowText(m_fontsize);



	int nSel = m_Listlayout.GetCurSel();
	CString sMap;

	if (nSel >= 0)
	{
		m_Listlayout.GetText(nSel, sMap);
		m_Map.Lookup(sMap, m_sLayOut);
	}




	CDialogEx::OnOK();
}

void CFormDefault::OnBnClickedButtonFont()
{
	/*char	*pFname = NULL;
	WORD	*pFstyle = NULL;
	WORD	*pFpoint = NULL;
	CFontDialog	fontD;

	if (fontD.DoModal() == IDOK)
	{
		m_fontname = fontD.GetFaceName();
		((CEdit*)GetDlgItem(IDC_EDIT_FONT))->SetWindowText(m_fontname);
	}*/
}

BOOL CFormDefault::PreTranslateMessage(MSG* pMsg)
{
// 	if(pMsg -> message == WM_KEYDOWN)
// 	{
// 		if(pMsg -> wParam == VK_RETURN)
// 		{
// 			switch (m_taborder)
// 			{
// 			case NAME:
// 				((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetFocus();
// 				((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetSel(0, -1);
// 				m_taborder = TITLE;
// 				break;
// 			case TITLE:
// 				((CEdit*)GetDlgItem(IDC_EDIT_DESC))->SetFocus();
// 				((CEdit*)GetDlgItem(IDC_EDIT_DESC))->SetSel(0, -1);
// 				m_taborder = DESC;
// 				break;
// 			case DESC:
// 				((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->SetFocus();
// 				((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->SetSel(0, -1);
// 				m_taborder = WIDTH;
// 				break;
// 			case WIDTH:
// 				((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->SetFocus();
// 				((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->SetSel(0, -1);
// 				m_taborder = HEIGHT;
// 				break;
// 			case HEIGHT:
// 				((CEdit*)GetDlgItem(IDC_EDIT_FONT))->SetFocus();
// 				((CEdit*)GetDlgItem(IDC_EDIT_FONT))->SetSel(0, -1);
// 				m_taborder = FSIZE;
// 				break;
// 			case FSIZE:
// 				((CEdit*)GetDlgItem(IDC_OK))->SetFocus();
// 				m_taborder = NONE;
// 				break;
// 			default:
// 			case NONE:
// 				((CEdit*)GetDlgItem(IDC_OK))->SetFocus();
// 				OnBnClickedOk();
// 				break;
// 			}
// 			return false;
// 		}
// 	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CFormDefault::OnCommand(WPARAM wParam, LPARAM lParam)
{
	/*switch (LOWORD(wParam))
	{
	case IDC_EDIT_TITLE:
		if (HIWORD(wParam) == )
		{
		}
		break;
	}*/
	return CDialogEx::OnCommand(wParam, lParam);
}

bool CFormDefault::Init()
{
	CString	dir = _T(""), name;
	int iIndex = 0;

	CFileFind ff;
	CString sPath, sRoot;
	m_Listlayout.AddString("NONE");
	sRoot = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	sPath.Format("%s\\mtlayout\\*.*", sRoot);

	BOOL bWorking = ff.FindFile(sPath); 
	while (bWorking)
	{
		bWorking = ff.FindNextFile();
		// IsFile
		if (ff.IsArchived())
		{
			CString _fileName = ff.GetFileName();


			dir = ff.GetFilePath();
			// folder 
			if (_fileName == _T(".") ||
				_fileName == _T("..") ||
				_fileName == _T("Thumbs.db")) continue;

			if (_fileName.Find(".xml") != -1)
			{
				name = ff.GetFileTitle();
				if (!name.IsEmpty())
				{
					m_Listlayout.AddString(name);
					m_Map.SetAt(name, dir);
				}
			}
		}
		// IsDirectory
		//if (finder.IsDirectory())
		//{
		//DirName = finder.GetFileName();
		//}
	}
	/*
	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_STATIC1);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	m_pWnd = new CPreWnd();
	if (!m_pWnd->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_DISABLED, rc, this, AFX_IDW_PANE_FIRST, NULL))
	{
	TRACE0(_T("Failed to create view window\n"));
	return -1;
	}
	*/

	m_Listlayout.SelectString(0, _T("NONE"));
	return true;
}


void CFormDefault::OnBnClickedListDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSel = m_Listlayout.GetCurSel();
	CString sMap;

	if (nSel > 0)
	{
		m_Listlayout.GetText(nSel, sMap);
		m_Map.Lookup(sMap, m_sLayOut);

		m_Listlayout.DeleteString(nSel);
		::DeleteFile(m_sLayOut);

		m_sLayOut.Empty();
	}
}
