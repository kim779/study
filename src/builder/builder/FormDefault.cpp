// FormDefault.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "axisMBuilder.h"
#include "FormDefault.h"
#include "afxdialogex.h"

#include "h/mainvar.h"
#include "h/seq.H"

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
	m_media = MEDIA_HTS;
}

CFormDefault::~CFormDefault()
{
}

void CFormDefault::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFormDefault, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &CFormDefault::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_HTS, &CFormDefault::OnBnClickedRadioHts)
	ON_BN_CLICKED(IDC_RADIO_MTS, &CFormDefault::OnBnClickedRadioMts)
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
	((CButton*)GetDlgItem(IDC_RADIO_HTS))->SetCheck(BST_CHECKED);


	GetPrivateProfileString(_T("Environment"), _T("Form_Width"), _T("700"), wb, sizeof(wb), m_workpath);
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
	CDialogEx::OnOK();
}


void CFormDefault::OnBnClickedRadioHts()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_width = _T("700");
	((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->SetWindowText(m_width);
	m_media = MEDIA_HTS;
}


void CFormDefault::OnBnClickedRadioMts()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_width = _T("380");
	((CEdit*)GetDlgItem(IDC_EDIT_WIDTH))->SetWindowText(m_width);
	m_media = MEDIA_MTS;
}
