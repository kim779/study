// CustomObjDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CustomObjDlg.h"
#include "afxdialogex.h"
#include "../h/mainvar.h"


// CCustomObjDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCustomObjDlg, CDialogEx)

CCustomObjDlg::CCustomObjDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomObjDlg::IDD, pParent)
{

}

CCustomObjDlg::~CCustomObjDlg()
{
}

void CCustomObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJ, m_listboxObj);
}


BEGIN_MESSAGE_MAP(CCustomObjDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustomObjDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustomObjDlg 메시지 처리기입니다.


BOOL CCustomObjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ObjList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCustomObjDlg::ObjList()
{
	CString path;
	path.Format("%s\\ctmobj\\*.*",(CString)(char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0));
	//검색 클래스
	CFileFind finder;

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(path); //

	CString fileName;
	CString strTmp;

	while (bWorking)
	{
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		//파일 일때
		if (finder.IsArchived())
		{
			//파일의 이름
			CString _fileName = finder.GetFileName();

			// 현재폴더 상위폴더 썸네일파일은 제외
			if (_fileName == _T(".") ||
				_fileName == _T("..") ||
				_fileName == _T("Thumbs.db")) continue;

			strTmp = fileName = finder.GetFileName();
			strTmp.MakeReverse();
			strTmp = strTmp.Left(3);
			strTmp.MakeReverse();
			strTmp.MakeUpper();
			if (strTmp == "DLL")
				m_listboxObj.AddString(fileName);
			//읽어온 파일 이름을 리스트박스에 넣음
		}
	}
}

void CCustomObjDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int idx  = m_listboxObj.GetCurSel();
	if (idx > -1)
		m_listboxObj.GetText(idx, m_strName);
	CDialogEx::OnOK();
}
