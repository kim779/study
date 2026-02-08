// FloatingHogaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FloatingHogaDlg.h"
#include "../chart_common/Grid/MainDefine.h"
#include "../chart_common/Grid/ColorTable.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IMainInfoMng.h"

//<<20100308_JS.Kim 솔로몬
//#include "../CommonTR/EU_s0001.h"
#include "../CommonTR/TranStruct_GNF.h"
//>>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg dialog

#define DLG_WIDTH	222
//#define DLG_HEIGHT	265
#define DLG_HEIGHT	243

CFloatingHogaDlg::CFloatingHogaDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CFloatingHogaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFloatingHogaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIconState = TRUE;
	m_bCapTure	 = FALSE;
	m_pMainDlg	 = NULL;

	m_strCode				= _T("");
	m_strOldCode			= _T("");
	
	// 윈도우 확장상태 ------------>
	m_bExpandRight			= FALSE;
	m_bExpandBottom			= FALSE;
	m_bOpenCol				= FALSE;
	m_bIsFirstLoad			= FALSE;
	m_bIsShowReserveCol		= FALSE;					// 주문예약칼럼 보이기/감추기
	// <----------------------------
		
	m_pGridCtrl		= NULL;	
	m_rectGrid		= CRect(0, 0, 0, 0);	// 그리드 	
		
	m_strCurPrice		= _T("");		// 현재가
	m_strPreClosePrice	= _T("");		// 전일종가
	m_nHighLimit		=  0;			// 상한가
	m_nLowLimit			=  0;			// 하한가
	m_strPreVol			= _T("");		// 전일거래량				JSJ_ITCastle_Add_040502
	m_strSEqualPer		= _T("");		// 동시간대비 전일거래량	JSJ_ITCastle_Add_040706
	m_nMaedoPosJango	=  0;			// 실제 매도주문 가능수량
	m_nHogaRow			= -1;
	m_nStaticRow		= -1;
	m_nChartPriceRow	= -1;			// 차트에서 받은 가격의 해당호가열
	m_strChartPrice		= _T("");		// 차트에서 받은 가격
	m_bMoveChartPriceRow= FALSE;		// 차트가격 해당열로 스크롤이동
	m_nHogaRangeForColor= -1;			// 호가색상 범위

	
	// Full Hoga 관련-------------------->
	m_bIsTenHoga		= TRUE;					// 10단계호가 <-> Full호가 
	m_bUseLimitHoga		= TRUE;					// 상하한 조회
	m_nTotalRowCnt		= -1;					// 전체 열갯수
	m_nCurPriceRow		= PRICE_ROW;			// 현재가열
	m_nLowLimitRow		= LOW_ROW;				// 하한가 열
	m_n5HogaSumRow		= TENHOGA_ROW_5HOSUM;	// 5단게합 표시열
	m_nTotalSumRow		= TENHOGA_ROW_TOTSUM;	// 전체합 표시열
	m_nExtraVolRow		= TENHOGA_ROW_EXTRAVOL;	// 시간외잔량열
	// <----------------------------------

	m_nThreeStateDaebiType = e_DAEBI;	// 등락으로 초기 설정 
//	m_hBitmap = NULL;
	m_dlgOriRect = CRect(0,0,0,0);
	// end

	m_pDrdsLib = NULL;
}


void CFloatingHogaDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatingHogaDlg)
	DDX_Control(pDX, IDC_STATIC_FLOTING_GRID, m_staticGridFrame);
	DDX_Control(pDX, IDC_STATIC_SYMBOL, m_staticSymbol);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staitcTilte);	
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrlButtonClose);	
	DDX_Control(pDX, IDC_BUTTON_SELL, m_ctrlButtonSell);	
	DDX_Control(pDX, IDC_BUTTON_BUY, m_ctrlButtonBuy);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFloatingHogaDlg, CRscDlg)
	//{{AFX_MSG_MAP(CFloatingHogaDlg)
	ON_WM_LBUTTONDOWN()	
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SELL, OnButtonSell)
	ON_BN_CLICKED(IDC_BUTTON_BUY, OnButtonBuy)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
//	ON_MESSAGE( WM_GET_BROD, OnGetBroad)		//20100308_JS.Kim 솔로몬
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg message handlers


void CFloatingHogaDlg::OnSize(UINT nType, int cx, int cy) 
{	
	CDialog::OnSize(nType, cx, cy);

	if(cy>100 && m_dlgOriRect.bottom>0)
	{
		if(m_pGridCtrl == NULL)
			InitGrid();
	}
}

#define WS_EX_LAYERED       0x00080000
#define LWA_ALPHA           0x00000002
BOOL CFloatingHogaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_staticSymbol.SetBkColor(RGB(185,205,230));
	m_staticSymbol.SetBkColor(RGB(92,135,183));
	m_staticSymbol.SetTextColor(CLR_FONT_DEFAULT);
	m_staticSymbol.SetTextColor(RGB(255,255,255));

	m_staitcTilte.SetBkColor(RGB(255,255,255));
	m_staitcTilte.SetTextColor(RGB(61,57,57));
	m_staitcTilte.SetText("5호가 창");
	
	
	// TODO: Add extra initialization here
	if(GetSafeHwnd())
	{
		GetWindowRect(m_dlgOriRect);
	}
	
	CRect rect(0,0,0,0), trect(0,0,0,0), dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(rect);//윈도우 좌표...
	GetWindowRect(m_dlgOriRect); //윈도우 좌표...
	
	trect.left		= rect.left	;
	trect.top		= rect.top	;
	trect.right		= rect.right;	
	trect.bottom	= rect.bottom;
	
	ScreenToClient(trect);
	if(m_bIconState)	
	{	
		CWnd* pWnd = GetParent();
		m_dlgOriRect.right = m_dlgOriRect.left + rect.Width();
		m_dlgOriRect.bottom = m_dlgOriRect.top + rect.Height();											
	}
	else
	{	
		m_dlgOriRect.right	= m_dlgOriRect.left	+ DLG_WIDTH;
		m_dlgOriRect.bottom	= m_dlgOriRect.top	+ DLG_HEIGHT;			
	}
	
	if(m_pMainDlg)
		m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1:0, (LPARAM)&m_dlgOriRect);

	int nID = 21;
	//<<20100308_JS.Kim 솔로몬
	//IGateManager* pGateMng;	AfxGetIGateManagerFX(pGateMng);
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	//>>
	if(pGateMng != NULL)
	{
		IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);
		CString strImageDir;
		strImageDir.Format("%s\\System\\Image\\Chart\\W38_h21.bmp", pManager->GetRootDir());

		CBitmap bitmap;
		bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
		m_ImageListOrder.Create(38, 21, ILC_MASK | ILC_COLORDDB, 1, 1);
		m_ImageListOrder.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

		m_ctrlButtonSell.FP_vSetImage(&m_ImageListOrder, 0, 1);
		m_ctrlButtonBuy.FP_vSetImage(&m_ImageListOrder, 3, 4);
	}

	MakeDrdsConnectAdvise();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CFloatingHogaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
		case UMSG_RECEIVEFROMSERVER:
			{
				if(m_strCode.GetLength()>0) OnCodeUnAdvise(m_strCode);	// Old Code해제
				stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;				
				SetHogaData((char*)pStR->m_pBuffer,pStR->m_dwLength);
				OnCodeAdvise(m_strCode);	// New Code등록
			}
		case WM_CLICKLABEL:
			{
				m_bCapTure = FALSE;
			}
			break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CFloatingHogaDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{	
	CRect rect(0,0,0,0), trect(0,0,0,0), dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(rect);//윈도우 좌표...
	GetWindowRect(dlgRect); //윈도우 좌표...
	
	trect.left		= rect.left	;
	trect.top		= rect.top	;
	trect.right		= rect.right;	
	trect.bottom	= rect.bottom;
	
	ScreenToClient(trect);
	if(PtInRect(trect, point))
	{
		if(m_bIconState)	
		{	
			dlgRect.right	= dlgRect.left	+ DLG_WIDTH;
			dlgRect.bottom	= dlgRect.top	+ DLG_HEIGHT;														
		}
		else
		{	
			CWnd* pWnd = GetParent();
			dlgRect.right = dlgRect.left + rect.Width();
			dlgRect.bottom = dlgRect.top + rect.Height();			
		}

		m_bIconState = !m_bIconState;
		
		if(m_pMainDlg)
			m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1 : 0, (LPARAM)&dlgRect);
		
		
	}
	
	CRscDlg::OnLButtonDblClk(nFlags, point);
}



void CFloatingHogaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bCapTure = TRUE;
	CRscDlg::OnLButtonDown(nFlags, point);
}

void CFloatingHogaDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bCapTure = FALSE;
	CRscDlg::OnLButtonUp(nFlags, point);
}

void CFloatingHogaDlg::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CRscDlg::OnMouseMove(nFlags, point);
}


void CFloatingHogaDlg::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CClientDC dc(this);
		int PointSize = 9;
		m_logGridFont.lfHeight = -MulDiv(PointSize, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
		m_logGridFont.lfWidth = 0;
		m_logGridFont.lfEscapement = 0;
		m_logGridFont.lfOrientation = 0;
		m_logGridFont.lfWeight = FW_THIN;
		m_logGridFont.lfItalic = FALSE;
		m_logGridFont.lfUnderline = FALSE;
		m_logGridFont.lfStrikeOut = FALSE;
		m_logGridFont.lfCharSet = DEFAULT_CHARSET;
		m_logGridFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
		m_logGridFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
		m_logGridFont.lfQuality = DEFAULT_QUALITY;
		m_logGridFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
		strcpy(m_logGridFont.lfFaceName, "굴림체");

		if( m_GridFont.GetSafeHandle() )
			m_GridFont.DeleteObject();

		m_GridFont.CreateFontIndirect(&m_logGridFont);

		// Create the Gridctrl object
		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		// Create the Gridctrl window
		GetDlgItem(IDC_STATIC_FLOTING_GRID)->GetWindowRect(m_rectGrid);		
		ScreenToClient(&m_rectGrid);
		m_pGridCtrl->Create(m_rectGrid, this, IDC_STATIC_FLOTING_GRID);
		m_pGridCtrl->SetWindowPos(NULL, m_rectGrid.left, m_rectGrid.top+1, m_rectGrid.Width(), m_rectGrid.Height()-1, SWP_NOZORDER);


		// fill it up with stuff
		m_pGridCtrl->SetEditable(TRUE);
		m_pGridCtrl->EnableDragAndDrop(FALSE);
		m_pGridCtrl->SetFont(&m_GridFont, FALSE);
		m_pGridCtrl->SetGridColor(COLOR_WHITE);
		m_pGridCtrl->SetGridLineColor(COLOR_GRIDLINE);
		m_pGridCtrl->EnableColumnHide();
		m_pGridCtrl->EnableRowHide();	

		// 그리드 모양을 Default로 한다
		InitGridRowCol();
	
		// 그리드의 모양을 바꾼다
		ChangeGrid_AllWidth();

		// 그리드의 셀 포맷
		InitGridFormat();
		m_pGridCtrl->ShowWindow(SW_SHOW);
	}
}

void CFloatingHogaDlg::InitGridRowCol()
{	
	try {
		int nRow, nCol;

		// 각각의 타입에 맞게 그리드 모양을 다시 설정해 준다
		nRow = TENHOGA_ROW_COUNT;
		nCol = TOTAL_COLUMN_COUNT;
		
		m_pGridCtrl->SetRowCount(nRow);
		m_pGridCtrl->SetColumnCount(nCol);
		m_pGridCtrl->SetFixedRowCount(FIXED_ROW_CNT_TEN);
		m_pGridCtrl->SetFixedColumnCount(0);
		m_pGridCtrl->SetBkColor(COLOR_WHITE);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}



// 그리드 Row, Column Width
void CFloatingHogaDlg::ChangeGrid_AllWidth()
{
	ChangeGrid_RowHeight();
	ChangeGrid_ColumnWidth();
}


// 그리드 Row Width 조정
void CFloatingHogaDlg::ChangeGrid_RowHeight()
{
	int nRow = m_pGridCtrl->GetRowCount();
	// 열의 높이 설정
	int row;
	for (row = 0; row < nRow; row++)
	{
		// ******** 10 단호가 **********
		if ( m_bIsTenHoga == TRUE )		
		{
			if( m_bExpandBottom == TRUE )		// 확장
			{
				m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
			}
			else								// 축소
			{
				if( MAEDO_ROW <= row && row <= MAEDO_ROW+4 )
				{
					m_pGridCtrl->SetRowHeight(row, 0);
				}
				else if( MAESU_ROW+5 <= row && row <= MAESU_ROW+9 )
				{
					m_pGridCtrl->SetRowHeight(row, 0);
				}
				else if (	(ETC_HOGAUP_ROW		== row) || 	// 기타 호가 상단
							(ETC_HOGADOWN_ROW	== row)	||	// 기타 호가 하단
							(MARKETPRICE_ROW	== row)
				)
				{
					m_pGridCtrl->SetRowHeight(row, 0);
				}
				else 
				{
					m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
					//m_pGridCtrl->SetRowHeight_NoReset(row, ROW_HEIGHT);
				}
			}
		}
		// ******** 전체 호가 **********
		else
		{
			m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
		}
	}

	m_pGridCtrl->Refresh();	
}


// 그리드 Column Width 조정
void CFloatingHogaDlg::ChangeGrid_ColumnWidth()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nColWdth[TOTAL_COLUMN_COUNT] = {0, };

	// column width "0" initialize (스크롤 문제) ------------->>
	for (int col = 0; col < nCol; col++)
	{
		m_pGridCtrl->SetColumnWidth(col, 0);
	}
	// <<-------------------------------------------------------


	// *****************************************************************************
	// nColGubun	0 : 10호가
	//				1 : 전체호가 1) 전체 열의 갯수가 10호가 그리드 칼럼보다 *작음*
	//							 2)	하단 확장 상태							
	//				2 : 전체호가 1) 전체 열의 갯수가 10호가 그리드 칼럼보다 *작음*
	//							 2) 하단 축소 상태
	//				3 : 전체호가 1) 전체 열의 갯수가 10호가 그리드 칼럼보다 *큰경우*
	// ******************************************************************************
	int nColGubun;
	if (m_bIsTenHoga == TRUE)
	{
		nColGubun = 0;
	}
	else
	{
		int nRow = m_pGridCtrl->GetRowCount();

		// ## 전체 열의 갯수가 10호가 그리드 칼럼보다 작은경우 ##
		if (nRow <= TENHOGA_ROW_COUNT)
		{
			if (m_bExpandBottom == TRUE)
				nColGubun = 1;
			else
				nColGubun = 2;
		}
		else
		{
			nColGubun = 3;
		}
	}


	// 행의 넓이 설정
	switch (nColGubun)
	{
	case 0:
	case 1:
		if ( m_bExpandRight == TRUE )			// 확장-VScroll No
		{
			nColWdth[MAEDO_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[LEFT_CAN_COL]	= (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;
			nColWdth[MAEDO_COL]		= MAEMAE_COL_WIDTH;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH;		
			
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH : 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH;
			nColWdth[MAESU_COL]		= MAEMAE_COL_WIDTH;
			nColWdth[RIGHT_CAN_COL] = (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;
			
			nColWdth[MAESU_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		else									// 축소-VScroll No
		{
			nColWdth[MAEDO_RESERVE_COL] = 0;
			nColWdth[LEFT_CAN_COL]	= 0;		
			nColWdth[MAEDO_COL]		= 0;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH_SH;		
			
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH	: 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH_SH;
			nColWdth[MAESU_COL]		= 0;
			nColWdth[RIGHT_CAN_COL] = 0;
			
			nColWdth[MAESU_RESERVE_COL]	= 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		break;

	case 2:
	case 3:
		if ( m_bExpandRight == TRUE )		// 확장- VScroll Yes
		{
			nColWdth[MAEDO_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[LEFT_CAN_COL]	= (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;		
			nColWdth[MAEDO_COL]		= MAEMAE_COL_WIDTH_SB;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH_SB;		
			
			//nColWdth[HOGA_COL]		= (m_bOpenCol) ? HOGA_COL_WIDTH_SB  : HOGA_COL_WIDTH_SB+DAEBI_COL_WIDTH;
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH : 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH_SB;
			nColWdth[MAESU_COL]		= MAEMAE_COL_WIDTH_SB;
			nColWdth[RIGHT_CAN_COL] = (m_bIsShowReserveCol) ? 0 : CANCEL_COL_WIDTH;
			
			nColWdth[MAESU_RESERVE_COL]	= (m_bIsShowReserveCol) ? RESERVE_COL_WIDTH : 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		else								// 축소- VScroll Yes
		{
			nColWdth[MAEDO_RESERVE_COL]	= 0;
			nColWdth[LEFT_CAN_COL]	= 0;		
			nColWdth[MAEDO_COL]		= 0;
			nColWdth[MAEDO_JAN_COL] = JAN_COL_WIDTH_SH-9;		
			
			nColWdth[HOGA_COL]		= HOGA_COL_WIDTH;
			nColWdth[DAEBI_COL]		= (m_bOpenCol) ? DAEBI_COL_WIDTH	: 0;
			
			nColWdth[MAESU_JAN_COL] = JAN_COL_WIDTH_SH-9;
			nColWdth[MAESU_COL]		= 0;
			nColWdth[RIGHT_CAN_COL] = 0;
			
			nColWdth[MAESU_RESERVE_COL]	= 0;
			nColWdth[BONG_COL]		= BONG_COL_WIDTH;
		}
		break;
	}

	
	// column width
	for (col = 0; col < nCol; col++)
	{
		m_pGridCtrl->SetColumnWidth(col, nColWdth[col]);
	}


	// 호가, 대비 헤더라인 이미지
	if( m_bOpenCol == TRUE)
	{
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, HOGA_COL, 0);
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, DAEBI_COL, 2);
	}
	else
		m_pGridCtrl->SetItemImage(TENHOGA_ROW_FIRST, HOGA_COL, 1);

	m_pGridCtrl->Refresh();
}




// JSJ_040417 Modify
void CFloatingHogaDlg::InitGridFormat()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nRow = m_pGridCtrl->GetRowCount();

	char *szTitle[] = {
		"Stop매도", "취", "매도주문", "매도잔량", "호 가", " 등락", "매수잔량", "매수주문", "취", "Stop매수", ""
		//"Stop매도", "취", "매도주문", "매도잔량", "매도잔량", "호 가", "대비", "매수잔량", "매수잔량", "매수주문", "취", "Stop매수", ""
		};

	if (m_nThreeStateDaebiType == e_DAEBI_PER)
		szTitle[DAEBI_COL] = "등락율";
	else if (m_nThreeStateDaebiType == e_PROFIT_PER)
		szTitle[DAEBI_COL] = " 수익";


	// 주문예약칼럼 Show/Hide시 취소칼럼 폭 변경으로 인한 타이틀 변경
	if (!m_bIsShowReserveCol)
		szTitle[LEFT_CAN_COL] = szTitle[RIGHT_CAN_COL] = "취소";

	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)		// JSJ_Modify
		{ 

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;
			
			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//|DT_END_ELLIPSIS;

			// Text
			Item.strText = "";

			if( row == TENHOGA_ROW_FIRST )
			{
				Item.strText = szTitle[col];

				if (col == HOGA_COL)
				{
					Item.iImage = 0;
					Item.mask  |= (GVIF_IMAGE);
				}
				
				if (col == DAEBI_COL)
					Item.nFormat |= DT_LEFT;
				else
					Item.nFormat |= DT_CENTER;
			}
			else
			{
				if( col == HOGA_COL )					
				{
					Item.nFormat |= DT_CENTER;

					// 기타호가는 셀단위 편집 가능하게(10단계호가)___JSJ_ADD_050126
//					if( row == ETC_HOGAUP_ROW || row == ETC_HOGADOWN_ROW)
//						Item.nState |= GVIS_CELLEDITABLE;
				}
				else if (col == DAEBI_COL)
				{
					Item.nFormat |= DT_RIGHT;
				}
				else if( col == MAEDO_JAN_COL || col == MAESU_JAN_COL )
				{
					Item.nFormat |= DT_RIGHT;
				}
				else if( col == LEFT_CAN_COL || col == RIGHT_CAN_COL )	
				{
//					if( m_pGridCtrl->InsideStaticLeft(row, col) )
//					{
//						Item.nState = GVIS_MODIFIED ;
//					}
//					else if( m_pGridCtrl->InsideStaticRight(row, col) )
//					{
//						Item.nState = GVIS_MODIFIED ;
//					}
//					
//					Item.nFormat |= DT_CENTER;
				}
				else
				{
					if( col == MAEDO_COL || col == MAESU_COL || col == MAEDO_RESERVE_COL || col == MAESU_RESERVE_COL )
						Item.nState = GVIS_MODIFIED ;
					
					Item.nFormat |= DT_CENTER;
				}

				// Bold Font ------------------------------------>>
//				if ( row <= m_nLowLimitRow && col == HOGA_COL)
//					Item.nState |= GVIS_BOLDFONT;
				// <<----------------------------------------------
			}

			
			// Color
			if(row == TENHOGA_ROW_FIRST)
			{
				Item.crBkClr = COLOR_GRIDHEADER;
				if (col == BONG_COL)
					Item.crBkClr = COLOR_WHITE;

				Item.crFgClr = COLOR_BLACK;
			}
			else if(row == HIGH_ROW)
			{
				if (col == HOGA_COL)
				{
					Item.crBkClr = COLOR_RED;
					Item.crFgClr = COLOR_WHITE;
				}
				else if (col == MAEDO_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAESU_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAEDO_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else
				{
					Item.crBkClr = COLOR_WHITE;
					Item.crFgClr = COLOR_BLACK;
				}
			}
			else if(row == m_nLowLimitRow)
			{
				if (col == HOGA_COL)
				{
					Item.crBkClr = COLOR_BLUE;
					Item.crFgClr = COLOR_WHITE;
				}
				else if (col == MAEDO_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAESU_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAEDO_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else
				{
					Item.crBkClr = COLOR_WHITE;
					Item.crFgClr = COLOR_BLACK;
				}
			}
			else if(row >= m_n5HogaSumRow )
			{
				Item.crBkClr = COLOR_TOTSUM_ROW;
				Item.crFgClr = COLOR_BLACK;
			}
			else
			{
				if (col == MAEDO_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAEDO_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_COL)
				{
					Item.crBkClr = COLOR_STATIC_MAESU_BG;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAEDO_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAEDO_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else if (col == MAESU_RESERVE_COL)
				{
					Item.crBkClr = COLOR_MAESU_RESERVE_COL;
					Item.crFgClr = COLOR_BLACK;
				}
				else
				{
					//{{ 2007.06.26 by LYH 예상쳬결 적용
					if(col == MAESU_JAN_COL && (row == PRICE_ROW-5 || row == PRICE_ROW-2))//예상체결가, 체결량 타이틀
						Item.crBkClr = COLOR_GRIDHEADER;
					else
						Item.crBkClr = COLOR_WHITE;
					//}}
					Item.crFgClr = COLOR_BLACK;
				}
			}
			
			// Cancel Column Color
			if ( row > TENHOGA_ROW_FIRST && (col == LEFT_CAN_COL || col == RIGHT_CAN_COL) )
			{
				Item.crBkClr = COLOR_CANCEL_COL;
				Item.crFgClr = COLOR_BLACK;
			}


			// SetItem
			m_pGridCtrl->SetItem(&Item);
		}
	}

	// 하단정보(5호가잔량, 총잔량, 시간외잔량)
	m_pGridCtrl->SetItemText(m_n5HogaSumRow, HOGA_COL, "5호가잔량");
	m_pGridCtrl->RedrawCell(m_n5HogaSumRow, HOGA_COL);
	m_pGridCtrl->SetItemText(m_nTotalSumRow, HOGA_COL, "총잔량");
	m_pGridCtrl->RedrawCell(m_nTotalSumRow, HOGA_COL);
	m_pGridCtrl->SetItemText(m_nExtraVolRow, HOGA_COL, "시간외");
	m_pGridCtrl->RedrawCell(m_nExtraVolRow, HOGA_COL);
}


void CFloatingHogaDlg::SendTrHoga()
{
	HogaMemoryFree();	
}

void CFloatingHogaDlg::RecvHogaData(int nLength , char* pData)
{
	int nLen = nLength;
	
	DisplayClear_AllHogaData();					// 호가데이터를 그리드에서 삭제

	if (m_bIsTenHoga == TRUE)
	{
		SetHogaData(pData, nLen);				// 호가데이터를 m_HogaData에 저장		
		
	}
}	

void CFloatingHogaDlg::DisplayClear_AllHogaData()
{
	int i;
	int nState;

	for( i=HIGH_ROW; i <= m_nLowLimitRow; i++ )
	{	
		m_pGridCtrl->SetItemText(i, HOGA_COL, "");
		m_pGridCtrl->SetItemText(i, MAEDO_JAN_COL, "");
		m_pGridCtrl->SetItemText(i, MAESU_JAN_COL, "");

		nState = m_pGridCtrl->GetItemState(i, HOGA_COL);
		nState &= ~GVIS_STATIC;
		m_pGridCtrl->SetItemState(i, HOGA_COL, nState);

		m_pGridCtrl->RedrawCell(i, HOGA_COL);
		m_pGridCtrl->RedrawCell(i, MAEDO_JAN_COL);
		m_pGridCtrl->RedrawCell(i, MAESU_JAN_COL);
	}
	for( i=m_nLowLimitRow; i <= m_nLowLimitRow+3; i++ )
	{
		m_pGridCtrl->SetItemText(i, MAEDO_JAN_COL, "");
		m_pGridCtrl->SetItemText(i, MAESU_JAN_COL, "");
		m_pGridCtrl->RedrawCell(i, MAEDO_JAN_COL);
		m_pGridCtrl->RedrawCell(i, MAESU_JAN_COL);
	}
}

// JSJ_ITCastle_Modify_040429
void CFloatingHogaDlg::SetHogaData(char *pData, int nSize, BOOL bGetBasicData/*=FALSE*/)
{
	//<<20100308_JS.Kim 솔로몬
	/*
	if(nSize <1 )
	{
		DisplayClear_AllHogaData();
		return;
	}
	CEU_s0001 eu_s0001;
	eu_s0001.Convert((EU_s0001_OutRec1*)pData);

	CString		sTemp, sTemp1, sTemp2, strText;
	int			nTemp1, n5HogaMaesuJan, n5HogaMaedoJan;
	int			nRow;
	int			nMask;
	COLORREF	crFgClr;
	

	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/5/12
	// Reason		: InfoStatic 추가 
	//			-. 상태정보(시,고,저,거래량 등)
	//-----------------------------------------------------------------------------
	// start
	CString strExpRate , strSHigh , strSLow , strSPreVol , strSVolPer;	
	double	fPreVol=0., fCurVol=0., fPercent=0.;
	// end


	nTemp1 = n5HogaMaesuJan = n5HogaMaedoJan = 0;

	m_strOldCode = m_strCode;	//기존것은 저장.

	//받은 종목 저장
	m_strCode = eu_s0001.m_szshcode;

	// 기준가 저장
	sTemp1 = m_Util.GetComma(eu_s0001.m_szrecprice);
	m_strPreClosePrice = sTemp1;

	// 현재가 저장
	sTemp1 = m_Util.GetComma(eu_s0001.m_szprice);
	m_strCurPrice = sTemp1;
	
	// 상한가
	nRow = HIGH_ROW;	
	m_nHighLimit = atoi(eu_s0001.m_szuplmtprice);
	strText = m_Util.GetComma(eu_s0001.m_szuplmtprice);
	crFgClr = COLOR_WHITE;
	nMask = GVIF_FGCLR;	
	CompareNUpdateHogaData(nRow, HOGA_COL, strText, nMask, crFgClr);

	if (m_bIsTenHoga == TRUE )	strText = "상한";
	else						strText = "";
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);

	// 하한가
	nRow	= m_nLowLimitRow;
	m_nLowLimit = atoi(eu_s0001.m_szdnlmtprice);
	strText = m_Util.GetComma(eu_s0001.m_szdnlmtprice);
	crFgClr = COLOR_WHITE;
	nMask = GVIF_FGCLR;	
	CompareNUpdateHogaData(nRow, HOGA_COL, strText, nMask, crFgClr);

	if (m_bIsTenHoga == TRUE )	strText = "하한";
	else						strText = "";
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	// 상한가, 하한가 등 기본정보만 얻은후 리턴
	if ( bGetBasicData == TRUE)
		return;
	
	//////////////////////////////////////////////////////////////////////////////////
	// 매도 호가 기준열
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;


	// 매도, 매수 호가데이터 초기화
	memset(&m_10HogaData, 0x00, TEN_HOGADATA_SIZE);	

	m_nHogaRow += 5;	
	// 매도 호가/잔량 5차
	m_10HogaData.nOfferho5 = atoi(eu_s0001.m_szofferho5);
	n5HogaMaedoJan += atoi(eu_s0001.m_szofferrem5);
	SetHogaData_Sub(eu_s0001.m_szofferho5, eu_s0001.m_szofferrem5, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 4차
	m_10HogaData.nOfferho4 = atoi(eu_s0001.m_szofferho4);
	n5HogaMaedoJan += atoi(eu_s0001.m_szofferrem4);
	SetHogaData_Sub(eu_s0001.m_szofferho4, eu_s0001.m_szofferrem4, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 3차
	m_10HogaData.nOfferho3 = atoi(eu_s0001.m_szofferho3);
	n5HogaMaedoJan += atoi(eu_s0001.m_szofferrem3);
	SetHogaData_Sub(eu_s0001.m_szofferho3, eu_s0001.m_szofferrem3, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 2차
	m_10HogaData.nOfferho2 = atoi(eu_s0001.m_szofferho2);
	n5HogaMaedoJan += atoi(eu_s0001.m_szofferrem2);
	SetHogaData_Sub(eu_s0001.m_szofferho2, eu_s0001.m_szofferrem2, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 1차
	m_10HogaData.nOfferho1 = atoi(eu_s0001.m_szofferho1);
	n5HogaMaedoJan += atoi(eu_s0001.m_szofferrem1);
	SetHogaData_Sub(eu_s0001.m_szofferho1, eu_s0001.m_szofferrem1, m_strPreClosePrice, TRUE);

	//////////////////////////////////////////////////////////////////////////////////
	// 매수 호가 기준열
	//////////////////////////////////////////////////////////////////////////////////   
	m_nHogaRow = MAESU_ROW;
	
	// 매수 호가/잔량 1차
	m_10HogaData.nBidho1 = atoi(eu_s0001.m_szbidho1);
	n5HogaMaesuJan += atoi(eu_s0001.m_szbidrem1);
	SetHogaData_Sub(eu_s0001.m_szbidho1, eu_s0001.m_szbidrem1, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 2차
	m_10HogaData.nBidho2 = atoi(eu_s0001.m_szbidho2);
	n5HogaMaesuJan += atoi(eu_s0001.m_szbidrem2);
	SetHogaData_Sub(eu_s0001.m_szbidho2, eu_s0001.m_szbidrem2, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 3차
	m_10HogaData.nBidho3 = atoi(eu_s0001.m_szbidho3);
	n5HogaMaesuJan += atoi(eu_s0001.m_szbidrem3);
	SetHogaData_Sub(eu_s0001.m_szbidho3, eu_s0001.m_szbidrem3, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 4차
	m_10HogaData.nBidho4 = atoi(eu_s0001.m_szbidho4);
	n5HogaMaesuJan += atoi(eu_s0001.m_szbidrem4);
	SetHogaData_Sub(eu_s0001.m_szbidho4, eu_s0001.m_szbidrem4, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 5차
	m_10HogaData.nBidho5 = atoi(eu_s0001.m_szbidho5);
	n5HogaMaesuJan += atoi(eu_s0001.m_szbidrem5);
	SetHogaData_Sub(eu_s0001.m_szbidho5, eu_s0001.m_szbidrem5, m_strPreClosePrice, FALSE);

	//---------------------------------------------------------------------------------------------->>
	// 5호가 매도 총 잔량
	nRow = m_n5HogaSumRow;		
	sTemp1.Format("%d", n5HogaMaedoJan);
	strText = m_Util.GetComma(sTemp1);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 5호가 매수 총 잔량
	nRow = m_n5HogaSumRow;		
	sTemp1.Format("%d", n5HogaMaesuJan);
	strText = m_Util.GetComma(sTemp1);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 매도 총 잔량
	nRow = m_nTotalSumRow;		
	strText = m_Util.GetComma(eu_s0001.m_sztotofferrem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 매수 총 잔량
	nRow = m_nTotalSumRow;		
	strText = m_Util.GetComma(eu_s0001.m_sztotbidrem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	// 시간외 매도잔량
	nRow = m_nExtraVolRow;		
	strText = m_Util.GetComma(eu_s0001.m_sztmofferrem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 시간외 매수잔량
	nRow = m_nExtraVolRow;		
	strText = m_Util.GetComma(eu_s0001.m_sztmbidrem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	// <<---------------------------------------------------------------------------------------------
	
	// 예상체결가직전현재가대비
	strExpRate.Format("%s", eu_s0001.m_szpreychange);

	// 고가
	strSHigh = m_Util.GetComma(eu_s0001.m_szhigh);
	
	// 저가
	strSLow = m_Util.GetComma(eu_s0001.m_szlow);	

	// ***** 10호가 그리드 데이터 *****
	int nHogaData[TENHOGA_SAVED_CNT] = {0,};
	if (m_bIsTenHoga == TRUE)
	{
		m_10HogaData.nHighLimit = m_nHighLimit;
		
		sTemp1 = m_pGridCtrl->GetItemText(ETC_HOGAUP_ROW, HOGA_COL);	sTemp1.Remove(',');
		m_10HogaData.nExtraUp = atoi(sTemp1);
		
		sTemp1 = m_pGridCtrl->GetItemText(m_nLowLimitRow-1, HOGA_COL);	sTemp1.Remove(',');
		m_10HogaData.nExtraDown = atoi(sTemp1);
		
		m_10HogaData.nLowLimit = m_nLowLimit;

		memcpy(nHogaData, &m_10HogaData, TEN_HOGADATA_SIZE);
	}
	//*********************************


	// ********************************************************************
	//  현재가 정보 스태틱 
	// *********************
	CString strSign, strChange, strChangeRate;
	CString strSVol, strSPrice;
	COLORREF clrChange;
	
	// 등락부호
	sTemp1.Format("%1.1s", eu_s0001.m_szsign);		sTemp1.TrimLeft();		sTemp1.TrimRight();
	strSign = eu_s0001.m_szsign;
	int nSign = atoi(eu_s0001.m_szsign);
	
	// 등락폭
	sTemp1 = eu_s0001.m_szchange;
	sTemp1.Remove('-');
	strChange = m_Util.GetComma(sTemp1);
	
	sTemp1 = m_Util.GetSignSimbol(nSign);
	strChange = sTemp1 + " " + strChange;
	
	// 등락율		
	sTemp1 = eu_s0001.m_szrate;
	sTemp1.Remove('-');
	
	sTemp1.Format("%.2f", atof(sTemp1));
	strChangeRate = sTemp1 + "%";

	// 누적거래량
	fCurVol = atoi(eu_s0001.m_szvolume);
	strSVol = m_Util.GetVolUnit(eu_s0001.m_szvolume);
	
	// 누적금액
	strSPrice =	m_Util.GetPriceUnit(eu_s0001.m_szvalue);

	// 전일거래량 
	fPreVol = atoi(eu_s0001.m_szjnilvolume);
	m_strPreVol = eu_s0001.m_szjnilvolume;
	strSPreVol = m_Util.GetVolUnit(eu_s0001.m_szjnilvolume);

	// 거래량 대비 %
	if (fCurVol == 0 || fPreVol == 0)
		strSVolPer = "";
	else
	{
		fPercent = fCurVol * 100. / fPreVol;
		sTemp1.Format("%.1f", fPercent);
		strSVolPer = m_Util.GetComma(sTemp1);
	}

	// #### 예상체결 ####
	CString		strExpPrice, strExpVol;
	CString		strPrice, strPrePrice;
	long		lChange=0, lPrice=0;
	
	m_chSign = eu_s0001.m_szsign[0];
	if(m_chSign == '9')
	{
		// 예상 체결가격
		strPrice = eu_s0001.m_szyeprice;

		strExpPrice = "*" + m_Util.GetComma(strPrice);
		// 예상 체결수량
		strExpVol = m_Util.GetComma(eu_s0001.m_szyevolume) + "주";

		strChangeRate = strExpRate + "%";

		double dExpRate = atof(strExpRate);

		if ( dExpRate > 0)
			clrChange = COLOR_RED;
		else if ( dExpRate < 0)
			clrChange = COLOR_BLUE;
		else
			clrChange = COLOR_BLACK;

		CompareNUpdateHogaData(9, MAESU_JAN_COL, "예상체결가", 0, COLOR_BLACK);
		CompareNUpdateHogaData(10, MAESU_JAN_COL, strExpPrice, GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(11, MAESU_JAN_COL, strChangeRate, GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(12, MAESU_JAN_COL, "예상체결량", 0, COLOR_BLACK);
		CompareNUpdateHogaData(13, MAESU_JAN_COL, strExpVol, 0, COLOR_BLACK);
	}
	else
	{
		clrChange = COLOR_BLACK;
		CompareNUpdateHogaData(9, MAESU_JAN_COL, "예상체결가", 0, COLOR_BLACK);
		CompareNUpdateHogaData(10, MAESU_JAN_COL, "*0", GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(11, MAESU_JAN_COL, "0.00%", GVIF_FGCLR, clrChange);
		CompareNUpdateHogaData(12, MAESU_JAN_COL, "예상체결량", 0, COLOR_BLACK);
		CompareNUpdateHogaData(13, MAESU_JAN_COL, "0주", 0, COLOR_BLACK);
	}
	// **********************************************************************


	// ****************************************************************
	// 현재가열 찾기
	// ***********************************************
	// *** 예상체결가 ***
	if (atol(strPrice) > 0)
	{
		// 예상체결가인 경우에는 종가만 그린다.
		// 시가, 고가, 저가, 종가(예상체결)
		//m_ChartData.nOpen = m_ChartData.nHigh = m_ChartData.nLow = m_ChartData.nClose = atoi(strPrice);
		//m_pGridCtrl->DrawCandleChart(&m_ChartData, &m_MapGridAllData, nHogaData);		// 캔들
	}
	// *** 현재가 ***
	else
	{
		strPrice = m_Util.GetNumber(m_strCurPrice);
	}

	if (m_bIsTenHoga == TRUE)
	{
		// 현재가열(상한가와 하한가 열은 체크 체외) ----------------->>>
		BOOL bFindRow = FALSE;
		int nPriceTemp = atoi(strPrice);

		for(int nCnt = 1; nCnt < TENHOGA_SAVED_CNT-1; nCnt++)
		{
			if (nPriceTemp ==  nHogaData[nCnt])
			{
				bFindRow = TRUE;
				m_nCurPriceRow = nCnt+HIGH_ROW;
				break;
			}
		}
		if (bFindRow == FALSE)
		{
			m_nCurPriceRow = -1;
		}
	}
	// *******************************************************************
	*/
	if(nSize <1 )
	{
		DisplayClear_AllHogaData();
		return;
	}
	HCQ01112_OUT	hcq01112;
	memcpy(&hcq01112, pData, HCQ01112_OUT_SZ);

	CString		sTemp = "", sTemp1 = "", sTemp2 = "", strText = "";
	int			nTemp1, n5HogaMaesuJan, n5HogaMaedoJan;
	int			nRow;

	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/5/12
	// Reason		: InfoStatic 추가 
	//			-. 상태정보(시,고,저,거래량 등)
	//-----------------------------------------------------------------------------
	// start
	CString strExpRate = "", strSHigh = "", strSLow = "", strSPreVol = "", strSVolPer = "";	
	double	fPreVol=0., fCurVol=0., fPercent=0.;
	// end


	nTemp1 = n5HogaMaesuJan = n5HogaMaedoJan = 0;

	m_strOldCode = m_strCode;	//기존것은 저장.

	//받은 종목 저장
	m_strCode = CString(hcq01112.jmcode, sizeof(hcq01112.jmcode));
	m_strCode.TrimRight();

// --> [Edit]  강지원 2008/10/23		( 품목의 정보를 얻어온다. )
	//<<20100308_JS.Kim 솔로몬 
	/*
	m_nLog			= 1;
	m_nLogDispSize	= 0;
	m_nPrecision	= 0;
	m_nTickSize		= 1;
	m_nValidPoint	= 0;
	m_nMask			= 1;

	GetCodeConvInfo( m_strCode, m_nLog, m_nLogDispSize, m_nPrecision, m_nValidPoint, m_nMask, m_nTickSize);
	*/
	//>>
// <-- [Edit]  강지원 2008/10/23

	// 기준가 저장
	sTemp1 = CString(hcq01112.jsprice, sizeof(hcq01112.jsprice));
	m_strPreClosePrice = m_Util.GetComma(sTemp1);
//	m_strPreClosePrice = sTemp1;

	// 현재가 저장
	sTemp1 = CString(hcq01112.price, sizeof(hcq01112.price));
	m_strCurPrice = m_Util.GetComma(sTemp1);

	//////////////////////////////////////////////////////////////////////////////////
	// 매도 호가 기준열
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;


	// 매도, 매수 호가데이터 초기화
	memset(&m_10HogaData, 0x00, TEN_HOGADATA_SIZE);	

	m_nHogaRow += 5;	
	// 매도 호가/잔량 5차
	sTemp = CString(hcq01112.mdhoga5, sizeof(hcq01112.mdhoga5));
	m_10HogaData.nOfferho5 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem5, sizeof(hcq01112.mdhorem5));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 4차
	sTemp = CString(hcq01112.mdhoga4, sizeof(hcq01112.mdhoga4));
	m_10HogaData.nOfferho4 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem4, sizeof(hcq01112.mdhorem4));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 3차
	sTemp = CString(hcq01112.mdhoga3, sizeof(hcq01112.mdhoga3));
	m_10HogaData.nOfferho3 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem3, sizeof(hcq01112.mdhorem3));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 2차
	sTemp = CString(hcq01112.mdhoga2, sizeof(hcq01112.mdhoga2));
	m_10HogaData.nOfferho2 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem2, sizeof(hcq01112.mdhorem2));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	// 매도 호가/잔량 1차
	sTemp = CString(hcq01112.mdhoga1, sizeof(hcq01112.mdhoga1));
	m_10HogaData.nOfferho1 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mdhorem1, sizeof(hcq01112.mdhorem1));
	n5HogaMaedoJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, TRUE);

	//////////////////////////////////////////////////////////////////////////////////
	// 매수 호가 기준열
	//////////////////////////////////////////////////////////////////////////////////   
	m_nHogaRow = MAESU_ROW;
	
	// 매수 호가/잔량 1차
	sTemp = CString(hcq01112.mshoga1, sizeof(hcq01112.mshoga1));
	m_10HogaData.nBidho1 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem1, sizeof(hcq01112.mshorem1));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 2차
	sTemp = CString(hcq01112.mshoga2, sizeof(hcq01112.mshoga2));
	m_10HogaData.nBidho2 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem2, sizeof(hcq01112.mshorem2));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 3차
	sTemp = CString(hcq01112.mshoga3, sizeof(hcq01112.mshoga3));
	m_10HogaData.nBidho3 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem3, sizeof(hcq01112.mshorem3));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 4차
	sTemp = CString(hcq01112.mshoga4, sizeof(hcq01112.mshoga4));
	m_10HogaData.nBidho4 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem4, sizeof(hcq01112.mshorem4));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	// 매수 호가/잔량 5차
	sTemp = CString(hcq01112.mshoga5, sizeof(hcq01112.mshoga5));
	m_10HogaData.nBidho5 = atoi(sTemp);
	sTemp1 = CString(hcq01112.mshorem5, sizeof(hcq01112.mshorem5));
	n5HogaMaesuJan += atoi(sTemp1);
	SetHogaData_Sub(sTemp, sTemp1, m_strPreClosePrice, FALSE);

	//---------------------------------------------------------------------------------------------->>
	// 5호가 매도 총 잔량
	nRow = m_n5HogaSumRow;		
	sTemp1.Format("%d", n5HogaMaedoJan);
	strText = m_Util.GetComma(sTemp1);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 5호가 매수 총 잔량
	nRow = m_n5HogaSumRow;		
	sTemp1.Format("%d", n5HogaMaesuJan);
	strText = m_Util.GetComma(sTemp1);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 매도 총 잔량
	nRow = m_nTotalSumRow;		
	sTemp = CString(hcq01112.totmdhorem, sizeof(hcq01112.totmdhorem));
	strText = m_Util.GetComma(sTemp);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	
	// 매수 총 잔량
	nRow = m_nTotalSumRow;		
	sTemp = CString(hcq01112.totmshorem, sizeof(hcq01112.totmshorem));
	strText = m_Util.GetComma(sTemp);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	// 고가
	sTemp = CString(hcq01112.high, sizeof(hcq01112.high));
	strSHigh = m_Util.GetComma(sTemp);
	
	// 저가
	sTemp = CString(hcq01112.low, sizeof(hcq01112.low));
	strSLow = m_Util.GetComma(sTemp);	

	// ***** 10호가 그리드 데이터 *****
	int nHogaData[TENHOGA_SAVED_CNT] = {0,};
	if (m_bIsTenHoga == TRUE)
	{
		m_10HogaData.nHighLimit = m_nHighLimit;
		
		sTemp1 = m_pGridCtrl->GetItemText(ETC_HOGAUP_ROW, HOGA_COL);	sTemp1.Remove(',');
		m_10HogaData.nExtraUp = atoi(sTemp1);
		
		sTemp1 = m_pGridCtrl->GetItemText(m_nLowLimitRow-1, HOGA_COL);	sTemp1.Remove(',');
		m_10HogaData.nExtraDown = atoi(sTemp1);
		
		m_10HogaData.nLowLimit = m_nLowLimit;

		memcpy(nHogaData, &m_10HogaData, TEN_HOGADATA_SIZE);
	}
	//*********************************


	// ********************************************************************
	//  현재가 정보 스태틱 
	// *********************
	CString strSign = "", strChange = "", strChangeRate = "";
	CString strSVol = "", strSPrice = "";
//	COLORREF clrChange;
	
	// 등락부호
	strSign = CString(hcq01112.sign, sizeof(hcq01112.sign));
//	sTemp1.Format("%1.1s", sTemp);		sTemp1.TrimLeft();		sTemp1.TrimRight();
//	strSign = hcq01112.m_szsign;
	int nSign = 5; //atoi(hcq01112.m_szsign);
	if ( strSign == "+" )
		nSign = 6;
	else if ( strSign == "-" )
		nSign = 4;
	else
		nSign = 5;
	
	// 등락폭
	sTemp1 = CString(hcq01112.change, sizeof(hcq01112.change));
	sTemp1.Remove('-');
	strChange = m_Util.GetComma(sTemp1);

	sTemp1 = m_Util.GetSignSimbol(nSign);
	strChange = sTemp1 + " " + strChange;
	
	// 등락율		
	sTemp1 = CString(hcq01112.drate, sizeof(hcq01112.drate));
	sTemp1.Remove('-');
//	sTemp1.Format("%.2f", atof(sTemp1));
	strChangeRate = sTemp1 + "%";

	// 누적거래량
	sTemp1 = CString(hcq01112.volume, sizeof(hcq01112.volume));
	fCurVol = atoi(sTemp1);
	strSVol = m_Util.GetVolUnit(sTemp1);

	CString strPrice = m_Util.GetNumber(m_strCurPrice);
// <-- [Edit]  강지원 2008/10/23
	if (m_bIsTenHoga == TRUE)
	{
		// 현재가열(상한가와 하한가 열은 체크 체외) ----------------->>>
		BOOL bFindRow = FALSE;
		int nPriceTemp = atoi(strPrice);

		for(int nCnt = 1; nCnt < TENHOGA_SAVED_CNT-1; nCnt++)
		{
			if (nPriceTemp ==  nHogaData[nCnt])
			{
				bFindRow = TRUE;
				m_nCurPriceRow = nCnt+HIGH_ROW;
				break;
			}
		}
		if (bFindRow == FALSE)
		{
			m_nCurPriceRow = -1;
		}
	}
	// *******************************************************************
	//>>
}

void CFloatingHogaDlg::SetHogaData_Sub(CString strHoga, CString strNumber, CString strPreClosePrice, BOOL bMaeDo)
{
	// -------------------------------------------------------------------------->>	
	// *** 전체호가인 경우 해당호가의 Row 구함 ***
	m_nHogaRangeForColor++;

	// 배경 색상
	COLORREF clrHoga;
	clrHoga = GetHogaGridBkColor(m_nHogaRangeForColor);


	if (m_nHogaRangeForColor >= 1 && m_nHogaRangeForColor <= 5)
	{
		m_pGridCtrl->SetItemBkColour(m_nHogaRow, HOGA_COL, clrHoga);
		m_pGridCtrl->RedrawCell(m_nHogaRow, HOGA_COL);
		m_pGridCtrl->SetItemBkColour(m_nHogaRow, DAEBI_COL, clrHoga);
		m_pGridCtrl->RedrawCell(m_nHogaRow, DAEBI_COL);
		m_pGridCtrl->SetItemBkColour(m_nHogaRow, MAEDO_JAN_COL, clrHoga);
		m_pGridCtrl->RedrawCell(m_nHogaRow, MAEDO_JAN_COL);
	}
	else
	{
		m_pGridCtrl->SetItemBkColour(m_nHogaRow, HOGA_COL, clrHoga);
		m_pGridCtrl->RedrawCell(m_nHogaRow, HOGA_COL);
		m_pGridCtrl->SetItemBkColour(m_nHogaRow, DAEBI_COL, clrHoga);
		m_pGridCtrl->RedrawCell(m_nHogaRow, DAEBI_COL);
		m_pGridCtrl->SetItemBkColour(m_nHogaRow, MAESU_JAN_COL, clrHoga);
		m_pGridCtrl->RedrawCell(m_nHogaRow, MAESU_JAN_COL);
	}
	// <<-----------------------------------------------------------------------------

	CString  sTemp1;
	CHogaData *pHogaData;

	// 호가 텍스트
	pHogaData = new CHogaData;
	pHogaData->m_nRow	 = m_nHogaRow;		
	pHogaData->m_nCol	 = HOGA_COL;
	pHogaData->m_strText = m_Util.GetComma(strHoga);


	// 호가 색상
	if (m_nHogaRow == HIGH_ROW || m_nHogaRow == m_nLowLimitRow)
	{
		pHogaData->m_crFgClr = COLOR_WHITE;
	}
	else 
	{
		int lTemp1, lTemp2;
		lTemp1 = atol(strHoga);
		CString sTemp;
		sTemp = m_Util.GetNumber(strPreClosePrice);
		lTemp2 = atol(sTemp);
		
		if( lTemp1 > lTemp2 )			
			pHogaData->m_crFgClr = COLOR_RED;		// 빨강
		else if( lTemp1 < lTemp2 )		
			pHogaData->m_crFgClr = COLOR_BLUE;		// 파랑
		else
			pHogaData->m_crFgClr = COLOR_BLACK;
	}

	pHogaData->m_nMask = GVIF_FGCLR;	
	m_HogaData.Add(pHogaData);				// 호가 추가	
	CompareNUpdateHogaData(pHogaData->m_nRow, HOGA_COL, pHogaData->m_strText, GVIF_FGCLR, pHogaData->m_crFgClr);
	
	// 잔량
	CString strText;
	int nCol = 0;
	nCol = (bMaeDo == TRUE) ? MAEDO_JAN_COL : MAESU_JAN_COL;
	sTemp1 = m_Util.GetComma(strNumber);
	strText = sTemp1;
	CompareNUpdateHogaData(m_nHogaRow, nCol, strText, GVIF_FGCLR, COLOR_BLACK);


	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/5/12
	// Reason		: 체결 리스트 추가 함에 따라 변동잔량 표시하지 않음
	//			-. 추후에 추가 할수 있으면 추가한다 ( 컬럼을 새로 나눠서 )
	//-----------------------------------------------------------------------------
	// start
	// 변동잔량
	//nCol = (bMaeDo == TRUE) ? MAESU_JAN_COL : MAEDO_JAN_COL;
	//strText = "";
	//CompareNUpdateHogaData(m_nHogaRow, nCol, strText, GVIF_FGCLR, COLOR_BLACK);
	// end

	m_nHogaRow++;
}

void CFloatingHogaDlg::CompareNUpdateHogaData(int nRow, int nCol, CString strText, int nMask, COLORREF crFgClr)
{
	CGridCellBase	*pCell = NULL;
	CString			strCellText;
	COLORREF		crFgColorTemp;
	
	// 현재 셀의 정보를 가져온다.
	pCell = m_pGridCtrl->GetCell(nRow, nCol);
	if( pCell == NULL )
		return;

	// 현재 셀과 들어온 셀을 비교해서 데이터가 같으면 되돌아간다.
	strCellText		= pCell->GetText();
	crFgColorTemp	= pCell->GetTextClr();
	if( strCellText == strText && crFgColorTemp == crFgClr)	// 셀 내용과 색상이 동일하면 리턴 JSJ_Modify
		return;
	
	if( strText == "0" )
	{
		strText = "";
	}
	
	// 텍스트 변경
	m_pGridCtrl->SetItemText(nRow, nCol, strText);

	// 색상 마스킹 되어 있으면 색상 변경
	if( nMask & GVIF_FGCLR )
		m_pGridCtrl->SetItemFgColour(nRow, nCol, crFgClr);

	m_pGridCtrl->RedrawCell(nRow, nCol);		
}

void CFloatingHogaDlg::HogaMemoryFree()
{
	int i=0; 
	int nCount = m_HogaData.GetSize();
	CObject *pObj=NULL;
	for(i=0; i<nCount; i++)
	{
		pObj = (CObject *) m_HogaData.GetAt(i);
		delete pObj;
	}

	m_HogaData.RemoveAll();
}

COLORREF CFloatingHogaDlg::GetHogaGridBkColor(int nHogaRange)
{
	COLORREF clrHoga;
	if ( nHogaRange == 1 )
		clrHoga = COLOR_MAEDO_10HOGA_6;
	else if ( nHogaRange == 2 )
		clrHoga = COLOR_MAEDO_10HOGA_7;
	else if ( nHogaRange == 3 )
		clrHoga = COLOR_MAEDO_10HOGA_8;
	else if ( nHogaRange == 4 )
		clrHoga = COLOR_MAEDO_10HOGA_9;
	else if ( nHogaRange == 5 )
		clrHoga = COLOR_MAEDO_10HOGA_10;
	else if ( nHogaRange == 6 )
		clrHoga = COLOR_MAESU_10HOGA_1;
	else if ( nHogaRange == 7)
		clrHoga = COLOR_MAESU_10HOGA_2;
	else if ( nHogaRange == 8 )
		clrHoga = COLOR_MAESU_10HOGA_3;
	else if ( nHogaRange == 9 )
		clrHoga = COLOR_MAESU_10HOGA_4;
	else if ( nHogaRange == 10 )
		clrHoga = COLOR_MAESU_10HOGA_5;

	return clrHoga;
}


void CFloatingHogaDlg::SetRealHogaData_Sub(CString strHoga, /*CString strNumber, CString strChangeVol,*/ CString strPreClosePrice, BOOL bMaeDo)
{	
	CString		sTemp1, strText;
	COLORREF	crFgClr;
	int			nMask;
	int			nCol;

	// 호가 텍스트
	nCol	= HOGA_COL;
	strText = m_Util.GetComma(strHoga);

	// 호가 색상
	if (m_nHogaRow == HIGH_ROW || m_nHogaRow == m_nLowLimitRow)
	{
		crFgClr = COLOR_WHITE;
	}
	else
	{
		int lTemp1, lTemp2;
		lTemp1 = atol(strHoga);
		CString sTemp;
		sTemp = m_Util.GetNumber(strPreClosePrice);
		lTemp2 = atol(sTemp);
		
		if( lTemp1 > lTemp2 )			
			crFgClr = COLOR_RED;		// 빨강
		else if( lTemp1 < lTemp2 )		
			crFgClr = COLOR_BLUE;		// 파랑
		else
			crFgClr = COLOR_BLACK;
	}

	nMask = GVIF_FGCLR;	
	CompareNUpdateHogaData(m_nHogaRow, nCol, strText, nMask, crFgClr);
	
	// 데이터를 변경 여부를 체크 한 후에 변경되었으면 호가 데이터를 비교해서 
	// m_HogaDataList에 호가 데이터를 추가한다.
	if( nCol == HOGA_COL && atol(strHoga) != 0 )
	{
		if( strText == "" )
		{
			m_nHogaRow++;
			return;
		}
		
		int nIndex = -1;
		int nRet = CheckRealHogaDataWithinHogaData(strText, &nIndex);

		if( nRet == BIG_HOGA || nRet == SMALL_HOGA )
		{
			// 호가 텍스트
			CHogaData *pHogaData;
			pHogaData = new CHogaData;
			pHogaData->m_nRow = m_nHogaRow;		
			pHogaData->m_nCol = HOGA_COL;
			pHogaData->m_strText = strText;
			
			// 호가 색상
			pHogaData->m_crFgClr = crFgClr;
			pHogaData->m_nMask = GVIF_FGCLR;
			
			// 호가 추가
			if( nRet == BIG_HOGA )
				m_HogaData.InsertAt(0, pHogaData);
			else if( nRet == SMALL_HOGA )
				m_HogaData.Add(pHogaData);
		}	
		else if( nRet == WITHIN_HOGA && nIndex != -1 )
		{
			// 호가 텍스트
			CHogaData *pHogaData;
			pHogaData = new CHogaData;
			pHogaData->m_nRow = m_nHogaRow;		
			pHogaData->m_nCol = HOGA_COL;
			pHogaData->m_strText = strText;
			
			// 호가 색상
			pHogaData->m_crFgClr = crFgClr;
			pHogaData->m_nMask = GVIF_FGCLR;
			
			// 호가 추가
			m_HogaData.InsertAt(nIndex, pHogaData);	
		}
	}
	
//	m_nHogaRow++;
}

int CFloatingHogaDlg::CheckRealHogaDataWithinHogaData(CString strData, int *nIndex)
{
	int nCount = m_HogaData.GetSize();
	int nHogaTop, nHogaBottom, nHogaPrice;

	CHogaData *pHogaDataTop = NULL;
	CHogaData *pHogaDataBottom = NULL;

	if( m_HogaData.GetSize() <= 0 )
		return NOT_FOUND;

	// 제일 상단의 호가를 구한다.
	pHogaDataTop = m_HogaData.GetAt(0);
	if( pHogaDataTop == NULL )
		return NOT_FOUND;

	// 제일 하단의 호가를 구한다
	pHogaDataBottom = m_HogaData.GetAt(nCount-1);
	if( pHogaDataBottom == NULL )
		return NOT_FOUND;
		
	nHogaTop = atoi(m_Util.GetNumber(pHogaDataTop->m_strText));
	if( nHogaTop == 0 )
		nHogaTop = atoi(m_Util.GetNumber(m_pGridCtrl->GetItemText(HIGH_ROW, HOGA_COL)));
	
	nHogaBottom = atoi(m_Util.GetNumber(pHogaDataBottom->m_strText));
	if( nHogaBottom == 0 )
		nHogaBottom = atoi(m_Util.GetNumber(m_pGridCtrl->GetItemText(m_nLowLimitRow, HOGA_COL)));

	nHogaPrice = atoi(m_Util.GetNumber(strData));

	// 호가 데이터와 비교
	if( nHogaTop < nHogaPrice )				// 매도 10단 호가와 비교
	{
		return BIG_HOGA;
	}
	else if( nHogaBottom > nHogaPrice )		// 매수 10단 호가와 비교
	{
		return SMALL_HOGA;
	}
	
	int i;	
	CHogaData *pHogaData = NULL;
	BOOL bFind = FALSE;
	for(i=0; i<nCount; i++)
	{
		pHogaData = m_HogaData.GetAt(i);
		if( pHogaData == NULL )
			continue;

		if( pHogaData->m_strText == strData )
		{
			pHogaData->m_nRow = m_nHogaRow;
			return EQUAL_HOGA;			
		}				
		else 
		{
			if( bFind == FALSE )
			{
				if( atol(m_Util.GetOnlyNumber(pHogaData->m_strText)) < atol(m_Util.GetOnlyNumber(strData)))
				{
					*nIndex = i;
					bFind = TRUE;
				}
			}
		}
	}

	return WITHIN_HOGA;
}



void CFloatingHogaDlg::OnDestroy() 
{
	MakeDrdsUnConnectUnAdvise();
	
	GetParent()->SendMessage(UMSG_DISCONNECT_HOGA_REAL, 0, 0);
	m_ImageListOrder.DeleteImageList();
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	HogaMemoryFree();	
	if(m_pGridCtrl)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	} 
	CRscDlg::OnDestroy();	
}

BOOL CFloatingHogaDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message) 
	{
		case WM_KEYDOWN	:
			{
				UINT nChar = (UINT)pMsg->wParam;
				switch(nChar)
				{
					case VK_RETURN :
					case VK_ESCAPE :
					{
						return TRUE;
						break;					
					}
				}
				break;
			}
	}
	return CRscDlg::PreTranslateMessage(pMsg);
}

BOOL CFloatingHogaDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{	
	static UINT msg = 512;
    if(msg == 513 && message == 512)
    {
        CPoint p;
        GetCursorPos(&p);
        ScreenToClient(&p);
        SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(p.x, p.y));
    }
    msg = message;

	return CRscDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CFloatingHogaDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, GetSysColor(COLOR_WINDOW));
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CFloatingHogaDlg::SetMarkRealCurPrice(char *pData, int nSize)
{

}

void CFloatingHogaDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	CRect dlgRect(0,0,0,0);

	m_staticSymbol.GetWindowRect(dlgRect);//윈도우 좌표...
	m_bIconState = !m_bIconState;
		
	if(m_pMainDlg)
		m_pMainDlg->SendMessage(USMG_PLEASE_RESIZE, m_bIconState ? 1 : 0, (LPARAM)&dlgRect);

}

void CFloatingHogaDlg::SetRealHogaData_Rem(CString strNumber, BOOL bMaeDo)
{
	CString		sTemp1;
	int			nCol;

	// 잔량
	nCol = (bMaeDo == TRUE) ? MAEDO_JAN_COL : MAESU_JAN_COL;
	sTemp1 = m_Util.GetComma(strNumber);
	CompareNUpdateHogaData(m_nHogaRow, nCol, sTemp1, GVIF_FGCLR, COLOR_BLACK);
}

void CFloatingHogaDlg::OnButtonBuy() 
{
	AfxMessageBox("매수화면 띄움");
}

void CFloatingHogaDlg::OnButtonSell() 
{
	AfxMessageBox("매도화면 띄움");
}

BOOL CFloatingHogaDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//-->
BOOL CFloatingHogaDlg::MakeDrdsConnectAdvise()
{
	//<<20100308_JS.Kim 솔로몬
	//IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 2);
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	//>>
	if(!pDrdsMng) return FALSE;

	m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	if(!m_pDrdsLib) return FALSE;

	m_pDrdsLib->DrdsInit(m_hWnd);
	int nRealType = 1;	// 1:호가
	m_pDrdsLib->DrdsAdvise(nRealType, "KXH1", "*", "");

	return TRUE;
}

BOOL CFloatingHogaDlg::MakeDrdsUnConnectUnAdvise()
{
	if(!m_pDrdsLib) return FALSE;

	//<<20100308_JS.Kim 솔로몬
	//IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 2);
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	//>>
	if(!pDrdsMng) return FALSE;

	//<<20100308_JS.Kim 솔로몬
	/*
	int nRealType = 1;	// 1:호가
	m_pDrdsLib->DrdsUnadvise(nRealType, "KXH1", "*", "");
// --> [Edit]  강지원 2008/12/23	( 리얼 해제 )
	m_pDrdsLib->DrdsUnadviseAll(m_hWnd);
// <-- [Edit]  강지원 2008/12/23

	pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
	*/
	//>>
	m_pDrdsLib = NULL;

	return TRUE;
}

void CFloatingHogaDlg::OnCodeAdvise(LPCSTR strCode)
{
	if(!m_pDrdsLib) return;

	int nRealType = 1;	// 1:호가
	m_pDrdsLib->DrdsAdvise(nRealType, "KXH1", "*", (LPSTR)strCode);
}

void CFloatingHogaDlg::OnCodeUnAdvise(LPCSTR strCode)
{
	if(!m_pDrdsLib) return;

	int nRealType = 1;	// 1:호가
	m_pDrdsLib->DrdsUnadvise(nRealType, "KXH1", "*", (LPSTR)strCode);
}

//<<20100308_JS.Kim 솔로몬
//#include "../../../Common/CommStructDef.h"
//#include "../CommonTR/EUREAL_KXH1.h"
//>>
long CFloatingHogaDlg::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	if(!m_pDrdsLib) return 0;

	int nGetRealType = -1;
	char* pRealStruct = NULL;
	m_pDrdsLib->GetRealStructData(wParam, lParam, pRealStruct, nGetRealType);
	if(!pRealStruct) return 0;

	int nRealType = 1;	// 1:호가
	if( nGetRealType == nRealType)			// 종목현재가
	{
//		stEUREAL_KXH1 *pEuRealKXH1 = (stEUREAL_KXH1*)pRealStruct;
		SetRealHogaData(pRealStruct);
		return 1L;
	}

	return 0;
}

//<<20100308_JS.Kim 솔로몬
//void CFloatingHogaDlg::SetRealHogaData(char* pstEUREAL_KXH1)
void CFloatingHogaDlg::SetRealHogaData(char* pstREAL_CAH1)
//>>
{
//<<20100308_JS.Kim 솔로몬
	/*
	if(!m_pGridCtrl)
		return;

	// 호가데이터의 기존 열 정보만 없앤다------------------->
	int nCount = m_HogaData.GetSize();
	CHogaData *pHogaData = NULL;
	for(int i=0; i<nCount; i++)
	{
		pHogaData = m_HogaData.GetAt(i);
		if( pHogaData == NULL )
			continue;

		pHogaData->m_nRow = -1;
	}
	// <----------------------------------------------------

	CEUREAL_KXH1 euReal_KXH1;
	euReal_KXH1.Convert((stEUREAL_KXH1*)pstEUREAL_KXH1);

	CString sTemp, sTemp1, strText;
	int		nRow;
	int		n5HogaMaesuJan = 0, n5HogaMaedoJan = 0;

	// 종목코드 체크
	if( m_strCode != euReal_KXH1.m_szkshcode )
		return;

	//////////////////////////////////////////////////////////////////////////
	// 실시간 매도 호가 기준열
	////////////////////////////////////////////////////////////////////////// 
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;

	// 매도 호가/잔량 5차
	m_nHogaRow += 5;
	m_10HogaData.nOfferho5 = atoi(euReal_KXH1.m_szofferho5);
	SetRealHogaData_Sub(euReal_KXH1.m_szofferho5, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_KXH1.m_szofferrem5, TRUE);
	

	// 매도 호가/잔량 4차
	m_nHogaRow++;
	m_10HogaData.nOfferho4 = atoi(euReal_KXH1.m_szofferho4);
	SetRealHogaData_Sub(euReal_KXH1.m_szofferho4, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_KXH1.m_szofferrem4, TRUE);

	// 매도 호가/잔량 3차
	m_nHogaRow++;
	m_10HogaData.nOfferho3 = atoi(euReal_KXH1.m_szofferho3);
	SetRealHogaData_Sub(euReal_KXH1.m_szofferho3, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_KXH1.m_szofferrem3, TRUE);
	
	// 매도 호가/잔량 2차
	m_nHogaRow++;
	m_10HogaData.nOfferho2 = atoi(euReal_KXH1.m_szofferho2);
	SetRealHogaData_Sub(euReal_KXH1.m_szofferho2, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_KXH1.m_szofferrem2, TRUE);

	// 매도 호가/잔량 1차
	m_nHogaRow++;
	m_10HogaData.nOfferho1 = atoi(euReal_KXH1.m_szofferho1);
	SetRealHogaData_Sub(euReal_KXH1.m_szofferho1, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_KXH1.m_szofferrem1, TRUE);
	
	//////////////////////////////////////////////////////////////////////////////////
	// 실시간 매수 호가 기준열
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAESU_ROW;  
	
	// 매수 호가/잔량 1차
	m_10HogaData.nBidho1 = atoi(euReal_KXH1.m_szbidho1);
	SetRealHogaData_Sub(euReal_KXH1.m_szbidho1, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_KXH1.m_szbidrem1, FALSE);
	
	// 매수 호가/잔량 2차
	m_nHogaRow++;
	m_10HogaData.nBidho2 = atoi(euReal_KXH1.m_szbidho2);
	SetRealHogaData_Sub(euReal_KXH1.m_szbidho2, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_KXH1.m_szbidrem2, FALSE);

	// 매수 호가/잔량 3차
	m_nHogaRow++;
	m_10HogaData.nBidho3 = atoi(euReal_KXH1.m_szbidho3);
	SetRealHogaData_Sub(euReal_KXH1.m_szbidho3, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_KXH1.m_szbidrem3, FALSE);

	// 매수 호가/잔량 4차
	m_nHogaRow++;
	m_10HogaData.nBidho4 = atoi(euReal_KXH1.m_szbidho4);
	SetRealHogaData_Sub(euReal_KXH1.m_szbidho4, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_KXH1.m_szbidrem4, FALSE);

	// 매수 호가/잔량 5차
	m_nHogaRow++;
	m_10HogaData.nBidho5 = atoi(euReal_KXH1.m_szbidho5);
	SetRealHogaData_Sub(euReal_KXH1.m_szbidho5, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_KXH1.m_szbidrem5, FALSE);

	// 총잔량 ---------------------------------------------------------------------------->>
	// 5호가 매도 총 잔량
	{
		for(int row=MAEDO_ROW+5; row < MAEDO_ROW+10; row++)
		{
			sTemp = m_pGridCtrl->GetItemText(row, MAEDO_JAN_COL);
			sTemp.Remove(',');
			n5HogaMaedoJan += atoi(sTemp);
		}
		sTemp1.Format("%d", n5HogaMaedoJan);
		strText = m_Util.GetComma(sTemp1);
		nRow = m_n5HogaSumRow;
		CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	}
	
	// 5호가 매수 총 잔량
	{
		for(int row=MAESU_ROW; row < MAESU_ROW+5; row++)
		{
			sTemp = m_pGridCtrl->GetItemText(row, MAESU_JAN_COL);
			sTemp.Remove(',');
			n5HogaMaesuJan += atoi(sTemp);
		}
		sTemp1.Format("%d", n5HogaMaesuJan);
		strText = m_Util.GetComma(sTemp1);
		nRow = m_n5HogaSumRow;
		CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	}

	// 매도 총 잔량
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(euReal_KXH1.m_sztotofferrem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);

	// 매수 총 잔량
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(euReal_KXH1.m_sztotbidrem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);

	*/
	if(!m_pGridCtrl)
		return;

	// 호가데이터의 기존 열 정보만 없앤다------------------->
	int nCount = m_HogaData.GetSize();
	CHogaData *pHogaData = NULL;
	for(int i=0; i<nCount; i++)
	{
		pHogaData = m_HogaData.GetAt(i);
		if( pHogaData == NULL )
			continue;

		pHogaData->m_nRow = -1;
	}
	// <----------------------------------------------------

	CREAL_CAH1 euReal_CAH1;
	euReal_CAH1.Convert((stEUREAL_CAH1*)pstREAL_CAH1);

	CString sTemp, sTemp1, strText;
	int		nRow;
	int		n5HogaMaesuJan = 0, n5HogaMaedoJan = 0;

	// 종목코드 체크
	if( m_strCode != euReal_CAH1.m_szkjmcode )
		return;

	//////////////////////////////////////////////////////////////////////////
	// 실시간 매도 호가 기준열
	////////////////////////////////////////////////////////////////////////// 
	m_nHogaRow = MAEDO_ROW;
	m_nHogaRangeForColor = 0;

	// 매도 호가/잔량 5차
	m_nHogaRow += 5;
	m_10HogaData.nOfferho5 = atoi(euReal_CAH1.m_szmdhoga5);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga5, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem5, TRUE);
	

	// 매도 호가/잔량 4차
	m_nHogaRow++;
	m_10HogaData.nOfferho4 = atoi(euReal_CAH1.m_szmdhoga4);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga4, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem4, TRUE);

	// 매도 호가/잔량 3차
	m_nHogaRow++;
	m_10HogaData.nOfferho3 = atoi(euReal_CAH1.m_szmdhoga3);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga3, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem3, TRUE);
	
	// 매도 호가/잔량 2차
	m_nHogaRow++;
	m_10HogaData.nOfferho2 = atoi(euReal_CAH1.m_szmdhoga2);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga2, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem2, TRUE);

	// 매도 호가/잔량 1차
	m_nHogaRow++;
	m_10HogaData.nOfferho1 = atoi(euReal_CAH1.m_szmdhoga1);
	SetRealHogaData_Sub(euReal_CAH1.m_szmdhoga1, m_strPreClosePrice, TRUE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmdhorem1, TRUE);
	
	//////////////////////////////////////////////////////////////////////////////////
	// 실시간 매수 호가 기준열
	//////////////////////////////////////////////////////////////////////////////////
	m_nHogaRow = MAESU_ROW;  
	
	// 매수 호가/잔량 1차
	m_10HogaData.nBidho1 = atoi(euReal_CAH1.m_szmshoga1);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga1, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem1, FALSE);
	
	// 매수 호가/잔량 2차
	m_nHogaRow++;
	m_10HogaData.nBidho2 = atoi(euReal_CAH1.m_szmshoga2);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga2, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem2, FALSE);

	// 매수 호가/잔량 3차
	m_nHogaRow++;
	m_10HogaData.nBidho3 = atoi(euReal_CAH1.m_szmshoga3);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga3, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem3, FALSE);

	// 매수 호가/잔량 4차
	m_nHogaRow++;
	m_10HogaData.nBidho4 = atoi(euReal_CAH1.m_szmshoga4);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga4, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem4, FALSE);

	// 매수 호가/잔량 5차
	m_nHogaRow++;
	m_10HogaData.nBidho5 = atoi(euReal_CAH1.m_szmshoga5);
	SetRealHogaData_Sub(euReal_CAH1.m_szmshoga5, m_strPreClosePrice, FALSE);
	SetRealHogaData_Rem(euReal_CAH1.m_szmshorem5, FALSE);

	// 총잔량 ---------------------------------------------------------------------------->>
	// 5호가 매도 총 잔량
	{
		for(int row=MAEDO_ROW+5; row < MAEDO_ROW+10; row++)
		{
			sTemp = m_pGridCtrl->GetItemText(row, MAEDO_JAN_COL);
			sTemp.Remove(',');
			n5HogaMaedoJan += atoi(sTemp);
		}
		sTemp1.Format("%d", n5HogaMaedoJan);
		strText = m_Util.GetComma(sTemp1);
		nRow = m_n5HogaSumRow;
		CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);
	}
	
	// 5호가 매수 총 잔량
	{
		for(int row=MAESU_ROW; row < MAESU_ROW+5; row++)
		{
			sTemp = m_pGridCtrl->GetItemText(row, MAESU_JAN_COL);
			sTemp.Remove(',');
			n5HogaMaesuJan += atoi(sTemp);
		}
		sTemp1.Format("%d", n5HogaMaesuJan);
		strText = m_Util.GetComma(sTemp1);
		nRow = m_n5HogaSumRow;
		CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
	}

	// 매도 총 잔량
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(euReal_CAH1.m_sztotmdhorem);
	CompareNUpdateHogaData(nRow, MAEDO_JAN_COL, strText, 0, COLOR_BLACK);

	// 매수 총 잔량
	nRow = m_nTotalSumRow;
	strText = m_Util.GetComma(euReal_CAH1.m_sztotmshorem);
	CompareNUpdateHogaData(nRow, MAESU_JAN_COL, strText, 0, COLOR_BLACK);
//>>
}