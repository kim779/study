
// inputLDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "inputL.h"
#include "inputLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_USER_LANGUAGE        62

const UINT maskHdr = GVMK_STATE | GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_IMAGE | GVMK_PARAM | GVMK_MISC;
const UINT maskItem = GVMK_STATE | GVMK_FORMAT | GVMK_TEXT | GVMK_FONT | GVMK_ATTR /*| GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_IMAGE | GVMK_PARAM | GVMK_SYMBOL */;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CinputLDlg 대화 상자

CinputLDlg::CinputLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CinputLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	CString sRoot, sFile;
	::GetCurrentDirectory(256, sRoot.GetBuffer(256));
	sRoot.ReleaseBuffer();

	sFile.Format(_T("%s\\workshop.ini"), sRoot);
	::GetPrivateProfileString(_T("Environment"), _T("RootDir"), _T(""), m_root.GetBuffer(256), 256, sFile);
	m_root.ReleaseBuffer();
}

void CinputLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CtlCombo);
}

BEGIN_MESSAGE_MAP(CinputLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CinputLDlg 메시지 처리기

BOOL CinputLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CRect rc;
	CString sData, sHeader, sTxt;
	CStringArray sArr, sArrRow;

	GetDlgItem(IDC_GRID)->GetWindowRect(rc);
	ScreenToClient(rc);

	LoadFile(sData);

	sHeader = Parser(sData, _T("\n"));
	while (!sHeader.IsEmpty())
	{
		sTxt = Parser(sHeader, _T("\t"));
		sTxt.Remove('\r');
		if (!sTxt.IsEmpty())
			sArr.Add(sTxt);
	}
	sTxt.Empty();
	
	if (sArr.GetCount() <= 0)
		return false;

	while (!sData.IsEmpty())
	{
		sTxt = Parser(sData, _T("\n"));
		sTxt.Remove('\r');
		sArrRow.Add(sTxt);
	}

	if (sArrRow.GetCount() > 0)
		sArrRow.RemoveAt(sArrRow.GetCount() - 1);

	int maxrow = max(sArrRow.GetCount(), 100);
	int maxcol = sArr.GetCount();

	m_CtlCombo.AddString(_T("Default"));
	m_Grid.Create(rc, this, 100, GVSC_VERT);
	m_Grid.Initial(maxrow, sArr.GetCount(), 1, 0);

	for (int ii = 0; ii < sArr.GetCount(); ii++)
	{
		m_Grid.SetColumnWidth(ii, 140);
		m_Grid.SetItemText(0, ii, sArr[ii]);				
		m_CtlCombo.AddString(sArr[ii]);
	}

	m_CtlCombo.SetCurSel(0);
	
	GVITEM item;	
	CFont font;
	LOGFONT lf;

	font.CreatePointFont(9, _T("굴림체"));
	font.GetLogFont(&lf);
	lf.lfHeight = 90;

	for (int row = 0; row < maxrow; row++)
	{
		sData.Empty();
		if (row < sArrRow.GetCount())
			sData = sArrRow[row];
		
		for (int ii = 0; ii < maxcol; ii++)
		{
			item.Clear();
			item.state = 0;
			item.font = lf;
			item.col = ii;
			item.row = row + 1;
			item.mask = maskItem;
			item.attr = GVAT_EDIT;
			item.format = GVFM_CENTER;

			if (!sData.IsEmpty())
				AfxExtractSubString(sTxt, sData, ii, '\t');
			else
				sTxt.Empty();
			item.text = sTxt;
			m_Grid.SetItem(&item);
		}
	}

	// grid line
	m_Grid.SetGridColor(RGB(200, 200, 200));
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CinputLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CinputLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CinputLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CinputLDlg::WriteFile(CString sData)
{
	CString sPath;
	CFile file;

	if (sData.IsEmpty())
		return false;

	sPath.Format(_T("%s\\TAB\\language.txt"), m_root);
	file.Open(sPath, CFile::modeCreate | CFile::modeReadWrite);
	WORD wBOM = 0xFEFF;	// or 0xFFFE
	try
	{
		// unicode first 
		file.Write(&wBOM, sizeof(wchar_t));
		file.Write(sData.GetBuffer(), sData.GetLength() * 2);
	}
	catch (CException* pErr)
	{
		pErr->Delete();
		return false;
	}
	file.Close();
	return true;
}

bool CinputLDlg::LoadFile(CString& sData)
{
	CFile file;
	CString sPath;

	sPath.Format(_T("%s\\TAB\\language.txt"), m_root);
	BOOL bflag = file.Open(sPath, CFile::modeRead);
	
	if (!bflag)
		return false;
	try
	{
		UINT  len = (UINT)file.GetLength();
		file.Read(sData.GetBuffer(len), len);
		sData.ReleaseBuffer();
	}
	catch (CException* pErr)
	{
		pErr->Delete();
		return false;
	}
	file.Close();
	return true;
}

void CinputLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString sData, ss, sItem;	
	for (int row = 0; row < m_Grid.GetRowCount(); row++)
	{	
		for (int col = 0; col < m_Grid.GetColumnCount(); col++)
		{			
			sItem = m_Grid.GetItemText(row, col);
			if (col == 0 && sItem.IsEmpty())
				break;
			else if (!ss.IsEmpty())
				ss += _T("\t");

			ss += sItem;
		}
		if (!ss.IsEmpty())
			sData += ss + _T("\r\n");
		ss.Empty();
	}	
	WriteFile(sData);
	
	if (((CinputLApp*)AfxGetApp())->m_hMain)
		::PostMessage(((CinputLApp*)AfxGetApp())->m_hMain, WM_USER, ID_USER_LANGUAGE, 0);
}

CString CinputLDlg::Parser(CString &srcstr, CString substr)
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
	return _T("");
}


int CinputLDlg::hexToInt(wchar_t* szHex)
{
	int hex = 0; // 반환될 값. 초기에는 0이다.
	int nibble; // 16진수의 한 니블(4비트)값을 담아둘 곳

	while (*szHex) {
		hex <<= 4;

		if (*szHex >= '0' && *szHex <= '9') {
			nibble = *szHex - '0';
		}
		else if (*szHex >= 'a' && *szHex <= 'f') {
			nibble = *szHex - 'a' + 10;
		}
		else if (*szHex >= 'A' && *szHex <= 'F') {
			nibble = *szHex - 'A' + 10;
		}
		else {
			nibble = 0;
		}

		hex |= nibble;

		szHex++;
	}
	return hex;
}