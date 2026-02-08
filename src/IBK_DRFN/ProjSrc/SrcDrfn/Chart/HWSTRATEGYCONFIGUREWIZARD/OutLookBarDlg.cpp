// OutLookBarDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgStrategyConfigure.h"
#include "OutLookBarDlg.h"
#include "StrategyConfigureWizardDef.h"
#include "../Common_ST/STControllerDef.h"
#include "../../inc/ISTManager.h"
#include "../../inc/IKSLibMng.h"
#include ".\outlookbardlg.h"


#define IDC_OUTLOOKBAR		1000
#define IDC_STListCtrl		1001

const UINT RMSG_OB_ITEMCLICK = ::RegisterWindowMessage("RMSG_OB_ITEMCLICK");
const UINT RMSG_OB_ITEMDBLCLK = ::RegisterWindowMessage("RMSG_OB_ITEMDBLCLK");

#define CRBACKGROUND		RGB(177,199,234)
#define DEFAULT_BKGRND_COLOR		RGB(247,248,252)

// COutLookBarDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COutLookBarDlg, CDialog)
COutLookBarDlg::COutLookBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutLookBarDlg::IDD, pParent)
{
	m_pParentWnd = pParent;
	m_crBk = CRBACKGROUND;
	m_arSTPath.RemoveAll();
}

COutLookBarDlg::~COutLookBarDlg()
{
}

void COutLookBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrategyList)
	DDX_Control(pDX, IDC_BTN_FINDST, m_btnFindST);
	DDX_Control(pDX, IDC_BTN_NEXTST, m_btnNextST);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutLookBarDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_FINDST, OnBnClickedBtnFindst)
	ON_BN_CLICKED(IDC_BTN_NEXTST, OnBnClickedBtnNextst)
END_MESSAGE_MAP()


// COutLookBarDlg 메시지 처리기입니다.

extern "C" __declspec(dllexport) LONG SCW_GetInterfaceOfStrategy(LONG dwType/*=0*/);

BOOL COutLookBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pSTLoadMnger = (CSTLoadManager*)::SCW_GetInterfaceOfStrategy(0);
	CString szSTData_STPath = ((CDlgStrategyConfigure*)m_pParentWnd)->m_strDataDir + "\\STDATA\\ST";
	CString szSTUser_STPath = ((CDlgStrategyConfigure*)m_pParentWnd)->m_strUserDir + "\\STDATA\\ST";
	//<<20100427_JS.Kim 솔로몬. 폴더 셋팅
	//CString szImageChartDir = ((CDlgStrategyConfigure*)m_pParentWnd)->m_strImageDir + "\\drchart";
	CString szImageChartDir = ((CDlgStrategyConfigure*)m_pParentWnd)->m_strDataDir + "\\Image\\drchart";
	//>>

	m_arSTPath.Add(szSTData_STPath);
	m_arSTPath.Add(szSTUser_STPath);

	//m_ilSTIcon.Create(IDB_IL_STICON, 16, 0, RGB(0,128,128));

	//W151_H21===========================================================================
	BOOL bOK = FALSE;
//	CBitmap bitmap;
//	CString strBitmapFile;	
//	if ( !m_ilW151H21.m_hImageList)
//	{		
//		if ( !m_ilW151H21.Create( 151, 21, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W151_H21.bmp file" );
//		}
//		strBitmapFile.Format("%s\\%s", szImageChartDir,  "W151_H21.bmp");		
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
//		m_ilW151H21.Add(&bitmap, RGB(255, 0, 255));
//		bitmap.DeleteObject();
//	}
//
//	//icon image===========================================================================
//	CString szILPath = szImageChartDir + "\\treeIcon.bmp";
//	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)szILPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
//	m_ilSTIcon.Create(9, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
//	m_ilSTIcon.Add(&bitmap, RGB(255, 0, 255));
//	bitmap.DeleteObject();
	CBitmap bitmap;
	CString strImageDir;
	strImageDir.Format("%s\\folder.bmp", szImageChartDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageListIcon.Create(18, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageListIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();

	//20061213 정유진 수정 
	//LeftBar 트리뷰헤더 이미지 수정
//@유진삭제 
	strImageDir = "";
	strImageDir.Format("%s\\outbar_icon.bmp", szImageChartDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageOutbarIcon.Create(20, 20, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageOutbarIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();
//@유진삭제 
//@신영삭제
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//		m_crBk = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//	else
		m_crBk = DEFAULT_BKGRND_COLOR;

	m_brBkColor.CreateSolidBrush(m_crBk);

	// 아웃룩바 생성
	DWORD dwf = fDragItems|fEditGroups|fEditItems|fRemoveGroups|
		fRemoveItems|fAddGroups|fAnimation;

	m_wndOutBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,100,100), this, IDC_OUTLOOKBAR, dwf);
	m_wndOutBar.SetOwner(this);
	m_wndOutBar.SetAnimationTickCount(20);
//	m_wndOutBar.SetImageList(&m_ilSTIcon, fSmallIcon);
//	m_wndOutBar.SetFolderImageList(&m_ilW151H21, 0, 2, 3);
	m_wndOutBar.SetImageList(&m_ImageListIcon, fSmallIcon);
	m_wndOutBar.SetFolderImageList(&m_ImageOutbarIcon, 0, 2, 3);

	COLORREF clrOLBkgrnd;
	COLORREF clrOLOutLine;
	if(((CDlgStrategyConfigure*)GetParent())->GetOutBarColor(clrOLBkgrnd, clrOLOutLine))
	{
		m_wndOutBar.cr3dFace = clrOLOutLine;
		m_wndOutBar.cr3dUnFace = clrOLOutLine;
		m_wndOutBar.crBackGroundColor1 = clrOLBkgrnd;
	}

	// 전략 로드
	LoadSTList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COutLookBarDlg::LoadSTList()
{
//	m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ilSTIcon);
	m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ImageListIcon);
	m_pSTLoadMnger->SetUseBoldFont(FALSE);
	m_pSTLoadMnger->UnloadST();

	int p_LoadOption = 1;
	m_pSTLoadMnger->LoadST2(&m_wndOutBar, &m_arSTPath, 0, 0, p_LoadOption);

	m_wndOutBar.MoveWindow(GetRect(IDC_OUTLOOKBAR));
}

void COutLookBarDlg::UpdateSTList(int nState, CString szSTPath)
{
	m_pSTLoadMnger->UpdateST(nState, szSTPath);
	m_wndOutBar.MoveWindow(GetRect(IDC_OUTLOOKBAR));
}

CString COutLookBarDlg::GetSTPathSelected(CString& szSTType)
{
	if(!m_szCurSTType.IsEmpty())
		szSTType = m_szCurSTType;

	return m_szCurFolder;
}

void COutLookBarDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	Resize();
}

void COutLookBarDlg::Resize()
{
	if(m_wndOutBar.GetSafeHwnd()) 
		m_wndOutBar.MoveWindow(GetRect(IDC_OUTLOOKBAR));
}

extern "C" __declspec(dllexport) void SCW_DeleteInterfaceOfStrategy(LONG dwKey, LONG dwType);
void COutLookBarDlg::OnDestroy()
{
	CDialog::OnDestroy();
	m_brBkColor.DeleteObject();
//	m_ilSTIcon.DeleteImageList();
//	m_ilW151H21.DeleteImageList();
	m_ImageListIcon.DeleteImageList();
	m_ImageOutbarIcon.DeleteImageList();

	m_pSTLoadMnger->UnloadST();
	if(m_pSTLoadMnger) ::SCW_DeleteInterfaceOfStrategy((LONG)m_pSTLoadMnger, 0);
}

void COutLookBarDlg::SetSTParent(CWnd* pParent)
{
	m_pParentWnd = pParent;
}

CRect COutLookBarDlg::GetRect(int nID)
{
	CRect rect;
	switch(nID)
	{
	case IDC_OUTLOOKBAR:
		{
			GetClientRect(rect);
			rect.left += 5;
			rect.top += 32;
			rect.bottom -= 6;
		}
		break;
	default:
		break;
	}
	return rect;
}

HBRUSH COutLookBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor) 
	{
//	case CTLCOLOR_DLG:
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_EDIT:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}
}

LRESULT COutLookBarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == RMSG_OB_ITEMDBLCLK) 
	{
		STMNG_ITEMINFO* pArg = (STMNG_ITEMINFO*)lParam;

		m_RunSTInfo.nSTType = pArg->nSTType;
		m_RunSTInfo.szSTName = pArg->szSTName;
		m_RunSTInfo.szSTPath = pArg->szSTPath;
		m_RunSTInfo.szSTRData = pArg->szSTRData;
		m_RunSTInfo.nFolderIndx = pArg->nFolderIndx;
		m_RunSTInfo.nItemIndx = pArg->nItemIndx;

		// 선택된 전략경로 (선택 전략명 & STType 여부)
		CString szFolderPath;
		szFolderPath = pArg->szSTPath;
		szFolderPath += "\\";
		szFolderPath += pArg->szSTName;

		// 선택된 전략 전체경로 (".ST"는 제외)
		STLISTITEMDATA	dataItem;
		dataItem.szSTPath = szFolderPath;
		dataItem.bUserFolder = pArg->nSTType;

		CWnd *pWnd = ((CDlgStrategyConfigure*)GetParent())->GetStrategyList();
		pWnd->SendMessage(UWM_ADDCOMPOUND, (WPARAM)&dataItem);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL COutLookBarDlg::IsUserFolder(int nFolder)
{
	int nTotalFolder = m_wndOutBar.GetFolderCount();
	if(nFolder>=nTotalFolder-2)  return TRUE;

	return FALSE;
}

BOOL COutLookBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터가 쳤을때 죽는 현상 방지
	if(pMsg->message == WM_KEYDOWN)   
	{
		if(pMsg->wParam == VK_F4 && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			pMsg->wParam = VK_ESCAPE;
			return GetParent()->PreTranslateMessage(pMsg);
		}
		else if(pMsg->wParam == VK_RETURN)
		{
			 OnBnClickedBtnFindst();
			return CDialog::PreTranslateMessage(pMsg);
		}
		else
			return FALSE;
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void COutLookBarDlg::OnBnClickedBtnFindst()
{
	CString szSTName;
	GetDlgItemText(IDC_EDIT_FINDST , szSTName);
	if(szSTName.IsEmpty()) {
		AfxMessageBox("검색명을 입력하십시요.");
		return;
	}

	m_nFindIndx = 0;
	m_arFindIndx.RemoveAll();
	int nSTCount = m_pSTLoadMnger->SearchST(szSTName, m_arFindIndx);

	if(nSTCount > 0) {
		int nFolderIndx = -1;
		int nItemIndx = -1;
		CString szSearchST = m_arFindIndx.GetAt(m_nFindIndx);
		::sscanf(szSearchST, "%d:%d", &nFolderIndx, &nItemIndx);

		m_pSTLoadMnger->SetSelectedItem(nFolderIndx, nItemIndx);
	}
	else {
		CString szMsg;
		szMsg.Format("[%s] 에 대한 검색결과가 없습니다.", szSTName);
		AfxMessageBox(szMsg);
	}
}

void COutLookBarDlg::OnBnClickedBtnNextst()
{
	int nSTCount = m_arFindIndx.GetSize();
	if(nSTCount == 0) {
		AfxMessageBox(_T("검색한 결과가 없습니다."));
		return;
	}

	if(++m_nFindIndx < nSTCount) {
		int nFolderIndx = -1;
		int nItemIndx = -1;
		CString szSearchST = m_arFindIndx.GetAt(m_nFindIndx);
		::sscanf(szSearchST, "%d:%d", &nFolderIndx, &nItemIndx);

		m_pSTLoadMnger->SetSelectedItem(nFolderIndx, nItemIndx);
	}
	else {
		m_nFindIndx = 0;
		AfxMessageBox(_T("검색한 결과를 모두 확인했습니다."));
	}
}
