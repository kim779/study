// InfoRight.cpp : implementation file
//

#include "stdafx.h"
//#include "hwstrategyanalysis.h"
#include "InfoRight.h"
#include "DlgAnalysis.h"

//Tab Header Add :KHD///
#include "TabAnalChart.h"
#include "TabEarnChart.h"
#include "TabResult.h"
#include "TabSonikChart.h"
#include "TabStep.h"
#include "TabTrade.h"
#include "../Common_ST/IDataItem.h"
#include ".\inforight.h"
#include "../../inc/IKSLibMng.h"
/////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT THREAD_MAKEHTMLResult(LPVOID lpVoid)
{
	CTabResult* pTabResult = (CTabResult*)lpVoid;
	pTabResult->MakeHtmlFile();

	return 0;
}

UINT THREAD_MAKEHTMLTrade(LPVOID lpVoid)
{
	CTabTrade* pTabTrade = (CTabTrade*)lpVoid;
	pTabTrade->MakeHtmlFile();

	return 0;
}

UINT THREAD_MAKEHTMLStep(LPVOID lpVoid)
{
	CTabStep* pTabStep = (CTabStep*)lpVoid;
	pTabStep->SetDataSpreadGrid();

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CInfoRight dialog
#define CRBACKGROUND				RGB(237,243,249)
#define DEFAULT_BKGRND_COLOR2		RGB(247,248,252)

//#define IDC_GRID	0x00100
#define MAX_COL 11
CInfoRight::CInfoRight(CWnd* pParent /*=NULL*/)
	: CEscDialog(CInfoRight::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoRight)
	//}}AFX_DATA_INIT
	m_bShowToggle	= TRUE;
	m_crBk			= CRBACKGROUND;
	m_nNum	= 0;
	m_Input			= NULL;

	m_SonikChart	= NULL;
	m_Step			= NULL;
	m_Trade			= NULL;
	m_Result		= NULL;
	m_EarnChart		= NULL;
//	m_AnalChart		= NULL;

	m_pChartItem	= NULL;
	//	m_pstDataIndex = NULL;
}


void CInfoRight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoRight)
	DDX_Control(pDX, IDC_BTN_RESULT, m_CtlResult);
	DDX_Control(pDX, IDC_BTN_SHOWALL, m_CtlShowAll);
	DDX_Control(pDX, IDC_BTN_SHOW, m_CtlSpread);
	DDX_Control(pDX, IDC_TAB1, m_CtlTab);
	//DDX_Control(pDX, IDC_EXPLORER, m_Browser);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_DIVIDE, m_ctlComboDivide);
	DDX_Control(pDX, IDC_STATIC_NAME, m_ctlstName);
	DDX_Control(pDX, IDC_STATIC_STNAME, m_ctlSTFILENAME);
	DDX_Control(pDX, IDC_STATIC_POSITION, m_ctlstPosition);
	DDX_Control(pDX, IDC_STATIC_PERIOD, m_ctlstPeriod);
	DDX_Control(pDX, IDC_STATIC_NUJUK, m_ctlstNujuk);
	DDX_Control(pDX, IDC_STATIC1, m_stJName1);
	DDX_Control(pDX, IDC_STATIC2, m_stSTName);
	DDX_Control(pDX, IDC_STATIC3, m_stPosition);
	DDX_Control(pDX, IDC_STATIC4, m_stPeriod);
	DDX_Control(pDX, IDC_STATIC5, m_stNujuk);
	DDX_Control(pDX, IDC_BTN_EXPORTEXCEL, m_btnExportExcel);
}


BEGIN_MESSAGE_MAP(CInfoRight, CDialog)
	//{{AFX_MSG_MAP(CInfoRight)
	ON_BN_CLICKED(IDC_BTN_SHOW, OnBtnShow)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_EXPORTEXCEL, OnBtnExportExcel)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_DIVIDE, OnCbnSelchangeComboDivide)
//	ON_NOTIFY(TCN_FOCUSCHANGE, IDC_TAB1, OnTcnFocusChangeTab1)
ON_NOTIFY(NM_CLICK, IDC_TAB1, OnNMClickTab1)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CInfoRight, CDialog)
    //{{AFX_EVENTSINK_MAP(CInfoRight)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/////////////////////////////////////////////////////////////////////////////
// CInfoRight message handlers
//버튼에 따라 다이얼로그 Size 조절 루틴 : KHD 
void CInfoRight::OnBtnShow() 
{
	// TODO: Add your control notification handler code here
	//DrawGraph();
	m_pChartItem->InvokeAddIn("ChartSTIndexAddIn.dll",7,(long)GetSafeHwnd());
}
//수정 : KHD : 2006.12.15
void CInfoRight::DrawGraph()
{
	CIStrategyItem* pStrategyItem = m_pDlgAnalysis->GetStrategyItem();
	int nCount = pStrategyItem->GetCount();
	ST_DRAWINFO_EX *pDrawInfo = (ST_DRAWINFO_EX *)pStrategyItem->GetPtrDrawInfo(nCount-1);
	m_pChartItem->InvokeAddIn("ChartSTIndexAddIn.dll",6,m_pDlgAnalysis->m_hTabChartEx);
	m_pChartItem->InvokeAddIn("ChartSTIndexAddIn.dll",5,(long)&m_pDlgAnalysis->m_stDataIndex);//Div: KHD 
	m_pChartItem->InvokeAddIn("ChartSTIndexAddIn.dll",4,(long)pDrawInfo);
	
}

BOOL CInfoRight::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//InitSkin(); : Because The Design Not Used by KHD : 2006.09.11
	m_ctlComboDivide.InsertString(0,"전체");
	m_ctlstName.SetStaticColor(RGB(0,0,0),RGB(255,255,255),RGB(0,0,0));
		m_crBk = DEFAULT_BKGRND_COLOR2;

	m_brBkColor.CreateSolidBrush(m_crBk);

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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Because The Design Not Used by KHD : 2006.09.11
void CInfoRight::InitHeader()
{
//	CString strPath = m_strFilePath+"\\보고서-Header.htm";
//	m_Browser.Navigate(strPath,NULL,NULL,NULL,NULL);
}
void CInfoRight::SetLeftInfo(CInputLeft* leftInfo)
{
	m_Input = leftInfo;
}

//CurSel 분할수 
void CInfoRight::SetRefresh(int CurSel)
{
	//DivShowButton Skin Apply
	m_nNum = m_pDlgAnalysis->m_nDivideCnt;
	for(int i = 0 ; i < m_nNum ;i++)//KHD
	{
		CString str; 
		str.Format("%d",i+1);
		m_ctlComboDivide.InsertString(i+1,str);
	}
		
	m_ctlComboDivide.SetCurSel(CurSel);//KHD
	if(m_Step)
	{
		m_Step->m_nDiv =m_nNum; // Div Number Save : 전체 
	}

	if(m_Trade)
		m_Trade->m_nDiv = CurSel;
	RefreshWebView();
}
void CInfoRight::SetParentsRect(CRect rt)
{
	m_rcParent = rt;
}

//TabDlgs Refresh!!
void CInfoRight::RefreshWebView()
{

	m_Result->SetDataSpreadGrid();
	m_Trade->SetDataSpreadGrid();
	m_Step->SetDataSpreadGrid();
	//AfxBeginThread(THREAD_MAKEHTMLStep, m_Step);

}
void CInfoRight::OnDestroy() 
{
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject();
	
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CDialog *pTabDlg = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pTabDlg = m_arrayTab.GetAt(nPos);
		pTabDlg->DestroyWindow();
		delete pTabDlg;
		pTabDlg = NULL;
	}
	m_arrayTab.RemoveAll();	
}

void CInfoRight::InitTab()
{
//@신영삭제
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//		m_crBk = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//	else
		m_crBk = DEFAULT_BKGRND_COLOR2;

	m_brBkColor.CreateSolidBrush(m_crBk);
	CRect rc;

	m_CtlTab.GetClientRect(&rc);
	m_CtlTab.SetTopLeftCorner(CPoint(rc.left-2,107));
	m_CtlTab.SetLeftOffset(0);
	m_CtlTab.SetUpperOffset(0);
	m_CtlTab.SetSelectedColor(RGB(0, 0, 0),DEFAULT_BKGRND_COLOR2);
	m_CtlTab.SetNormalColor(RGB(0,0,0), RGB(255,255,255) , RGB(226,225,233));
	m_CtlTab.SetTabBkColor(DEFAULT_BKGRND_COLOR2);

	CRect rcItem;

	m_ctlComboDivide.GetClientRect(rcItem);
	int nHeight = rcItem.Height() + 1;
	m_Result = new CTabResult;
	m_Result->SetParentDlg(m_pDlgAnalysis);
	m_Result->SetGridFont(&m_Font);
	m_Result->m_strFilePath = m_strFilePath;
	m_Result->Create(CTabResult::IDD, this);

	m_arrayTab.Add(m_Result);

	m_Step = new CTabStep;
	m_Step->m_strFilePath = m_strFilePath;
	m_Step->SetCInputLeft(m_Input);
	m_Step->SetParentDlg(m_pDlgAnalysis);
	m_Step->SetGridFont(&m_Font);
	m_Step->Create(CTabStep::IDD, this);
	m_arrayTab.Add(m_Step);

	m_Trade = new CTabTrade;
	m_Trade->m_strFilePath = m_strFilePath;
	m_Trade->SetCInputLeft(m_Input);
	m_Trade->SetParentDlg(m_pDlgAnalysis);
	m_Trade->SetGridFont(&m_Font);
	m_Trade->Create(CTabTrade::IDD, this);

	m_arrayTab.Add(m_Trade);

	m_EarnChart = new CTabEarnChart;
	m_EarnChart->SetCInputLeft(m_Input);
	m_EarnChart->SetParentDlg(m_pDlgAnalysis);
	m_EarnChart->Create(CTabEarnChart::IDD, this);
	m_arrayTab.Add(m_EarnChart);

	m_SonikChart = new CTabSonikChart;
	m_SonikChart->SetCInputLeft(m_Input);
	m_SonikChart->SetParentDlg(m_pDlgAnalysis);
	m_SonikChart->Create(CTabSonikChart::IDD, this);
	m_arrayTab.Add(m_SonikChart);

	//m_CtlTab.AddTab(m_AnalChart,"분석차트");
	m_CtlTab.AddTab(m_Result,"성과분석");
	m_CtlTab.AddTab(m_Step,"전진분석");
	m_CtlTab.AddTab(m_Trade,"매매내역");
	m_CtlTab.AddTab(m_EarnChart,"수익률차트");
	m_CtlTab.AddTab(m_SonikChart,"손익분포차트");

	//상위 Static 초기화
	//이름

	m_stJName1.SetStaticColor(RGB(204,204,204),RGB(230,239,247),RGB(51,51,51));
	m_stSTName.SetStaticColor(RGB(204,204,204),RGB(230,239,247),RGB(51,51,51));
	m_stPosition.SetStaticColor(RGB(204,204,204),RGB(230,239,247),RGB(51,51,51));
	m_stPeriod.SetStaticColor(RGB(204,204,204),RGB(230,239,247),RGB(51,51,51));
	m_stNujuk.SetStaticColor(RGB(204,204,204),RGB(230,239,247),RGB(51,51,51));
	//상위 Static
	m_ctlstName.SetStaticColor(RGB(204,204,204),RGB(255,255,255),RGB(51,51,51));
	m_ctlSTFILENAME.SetStaticColor(RGB(204,204,204),RGB(255,255,255),RGB(51,51,51));
	m_ctlstPosition.SetStaticColor(RGB(204,204,204),RGB(255,255,255),RGB(51,51,51));
	m_ctlstPeriod.SetStaticColor(RGB(204,204,204),RGB(255,255,255),RGB(51,51,51));
	m_ctlstNujuk.SetStaticColor(RGB(204,204,204),RGB(255,255,255),RGB(51,51,51));

}


HBRUSH CInfoRight::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CInfoRight::OnBtnShowall() 
{
	// TODO: Add your control notification handler code here
	ST_DATAINDEX Index;
	int nSize =	m_pDlgAnalysis->m_stDataIndex.GetSize();
	if(nSize>1)
	{
		Index = m_pDlgAnalysis->m_stDataIndex.GetAt(0);
		m_Result->m_iDateIndex = 0;
		m_pDlgAnalysis->SetCalculateData(0);
		RefreshWebView();
	}
}

void CInfoRight::AllUpdateDlg()
{
	CDlgInterface *pDlgInterface = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pDlgInterface = m_arrayTab.GetAt(nPos);
		pDlgInterface->UpdateDlg();
	}
}

void CInfoRight::InitCodeGubun(BOOL bInitGubun)
{
	CDlgInterface *pDlgInterface = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pDlgInterface = m_arrayTab.GetAt(nPos);
		pDlgInterface->m_bIsJusik = bInitGubun;
	}
}

void CInfoRight::OnBtnResult() 
{
	// TODO: Add your control notification handler code here
	ApplyST();
}

//Graph 그리지 않을때... 사용된다. : NOt Used KHD
void CInfoRight::ApplyST()
{
	m_pDlgAnalysis->Result();
}


void CInfoRight::SetFocusIndex(int nIndex)
{
	CDlgInterface *pDlgInterface = NULL;
	int nSize = m_arrayTab.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pDlgInterface = m_arrayTab.GetAt(nPos);
		pDlgInterface->SetChartFocus(nIndex);
	}
}



void CInfoRight::OnCbnSelchangeComboDivide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int Index =m_ctlComboDivide.GetCurSel();
	m_Result->m_iDateIndex = Index;
	m_pDlgAnalysis->SetCalculateData(Index);
	RefreshWebView();

}


void CInfoRight::OnNMClickTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	//0부터 옵니다... 각 옵션마다 창을 늘리도록 하죠!!
	int i = m_CtlTab.GetCurSel();

}

void CInfoRight::OnBtnExportExcel() 
{
	// TODO: Add your control notification handler code here
	if(m_Trade)
		m_Trade->SetDataToExcel();
}