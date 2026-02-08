// TabCodeSearchCommodity.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "TabCodeSearchCommodity.h"
#include "../chart_common/Grid/ColorTable.h"
#include ".\tabcodesearchcommodity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef CArray <int, int>	CIntArray;

#define ROW_HEIGHT 15
#define ROW_WIDTH  48
#define COL_CENTER 1
#define TITLE_ROW 1
enum		{EXERCISE, FIRST, SECOND, THIRD, FOURTH};	// 콜 옵션 종목 

CTabCodeSearchCommodity::CTabCodeSearchCommodity(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearchCommodity::IDD, pParent)
	, m_bSpread(FALSE)
	, m_nOptionMarket(-1)
{
	//{{AFX_DATA_INIT(CTabCodeSearchCommodity)
	m_pGridCtrl = NULL;
	m_nIndex = 1;
	//}}AFX_DATA_INIT
}

void CTabCodeSearchCommodity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearchCommodity)
	DDX_Control(pDX, IDC_COMBO_MONTH, m_ctrlComboMonth);
	DDX_Control(pDX, IDC_LIST_FUTURE, m_listFuture);
	DDX_Control(pDX, IDC_LIST_FUTURE2, m_listFuture2);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RDO_FUTURE, m_bSpread);
}

BEGIN_MESSAGE_MAP(CTabCodeSearchCommodity, CDialog)
	//{{AFX_MSG_MAP(CTabCodeSearchCommodity)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_CBN_EDITCHANGE(IDC_COMBO_MONTH, OnEditchangeComboMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, OnSelchangeComboMonth)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST_FUTURE, OnSelchangeListFuture)
	ON_LBN_SELCHANGE(IDC_LIST_FUTURE2, OnSelchangeListFuture2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RDO_FUTURE, OnBnClickedRdoFuture)
	ON_BN_CLICKED(IDC_RDO_SPREAD, OnBnClickedRdoSpread)
END_MESSAGE_MAP()

void CTabCodeSearchCommodity::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	RecalcLayout();	
}

BOOL CTabCodeSearchCommodity::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	// TODO: Add extra initialization here
	int	nID = 12; // Data 디렉토리
	m_strDataDirPath = (LPCTSTR)(LPCSTR)AfxGetPctrData(nID);
	InitMasterData();
	InitGrid();
	InsertMasterData(m_nIndex);			// master file data로 grid edit여부/color값 setting
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CRect CTabCodeSearchCommodity::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(GAP_TABDLG,GAP_TABDLG,GAP_TABDLG,GAP_TABDLG);
	switch(nID)
	{
	case IDC_LIST_FUTURE:
		rect.top = rect.top + (HEIGHTOFCOMBO * 2);
		//rect.bottom = rect.bottom - HEIGHTOFJONGSTATUS - GAP_TABDLG - HEIGHTOFBUTTON - GAP_TABDLG;
		rect.bottom = rect.top + 110;
		break;		
	case IDC_LIST_FUTURE2:
		rect.top = rect.top + (HEIGHTOFCOMBO * 2) + 110;
		//rect.bottom = rect.bottom - HEIGHTOFJONGSTATUS - GAP_TABDLG - HEIGHTOFBUTTON - GAP_TABDLG;
		rect.bottom = rect.top + 100;
		break;		
	case ID_GRIDCTRL:
		rect.top = rect.top + (HEIGHTOFCOMBO * 2) + 215;
		//rect.top = rect.bottom - GAP_TABDLG - HEIGHTOFJONGSTATUS - HEIGHTOFBUTTON;
		rect.bottom = rect.bottom - GAP_TABDLG;// - HEIGHTOFBUTTON;
		break;
	default:
		break;
	}
	return rect;
}

void CTabCodeSearchCommodity::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listFuture);
	RecalcLayoutCtrl(m_listFuture2);
	if(m_pGridCtrl)
		RecalcLayoutCtrl(*m_pGridCtrl,ID_GRIDCTRL);	
}

void CTabCodeSearchCommodity::OnDestroy() 
{
	CTabDlg::OnDestroy();

	// TODO: Add your message handler code here
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if(m_pGridCtrl)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	} 
}

// 종목코드 grid init
void CTabCodeSearchCommodity::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		CRect rcRect;
		GetDlgItem(IDC_FRAME_OPTION)->GetWindowRect(rcRect);		
		ScreenToClient(&rcRect);		

		m_pGridCtrl->Create(rcRect, this, ID_GRIDCTRL);

		///////////////////////////////////////////////////
		// 각종 설정
		//m_pGridCtrl->SetEditable(TRUE);				// CInplaceEdit 설정
		//		m_pGridCtrl->SetFont(GetParent()->GetFont());
		m_pGridCtrl->SetGridColor(COLOR_GRIDHEADER);		// Grid Color 설정
		m_pGridCtrl->SetBkColor(COLOR_WHITE);			// Grid Back Color 설정
		m_pGridCtrl->EnableColumnHide();				// Column 숨길 수 있게 설정
		m_pGridCtrl->EnableRowHide();					// Row 숨길 수 있게 설정
		m_pGridCtrl->EnableTitleTips(FALSE);			// ToolTip 설정
		m_pGridCtrl->SetGridLineColor(COLOR_GRIDLINE);	// Grid Line color

		///////////////////////////////////////////////////
		// 그리드 모양을 Default로 한다
		InitGridRowCol();

		// 그리드의 모양을 바꾼다
		ChangeGridRowCol();

		// 그리드에 데이터를 추가한다.
		InsertGridData();
	}
}

void CTabCodeSearchCommodity::InitGridRowCol()
{	
	try {
		int nRow, nCol;

		nRow = m_listMaster.GetCount() + TITLE_ROW;
		//nCol = DEFAULT_COLUMN_COUNT;
		nCol = 3;

		m_pGridCtrl->SetRowCount(nRow);				// Row ¼³A¤
		m_pGridCtrl->SetColumnCount(nCol);			// Column ¼³A¤
		//		m_pGridCtrl->SetFixedRowCount(1);	// Fixed Row ¼³A¤
		m_pGridCtrl->SetFixedColumnCount(0);		// Fixed Column ¼³A¤

		//		m_pGridCtrl->SetColCenter(COL_CENTER);		// Column Aß¾O ¼³A¤
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}

void CTabCodeSearchCommodity::ChangeGridRowCol()
{
	int nCol = m_pGridCtrl->GetColumnCount();		// Column Count 구하기
	int nRow = m_pGridCtrl->GetRowCount();			// Row Count 구하기

	// fill rows/cols with text
	for (int row = 0; row < nRow; row++)
		m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);

	for (int col = 0; col < nCol; col++)
		m_pGridCtrl->SetColumnWidth(col, ROW_WIDTH);

	m_pGridCtrl->SetColumnWidth(COL_CENTER, ROW_WIDTH + 13);		

	m_pGridCtrl->Refresh();
}

// 빈 데이터 설정 및 추가
// 나중에 SetItemText(CString) 만으로 데이터를 추가할 수 있게 미리 설정
void CTabCodeSearchCommodity::InsertGridData()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nRow = m_pGridCtrl->GetRowCount();

	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)
		{ 

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE|GVIF_PARAM;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;

			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX|DT_CENTER;

			Item.strText = GetInsertGridDataText(row, col);
			//			Item.crBkClr = GetColorTable(row, col);
			Item.crBkClr = RGB(238,239,243);
			if(row == 0 && col < COL_CENTER)			Item.crBkClr = RGB(244, 180, 180);
			else if(row == 0 && col > COL_CENTER)		Item.crBkClr = RGB(180, 207, 244);
			Item.crFgClr = COLOR_BLACK;
			//Item.crSelectedBkClr = COLOR_GRAY_LIGHT;

			//Item.lParam = NOEDIT;

			m_pGridCtrl->SetItem(&Item);
		}
	}	

}

CString CTabCodeSearchCommodity::GetInsertGridDataText(int row, int col)
{

	if (row != 0 )		return "";

	if (col == COL_CENTER)	return "행사가";
	else if (col < COL_CENTER)	return "콜";
	else if (col > COL_CENTER)	return "풋";

	//	int nIndex;
	//	if (col < COL_CENTER)	nIndex = COL_CENTER	- col;
	//	else					nIndex = col - COL_CENTER;

	CString strText;
	strText.Format("%d월", m_nMonth.nInfo[m_nIndex] % 100);
	return strText;
}

// master file data로 grid edit여부/color값 setting
void CTabCodeSearchCommodity::InsertMasterData(int index)
{
	int nRow, nCol, nRowTotal, nExercise, nATM = 0;

	CString		strPrice, strTmp;
	COLORREF	clrText;

	// ATM
	CString strPath, strATM;
	strPath.Format("%s\\%s",  m_strDataDirPath, "master\\atm.dat");
	char szAtm[100]; memset(szAtm,0x00,sizeof(szAtm));
	::GetPrivateProfileString("ATM", "PRICE", "", szAtm, 100, strPath);
	strATM = (LPCSTR)(LPSTR)szAtm;
	//m_pDBMgr->GetOptionATM(strATM);
	if (strlen(strATM)> 0)	nATM = (int)atof(strATM);

	nRowTotal = m_pGridCtrl->GetRowCount();
	nRow = nRowTotal;
	for (POSITION pos = m_listMaster.GetHeadPosition(); pos; )
	{
		codeinfo& info = m_listMaster.GetNext(pos);

		nExercise = info.nInfo[EXERCISE];
		nRow--;

		// 행 check
		if (nRow < TITLE_ROW || nRow >= nRowTotal)		continue;

		clrText = (nATM == nExercise) ? COLOR_RED : COLOR_BLACK;
		UpdateHogaData(nRow, COL_CENTER, GetRealExercise(nExercise) , clrText, 1);
		//		for (int i=FIRST; i <= FOURTH; i++)
		//		{
		if (info.nInfo[index] == 0)	
		{
			for (int j=0; j < 2; j++)
			{
				nCol = (j == 0)	? (COL_CENTER - 1) : (COL_CENTER + 1);
				m_pGridCtrl->SetItemText(nRow, nCol, "X");
			}
			continue;
		}

		for (int j=0; j < 2; j++)
		{

			nCol = (j == 0)	? (COL_CENTER - 1) : (COL_CENTER + 1);

			m_pGridCtrl->SetItemText(nRow, nCol, " ");
			m_pGridCtrl->SetItemBkColour(nRow, nCol, COLOR_WHITE);
		}
		//		}
	}
	m_pGridCtrl->Refresh();

}	

// 옵션 동일 행사가내에서의 콜 -> 풋 및 만기일의 오름 차순으로 정렬 
int OptEvtPriceItemFuncCommodity(const void* p1,const void *p2)
{
	COptionItem *q1 = *(COptionItem**)p1;
	COptionItem *q2 = *(COptionItem**)p2;

	if(q1->m_nCallPut > q2->m_nCallPut)
	{
		return 1; 
	}
	else if(q1->m_nCallPut < q2->m_nCallPut)
	{
		return -1; 
	}
	else
	{
		if(q1->m_nYear > q2->m_nYear)
		{
			return 1;
		}
		else if(q1->m_nYear < q2->m_nYear)
		{
			return -1; 
		}
		else
		{
			if(q1->m_nMonth > q2->m_nMonth)
			{
				return 1; 
			}
			else if(q1->m_nMonth < q2->m_nMonth)
			{
				return -1; 
			}
			else
			{
				if(q1->m_nEvtPrice > q2->m_nEvtPrice)
				{
					return 1; 
				}
				else if(q1->m_nEvtPrice < q2->m_nEvtPrice)
				{
					return -1; 
				}
				else
					return 0;
			}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
// 만든이            : 이유정(YouJeong, Lee)
// Email             : lotte222@dooriic.co.kr
// 만든 날짜         : 2004/2/4
// 함수 이름         : CDlgSetup::InitMasterData
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터 없음     : 
// 함수 설명         : 메인에서 master file list pointer를 얻어 codeinfo list 생성
// 변경				 : //003 2006.06.30 {{ Koscom 형식으로 데이터 얻어오도록 수정 }} 
///////////////////////////////////////////////////////////////////////////////// 
BOOL CTabCodeSearchCommodity::InitMasterData()
{
	int nID = 5;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	m_pDataManager = (IMasterDataManager*)pGateMng->GetInterface(nID);

	if (m_pDataManager == NULL)		return FALSE;


	//////////////////////////////////////////////////////////////////////////
	// 선물
	BOOL bUSD = FALSE, bGOLD = FALSE, bCD = FALSE, bKTB = FALSE, b5TB = FALSE, bMSB = FALSE, bYEN = FALSE, bSYR = FALSE;
	int iIdx = 0;

//@유진삭제
//	CByteArray* parByteMarket = new CByteArray;
//	if(m_pDataManager->GetMarketArray(parByteMarket))
//	{
//		for(int nIndex=0; nIndex < parByteMarket->GetSize(); nIndex++)
//		{
//			BYTE nMarket = parByteMarket->GetAt(nIndex);
//			switch(nMarket)
//			{
//			case 13:	// 13 = KOFEX 시장아이디-USD
//				if(!bUSD)
//				{
//					iIdx = m_listFuture.AddString(_T("USD"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bUSD = TRUE;
//				}
//				break;
//
//			case 15:	// 15 = KOFEX 시장아이디-GOLD
//				if(!bGOLD)
//				{
//					iIdx = m_listFuture.AddString(_T("GOLD"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bGOLD = TRUE;
//				}
//				break;
//
//			case 16:	// 16 = KOFEX 시장아이디-CD -> LKTB
//				if(!bCD)
//				{
//					// 시장명 변경 - ojtaso (20080222)
//					//iIdx = m_listFuture.AddString(_T("CD"));
//					iIdx = m_listFuture.AddString(_T("LKTB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bCD = TRUE;
//				}
//				break;
//
//			case 17:	// 17 = KOFEX 시장아이디-KTB
//				if(!bKTB)
//				{
//					iIdx = m_listFuture.AddString(_T("KTB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bKTB = TRUE;
//				}
//				break;
//
//			case 62:	// 62 = KOFEX 시장아이디-5TB
//				if(!b5TB)
//				{
//					iIdx = m_listFuture.AddString(_T("5TB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					b5TB = TRUE;
//				}
//				break;
//
//			case 61:	// 61 = KOFEX 시장아이디-MSB
//				if(!bMSB)
//				{
//					iIdx = m_listFuture.AddString(_T("MSB"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bMSB = TRUE;
//				}
//				break; 
//
//			case 18:	// 18 = KOFEX 시장아이디-YEN
//				if(!bYEN)
//				{
//					iIdx = m_listFuture.AddString(_T("YEN"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bYEN = TRUE;
//				}
//				break;
//
//			case 19:	// 19 = KOFEX 시장아이디-SYR
//				if(!bSYR)
//				{
//					iIdx = m_listFuture.AddString(_T("SYR"));
//					m_listFuture.SetItemData(iIdx, nMarket);
//					bSYR = TRUE;
//				}
//				break;
//			}
//		}
//
//	}
//@유진삭제

	// }}

	return TRUE;
}

// 행사가에 해당하는 row 찾기
int  CTabCodeSearchCommodity::GetGridRow(int nExercise)
{
	// int(세자리) -> long(다섯자리) (예:92 ==> 9250 : 92.50)
	long lPrice = GetRealExercise(nExercise);

	return TITLE_ROW + ((m_lMaxLimit - lPrice)); 
}

// row에 해당하는 행사가 찾기
int  CTabCodeSearchCommodity::GetGridExercise(int row)
{
	long lPrice = m_lMaxLimit - (row - TITLE_ROW)* 250; 

	return (int)(lPrice / 100);
}

// double -> string data 변경 & redraw
void CTabCodeSearchCommodity::UpdateHogaData(int nRow, int nCol, double dPrice, COLORREF crFgClr, int nPoint/*=2*/)
{

	CString strText;
	strText.Format("%.*f", nPoint, dPrice);

	// 텍스트 변경
	m_pGridCtrl->SetItemText(nRow, nCol, strText);

	// 색상 마스킹 되어 있으면 색상 변경
	m_pGridCtrl->SetItemFgColour(nRow, nCol, crFgClr);

	m_pGridCtrl->RedrawCell(nRow, nCol);
}

// 행사가로 set
long CTabCodeSearchCommodity::GetRealExercise(int nPrice)
{ 
	long lPrice = nPrice / 10.; 
	return lPrice;
}

void CTabCodeSearchCommodity::OnEditchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_ctrlComboMonth.GetCurSel();
	InsertMasterData(nSel);
}

void CTabCodeSearchCommodity::OnSelchangeComboMonth() 
{
	// TODO: Add your control notification handler code here
	m_nIndex = m_ctrlComboMonth.GetCurSel() +1;
	InsertGridData();
	InsertMasterData(m_nIndex);
}

LRESULT CTabCodeSearchCommodity::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_NOTIFY)
	{
		NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
		int nCode = pNmgv->hdr.code;

		if(nCode == GVN_SELCHANGING)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
			{
				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
				if(!bInsertMode)
				{
					CString str;
					str = m_pGridCtrl->GetItemText(pNmgv->iClickRow,pNmgv->iClickCol);

					// 거래대상종목이 아닌경우 return
					if (str.Find("X") >= 0)						
						return TRUE;

					CString strCodeSelected;
					GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
					if(!strCodeSelected.IsEmpty())
					{
						STINPUTCODEDATA stInputCodeData;
						stInputCodeData.m_strCode = strCodeSelected;
						m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
					}
					return TRUE;
				}	
				// Nonzero if message was handled; otherwise 0.
				//				return fnWhenClickGridJango(FALSE, pNmgv->iClickRow, pNmgv->iClickCol);
			}
		}
		else if(nCode == NM_DBLCLK)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
			{
				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
				if(bInsertMode)
				{
					CString str;
					str = m_pGridCtrl->GetItemText(pNmgv->iClickRow,pNmgv->iClickCol);

					// 거래대상종목이 아닌경우 return
					if (str.Find("X") >= 0)						
						return TRUE;

					CString strCodeSelected;
					GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
					if(!strCodeSelected.IsEmpty())
					{
						STINPUTCODEDATA stInputCodeData;
						stInputCodeData.m_strCode = strCodeSelected;
						m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
					}
					return TRUE;
				}	
				// Nonzero if message was handled; otherwise 0.
				//				return fnWhenClickGridJango(FALSE, pNmgv->iClickRow, pNmgv->iClickCol);
			}
		}
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CTabCodeSearchCommodity::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		{
			pDC->SetBkColor(m_crBk);
			return m_brBkColor;
		}		
		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// 만든이            : 이유정(YouJeong, Lee)
// Email             : lotte222@dooriic.co.kr
// 만든 날짜         : 2004/2/6
// 함수 이름         : CDlgSetup::GetCodeInfo
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터
//		int nType : 요청하는 정보(ROWCOL : 행/열 정보, CODE:종목코드)
//		int& row :  
//		int& col : 
//		st_Code& code  : 
// 함수 설명         : 행/열정보 <-> 종목코드 상호 변환
///////////////////////////////////////////////////////////////////////////////
BOOL CTabCodeSearchCommodity::GetCodeInfo(int nType, int& row, int& col, CString& strCode)
{
	int nRowTotal = m_pGridCtrl->GetRowCount();
	if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)		return FALSE;

	int nGubun;
	if (col < COL_CENTER)
		nGubun = 2;
	else
		nGubun = 3;

	int nYear = (int)(m_nMonth.nInfo[m_nIndex] / 100);
	int nMonth = m_nMonth.nInfo[m_nIndex] % 100; 

	double dEvtPrice = atof(m_pGridCtrl->GetItemText(row, COL_CENTER));
//@유진삭제
//	return m_pDataManager->GetCommodityOptionItemCode(m_nOptionMarket, nGubun, nYear, nMonth, dEvtPrice, strCode);
//@유진삭제
	return TRUE;
}

void CTabCodeSearchCommodity::OnSelchangeListFuture() 
{
	// TODO: Add your control notification handler code here
	m_listFuture2.ResetContent();

	int nSel = m_listFuture.GetCurSel();
	if(nSel == LB_ERR)
		return;

	UpdateData();

	CStringArray* pastrJongName;	// 옵션 종목 명 
	BYTE nMarket = m_listFuture.GetItemData(nSel);

	pastrJongName = new CStringArray;
//@유진삭제
//	m_pDataManager->GetMarketItemName(nMarket, pastrJongName, m_bSpread);
//@유진삭제


	for(int i = 0; i < pastrJongName->GetSize(); i++)
	{
		m_listFuture2.AddString(pastrJongName->GetAt(i));
		m_listFuture2.SetItemData(i, nMarket);
	}

	delete pastrJongName;	// 옵션 종목 명 

	//003 2006.06.30 
	// 옵션
	m_listMaster.RemoveAll();
	m_ctrlComboMonth.ResetContent();
	for (i=0; i < 5; i++)
		m_nMonth.nInfo[i] = 0;

	CStringArray* pastrJongCode;	// 옵션 종목 코드 
	CIntArray* panCallPut;
	CIntArray* panYear;
	CIntArray* panMonth;
	CIntArray* panEvtPrice;

	pastrJongCode = new CStringArray;
	panCallPut = new CIntArray;
	panYear = new CIntArray;
	panMonth = new CIntArray;
	panEvtPrice = new CIntArray;

	int nIndex = 0;
	if(nMarket == 13)
		m_nOptionMarket = 14;
	else
		m_nOptionMarket = -1;

//@유진삭제
//	m_pDataManager->GetCommodityOptionItemCode(m_nOptionMarket, pastrJongCode, panCallPut, panYear, panMonth, panEvtPrice);
//@유진삭제

	// 콜 및 옵션의 만기일 오름 차순으로 정렬 
	int nCount = pastrJongCode->GetSize();
	// 콜 및 옵션의 만기일 오름 차순으로 정렬 
	if(nCount)
	{
		COptionItem **pData = new COptionItem * [nCount];
		for(int i=0; i<pastrJongCode->GetSize(); i++)
		{
			pData[i] = new COptionItem;
			pData[i]->SetItem(pastrJongCode->GetAt(i), panCallPut->GetAt(i), panYear->GetAt(i), panMonth->GetAt(i), panEvtPrice->GetAt(i));
		}

		::qsort(pData, nCount, sizeof(void*), OptEvtPriceItemFuncCommodity);

		for(i=0; i<nCount; i++)
		{
			pastrJongCode->SetAt(i, pData[i]->m_strCode);
			panCallPut->SetAt(i, pData[i]->m_nCallPut);
			panYear->SetAt(i, pData[i]->m_nYear);
			panMonth->SetAt(i, pData[i]->m_nMonth);
			panEvtPrice->SetAt(i, pData[i]->m_nEvtPrice);
			delete pData[i];
			//m_OptEvtPriveList.m_lstEvtPriveItem.Add(pData[i]);
		}

		delete pData;
	}

	//	if (pListP == NULL)		return FALSE;
	CString  strCode, strMonth;
	int		 nGubun, nMonth, nExercise, nMonthNow; 
	int		 nIndexMonth = EXERCISE;
	BOOL	 bFind;

	nMonthNow = 0;
	int MaxExercise = 0;
	for(nIndex=0; nIndex < pastrJongCode->GetSize(); nIndex++)
	{
		strCode = pastrJongCode->GetAt(nIndex); 	
		strCode.TrimLeft();		strCode.TrimRight();

		if (strCode.GetLength() >= 8)// && nGubun != 3)
		{
			//			nGubun = atoi(strCode.Left(1));
			CString strTemp;
			strTemp = strCode.Left(1);
			nGubun = atoi(strTemp);

			if (nGubun == 3)	 break;		// 콜 마스터만 read

			nMonth = (panYear->GetAt(nIndex) * 100) + panMonth->GetAt(nIndex);
			nExercise = panEvtPrice->GetAt(nIndex);

			if (MaxExercise < nExercise)		MaxExercise = nExercise;

			if (nMonthNow != nMonth)	
			{	
				nIndexMonth++;
				m_nMonth.nInfo[nIndexMonth] = nMonthNow = nMonth; 
			}

			bFind = FALSE;
			for(POSITION pos=m_listMaster.GetHeadPosition(); pos; )
			{
				codeinfo&	info  = m_listMaster.GetNext(pos);
				if (info.nInfo[EXERCISE] == nExercise)
				{
					info.nInfo[nIndexMonth] = 1;
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				codeinfo	info;
				info.nInfo[EXERCISE] = nExercise;
				info.nInfo[nIndexMonth] = 1;

				m_listMaster.AddTail(info);
			}
		}
	}

	for(i=1; i<5; i++)
	{
		if(m_nMonth.nInfo[i])
		{
			strCode.Format("%d월", m_nMonth.nInfo[i] % 100);
			m_ctrlComboMonth.AddString(strCode);
		}
	}
	m_ctrlComboMonth.SetCurSel(0);

	// 최고 행사가 산출
	if (MaxExercise > 0)	m_lMaxLimit = GetRealExercise(MaxExercise);

	delete pastrJongCode;
	delete panCallPut;
	delete panYear;
	delete panMonth;

	InitGridRowCol();

	// 그리드의 모양을 바꾼다
	ChangeGridRowCol();

	// 그리드에 데이터를 추가한다.
	InsertGridData();
	InsertMasterData(m_nIndex);			// master file data로 grid edit여부/color값 setting
}

void CTabCodeSearchCommodity::OnSelchangeListFuture2() 
{
	// TODO: Add your control notification handler code here
	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
	if(!bInsertMode)
	{
		int nSel = m_listFuture2.GetCurSel();
		if(nSel == LB_ERR)
			return;

		UpdateData();

		BYTE nMarket = m_listFuture2.GetItemData(nSel);
		CString strItemName;
		m_listFuture2.GetText(nSel, strItemName);

		CString strCodeSelected = _T("");
//@유진삭제
//		m_pDataManager->GetMarketItemCode(nMarket, strItemName, strCodeSelected, m_bSpread);
//@유진삭제

		if(!strCodeSelected.IsEmpty())
		{
			STINPUTCODEDATA stInputCodeData;
			stInputCodeData.m_strCode = strCodeSelected;
			m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
		}
	}	
}

void CTabCodeSearchCommodity::OnBnClickedRdoFuture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_bSpread)
		return;

	OnSelchangeListFuture();
}

void CTabCodeSearchCommodity::OnBnClickedRdoSpread()
{
	if(m_bSpread)
		return;

	OnSelchangeListFuture();
}
