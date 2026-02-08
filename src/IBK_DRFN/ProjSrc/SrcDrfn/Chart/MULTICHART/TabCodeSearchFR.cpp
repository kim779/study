// TabCodeSearchFR.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "TabCodeSearchFR.h"
#include "./define/ChartMsgDef.h"
#include "DataInListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFR dialog

CString strCodes[] =
{
	"NII@NI225", "HSI@HSI", "HSI@HSCE", "TWS@TI01", "SHS@000001", "SHS@000002", "SHS@000003", "SZS@399107",
	"SGI@STI", "LNS@FTSE100", "PAS@CAC40", "XTR@DAX30", "DJI@DJI",
	"DJI@DJT", "DJI@DJU", "NAS@IXIC", "NAS@NDX", "NAS@IXK",
	"SPI@SPX", "USI@SOXX", "AMS@BPI", "IDI@JKSE", "INI@BSE30",
	"MYI@KLSE", "PHI@COMP",	"THI@SET", "BEI@MFX", "DKI@KFX",
	"GRI@ASE", "ITI@MIBTEL", "NLI@AEX", "RUI@RTSI", "PLI@WIGI",
	"ESI@IBEX", "CHI@SSMI",	"BRI@BVSP", "MXI@IPC", "CAI@SPTSX",
	"VNI@VIDX", "AUI@AORD",
};
//"GSPF00","GNDF00", 

CString strNats[] =
{
	"일본", "홍콩", "홍콩", "대만", "중국", "중국", "중국", "중국", 
	"싱가폴", "영국", "프랑스", "독일", "미국",
	"미국", "미국",	"미국", "미국", "미국",
	"미국", "미국",	"미국", "인도네시아", "인도",
	"말레이시아", "필리핀",	"태국", "벨기에", "덴마크",
	"그리스", "이탈리아", "네덜란드", "러시아", "폴란드",
	"스페인", "스위스",	"브라질", "멕시코", "캐나다",
	"베트남", "호주",
};

CTabCodeSearchFR::CTabCodeSearchFR(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearchFR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabCodeSearchFR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInsertingInListCode = FALSE;
}


void CTabCodeSearchFR::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearchFR)
	DDX_Control(pDX, IDC_LISTCODE, m_listCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabCodeSearchFR, CTabDlg)
	//{{AFX_MSG_MAP(CTabCodeSearchFR)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LISTCODE, OnClickListcode)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCODE, OnDblclkListcode)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFR message handlers


BOOL CTabCodeSearchFR::OnInitDialog() 
{
	CTabDlg::OnInitDialog();
	
	m_listCode.DeleteAllItems();
	InitColumns();
	m_listCode.SetHilightColor(RGB(0,0,0), RGB(180,200,240));
	InsertData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabCodeSearchFR::InitColumns()
{
	m_listCode.SetExtendedStyle(LVS_EX_FULLROWSELECT);// | LVS_EX_GRIDLINES);
	m_listCode.SetRowHeight(17);

	int colwidths[3] = { 102,40 };	// sixty-fourths
	TCHAR *	lpszHeaders[] = { _T("지수명"),	_T("국가"),	NULL };

	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (int i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidths[i];
		m_listCode.InsertColumn(i, &lvcolumn);
	}
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		m_listCode.SetColumn(i, &lvcolumn);
	}	
}

void CTabCodeSearchFR::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listCode);
}

BOOL CTabCodeSearchFR::InsertData()
{
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	IMasterDataManager* pDataManager;
	if(pGateMng)
		pDataManager = (IMasterDataManager*)pGateMng->GetInterface(5);

	//CStringArray* pastrSymbol;	// 해외주요지수 심벌
	//CStringArray* pastrName;	// 해외주요지수 종목명
	//CStringArray* pastrNatCode;	// 해외지수 코드
	//CStringArray* pastrNatName;	// 해외국가명

	//pastrSymbol = new CStringArray;
	//pastrName = new CStringArray;
	//pastrNatCode = new CStringArray;
	//pastrNatName = new CStringArray;

	int nItemNum = m_listCode.GetItemCount();

	// 국가코드를 얻어온다
	//BOOL bRet = pDataManager->GetFRMst(pastrSymbol, pastrName);
	CString strSymbol, strName, strTemp, strCode, strNat, strMarket;
	
	// 해외주요지수를 얻어온다. 
	//bRet = pDataManager->GetFRNationMst(pastrNatCode, pastrNatName);

	m_bInsertingInListCode = TRUE;
	for(int i=0; i < sizeof(strCodes)/sizeof(LPCTSTR); i++)
	{
		strSymbol = strCodes[i];		// 해외지수 심벌
		strName = pDataManager->GetCodeInfo("", 1, strSymbol);			// 해외지수명
		//strCode = strTemp.Right(3);				// 해외지수 코드
		
		strNat = strNats[i];				// 국가명 얻었다.

		m_listCode.InsertItem(nItemNum, strName);
		CDataInListCtrl *pData = NULL;
		pData = (CDataInListCtrl *)m_listCode.GetItemData(nItemNum);
		if(!pData)
		{
			pData = new CDataInListCtrl;
			m_listCode.SetItemData(nItemNum, (DWORD)pData);
		}
		pData->m_strJongCode = strSymbol;			// 지수코드
		pData->m_strCodeName = strName;			// 지수명
		pData->m_strTemp	= strNat;			// 국가명
		pData->m_strTemp2	= strSymbol;		// 심벌명
		nItemNum++;
	}
	m_listCode.SetItemCountEx(nItemNum);
	m_listCode.Invalidate();
	m_bInsertingInListCode = FALSE;

	//delete pastrSymbol;
	//delete pastrName;
	//delete pastrNatCode;
	//delete pastrNatName;

	return TRUE;
}

CRect CTabCodeSearchFR::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(GAP_TABDLG,GAP_TABDLG,GAP_TABDLG,GAP_TABDLG);
	switch(nID)
	{
	case IDC_LISTCODE:
		rect.top = rect.top + GAP_TABDLG;
		rect.bottom = rect.bottom - GAP_TABDLG;
		break;		
	default:
		break;
	}
	return rect;
}

void CTabCodeSearchFR::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	RecalcLayout();		
}

BOOL CTabCodeSearchFR::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* phdr = (NMHDR*)lParam;
	if(phdr->hwndFrom == m_listCode.m_hWnd)
	{
		switch(phdr->code)		{
		case LVN_GETDISPINFO:
			{
				NMLVDISPINFO* pLvdi;
				pLvdi = (NMLVDISPINFO*)lParam;
				GetDispInfo(&pLvdi->item);
			}
			if(pResult != NULL) *pResult = 0;
			break;
//		case LVN_ODCACHEHINT:
//			if(pResult != NULL) *pResult = 0;
//			break;
//		case LVN_ODFINDITEM:
//			if(pResult != NULL) *pResult = 0;
//			break;
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CTabCodeSearchFR::GetDispInfo(LVITEM* pItem)
{
	CDataInListCtrl* pData = (CDataInListCtrl*)m_listCode.GetItemData(pItem->iItem);

	if(pData == NULL)
		return;

	if(pItem->mask & (LVIF_TEXT | LVIF_PARAM))	
	{
		switch(pItem->iSubItem) 
		{
			case 0:
				lstrcpy(pItem->pszText, pData->m_strCodeName);
				break;
			case 1:
				lstrcpy(pItem->pszText, pData->m_strTemp);	// 국가명
				break;
		}
	}
}

void CTabCodeSearchFR::OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
	if(!bInsertMode&&!m_bInsertingInListCode)
	{
		int nCurSel = m_listCode.GetCurSel();

		if(nCurSel >= 0)
		{
			CDataInListCtrl *pData = NULL;
			pData = (CDataInListCtrl *)m_listCode.GetItemData(nCurSel);

			CString strCodeSelected = pData->m_strJongCode;
			if(!strCodeSelected.IsEmpty())
			{
				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = strCodeSelected;
				m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
			}
		}
	}
	
	*pResult = 0;
}

void CTabCodeSearchFR::OnDestroy() 
{
	CTabDlg::OnDestroy();
	
	CDataInListCtrl *pData = NULL;
	int nCnt = m_listCode.GetItemCount();
	for(int i=0;i<nCnt;i++)
	{
		pData = (CDataInListCtrl *)m_listCode.GetItemData(i);
		if(pData)
		{
			delete pData;
			pData = NULL;
		}
	}
	m_listCode.DeleteAllItems();
}

void CTabCodeSearchFR::OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
	if(bInsertMode&&!m_bInsertingInListCode)
	{
		int nCurSel = m_listCode.GetCurSel();

		if(nCurSel >= 0)
		{
			CDataInListCtrl *pData = NULL;
			pData = (CDataInListCtrl *)m_listCode.GetItemData(nCurSel);

			CString strCodeSelected = pData->m_strJongCode;
			if(!strCodeSelected.IsEmpty())
			{
				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = strCodeSelected;
				m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
			}
		}
	}
	
	*pResult = 0;
}