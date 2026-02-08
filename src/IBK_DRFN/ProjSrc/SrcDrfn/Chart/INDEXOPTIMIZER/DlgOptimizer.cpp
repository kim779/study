// DlgOptimizer.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "DlgOptimizer.h"
//Include//
#include "../Common_ST/StrategyLoader.h"
#include "../Common_ST/STControllerDef.h"
#include ".\dlgoptimizer.h"
#include "../../inc/ISTManager.h"
#include "../../inc/IKSLibMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptimizer dialog
#define CRBACKGROUND	RGB(211,223,241)
#define DEFAULT_BKGRND_COLOR		RGB(247,248,252)

#define IDC_GRID	0x00100
#define IDC_GRID2   0x00111


// CDlgSetEnv dialog
#define		NID_CODEINPUT		4578
#define		NID_ACCTINPUT		4579
#define		NID_FUTOPTINPUT		4582

// Strage Combo Mecro //////////////////////////////////////////////
#define STR_INDEX_CROSSABOVE_BASIS		"지표선이 기준선 상향 돌파"//기준선상향 (0선이 아닐경우 사용자인터페이스에 따라 옵션에 추가)
#define STR_INDEX_CROSSBELOW_BASIS		"지표선이 기준선 하향 돌파"//기준선하향(0일경우 0을 옵션에 추가) 
#define STR_INDEX_CROSSABOVE_ANOTHER	"지표선이 Signal선 상향 돌파"//Signal 선 상향.. + SignalIndex(Option)
#define STR_INDEX_CROSSBELOW_ANOTHER	"지표선이 Signal선 하향 돌파"//Signal 선 하향  +SignalIndex(Option)
#define STR_PRICE_CROSSABOVE_ANOTHER	"주가가 하한선(밴드) 상향 돌파"//주가가가 상향(한선 Index)
#define STR_PRICE_CROSSBELOW_ANOTHER	"주가가 하한선(밴드) 하향 돌파"//주가가 하향 

#define STR_INDEX_ZEROABOVE_BASIS     "지표선이 0선 상향 돌파"
#define STR_INDEX_ZEROBELOW_BASIS     "지표선이 0선 하향 돌파"

#define STR_INDEX_ABOVEBAN_BASIS	"지표선 상승반전"
#define STR_INDEX_BELOWBAN_BASIS	"지표선 하락반전"
	
///////////////////////////////////////////////////////////////////
#define		IDC_OUTLOOKBAR			1000
#define		IDC_STListCtrl			1001 //
#define MAX_COL 15
#define FILE_OPTIMIZE_CFG	"\\STDATA\\IndexOptimize.ini"

const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");
const UINT RMSG_OB_ITEMDBLCLK	= ::RegisterWindowMessage("RMSG_OB_ITEMDBLCLK");
const UINT RMSG_GETDLLSIZE		= ::RegisterWindowMessage("RMSG_GETDLLSIZE");

CDlgOptimizer::CDlgOptimizer(CWnd* pParent /*=NULL*/)
	: CEscDialog(CDlgOptimizer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptimizer)
//	m_EditGijun = 1;
	//}}AFX_DATA_INIT
	m_crBk = CRBACKGROUND;
	m_pStrategyItem = NULL;
	m_strGijun = "일";
	m_Code = "";
	m_nGijun = G_DATE;
	m_bReceive = FALSE;
	m_nTest =0;
	m_lSendNum =0;

	m_pDlgTimeSetup = NULL;
	m_nOptimizerDiv = 1001;
	m_pChartInterface = NULL;
	m_bGridClick = FALSE;
	m_STValNum = 0;
}


void CDlgOptimizer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptimizer)
	DDX_Control(pDX, IDC_BTN_ADDALL, m_CtlAddAll);
	DDX_Control(pDX, IDC_BTN_ADDONE, m_CtlAddOne);
	DDX_Control(pDX, IDC_BTN_DELALL, m_CtlDelAll);
	DDX_Control(pDX, IDC_BTN_DELONE, m_CtlDelOne);
	DDX_Control(pDX, IDC_BTN_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BTN_OPT, m_BtnOpt);
	DDX_Control(pDX, IDC_TAB1, m_CtlTab);
	DDX_Control(pDX, IDC_LIST_ST, m_listST);
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_PROGRESS, m_CtlProgress);
}


BEGIN_MESSAGE_MAP(CDlgOptimizer, CDialog)
	//{{AFX_MSG_MAP(CDlgOptimizer)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OPT, OnBtnOpt)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WMU_SET_KEYCODE, OnSetKeyCodeChange)
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessagge)
	ON_BN_CLICKED(IDC_BTN_ADDALL, OnBnClickedBtnAddall)
	ON_BN_CLICKED(IDC_BTN_ADDONE, OnBnClickedBtnAddone)
	ON_BN_CLICKED(IDC_BTN_DELALL, OnBnClickedBtnDelall)
	ON_BN_CLICKED(IDC_BTN_DELONE, OnBnClickedBtnDelone)
	ON_REGISTERED_MESSAGE(RMSG_OB_ITEMDBLCLK	,OnRmsgObItemDBClk)
	ON_REGISTERED_MESSAGE(RMSG_GETDLLSIZE, OnRmsgGetDllSize)
	ON_LBN_DBLCLK(IDC_LIST_ST, OnLbnDblclkListSt)
	ON_REGISTERED_MESSAGE(RMSG_STTREE_ITEMDBLCLK,OnRmsgSTTreeItemDBClk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptimizer message handlers
// CDlgAnalysis message handlers
void	CDlgOptimizer::FormInit()
{
}

void CDlgOptimizer::FormLoad()
{
}

BOOL CDlgOptimizer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxEnableControlContainer();

	// TODO: Add extra initialization here
	
	//IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
	//if(pKSLibMng)
	//	m_crBk = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
	//else
	m_crBk = DEFAULT_BKGRND_COLOR;
//	m_crBk = DEFAULT_BKGRND_COLOR;
	m_brBkColor.CreateSolidBrush(m_crBk);
	//((CButton*)GetDlgItem(IDC_RADIO_DAY))->SetCheck(TRUE);
	//m_CtlEditGijun.EnableWindow(FALSE);
	//m_CtlEditBong.SetWindowText("1");
	
	LOGFONT lf;
	::memset(&lf, 0x00, sizeof(LOGFONT));
	lf.lfWidth = 0; 
	lf.lfEscapement = 0; 
	lf.lfOrientation = 0; 
	lf.lfWeight = FW_NORMAL; 
	lf.lfItalic = 0; 
	lf.lfUnderline = 0; 
	lf.lfStrikeOut = 0; 
	lf.lfCharSet = DEFAULT_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = FF_ROMAN; 
	strcpy(lf.lfFaceName, "굴림체");
	lf.lfHeight = 12;
	m_Font.CreateFontIndirect(&lf);

	InitCodeInfo();
	InitTab();
	InitGrid();
	ReSize();
	InitControls();
	LoadOutlookBar();//KHD : OutlookBar
	InitStrategy();
	InitSkin();
	CreateTImeSetupDlg();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//최적화 옵션에 따라 컨트롤과 그리드를 새로 세팅하는 루틴 :KHD
void CDlgOptimizer::HideControl()
{

	
}
//KHD : OutlookBar Add 2006.08.30 
HINSTANCE hTradeInst = NULL;
void CDlgOptimizer::LoadOutlookBar()
{
	CRect rc2;
	GetDlgItem(IDC_STFRAME)->GetWindowRect(rc2);
	ScreenToClient(rc2);
	rc2.top += 3;

	CString szImageChartDir = m_strImageDir + "\\drchart";
	//W190_H21===========================================================================
	BOOL bOK = FALSE;
//	CBitmap bitmap;
//	CString strBitmapFile;
//	CString szImageChartDir = m_strImageDir + "\\drchart";
//	if ( !m_ilW190H21.m_hImageList)
//	{		
//		if ( !m_ilW190H21.Create( 151, 21, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
//		{
//			AfxMessageBox ( "Can not Find W151_H21.bmp file" );
//		}
//		strBitmapFile.Format("%s\\%s", szImageChartDir,  "W151_H21.bmp");		
//		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
//			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
//		m_ilW190H21.Add(&bitmap, RGB(255, 0, 255));
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

	DWORD dwf = fDragItems|fEditGroups|fEditItems|fRemoveGroups|fRemoveItems|fAddGroups|fAnimation;
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		typedef	LONG (FnSCW_IGfxOutBarCtrl)(int nType, long dwValue, long dwReserved1);
		if(!m_hSTConfigureWizardInst)
			m_hSTConfigureWizardInst  = LoadLibrary("Chart\\HWStrategyConfigureWizard.dll");
		if(!m_hSTConfigureWizardInst) return;

		FnSCW_IGfxOutBarCtrl* pFunc = (FnSCW_IGfxOutBarCtrl*)GetProcAddress(m_hSTConfigureWizardInst,_T("SCW_IGfxOutBarCtrl"));
		if(pFunc == NULL) return ;

		m_pWndOutBar = (IGfxOutBarCtrl2*)pFunc(0, 0, 0);

		m_pWndOutBar->CreateOutBar(WS_CHILD|WS_VISIBLE, rc2, this, IDC_OUTLOOKBAR, dwf);
		m_pWndOutBar->Setcr3dFace(RGB(191,211,233));
		m_pWndOutBar->Setcr3dUnFace(RGB(218,218,218));
		m_pWndOutBar->GetCoutBarCtrl()->SetOwner(this);

		int  nfSmallIcon = 1 << 0;
//		m_pWndOutBar->SetImageList(&m_ilSTIcon, nfSmallIcon);
//		m_pWndOutBar->SetFolderImageList(&m_ilW190H21, 0, 2, 3);
		m_pWndOutBar->SetImageList(&m_ImageListIcon, nfSmallIcon);
		m_pWndOutBar->SetFolderImageList(&m_ImageOutbarIcon, 0, 2, 3);
	}
	else
	{
		m_wndOutBar.Create(WS_CHILD|WS_VISIBLE, rc2, this, IDC_OUTLOOKBAR, dwf);
		m_wndOutBar.cr3dFace =  RGB(191,211,233);
		m_wndOutBar.cr3dUnFace = RGB(239,242,247);
		m_wndOutBar.SetOwner(this);

		int  nfSmallIcon = 1 << 0;
//		m_wndOutBar.SetImageList(&m_ilSTIcon, nfSmallIcon);
//		m_wndOutBar.SetFolderImageList(&m_ilW190H21, 0, 2, 3);
		m_wndOutBar.SetImageList(&m_ImageListIcon, nfSmallIcon);
		m_wndOutBar.SetFolderImageList(&m_ImageOutbarIcon, 0, 2, 3);

		COLORREF clrOLBkgrnd;
		COLORREF clrOLOutLine;
		if(m_pChartInterface && ((ISTInterface*)m_pChartInterface)->GetOutBarColor(clrOLBkgrnd, clrOLOutLine))
		{
			m_wndOutBar.cr3dFace = clrOLOutLine;
			m_wndOutBar.cr3dUnFace = clrOLOutLine;
			m_wndOutBar.crBackGroundColor1 = clrOLBkgrnd;
		}
	}
	m_arSTPath.RemoveAll();
	LoadSTList();
}

void CDlgOptimizer::LoadSTList()
{
	//@@@06.12.07[[
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		LoadSTList_SSStyle();
		return;
	}
	//@@@06.12.07]]
	if(gCURRENT_COMPANY_ID == nPCTRComID_EUGENE)
	{
		typedef	LONG (FPSCW_GetInterfaceOfStrategy)(LONG dwType/*=0*/);
		hTradeInst = LoadLibrary("HWStrategyConfigureWizard.dll");
		if(hTradeInst == NULL) return;

		FPSCW_GetInterfaceOfStrategy* pFunc = (FPSCW_GetInterfaceOfStrategy*)GetProcAddress(hTradeInst,_T("SCW_GetInterfaceOfStrategy"));
		if(pFunc == NULL) return ;

		LONG dwType = 0;	//CSTLoadManager*
		m_pSTLoadMnger = (ISTLoadManager2*)pFunc(dwType);
		if(m_pSTLoadMnger)
		{
			CString szDataDir = (LPCSTR)AfxGetPctrData(12);	// 12 : Data Directory
			CString szUserDir = (LPCSTR)AfxGetPctrData(17);	// 17 : User Directory

			CString szSTData_STPath = szDataDir + "\\" + STFOLDER_ST;
			m_arSTPath.Add(szSTData_STPath);
			if(m_nOptimizerDiv != VAL_OPTIMIZER)//변수최적화일때는 사용자 전량 로드 하지 않는다. KHD :2007.1.18 
			{
				CString szSTUser_STPath = szUserDir + "\\" + STFOLDER_ST;
				m_arSTPath.Add(szSTUser_STPath);
			}	

//			m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ilSTIcon);
			m_pSTLoadMnger->Init(TRUE, "*.ST", &m_ImageListIcon);
			m_pSTLoadMnger->SetUseBoldFont(FALSE);

			int p_LoadOption = 1;
			m_pSTLoadMnger->LoadST2(&m_wndOutBar, &m_arSTPath, 0, 0, p_LoadOption);
		}
	}
}

//OutlookBar End
void CDlgOptimizer::InitStrategy()
{
//
//#ifdef _DEBUG
// CString strSystemPath = m_strRootPath +  "\\" + STSYSTEM_D;
// CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
// CString strSystemPath = m_strRootPath + "\\" + STSYSTEM;
// CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//	m_stArrayPath.RemoveAll(); 
//	m_stArrayPath.Add(strSystemPath);
//	m_stArrayPath.Add(strUserPath);
//
//	if(!m_pStrategyItem)
//	{
//		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// 메모리 생성 
//		if(m_pStrategyItem == NULL) return;
//		//소켓, DBMGr, 키값으로 생성 
//	//	int nRetValue = m_pStrategyItem->Create(this,m_hSocket,(long)&m_IDrMLib_CDBMgr,(long)m_pStrategyItem,m_stArrayPath);
//	//	if(nRetValue==-1) 
//		{
//			m_pStrategyItem->DestoryWindow();
//			delete m_pStrategyItem;
//			m_pStrategyItem = NULL;
//		}
//	}
//	m_DlgTrade->SetStrategyItem(m_pStrategyItem);
}
HBRUSH CDlgOptimizer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
void CDlgOptimizer::InitCodeInfo()
{
}

LRESULT CDlgOptimizer::OnSetKeyCodeChange(WPARAM wp, LPARAM lp)
{
	return TRUE;
}

CString CDlgOptimizer::GetJMNameAndGubunFromCode(CString csCode)
{
	return "";
}

void CDlgOptimizer::InitGrid()
{

	if(!m_GridTap.GetSafeHwnd())
	{
		CRect rect1;
		rect1.left = 193;
		rect1.right = 860;
		rect1.bottom = 195;
		rect1.top = -20;
		
		rect1.DeflateRect(0,20);
		m_GridTap.Create(rect1,this,IDC_GRID2,WS_CHILD | WS_TABSTOP | WS_VISIBLE   );
	
		m_GridTap.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
		
		m_GridTap.SetGridLineColor(RGB(204,204,204));
		if(m_nOptimizerDiv == INDEX_OPTIMIZER)
			m_GridTap.SetColumnCount(12);
		if(m_nOptimizerDiv == TIME_OPTIMIZER)
			m_GridTap.SetColumnCount(8);
		if(m_nOptimizerDiv == VAL_OPTIMIZER)
			m_GridTap.SetColumnCount(6);
		m_GridTap.SetFixedRowCount(1);
		m_GridTap.SetColumnResize(TRUE);
		m_GridTap.SetRowCount(1);
		m_GridTap.SetListMode();
		m_GridTap.SetRowHeight(0,20);
		m_GridTap.SetBkColor(RGB(255,255,255));		
		m_GridTap.SetEditable(FALSE);
		m_GridTap.EnableTitleTips(FALSE);
		m_GridTap.SetTrackFocusCell(FALSE);
		m_GridTap.SetSingleColSelection(FALSE);
		m_GridTap.SetSingleRowSelection();
		m_GridTap.SetFixedRowSelection(FALSE);
		m_GridTap.SetFixedColumnSelection(FALSE);
		m_GridTap.SetFont(&m_Font);
	}
}
//전략에 따라 Grid 컬럼 세팅 하는 부분..
void CDlgOptimizer::ReSize()
{
	if(m_GridTap.GetSafeHwnd())
	{
		char *psztitle_Index[MAX_COL]=
		{"Row","전략명","총손익","평균손익","총이익","총손실","최대이익","최대손실","수익률","ProfitFactor","승률","거래"};//전략최적화;
		char *psztitle_Val[MAX_COL]={"Row","총손익","평균손익", "총이익", "총손실", "최대이익"};//변수최적화;
		char *psztitle_Time[MAX_COL]= {"Row","지표명","봉","총손익","평균손익","총이익","총손실","최대이익"};//시간최적화;
		
		int lWidth_Index[MAX_COL]= {0, 100, 70, 60, 70, 60, 75, 75, 60, 80, 45, 45};
		int lWidth_Val[MAX_COL]={0,100, 70, 80, 70, 70};
		int lWidth_Time[MAX_COL]= {0, 120, 7, 75, 50, 75,75,50};
		
		char *psztitle[MAX_COL];
		int  lWidth[MAX_COL];
		switch(m_nOptimizerDiv)
		{
		case INDEX_OPTIMIZER:
			memcpy(psztitle,psztitle_Index,sizeof(psztitle_Index));
			memcpy(lWidth, lWidth_Index, sizeof(lWidth_Index));		
			break;
		case VAL_OPTIMIZER:
			memcpy(psztitle,psztitle_Val,sizeof(psztitle_Val));
			memcpy(lWidth, lWidth_Val, sizeof(lWidth_Val));		
			break;
		case TIME_OPTIMIZER:
			memcpy(psztitle,psztitle_Time,sizeof(psztitle_Time));
			memcpy(lWidth, lWidth_Time, sizeof(lWidth_Time));		
			break;
		}
		
		CString str;
		GV_ITEM Item;
		int Colnum =0;
		if(m_nOptimizerDiv!= VAL_OPTIMIZER || m_pDlgTimeSetup ==NULL)
			Colnum = m_GridTap.GetColumnCount();
		else//변수최적화일경우 //
		{
			Colnum = 6 + m_STValNum;
			m_GridTap.SetColumnCount(Colnum);
		}
		for (int col = 0; col < Colnum; col++)
		{
			
    		Item.mask = GVIF_TEXT;
			Item.row = 0;
			Item.col = col;
			if(m_nOptimizerDiv == VAL_OPTIMIZER && m_pDlgTimeSetup!=NULL)
			{
				if( col < m_STValNum)
				{				
					Item.strText = m_pDlgTimeSetup->m_ValueOptimizerArray.GetAt(col).nTitle;
					m_GridTap.SetColumnWidth(col,65);
				}
				else
				{
					Item.strText = psztitle[col-m_STValNum];
					m_GridTap.SetColumnWidth(col,lWidth[col-m_STValNum]);
				}
			}
			else
			{
				Item.strText = psztitle[col];
				m_GridTap.SetColumnWidth(col,lWidth[col]);
			}
			Item.crBkClr = RGB(230,239,247);//RGB(175,194,221); // or - m_GridSTList.SetItemBkColour(row, col, clr);
			Item.crFgClr = RGB(51,51,51);    // or - m_GridSTList.SetItemFgColour(row, col, RGB(255,0,0));				    
			Item.nFormat = DT_CENTER;
			Item.mask    |= (GVIF_BKCLR|GVIF_FGCLR|GVIF_FORMAT);
			
			m_GridTap.SetItem(&Item);

		}
  		m_GridTap.ExpandColumnsToFit();	
	}

}

void CDlgOptimizer::AutoSizeColumns()
{
	if(m_GridTap.GetSafeHwnd())
	{
		if(m_GridTap.IsVisibleVScroll()==FALSE)
		{
			ReSize();
			return;
		}

		switch(m_nOptimizerDiv)
		{
		case INDEX_OPTIMIZER:
			m_GridTap.AutoSizeColumn(1);
			m_GridTap.AutoSizeColumn(3);
			m_GridTap.AutoSizeColumn(8);
			break;
		case VAL_OPTIMIZER:
			m_GridTap.AutoSizeColumn(5);
			break;
		case TIME_OPTIMIZER:
			m_GridTap.AutoSizeColumn(1);
			m_GridTap.AutoSizeColumn(4);
			m_GridTap.AutoSizeColumn(7);
			break;
		}
	}
}

void CDlgOptimizer::InitTab()
{
	CRect rc;
	m_CtlTab.GetClientRect(&rc);
	rc.left = 193;
	rc.top  = 195;
	m_strDataDir += "\\STData";
	m_CtlTab.SetTopLeftCorner(CPoint(rc.left,rc.top));
	m_CtlTab.SetLeftOffset(0);
	m_CtlTab.SetUpperOffset(0);
	m_CtlTab.SetSelectedColor(RGB(0, 0, 0),CRBACKGROUND);
	m_CtlTab.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
	m_CtlTab.SetTabBkColor(RGB(234,235,233));
	
	m_DlgResult = new CDlgResult;
	m_DlgResult->SetGridFont(&m_Font);
	m_DlgResult->Create(CDlgResult::IDD, this);
	m_DlgResult->SetOptimizer(this);
	m_DlgResult->SetDataDir(m_strDataDir);
	m_DlgResult->OpenWebView("\\IndexOptimizer.htm"); 

	m_arrayTab.Add(m_DlgResult);


	m_DlgSooik  = new CTabSoonikChart;
	m_DlgSooik->Create(CTabSoonikChart::IDD,this);
	m_DlgSooik->SetParentDlg(this);
	m_arrayTab.Add(m_DlgSooik);

	m_DlgSonik  = new CTabSonikChart;
	m_DlgSonik->Create(CTabSonikChart::IDD,this);
	m_DlgSonik->SetParentDlg(this);
	m_arrayTab.Add(m_DlgSonik);

	m_DlgTrade  = new CDlgTrade;
	m_DlgTrade->SetGridFont(&m_Font);
	m_DlgTrade->Create(CDlgTrade::IDD,this);
	m_DlgTrade->SetParentDlg(this);
	m_DlgTrade->SetDataDir(m_strDataDir);
	m_DlgTrade->OpenWebView("\\IOTrade.htm");
	m_arrayTab.Add(m_DlgTrade);

	m_CtlTab.AddTab(m_DlgResult,"성과분석");
	m_CtlTab.AddTab(m_DlgTrade,"매매내역");
	m_CtlTab.AddTab(m_DlgSooik,"수익률차트");
	m_CtlTab.AddTab(m_DlgSonik,"손익분포차트");
}

void CDlgOptimizer::OnDestroy() 
{
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject();
	
	CDialog::OnDestroy();

	m_ImageListIcon.DeleteImageList();
	m_ImageOutbarIcon.DeleteImageList();

	DestoryTImeSetupDlg();

	if(m_pStrategyItem)
	{
		m_pStrategyItem->DestoryWindow();
		delete m_pStrategyItem;
		m_pStrategyItem = NULL;
	}

	OnBnClickedBtnDelall();
	// TODO: Add your message handler code here
	CDialog *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		pTabDlg->DestroyWindow();
		delete pTabDlg;
	}

	ST_VALUESET *ValueSet;
	for(int i =0 ;i< m_arrayValue.GetSize();i++)
	{
		ValueSet = m_arrayValue.GetAt(i);
		delete ValueSet;
	}
	m_arrayTab.RemoveAll();	

	//@@@06.12.07[[
	{
		UnLoadSTList();
	   FreeLibrary(hTradeInst);
		typedef	LONG (FnSCW_IGfxOutBarCtrl)(int nType, long dwValue, long dwReserved1);

		FnSCW_IGfxOutBarCtrl* pFunc = (FnSCW_IGfxOutBarCtrl*)GetProcAddress(m_hSTConfigureWizardInst ,_T("SCW_IGfxOutBarCtrl"));
		if(pFunc == NULL) return ;

		if(m_pWndOutBar) {
			// m_pWndOutBar->GetCoutBarCtrl()->DestroyWindow();
			pFunc(1, (long)m_pWndOutBar, 0);
			m_pWndOutBar = NULL;
		}
	}
	//@@@06.12.07]]
}
void CDlgOptimizer::InitControls()
{
	//Init 지표 Index //
	return;
	CStdioFile IndexFile;
	CString strIndexData,str;
	if(!IndexFile.Open(m_strDataDir+"\\SystemTradingIndexList.dat",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}
// 추가 시키지 말아야 할 종목들
	char str1[265];
	char* p ;
	CStringArray RemoveData;
	::GetPrivateProfileString("REMOVE","DATA","No",str1,80,m_strDataDir+FILE_OPTIMIZE_CFG);
	 p = strtok(str1,",");
	while(p!=NULL)
	{
		RemoveData.Add(p);
		p = strtok(NULL,",");		
	}
////////////////////////////////////////////
	for(;;){
		if(IndexFile.ReadString(str))
		{
			//지표명 Combo Init 
			int start = str.Find(";",0);
			strIndexData = str.Left(start);
			BOOL IsData = TRUE;
			for(int i=0;i<RemoveData.GetSize();i++)
			{
				if(RemoveData.GetAt(i)==strIndexData)
				{
					RemoveData.RemoveAt(i);
					IsData = FALSE;
					break;
				}
			}
			if(IsData == FALSE) continue;
	//		m_CtlComIndex.AddString(strIndexData);

			str.Delete(0,start+1);

			//SD 파일 저장 
			start = str.Find(";",0);
			strIndexData = str.Left(start);	
			m_stArraySDFileName.Add(strIndexData);
			str ="";
		}
		else
			break;
	}	
	IndexFile.Close();


    
}


void CDlgOptimizer::MedoMesuComboDataParsing()
{

	//CString strIndex;
	//strIndex = SearchFileToData("Index",m_strDataDir+FILE_OPTIMIZE_CFG);
	//m_CtlComMesu.ResetContent();
	//m_CtlComMedo.ResetContent();
	////00000 : Signal돌파, 0선 돌파 , 기준선 돌파 , 지표선반전,밴드돌파 
	//if(strIndex == "N")
	//{
	//	AfxMessageBox("현재 전략명에 대한 정보가 없습니다.");
	//	return;
	//}
	//else //00000파싱 
	//{
	//	for(int i = 0 ;i < strIndex.GetLength();i++)
	//	{
	//		CString OneData = strIndex.GetAt(i); 
	//		int DataIndex = atoi(OneData);
	//		switch(i)
	//		{
	//
	//		case 0: //Signal
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSABOVE_ANOTHER);
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSBELOW_ANOTHER);
	//				
	//				m_CtlComMedo.AddString(STR_INDEX_CROSSABOVE_ANOTHER);
	//				m_CtlComMedo.AddString(STR_INDEX_CROSSBELOW_ANOTHER);
	//			}
	//			break;
	//		case 1: // 0 Line
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_ZEROABOVE_BASIS);
	//				m_CtlComMesu.AddString(STR_INDEX_ZEROBELOW_BASIS);

	//				m_CtlComMedo.AddString(STR_INDEX_ZEROABOVE_BASIS);
	//				m_CtlComMedo.AddString(STR_INDEX_ZEROBELOW_BASIS);
	//			}
	//			break;
	//		case 2: // 기준선 : 인터페이스 필요
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSABOVE_BASIS);
	//				m_CtlComMesu.AddString(STR_INDEX_CROSSBELOW_BASIS);

	//				m_CtlComMedo.AddString(STR_INDEX_CROSSABOVE_BASIS);
	//				m_CtlComMedo.AddString(STR_INDEX_CROSSBELOW_BASIS);
	//			}
	//			break;
	//		case 3: // 지표선반전  
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_INDEX_ABOVEBAN_BASIS);
	//				m_CtlComMesu.AddString(STR_INDEX_BELOWBAN_BASIS);

	//				m_CtlComMedo.AddString(STR_INDEX_ABOVEBAN_BASIS);
	//				m_CtlComMedo.AddString(STR_INDEX_BELOWBAN_BASIS);
	//			}
	//			break;
	//		case 4: // 밴드돌파
	//			if(DataIndex)
	//			{
	//				m_CtlComMesu.AddString(STR_PRICE_CROSSABOVE_ANOTHER);
	//				m_CtlComMesu.AddString(STR_PRICE_CROSSBELOW_ANOTHER);
	//				
	//				m_CtlComMedo.AddString(STR_PRICE_CROSSABOVE_ANOTHER);
	//				m_CtlComMedo.AddString(STR_PRICE_CROSSBELOW_ANOTHER);
	//			}
	//			break;
	//		}
	//	}
	//}
	
}

CString CDlgOptimizer::SearchFileToData(CString strIndex, CString strPath)
{
	/*CString strDataName;
	CHAR inBuf[80]; 
	m_CtlComIndex.GetLBText(m_CtlComIndex.GetCurSel(),strDataName);
	::GetPrivateProfileString(strDataName,strIndex,"N",inBuf,80,strPath);
	strIndex = inBuf;*/
	

	return strIndex;
}
//기준값 및 봉값 점검 하는 함수 
BOOL CDlgOptimizer::IsCurrentChartData(CString strValue)
{
	//int nMin;
	//if(m_nGijun == G_MINUTE)//분일 경우 
	//{
	//	m_CtlEditGijun.GetWindowText(m_strGijun);
	//	
	//	nMin = atoi(m_strGijun);
	//	if(nMin<0 || nMin>61)
	//	{
	//		AfxMessageBox("기준분을 1 ~ 60으로 다시 입력하세요");
	//		m_CtlEditGijun.SetFocus();
	//		return FALSE;
	//	}
	//	m_strGijun +="분";
	//}
	//else 
	//	m_strGijun = "일";

	//nMin = atoi(strValue);
	//if(nMin<0||nMin>1001)
	//{
	//	AfxMessageBox("봉수를 1 ~ 1000 사이의 수로 세팅하세요");
	//	m_CtlEditBong.SetFocus();
	//	return FALSE;
	//}	

	return TRUE;
}
//다시 컨트롤 초기화 하는 함수 
void CDlgOptimizer::RemoveAllControlSet()
{
	/*ST_VALUESET *ValueSet;
	for(int j =0; j< m_arrayValue.GetSize();j++)
	{
		ValueSet = m_arrayValue.GetAt(j);
		ValueSet->stEdit->ShowWindow(SW_HIDE);
		ValueSet->stSpinBtn->ShowWindow(SW_HIDE);
		ValueSet->stStatic->ShowWindow(SW_HIDE);
		
		ValueSet->stEdit->SetWindowText("0");
		ValueSet->stEdit->SetReadOnly(FALSE);
		ValueSet->stEdit->MoveWindow(ValueSet->rt);
		
		ValueSet->stSpinBtn->EnableWindow(TRUE);
		
	}*/
}


//최적화 버튼 :KHD

typedef struct {
	long pThisPoint;
}STOPTTHREAD;
UINT StartOptThread(LPVOID pParam)
{
	STOPTTHREAD* pInData = (STOPTTHREAD*)pParam;
	CDlgOptimizer* pDlg = (CDlgOptimizer*)pInData->pThisPoint;
	pDlg->StartOpt();

	return 0;
}

void CDlgOptimizer::StartOpt() 
{
	STOPTTHREAD inData;
	inData.pThisPoint = (long)this;

	AfxBeginThread(StartOptThread, &inData);
}

void CDlgOptimizer::OnBtnOpt()
{
	// TODO: Add your control notification handler code here
	//시간, 전략, 변수 최적화 공통 루틴//////////////////
	//전략 파일 경로 얻어오기
	CString szSTPath;

	CStringArray stArraySTPath;
	CStringArray stArrayPath;
	
	//그리드를 초기화 합니다.
	//for(int i =1;i<m_GridTap.GetRowCount();i++)
	m_GridTap.SetRowCount(1);
	m_bGridClick = FALSE;
	if(m_nOptimizerDiv == TIME_OPTIMIZER || m_nOptimizerDiv == INDEX_OPTIMIZER)
	{
			
		int nListCount = m_listST.GetCount();
		if(nListCount <=0){
			AfxMessageBox("전략이 리스트에 없습니다.");
			return;
		}
		STMNG_ITEMINFO* pItemInfo = NULL;
		for(int nIndx = 0; nIndx < nListCount; ++nIndx)
		{
			if(pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nIndx))
			{
				szSTPath	= pItemInfo->szSTPath +"\\"+ pItemInfo->szSTName + ".st";
				stArraySTPath.Add(szSTPath);
			}
		}
	}
//@유진삭제
//#ifdef _DEBUG
//	CString strSystemPath = m_strRootPath +  "\\" + STSD_D;
//	CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
//	CString strSystemPath = m_strRootPath + "\\" + STSD;
//	CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//@유진삭제
//@유진추가
	CString strSystemPath = m_strRootPath + "\\" + STSD;
	CString strUserPath = m_strRootPath + "\\" + STUSER;
//@유진추가
	
	stArrayPath.Add(strSystemPath);//Data
	stArrayPath.Add(strUserPath);//User
	//전략메니져 생성
	if(!m_pStrategyItem)
	{
		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// 메모리 생성 
		if(m_pStrategyItem == NULL) return;
		int nRetValue = m_pStrategyItem->Create(this, (long)m_pStrategyItem,stArrayPath,m_pChartInterface);
		if(nRetValue==-1) 
		{
			m_pStrategyItem->DestoryWindow();
			delete m_pStrategyItem;
			m_pStrategyItem = NULL;
		}
	}
	m_DlgTrade->SetStrategyItem(m_pStrategyItem);
	///////////////////////////공통루틴끝////////////////////////////////

///////////////시간최적화 및 전략최적화 루틴 //////////////////////////////
	BOOL bRetValue;
	if(m_nOptimizerDiv == INDEX_OPTIMIZER)
	{
		for(int nIndx =0;nIndx< stArraySTPath.GetSize();nIndx++)
		{
			szSTPath = stArraySTPath.GetAt(nIndx);
			bRetValue = m_pStrategyItem->LoadST(szSTPath,m_stCode,m_nCount,m_stEndDate); //

			if(bRetValue)
			{
				m_pStrategyItem->Run();
			}
		}
	}
	if(m_nOptimizerDiv == TIME_OPTIMIZER)
	{
		//시간최적화 봉수 및 시작값 증가값을 가져온다.
		int BongStart = GetDlgItemInt(IDC_EDIT_START);
		int BongEnd   = GetDlgItemInt(IDC_EDIT_END);
		int BongIncrease = GetDlgItemInt(IDC_EDIT_ADV);
		//봉값 점검 
		if(BongStart > BongEnd || BongEnd < BongIncrease || BongEnd ==0 || BongStart ==0)
		{
			{
				m_pStrategyItem->DestoryWindow();
				delete m_pStrategyItem;
				m_pStrategyItem = NULL;
			}
			AfxMessageBox("봉 입력값이 잘못 설정되었습니다. 다시 설정해주십시오");
			return;
		}

		if(BongStart >= 1000 || BongEnd >= 1000 || BongIncrease >= 1000)
		{
			{
				m_pStrategyItem->DestoryWindow();
				delete m_pStrategyItem;
				m_pStrategyItem = NULL;
			}		
			AfxMessageBox("1000 이상의 값은 사용할 수 없습니다. 다시 입력바랍니다.");
			return;
		}

		for(int nBong = BongStart; nBong<=BongEnd; nBong+=BongIncrease)
		{   m_nBong = nBong;
			m_pStrategyItem->SetTimeBong(m_nBong); // 봉의 갯수 세팅

			for(int nIndx =0;nIndx< stArraySTPath.GetSize();nIndx++)
			{
				szSTPath = stArraySTPath.GetAt(nIndx);
				bRetValue = m_pStrategyItem->LoadST(szSTPath,m_stCode,m_nCount/nBong,m_stEndDate); //
				if(bRetValue)
				{
					m_pStrategyItem->Run();
				}
			}
		}
	}

	////////////Calculate SendValue//////////////////////////
	if(m_nOptimizerDiv == VAL_OPTIMIZER)
	{
		m_pDlgTimeSetup->SetInputData();//최적화 변수를 세로 세팅한다.
		bRetValue = m_pStrategyItem->LoadST(m_stPath,m_stCode,m_nCount,m_stEndDate); //
		m_ValueOptimizerArray.Copy(m_pDlgTimeSetup->m_ValueOptimizerArray);//계산할 변수를 가져온다
		int nSize = m_ValueOptimizerArray.GetSize();
		// nSize가 0인경우는 입력값이 필요없는 경우임.
		//if(!nSize) 
		//{
		//	m_pStrategyItem->DestoryWindow();
		//	delete m_pStrategyItem;
		//	m_pStrategyItem = NULL;
		//	AfxMessageBox("작업중...");
		//	return;
		//}
		if(nSize > 0) 
		{
			ST_VALOTM stValOtm;
			for(int nValIndx = 0; nValIndx < nSize; ++nValIndx)
			{
				stValOtm = m_ValueOptimizerArray.GetAt(nValIndx);
				if(stValOtm.nStart >= 1000 || stValOtm.nEnd >= 1000 || stValOtm.nAdv >= 1000)
				{
					{
						m_pStrategyItem->DestoryWindow();
						delete m_pStrategyItem;
						m_pStrategyItem = NULL;
					}		
					AfxMessageBox("1000 이상의 값은 사용할 수 없습니다. 다시 입력바랍니다.");
					return;
				}
			}
		}

		ST_VALOTM stVal;
		double nOne =0;
		CArray<double,double> *nStart = new CArray<double,double>;
		double nRoof=1;
		CStringArray stArraySendData;
		int i = 0;
		for( i=0;i<m_ValueOptimizerArray.GetSize();i++)//총 루프수를 구합니다.
		{
			stVal = m_ValueOptimizerArray.GetAt(i);
			nStart->Add(stVal.nStart);//변수의 시작점을 먼저 가지고 있는다.

			double nRoof1;
			if(stVal.nEnd == stVal.nStart)
				nRoof1 = 1;
			else
				nRoof1 = ((stVal.nEnd - stVal.nStart)/stVal.nAdv)+1; 
			nRoof *= nRoof1;
		}

		int nFlag = 0;// 현재 가지고 올 배열의 Index 를 가지고 있다.
		for(i =0; i<(int)nRoof;i++)
		{
			if(nStart->GetAt(nFlag) > m_ValueOptimizerArray.GetAt(nFlag).nEnd)
			{
				nStart->SetAt(nFlag,m_ValueOptimizerArray.GetAt(nFlag).nStart);//넘으면 처음값으로 세팅을 하지..
				++nFlag; //
				if(nFlag == nSize)
				{
					nFlag = 0;
				}
				if(nFlag<nSize)//그 다음값을 하나 증가 시키죠..
				{
					for(int k=nFlag;k<nStart->GetSize();k++)//증가시킬값을 찾습니다.
					{
						double val2= nStart->GetAt(k);
						val2+= m_ValueOptimizerArray.GetAt(k).nAdv;
						if(val2 <= m_ValueOptimizerArray.GetAt(k).nEnd)//증가시키고 Break
						{
							nStart->SetAt(k,val2);
							break;
						}
						else//증가값보다 크다면 Start값으로
						{
							nStart->SetAt(k,m_ValueOptimizerArray.GetAt(k).nStart);
						}
					}
					nFlag = 0;
				}
			}

			CString stOption;
			for(int j =0;j<nStart->GetSize();j++)//출력루틴 
			{
				CString stValue;
				double Val = nStart->GetAt(j);//계산된 결과값을 저장한다.
				stValue.Format("%s%d=%.2f,",INPUTVALUE, j, Val);//Grid클릭시에 로딩해야 하므로 Wnd에 저장해놓는다.
				stOption += stValue;
			}

			stOption.Delete(stOption.GetLength()-1,1);
			if(bRetValue)
			{
				m_pStrategyItem->SetSendValString(stOption);
				m_pStrategyItem->SetValOptimizer((LONG)nStart);
				bRetValue = m_pStrategyItem->LoadST(m_stPath,m_stCode,m_nCount,m_stEndDate); //
				m_pStrategyItem->Run(stOption);
			}
		 
			if( m_ValueOptimizerArray.GetAt(nFlag).nAdv==0 && nSize > nFlag+1)
				nFlag++;
			stOption ="";
			double val = nStart->GetAt(nFlag);
			val+= m_ValueOptimizerArray.GetAt(nFlag).nAdv;		
			nStart->SetAt(nFlag,val);

		}
		
		nStart->RemoveAll();
		delete nStart;
	}
}
//여기서 로드를 하죠...
LRESULT CDlgOptimizer::OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam)
{
	//최적화 초기화 공통 로직
	m_pStrategyItem = (CIStrategyItem*)wParam;	
	ST_DRAWINFO* pInfo = (ST_DRAWINFO*)lParam;
	CPtrArray* pPtrArrayPlot = pInfo->pPtrArraySDataItem;
	m_pDataItem = (CISDataItem *)pPtrArrayPlot->GetAt(0);
	long nTotalData	= m_pDataItem->GetSize();
	m_pStrategyItem->Stop();
	m_pStrategyItem->CalculateStatisticalInfo(0, nTotalData-1);//데이터를 전체로..
	
	if(m_bGridClick)//그리드 클릭시엔 밑의 텝에 바로 데이터를 보여줘야 한다.
	{
		m_DlgResult->SetDataSpreadGrid();
		//m_DlgResult->OpenWebView("\\IndexOptimizerData.htm");
		m_DlgTrade->SetDataSpreadGrid();
		//m_DlgTrade->OpenWebView("\\IOTradeData.htm");
		return 0L;
	}	
	//그리드에 추가 루틴 
	////// 전략명 및 변수명 추가 및 시간에 대한 각각 그리드에 추가 루틴 
	CString str;
	int Plus = 0; // 각 최적화 마다 그리드에 그리는 Column 이 다르므로 추가 
	if(m_nOptimizerDiv == INDEX_OPTIMIZER || m_nOptimizerDiv== TIME_OPTIMIZER)
	{
		m_GridTap.InsertRow("");
		//@@061208 전략명가져오기 수정.[[
		// index를 엉뚱한 값을 넘겨주면 전략명이 넘어가고, 0이나 등등 유효한 값을 넘기면 해당sd의 이름을 넘겨준다.
		// 그래서 -1값으로 넘김.
		//str=m_pStrategyItem->GetName(0);//전략명가져옵니다..
		//str.Delete(str.GetLength()-3, 3);

		str=m_pStrategyItem->GetName(-1); //전략명
		int nEndPos = -1;
		if((nEndPos = str.Find(gSTGubunKey)) != -1)
			str = str.Left(nEndPos);
		if((nEndPos = str.Find('.')) != -1)
			str= str.Left(nEndPos);
		//@@061208 전략명가져오기 수정.]]
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,1,str);
		if(m_nOptimizerDiv == TIME_OPTIMIZER)//봉갯수 세팅 
		{
			str.Format("%d",m_pStrategyItem->GetTimeBong());
			m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,2,str);
			Plus =1;
		}
	}
	if(m_nOptimizerDiv == VAL_OPTIMIZER)
	{	//"%s%d=%lf,
		m_GridTap.InsertRow("");
		str.Format("%s",m_pStrategyItem->GetValOptimizer());
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,1,str);//변수값세팅
		CArray<double,double> *ValData = (CArray<double,double>*)m_pStrategyItem->GetValOptimizer();
		ST_VALOTM stVal = 	m_ValueOptimizerArray.GetAt(0);
		int j = 0;
		for( j =0;j<ValData->GetSize();j++)//출력루틴 
		{
			CString stValue;
			double Val = ValData->GetAt(j);//계산된 결과값을 저장한다.
			if(stVal.nDataType ==2)
				stValue.Format("%.2f",Val);
			else
				stValue.Format("%d",(INT)Val);
			m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,j,stValue);//변수값세팅
		}
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,j,m_pStrategyItem->GetSendValString());
		Plus =j-1;
	}
	//지표최적화에서 가져올 변수들은?? 그리드에 값 뿌려준다.3,9,4,5,7,8,0,18,15 
	////////시간과 변수 최적화 공통 사항 /////////////////////
	double Profit = m_pStrategyItem->GetStatisticalInfo(PROFITLOSSALL);//총손익
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,2+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(PROFITLOSSAVG);//평균손익
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,3+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(PROFITALL);//총이익
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,4+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(LOSSALL);//총손실
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,5+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	Profit = m_pStrategyItem->GetStatisticalInfo(MAXPROFIT);//최대이익
	str.Format("%d",(int)Profit);
	str = m_DlgResult->GetComma(str);
	m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,6+Plus,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	//////////////////////////
	if(m_nOptimizerDiv == INDEX_OPTIMIZER)//전략 최적화시에만 필요합니다.
	{
		Profit = m_pStrategyItem->GetStatisticalInfo(MAXLOSS);//최대손실
		str.Format("%d",(int)Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,7,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(PROFITRATEACCMULATED);//수익률
		str.Format("%.2f",Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,8,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(PROFITLOSSRATEALL);//Profit Factor
		str.Format("%.2f",Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,9,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(RATEPROFIT);//승률
		str.Format("%.2f",Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,10,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		Profit = m_pStrategyItem->GetStatisticalInfo(CNTALL);//거래수
		str.Format("%d",(int)Profit);
		str = m_DlgResult->GetComma(str);
		m_GridTap.SetItemText(m_GridTap.GetRowCount()-1,11,str,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	}
	//AutoSizeColumns();
	m_GridTap.Refresh();
	return 0;
}
BOOL CDlgOptimizer::CalculateSendData()
{
	//범위값 구하기
//	CString Data,strD;
//	m_strArSendData.RemoveAll();
//	for(int k=0;k<m_strNum.GetSize();k++)
//	{
//		m_arrayValue.GetAt(k)->stEdit->GetWindowText(strD);
//		CString st;
//		st.Format("%s%d=%s,",INPUTVALUE, k, strD);
//		Data+=st;
//	}
//	Data.Delete(Data.GetLength()-1);
//	m_strArSendData.Add(Data);
//	return TRUE;

//	CStringArray NowValue;
//	CString SendValue;
//	int nValue = 1;
//	int nRow=0;
//	int MaxData = 0;
//	int MaxDataIndex = 0;
//	m_ArGridData.RemoveAll();
////	m_strArSendData.RemoveAll();
//	m_ArstrGridData.RemoveAll();
//	m_ArRunvalue.RemoveAll();
//	//m_strNum도 다시 가져와야 한다!!!
//	for(int i=0;i<m_strNum.GetSize();i++)
//	{
//		ST_RUNVALUESET ValueSet;
//		m_arrayValue.GetAt(i)->stEdit->GetWindowText(SendValue);
//		//Add를 할때... SendValue가 숫자인지를 파악하여 0이나 문자열을 저장한다.			
//		if(IsNumber(SendValue,0))
//		{
//			if(MaxData == 0) MaxDataIndex =i; //MaxData는 row의 첫 배열의 최대값을 나타낸다.
//			int nValue = atoi(SendValue);//
//			ValueSet.start = 1;
//			ValueSet.end = nValue*2;
//			ValueSet.bNumber = TRUE;
//			NowValue.Add("1");
//			MaxData++;
//		}
//		else
//		{
//			ValueSet.start = ValueSet.end = 0;
//			ValueSet.bNumber = FALSE;
//			NowValue.Add(SendValue);
//		}
//		m_ArRunvalue.Add(ValueSet);
//	}
//
//	ST_RUNVALUESET ValueSet;
//	nRow =NowValue.GetSize()-1;
//	if(nRow<0) return FALSE;
//	ValueSet = m_ArRunvalue.GetAt(nRow);
//	int prevData;
//	//루틴 돌기 
//	for(;;)
//	{
//		SendValue ="";
//		ValueSet = m_ArRunvalue.GetAt(nRow);
//		
//		if(ValueSet.end <=nValue ||ValueSet.bNumber!=TRUE)
//		{
//			int UpValue;
//			--nRow;
//
//			if(nRow<0) break;
//			
//			SendValue = NowValue.GetAt(nRow);
//			if(IsNumber(SendValue,0))
//				UpValue = atoi(NowValue.GetAt(nRow));
//			else
//				continue;
//
//			ValueSet = m_ArRunvalue.GetAt(nRow);
//			++UpValue;
//			if(ValueSet.end <= UpValue) continue;
//			
//			for(int l=nRow+1; l < NowValue.GetSize();l++)
//			{
//				SendValue = NowValue.GetAt(l);
//				if(IsNumber(SendValue,0))
//					NowValue.SetAt(l,"1");//전체1으로..
//				else
//					continue;
//			}
//			SendValue.Format("%d",UpValue);
//			NowValue.SetAt(nRow,SendValue);
//			nRow = NowValue.GetSize()-1;
//			nValue = 1;
//		}
//		else
//		{
//			//여기서 String을 만들어야지....
//			//strAdditionalItem.Format("%s%d=%s",INPUTVALUE(메크로),
//			//nIndexAdditional(순서),strPacketName(범위값)); m_strArSendArray에 저장..
//			prevData = nValue;
//			CString strAdditionalItem, strByun;
//			SendValue.Format("%d",nValue);
//			if(nRow== MaxDataIndex) break;//마지막에만 변수를 세팅해야 한다.
//			NowValue.SetAt(nRow,SendValue);
//			for(int j=0;j<NowValue.GetSize();j++)
//			{
//				CString str;
//				str = NowValue.GetAt(j);
//
//			//	if(IsNumber(str,0))
//			//	{
//					str.Format("%s%d=%s,",INPUTVALUE, j, str);
//					strAdditionalItem+=str;
//			//	}
//					str = NowValue.GetAt(j);
//				if(IsNumber(str,0)){
//				str.Format(" [%s]",str);
//				strByun+=str;
//				}
//
//			}
//			strAdditionalItem.Delete(strAdditionalItem.GetLength()-1);
//		//	AfxMessageBox(strAdditionalItem);
//			ST_STRGRIDDATA strData;
//			strData.strGridData = strAdditionalItem;
//			strData.Byunsu = strByun;
//			m_ArstrGridData.Add(strData);
//		//	m_strArSendData.Add(strAdditionalItem);
//			++nValue;
//		
//		}
//	}
//	//Progress Init
//	m_CtlProgress.SetRange(0,m_ArstrGridData.GetSize());
	return TRUE;
}

LRESULT CDlgOptimizer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgOptimizer::OnTimer(UINT nIDEvent) 
{
	
	// TODO: Add your message handler code here and/or call default
//	long Size = m_ArstrGridData.GetSize();
//
//	if(m_lSendNum == Size||m_lSendNum > Size)
//	{
//	//	AfxMessageBox("끝이거덩");
//		KillTimer(m_nTimer);		
//		m_pStrategyItem->Stop();
//		m_lSendNum++;
//		m_CtlProgress.SetPos(m_lSendNum);
//		GetDlgItem(IDC_BTN_OPT)->ShowWindow(TRUE);
//		GetDlgItem(IDC_BTN_STOP)->ShowWindow(FALSE);
//		SpreadGridData();
//		SetControlState(TRUE);
//		CDialog::OnTimer(nIDEvent);
//		return;
//	}
//	BOOL s = KillTimer(m_nTimer);
//	
//	m_pStrategyItem->Stop();
//	ST_STRGRIDDATA GridData;
//	GridData = m_ArstrGridData.GetAt(m_lSendNum);
//	BOOL d=	m_pStrategyItem->Run(GridData.strGridData);
//
////	CString str;
////	str.Format("Data: %s Num:%d",m_strArSendData.GetAt(m_lSendNum),m_lSendNum );
////	GetDlgItem(IDC_STATIC_TEST)->SetWindowText(str);
////	SetDlgItemText(IDC_STATIC_TEST, str);
//	m_lSendNum++;
//	m_CtlProgress.SetPos(m_lSendNum);
//	m_bReceive = FALSE;

	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgOptimizer::IsNumber(CString str, int nIndex)
{
	BOOL bNumber;
	TCHAR c = str.GetAt(nIndex);
	if(c>='0' && c<='9')
		bNumber = TRUE;
	else
		bNumber = FALSE;
	return bNumber;
}

void CDlgOptimizer::SpreadGridData()
{

//	ST_GRIDVALUEDATA GridData;
//	ST_STRGRIDDATA  strGridData;
//	CString str;
//	int nSize =m_ArGridData.GetSize();
//	if(nSize == 0) return;
//	//m_GridStatus.SetRowCount(nSize+1);
/////////상단 그리드 ////////////////////////////
////처음에 세팅된 변수로 가져온다. 
//	GridData = m_ArGridData.GetAt(nSize/2);
//	strGridData = m_ArstrGridData.GetAt(nSize/2);
//	str.Format("%d",nSize/2);
//	m_GridTap.SetItemText(1,0, str);
//	m_GridTap.SetItemText(1,1, strGridData.Byunsu);
//	m_GridTap.SetItemText(1,2, GridData.RateCurrent);
//	m_GridTap.SetItemText(1,3, GridData.LossSavg);
//	m_GridTap.SetItemText(1,4, GridData.ProfitAll);
//	m_GridTap.SetItemText(1,5, GridData.LossAll);
//	m_GridTap.SetItemText(1,6, GridData.MaxProfit);
//	m_GridTap.SetItemText(1,7, GridData.MaxLoss);
//	m_GridTap.SetItemText(1,8, GridData.RateAccm);
//	m_GridTap.SetItemText(1,9, GridData.ProfitFactor);
//	m_GridTap.SetItemText(1,10,GridData.RateProfitLoss);
//	m_GridTap.SetItemText(1,11,GridData.CntAll);
//	
//	
//	int dd = m_ArstrGridData.GetSize();
///////하단 그리드 ////////////////////////////////
	/*for(int i = 0; i<nSize;i++)
	{
		GridData = m_ArGridData.GetAt(i);
	
		str.Format("%d",i);
		strGridData = m_ArstrGridData.GetAt(i);
		m_GridStatus.SetItemText(i+1,0, str);
		m_GridStatus.SetItemText(i+1,1, strGridData.Byunsu);
		m_GridStatus.SetItemText(i+1,2, GridData.RateCurrent);
		m_GridStatus.SetItemText(i+1,3, GridData.LossSavg);
		m_GridStatus.SetItemText(i+1,4, GridData.ProfitAll);
		m_GridStatus.SetItemText(i+1,5, GridData.LossAll);
		m_GridStatus.SetItemText(i+1,6, GridData.MaxProfit);
		m_GridStatus.SetItemText(i+1,7, GridData.MaxLoss);
		m_GridStatus.SetItemText(i+1,8, GridData.RateAccm);
		m_GridStatus.SetItemText(i+1,9, GridData.ProfitFactor);
		m_GridStatus.SetItemText(i+1,10,GridData.RateProfitLoss);
		m_GridStatus.SetItemText(i+1,11,GridData.CntAll);
	}

	m_GridStatus.SortTextItems(2,TRUE);*/
	m_GridTap.Refresh();
	//m_GridStatus.Refresh();
}


void CDlgOptimizer::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	m_pStrategyItem->Stop();
	KillTimer(m_nTimer);		
	long Size = m_ArstrGridData.GetSize();
	m_lSendNum = Size;
	//m_CtlProgress.SetPos(m_lSendNum);
	GetDlgItem(IDC_BTN_STOP)->ShowWindow(FALSE);
	GetDlgItem(IDC_BTN_OPT)->ShowWindow(TRUE);
	//SpreadGridData();
	SetControlState(TRUE);
}

void CDlgOptimizer::SetControlState(BOOL State)
{
	//if(State)// 컨트롤들을 Enable 시킨다. 
	//{
	//	GetDlgItem(IDC_COM_INDEX)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_COM_MEDO)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_COM_MESU)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_MEDOGIJUN)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SPIN_MEDO)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_MESUGIJUN)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SPIN_MESU)->EnableWindow(TRUE);
	//	ST_VALUESET *ValueSet;
	//	for(int i =0 ;i< m_arrayValue.GetSize();i++)
	//	{
	//		ValueSet = m_arrayValue.GetAt(i);
	//		ValueSet->stEdit->EnableWindow(TRUE);
	//		ValueSet->stSpinBtn->EnableWindow(TRUE);
	//	}
	//}
	//else
	//{
	//	GetDlgItem(IDC_COM_INDEX)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_COM_MEDO)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_COM_MESU)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_MEDOGIJUN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SPIN_MEDO)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_MESUGIJUN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SPIN_MESU)->EnableWindow(FALSE);
	//	ST_VALUESET *ValueSet;
	//	for(int i =0 ;i< m_arrayValue.GetSize();i++)
	//	{
	//		ValueSet = m_arrayValue.GetAt(i);
	//		ValueSet->stEdit->EnableWindow(FALSE);
	//		ValueSet->stSpinBtn->EnableWindow(FALSE);
	//	}
	//}
}

BOOL CDlgOptimizer::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	//Grid에서 이벤트 발생 할때 마다 SendMessage로 구조체에 정보를 채워 
	//아이디와 함께일루 보낸다.
	
	NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
	//Grid클릭시 거기에 대한 전략을 로드해야한다.
	if(wParam==IDC_GRID2)
	{
		if(pNmgv->hdr.code==NM_CLICK)
		{
			if(pNmgv->iRow < 0)
				return CDialog::OnNotify(wParam, lParam, pResult);
			
			m_bGridClick = TRUE;
			if(pNmgv->iRow ==0)
			{
				//m_GridTap.SetSortColumn(pNmgv->iColumn);
				if(m_nOptimizerDiv == INDEX_OPTIMIZER && pNmgv->iColumn==1 )
					m_GridTap.SortItems(m_GridTap.pfnCellTextCompare,pNmgv->iColumn,!m_GridTap.GetSortAscending());
				else
					m_GridTap.SortItems(m_GridTap.pfnCellNumericCompare,pNmgv->iColumn,!m_GridTap.GetSortAscending());
				m_GridTap.Refresh();
				return CDialog::OnNotify(wParam, lParam, pResult);
			}
			if(m_nOptimizerDiv!=VAL_OPTIMIZER)
			{
				CString stSTName;
				stSTName = m_GridTap.GetItemText(pNmgv->iRow,1);//전략명을 읽어온다. 
				if(stSTName=="") return CDialog::OnNotify(wParam, lParam, pResult);
				CString szSTPath;
				int nListCount = m_listST.GetCount();
				STMNG_ITEMINFO* pItemInfo = NULL;
				for(int nIndx = 0; nIndx < nListCount; ++nIndx)
				{
					if(pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nIndx))
					{	
						szSTPath = pItemInfo->szSTName;
						int nEndPos = -1;
						if((nEndPos = szSTPath.Find(gSTGubunKey)) != -1)
							szSTPath = szSTPath.Left(nEndPos);
						if(szSTPath == stSTName)
						{
							szSTPath	= pItemInfo->szSTPath +"\\"+ pItemInfo->szSTName + ".st";
							
							break;
						}
					}
				}
				if(m_nOptimizerDiv == TIME_OPTIMIZER)//해당봉수를 세팅한다.
				{
					m_pStrategyItem->SetTimeBong(atoi(m_GridTap.GetItemText(pNmgv->iRow,2)));
				}
				//클릭할때 마다 아래의 텝을 채우기 위해 해당 전략을 다시 로드하여 Run해야 한다.
				BOOL bRetValue = m_pStrategyItem->LoadST(szSTPath,m_stCode,m_nCount,m_stEndDate); //
				if(bRetValue)
				{
					m_pStrategyItem->Run();
				}
			}else{//변수 최적화에서 사용.....
				CString stSendVal;
				stSendVal = m_GridTap.GetItemText(pNmgv->iRow,m_STValNum);
				BOOL bRetValue = m_pStrategyItem->LoadST(m_stPath,m_stCode,m_nCount,m_stEndDate);
				if(bRetValue)
				{
					m_pStrategyItem->Run(stSendVal);
				}
			}

		}  
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgOptimizer::SetSTMessageType(short Smt)
{
	m_STMessageType = Smt;
}

void CDlgOptimizer::InitSkin()
{
	//m_BtnDef.SetSkin(IDB_BITMAP1_up,IDB_BITMAP1_over,IDB_BITMAP1_up,IDB_BITMAP1_dis);
	//m_BtnStop.SetSkin(IDB_BITMAP3_up,IDB_BITMAP3_over,IDB_BITMAP3_up,IDB_BITMAP3_dis);
	//m_BtnOpt.SetSkin(IDB_BITMAP2_up,IDB_BITMAP2_over,IDB_BITMAP2_up,IDB_BITMAP2_dis);
}

void CDlgOptimizer::OnBnClickedBtnAddall()
{
	LIST_STMNG_ITEMINFO STListInfo;
	STListInfo.RemoveAll();
	
	m_pSTLoadMnger->GetFolderST((DWORD)&STListInfo);

	STMNG_ITEMINFO* pItemInfo = NULL;
	CString szSTName;
	POSITION pos = STListInfo.GetHeadPosition();
	int nAtPos = -1;
	while(pos) {
		if(pItemInfo = (STMNG_ITEMINFO*)STListInfo.GetNext(pos)) 
		{
			szSTName = pItemInfo->szSTName;
			nAtPos = szSTName.Find(gSTGubunKey);
			if(nAtPos > 0)
				szSTName = szSTName.Mid(0, nAtPos);

			//if(m_listST.FindString(0,szSTName)==LB_ERR)//찾지 못했을때 추가!!KHD
			//{
				m_listST.AddString(szSTName);
				m_listST.SetItemData(m_listST.GetCount()-1, (DWORD)pItemInfo);
			//}
		}
	}
}

void CDlgOptimizer::OnBnClickedBtnAddone()
{
	LIST_STMNG_ITEMINFO STListInfo;
	STListInfo.RemoveAll();

	m_pSTLoadMnger->GetItemST((DWORD)&STListInfo);
	
	STMNG_ITEMINFO* pItemInfo = NULL;
	CString szSTName;
	POSITION pos = STListInfo.GetHeadPosition();
	int nAtPos = -1;
	while(pos) {
		if(pItemInfo = (STMNG_ITEMINFO*)STListInfo.GetNext(pos)) {
			szSTName = pItemInfo->szSTName;
			nAtPos = szSTName.Find(gSTGubunKey);
			if(nAtPos > 0)
				szSTName = szSTName.Mid(0, nAtPos);

			//if(m_listST.FindString(0,szSTName)==LB_ERR)//찾지 못했을때 추가!!KHD
			//{
				m_listST.AddString(szSTName);
				m_listST.SetItemData(m_listST.GetCount() - 1, (DWORD)pItemInfo);
			//}
		}
	}
}

void CDlgOptimizer::OnBnClickedBtnDelall()
{
	int nListCount = m_listST.GetCount();
	STMNG_ITEMINFO* pItemInfo = NULL;
	for(int nIndx = 0; nIndx < nListCount; ++nIndx)
	{
		if(pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nIndx)) {
			delete pItemInfo;
			pItemInfo = NULL;
		}
	}

	m_listST.ResetContent();
}

void CDlgOptimizer::OnBnClickedBtnDelone()
{
	
	int nCurSel = m_listST.GetCurSel();
	STMNG_ITEMINFO* pItemInfo = (STMNG_ITEMINFO*)m_listST.GetItemData(nCurSel);
	m_listST.DeleteString(nCurSel);
}

//시간, 변수, 전략 최적화에 따라 스킨이 달리 세팅이된다. 
void CDlgOptimizer::CreateTImeSetupDlg()
{

	switch(m_nOptimizerDiv) {
	case INDEX_OPTIMIZER:
		{
			GetDlgItem(IDC_STATIC_START)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_END)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ADV)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_START)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_ADV)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_END)->ShowWindow(SW_HIDE);
			// m_listST.SetWindowPos(NULL,5,310,190,150,SWP_SHOWWINDOW);
			
		}
		break;
	case VAL_OPTIMIZER:
		{
			m_pDlgTimeSetup = new CDlgTimeSetup;

			BOOL bRet = m_pDlgTimeSetup->Create(CDlgTimeSetup::IDD, this);

			if(bRet)
			{
				CRect rect;
				GetDlgItem(IDC_STATIC_TIME)->GetWindowRect(rect);
				ScreenToClient(rect);
				rect.top += 1;
				m_pDlgTimeSetup->MoveWindow(rect);
				m_pDlgTimeSetup->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_ADDALL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_ADDONE )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_DELALL )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_DELONE )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_LIST_ST )->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_START)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_END)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_ADV)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_START)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_ADV)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_END)->ShowWindow(SW_HIDE);

			}
		}
		break;
	case TIME_OPTIMIZER://Default이긴 하지만..
		{
			GetDlgItem(IDC_STATIC_START)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_END)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_ADV)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_START)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_ADV)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_END)->ShowWindow(SW_SHOW);
			
			SetDlgItemText(IDC_EDIT_START,"1");
			SetDlgItemText(IDC_EDIT_END,"5");
			SetDlgItemText(IDC_EDIT_ADV,"1");
		}
		break;
	default:
		AfxMessageBox("최적화가 잘못 선택되었습니다.");
	}
	
}

void CDlgOptimizer::DestoryTImeSetupDlg()
{
	if(m_pDlgTimeSetup) delete m_pDlgTimeSetup;
	m_pDlgTimeSetup=NULL;
}

//챠트데이터 가공하는곳....
// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
int CDlgOptimizer::CSTUserData::GetChartData(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwVolume, LONG& dwDataExt, int& nBong)
{
	MYMETHOD_PROLOGUE(CDlgOptimizer, STUserData)
	ISTInterface* pSTInterface = (ISTInterface*)pThis->m_pChartInterface;
	if(pSTInterface)
	{
		//바꿔치기 루틴 필요!!
		ISTAnalysisInput* pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		int nCnt = pMng->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose,dwVolume, dwDataExt);//실질데이터
		nBong = pThis->m_pStrategyItem->GetTimeBong();//현재 세팅된 봉의 갯수로 계산 합니다.
		if(nBong ==1) return 0;//1에서는 루틴을 탈필요가 없으므로.. 값 그대로 반환
		//배열 및 데이터Copy 생성
		CSDataItem* dTimeDataTime		= new CSDataItem;
		CSTDataItem* dTimeDataOpen		= new CSTDataItem;
		CSTDataItem* dTimeDataHigh		= new CSTDataItem;
		CSTDataItem* dTimeDataLow		= new CSTDataItem;
		CSTDataItem* dTimeDataClose		= new CSTDataItem;

		double nBongOpen; 
		double nBongHigh;
		double nBongLow;
		double nBongClose;
		int nStartI;
		int nEnd = (nCnt-(nCnt%nBong)); //-> 나눠서 에누리는 제한다.
		for(int i = 0; i<nEnd ; i+=nBong)
		{	
			nStartI = i;
			nBongOpen	= ((CSTDataItem*)dwDataOpen)->GetAt(nStartI);
			nBongClose	= ((CSTDataItem*)dwDataClose)->GetAt(nStartI+nBong-1);
			//처음 값을 세팅한다.
			nBongHigh		=((CSTDataItem*)dwDataHigh)->GetAt(nStartI);
			nBongLow		=((CSTDataItem*)dwDataLow)->GetAt(nStartI);
			for(int j=0;j<nBong;j++)	
			{
				nBongHigh = max(nBongHigh, ((CSTDataItem*)dwDataHigh)->GetAt(nStartI+j));
				nBongLow  = min(nBongLow,  ((CSTDataItem*)dwDataLow)->GetAt(nStartI+j));
			}

			dTimeDataTime->Add(((CSDataItem*)dwDataTime)->GetAt(nStartI));
			dTimeDataOpen->Add(nBongOpen);
			dTimeDataHigh->Add(nBongHigh);
			dTimeDataLow->Add(nBongLow);
			dTimeDataClose->Add(nBongClose);
		}

		dwDataTime  = (LONG)dTimeDataTime;
		dwDataOpen	= (LONG)dTimeDataOpen;
		dwDataHigh	= (LONG)dTimeDataHigh;
		dwDataLow	= (LONG)dTimeDataLow;
		dwDataClose	= (LONG)dTimeDataClose;
		//////////////////////////////////////////////////////
		return nCnt;
	}
	return 0; // 데이터Count 리턴
}
LRESULT CDlgOptimizer::OnRmsgObItemDBClk(WPARAM wParam, LPARAM lParam)
{
	STMNG_ITEMINFO* pArg = (STMNG_ITEMINFO*)lParam;
	if(m_nOptimizerDiv == VAL_OPTIMIZER)
	{
		CString szRootDir = (LPCSTR)AfxGetPctrData(10);	// 10 : RootDir
		//CString strUserPath = szRootDir +"\\Data\\STData\\VarOptima.cfg";
		CString strUserPath = szRootDir +"\\Icss\\STData\\VarOptima.cfg";	//@Solomon
		CString szSTName;
		m_pDlgTimeSetup->Reset();
		szSTName = pArg->szSTName; 
		szSTName=szSTName.Mid(0, szSTName.Find(gSTGubunKey));
		if(m_pDlgTimeSetup->LoadCFG(strUserPath, szSTName)==FALSE)
		{
			AfxMessageBox("변수 최적화에서 사용할 수 없는 전략입니다.");
			return 0L;
		}
		m_pDlgTimeSetup->ParseCFG();
		//현재 더블클릭 된 전략 풀Path 저장한다.
		m_stPath = pArg->szSTPath + "\\" +pArg->szSTName+".st";
		m_GridTap.SetRowCount(1);
		m_STValNum =m_pDlgTimeSetup->m_ValueOptimizerArray.GetSize();
		
		ReSize();
		
	}
	else{
		OnBnClickedBtnAddone();
	}

	/*m_xSTAnalysisInput.m_Info.nSTType		= pArg->nSTType;
	m_xSTAnalysisInput.m_Info.szSTName		= pArg->szSTName;
	m_xSTAnalysisInput.m_Info.szSTPath		= pArg->szSTPath;
	m_xSTAnalysisInput.m_Info.szSTRData		= pArg->szSTRData;	
	m_xSTAnalysisInput.m_Info.nFolderIndx	= pArg->nFolderIndx;
	m_xSTAnalysisInput.m_Info.nItemIndx		= pArg->nItemIndx;*/
	return 0L;
}

	

void CDlgOptimizer::UnLoadSTList()
{
	if(!m_pSTLoadMnger) return;

	typedef	void (FPSCW_DeleteInterfaceOfStrategy)(LONG dwKey, LONG dwType);
	if(hTradeInst == NULL) return;

	FPSCW_DeleteInterfaceOfStrategy* pFunc = (FPSCW_DeleteInterfaceOfStrategy*)GetProcAddress(hTradeInst,_T("SCW_DeleteInterfaceOfStrategy"));
	if(pFunc == NULL) return ;

	LONG dwType = 0;	//CSTLoadManager*
	if(m_pSTLoadMnger)
	{
		m_pSTLoadMnger->UnloadST();
		pFunc((LONG)m_pSTLoadMnger, dwType);
	}	
}


LRESULT CDlgOptimizer::OnRmsgGetDllSize(WPARAM wParam, LPARAM lParam)
{
	CPoint* pSize = (CPoint*)wParam;
	pSize->x = 865;
	pSize->y = 532;

	return 0;
}


void CDlgOptimizer::OnLbnDblclkListSt()
{
	OnBnClickedBtnDelone();
}
//@@@06.12.07[[


void CDlgOptimizer::LoadSTList_SSStyle()
{
	if(!m_pWndOutBar) return;

	m_STTree.m_szRootDir = m_strRootPath;
	m_STBasicTree.m_szRootDir = m_strRootPath;

	DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS;
	CWnd* pParentWnd = m_pWndOutBar->GetCoutBarCtrl();

	UINT nID = 1000;
	BOOL bResult = m_STTree.Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
	m_STTree.LoadSTTree();

	nID++;
	bResult = m_STBasicTree.Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
	m_STBasicTree.LoadSTTree();

	m_pWndOutBar->AddFolderBar("초급", &m_STBasicTree);
	m_pWndOutBar->AddFolderBar("고급", &m_STTree);

	m_pWndOutBar->SetSelFolder(0);
}
//LONG CDlgOptimizer::OnRmsgSTTreeItemClk(WPARAM wParam, LPARAM lParam)
//{	
//	return 0L;
//	//return OnRmsgObItemClk(wParam, lParam);
//}

LONG CDlgOptimizer::OnRmsgSTTreeItemDBClk(WPARAM wParam, LPARAM lParam)
{	
	return OnRmsgObItemDBClk(wParam, lParam);
}
//@@@06.12.07]]
