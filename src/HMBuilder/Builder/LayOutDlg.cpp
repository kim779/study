// LayOutDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "LayOutDlg.h"
#include "afxdialogex.h"


// CLayOutDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLayOutDlg, CDialogEx)

CLayOutDlg::CLayOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLayOutDlg::IDD, pParent)
{
	m_sLayOut.Empty();
	m_Map.RemoveAll();
	m_pWnd = nullptr;
}

CLayOutDlg::~CLayOutDlg()
{
}

void CLayOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListLayOut);
}


BEGIN_MESSAGE_MAP(CLayOutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLayOutDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_DELETE, &CLayOutDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CLayOutDlg 메시지 처리기입니다.


BOOL CLayOutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Font.CreatePointFont(100, "굴림체");
	m_ListLayOut.SetFont(&m_Font);
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool CLayOutDlg::Init()
{
	CString	dir = _T(""), name;
	int iIndex = 0;

	CFileFind ff;
	CString sPath;

	sPath.Format("%s\\mtlayout\\*.*", m_sRoot);

	BOOL bWorking = ff.FindFile(sPath); //
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
					m_ListLayOut.AddString(name);
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
	return true;
}

void CLayOutDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSel = m_ListLayOut.GetCurSel();
	CString sMap;

	if (nSel >= 0)
	{
		m_ListLayOut.GetText(nSel, sMap);
		m_Map.Lookup(sMap, m_sLayOut);
	}

	CDialogEx::OnOK();
}


void CLayOutDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_Font.DeleteObject();
	if (m_pWnd)
	{
		m_pWnd->DestroyWindow();
		delete m_pWnd;
	}
}


void CLayOutDlg::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nSel = m_ListLayOut.GetCurSel();
	CString sMap;

	if (nSel >= 0)
	{
		m_ListLayOut.GetText(nSel, sMap);
		m_Map.Lookup(sMap, m_sLayOut);

		m_ListLayOut.DeleteString(nSel);
		::DeleteFile(m_sLayOut);

		m_sLayOut.Empty();
	}

}
