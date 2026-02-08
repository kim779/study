// CodeGrid.cpp: implementation of the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "Grid_Comm.h"
#include "../resource.h"

#include "../MainFrmWnd.h"
#include "../../Dr_Include/CompanySS.h"

#include "../../../../SrcSite/include/axis_msg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		HEADNAME_RESULT[] = { _T("시간"), _T("종목"), _T("신호"), _T("구분"), _T("강도") };
//const		int				HEADWIDTH_RESULT[] = { 40, 60, 200, 50, 30 };
// 080617 종목명 넓이 수정.
const		int				HEADWIDTH_RESULT[] = { 40, 80, 125, 50, 40};


#define ROW_DEFHEIGHT									18
#define ROW_HEAD_DEFHEIGHT						20

const   COLORREF	clrHeader		 = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine	 = COLOR_GRID_LINE;

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
//#define BLACK		RGB(0,0,0)
#define BLACK	RGB(57,57,57)

const   int     MAX_COL = 30;


//# 삼성증권
#define	UM_CHANGE_PARENT			(WM_USER + 450)		// 독립실행시 프레임 윈도우 변경을 통지하기위해...

BEGIN_MESSAGE_MAP(CGrid_Comm,CUGCtrl)
//{{AFX_MSG_MAP(ShowGrid)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG_MAP
ON_WM_DESTROY()
//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)

//# 삼성증권
ON_MESSAGE(UM_CHANGE_PARENT,	OnChangeChildFrameWindowHandle)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid_Comm::CGrid_Comm()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	
	m_clrHeader  = COLOR_GRID_LINE;
	m_clrSelRow  = COLOR_GRID_SEL_ROW;
	
	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;
	
	m_bCallAdjustOneTime = FALSE;
	
	m_bShowCol = TRUE;
	m_nHideCol = 1;
	
	m_nPrevRow = -1;
	m_nLastCol = -1;
	
	//<< 07.07.10 이준옥
	m_pDragBitmap           = NULL;
	m_pDragImage			= NULL;
	m_bDragAndDrop			= FALSE;
	m_MouseMode				= MOUSE_NOTHING;
	m_bFocus				= FALSE;
	m_lDragRow				= -1;
	m_nDragCol				= 1;	// 종목명위치
	m_uGridNotifyControlMsg = ::RegisterWindowMessage("FN_GRID_NOTIFY_CONTROL_MSG");
	//>> 07.07.10 이준옥
}

CGrid_Comm::~CGrid_Comm()
{
	m_fontDefault.DeleteObject();
	m_penGridLine.DeleteObject();
}

void CGrid_Comm::OnDestroy()
{
	m_tipInfo.RemoveTool(this);
}

void CGrid_Comm::ClearAll()
{
	m_nLastSelRow = -1;
	
	for (int nCount = 0; nCount < GetNumberRows(); nCount++)
		RemoveDataSource(nCount);
	
	while(GetNumberRows()) 
	{
		m_GI->m_defDataSource->DeleteRow(0);
		m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}
	
	QuickSetCellType(m_nSortCol, -1, 0);
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortCol = -1;
	
	SetNumberRows(0);
	RedrawAll();
}

BOOL CGrid_Comm::PreTranslateMessage(MSG* pMsg) 
{
	//<< 07.07.10 이준옥
	if( pMsg->message == WM_LBUTTONDOWN )
	{
		CPoint point = pMsg->pt;
		//ScreenToClient(&point);
		FnLButtonDown(point);
	}
	else if( pMsg->message == WM_LBUTTONUP)
	{
		CPoint point = pMsg->pt;
		//ScreenToClient(&point);
		FnLButtonUp(point);
	}
	else if( pMsg->message == WM_MOUSEMOVE)
	{
		CPoint point = pMsg->pt;
		//ScreenToClient(&point);
		FnMouseMove(point);
		
		//D&D할때 스크롤 현상이 있어서 방지하기 위해서 return 1L함.
		if(m_MouseMode == MOUSE_CODE_DRAG)	return 1L;
	}
	//>> 07.07.10 이준옥
	else if( pMsg->message == WM_RBUTTONUP)
	{
		CPoint point = pMsg->pt;
		FnRButtonUp(point);
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CGrid_Comm::OnSetup()
{
/*
SetSH_Width(0);
SetSH_NumberCols(0);
SetHighlightRow(FALSE);

  SetDefRowHeight(ROW_DEFHEIGHT);
  SetTH_Height(ROW_HEAD_DEFHEIGHT);
  
	SetCancelMode(FALSE);
	EnableMenu(TRUE);
	
	  EnableUpdate(TRUE);
	  UseHints(TRUE);
	  
		m_tipInfo.Create(this, "BASE", 1);
		m_tipInfo.SetFont(&m_fontDefault);
		m_tipInfo.AddTool(this, (LPCTSTR)" ");
		
		  EnableExcelBorders(FALSE);
		  m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
		  
			SetVScrollMode(UG_SCROLLTRACKING);
			SetHScrollMode(UG_SCROLLNORMAL);
	*/
}

void CGrid_Comm::SetInit(CString strType)
{
	SetSH_Width(0);
	SetSH_NumberCols(0);
	
	SetDefRowHeight(ROW_DEFHEIGHT);
	SetTH_Height(ROW_HEAD_DEFHEIGHT);
	
	SetCancelMode(FALSE);
	EnableMenu(TRUE);
	
	EnableUpdate(TRUE);
	UseHints(TRUE);
	
	m_tipInfo.Create(this, "BASE", 1);
	m_tipInfo.SetFont(&m_fontDefault);
	m_tipInfo.AddTool(this, (LPCTSTR)" ");
	
	EnableExcelBorders(FALSE);
	
	SetVScrollMode(UG_SCROLLNORMAL);
	SetHScrollMode(UG_SCROLLNORMAL);
	
	HeaderSetup(strType);
	
	SetHighlightRow(TRUE);
	SetUserSizingMode(FALSE);
	AdjustComponentSizes();
	
	if(m_hChildFrame==NULL)
	{
		CWnd *pwndChildFrm = GetFrameWnd(this);
		m_hChildFrame = pwndChildFrm->GetSafeHwnd();
	}
	
	m_bCandleLoadFalg = LoadCandleIndexToXml();
	CSize szFont = GetFontSize();
	m_nFontHeight = szFont.cy;
	m_nFontWidth = szFont.cx;
}

long CGrid_Comm::OnChangeChildFrameWindowHandle(WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;
	
	return 0L;
}

CWnd* CGrid_Comm::GetFrameWnd(CWnd* pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void CGrid_Comm::HeaderSetup(CString strType)
{
	ClearAll();
	
	int nColCnt;
	if (strType == "RESULT")
		nColCnt = _countof(HEADNAME_RESULT);
	
	SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		
		if (strType == "RESULT")
		{
			QuickSetText(nCol, -1, HEADNAME_RESULT[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_RESULT[nCol]));
		}
		
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}
	
	m_strType = strType;
	
	RedrawAll();
}

const LPCSTR TYPE_MARKETCODE[] = {_T("0001"), _T("1001"), _T("2001"), _T("3001"), _T("4001")};
void CGrid_Comm::SetAddRow(BOOL bReal, SignalData *pstSignalData, SIGNAL_CONDITION stSIGNAL_CONDITION, 
						   int ntotCol, int nAddRow)
{
	CUGCell cell;
	COLORREF clrBk;
	
	CString strData;
	int nRow, nCol;
	if (bReal == TRUE)
	{
		if (InsertRow(0) == UG_SUCCESS)
		{
			nRow = 0;
			GetEvenOddBkColor(GetNumberRows() - 1, clrBk);  
		}
		else
			return;
	}
	else if (bReal == FALSE)
	{
		if (nAddRow == -1)
			return;
		
		nRow = nAddRow;
		GetEvenOddBkColor(nRow, clrBk);
	}
	
	nCol = 0;   //# 시간
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData = "";
	if (pstSignalData->strTime.IsEmpty() == FALSE)
		strData = pstSignalData->strTime.Left(2) + ":" + pstSignalData->strTime.Mid(2, 2);
	//pstSignalData->strTime.Right(2);
	cell.SetText(strData);
	
	strData = pstSignalData->strDate + pstSignalData->strTime;
	cell.SetOtherDataCellText(strData);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	
	nCol = 1;   //# 종목
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetText(pstSignalData->strName);
	cell.SetOtherJongCode(pstSignalData->strCode);
	cell.SetOtherDataInt(pstSignalData->nMarket);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	
	
	nCol = 2;   //# 신호
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetText(pstSignalData->strTitle);
	cell.SetTextColor(pstSignalData->clrTitle);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	
	nCol = 3;   //# 구분
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData = "";
	if (((int)TYPE_SIGNAL_NEWS == pstSignalData->nRealType) || ((int)TYPE_REAL_NEWS == pstSignalData->nRealType))
		strData = "뉴스";
	else if (((int)TYPE_SIGNAL_SYS == pstSignalData->nRealType) || ((int)TYPE_SIGNAL_ALERT == pstSignalData->nRealType))
		strData = "시스템";
	cell.SetText(strData);
	
	cell.SetOtherDataInt(pstSignalData->nRealType);
	cell.SetOtherJongCode(pstSignalData->strCode);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	
	nCol = 4;   //# 강도
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", pstSignalData->nPoint);
	cell.SetText(strData);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	
	if (bReal == TRUE)
		SetSelectRow(m_nLastSelRow, m_nLastSelRow + 1);
	
	{
		CString szData[MAX_COL];
		char szTemp[17] = {0,};
		double dTemp;
		CUGCell cellItemDel, cellItemName, cellData[MAX_COL];
		
		POSITION pos;
		HeadInfoType stHeadInfoType;
		
		int nRealDataIndex = 0;
		int nArrIndex = 0;
		
		CList<HeadInfoType, HeadInfoType&> *m_plistFixHeadSys;
		m_plistFixHeadSys = &((CMainFrmWnd*)GetParent())->m_listFixHeadSys;
		
		if(m_plistField)
		{
			for(nCol = 0; nCol < ntotCol; nCol++)
			{
				pos = m_plistField->FindIndex(nCol);
				stHeadInfoType = m_plistField->GetAt(pos);
				
				POSITION _pos;
				HeadInfoType _stHeadInfpType;
				int nSeverDataIndex = 0; // 서버에서 내려온 값을 뽑아내기 위한 인덱스
				for(nSeverDataIndex = 0; nSeverDataIndex < m_plistFixHeadSys->GetCount(); nSeverDataIndex++)
				{
					_pos = m_plistFixHeadSys->FindIndex(nSeverDataIndex);
					_stHeadInfpType = m_plistFixHeadSys->GetAt(_pos);
					if(stHeadInfoType.strHeadName == _stHeadInfpType.strHeadName)
						break;
				}
				nSeverDataIndex *= 16;
				
				// data setting
				int nDataLength = stHeadInfoType.nDataLength;
				int	nUnitNum = stHeadInfoType.nUnitNum;
				
				if(stHeadInfoType.strDataType == _T("code"))
					continue;
				
				else if(stHeadInfoType.strDataType == _T("candle"))
				{
					nRealDataIndex++;
					continue;
				}
				
				else if(stHeadInfoType.strDataType == _T("pattern"))
				{
					nRealDataIndex++;
					continue;
				}
				
				else if(stHeadInfoType.strDataType == _T("debi"))
				{
					memset(szTemp, NULL, sizeof(szTemp));
					memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
					dTemp = atof(szTemp);
					
					CString szSign = (dTemp > 0) ? "+" : "-";
					szSign = (dTemp == 0) ? " " : szSign;
					
					szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
					nRealDataIndex++;
				}
				
				else if(stHeadInfoType.strDataType == _T("debiuel"))
				{
					memset(szTemp, NULL, sizeof(szTemp));
					memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
					dTemp = atof(szTemp);
					
					CString szSign = _T("");
					szSign = (dTemp == 0) ? " " : szSign;
					szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
					nRealDataIndex++;
				}
				
				else if(stHeadInfoType.strDataType == _T("date"))
				{
					memset(szTemp, NULL, sizeof(szTemp));
					memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
					dTemp = atof(szTemp);
					
					szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
					nRealDataIndex++;
				}
				
				else if(stHeadInfoType.strDataType == _T("time"))
				{
					memset(szTemp, NULL, sizeof(szTemp));
					memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
					dTemp = atof(szTemp);
					
					szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
					nRealDataIndex++;
				}
				
				else
				{
					memset(szTemp, NULL, sizeof(szTemp));
					memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
					dTemp = atof(szTemp);
					
					CString szSign = (dTemp > 0) ? " " : "-";
					szSign = (dTemp == 0) ? " " : szSign;
					
					if(stHeadInfoType.strDataType == "int")
						szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");
					else if(stHeadInfoType.strDataType == "double")
					{
						szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
					}
					else
						szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, FALSE, "");
					
					nRealDataIndex++;
				}
			}
		}
		
		int nCol_Offset = 5;
		COLORREF clrSignText;
		nRealDataIndex = 0;
		
		for(nCol = 0; nCol < ntotCol; nCol++)
		{
			pos = m_plistField->FindIndex(nCol);
			stHeadInfoType = m_plistField->GetAt(pos);
			
			if(stHeadInfoType.strHeadName == "char")
				cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
			else if ((stHeadInfoType.strDataType == "double") || (stHeadInfoType.strDataType == "int"))
			{
				
				cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_NUMBER | UGCELLDATA_OTHER);
			}
			if (stHeadInfoType.strAlignment == "center")
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
			else if (stHeadInfoType.strAlignment == "left")
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
			else if (stHeadInfoType.strAlignment == "right")
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNRIGHT);
			
			cellData[nCol + nCol_Offset].SetReadOnly(TRUE);
			
			if (stHeadInfoType.strDataType == "code")  //## CODE ##
			{
			}
			else if (stHeadInfoType.strDataType == "candle")  //# 캔들 
			{
				memset(szTemp, NULL, sizeof(szTemp));
				
				szData[nRealDataIndex] = FindCandleIdFromName((long)atol(szTemp));
				
				if (!szData[nRealDataIndex].IsEmpty())
				{
					szData[nRealDataIndex].TrimRight();
					szData[nRealDataIndex].TrimLeft();
				}
				
				cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
				cellData[nCol + nCol_Offset].m_string = szData[nRealDataIndex];
				
				cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
				cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
				nRealDataIndex++;
			}
			else if (stHeadInfoType.strDataType == "pattern")  //# 패턴
			{
				memset(szTemp, NULL, sizeof(szTemp));
				
				szData[nRealDataIndex] = FindPatenIdFromName(atol(szTemp));
				
				if (!szData[nRealDataIndex].IsEmpty())
				{
					szData[nRealDataIndex].TrimRight();
					szData[nRealDataIndex].TrimLeft();
				}
				
				cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
				cellData[nCol + nCol_Offset].m_string = szData[nRealDataIndex];
				
				cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
				cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
				nRealDataIndex++;
			}
			else if(stHeadInfoType.lHeadID == 9072 || stHeadInfoType.lHeadID == 9073) // 일봉, L일봉H
			{
				CString strCandleData;
				CString strTemp;
				
				memset(szTemp, NULL, sizeof(szTemp));	// 전일종가
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[8 * 16], 16);
				strTemp.Format("%s|", szTemp);
				strCandleData += strTemp;
				
				memset(szTemp, NULL, sizeof(szTemp));	// 시
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[5 * 16], 16);
				strTemp.Format("%s|", szTemp);
				strCandleData += strTemp;
				
				memset(szTemp, NULL, sizeof(szTemp));	// 고
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[6 * 16], 16);
				strTemp.Format("%s|", szTemp);
				strCandleData += strTemp;
				
				memset(szTemp, NULL, sizeof(szTemp));	// 저
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[7 * 16], 16);
				strTemp.Format("%s|", szTemp);
				strCandleData += strTemp;
				
				memset(szTemp, NULL, sizeof(szTemp));	// 종(현재가)
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[0], 16);
				strTemp.Format("%s|", szTemp);
				strCandleData += strTemp;
				
				if(stHeadInfoType.lHeadID == 9072)
				{
					cellData[nCol + nCol_Offset].SetCellTypeEx(UGCELL_CANDLE_SET);
					cellData[nCol + nCol_Offset].SetCandleData(strCandleData);
				}
				else
				{
					cellData[nCol + nCol_Offset].SetCellTypeEx(UGCELL_LHCANDLE_SET);
					cellData[nCol + nCol_Offset].SetLHCandleData(strCandleData);
				}			
				
				nRealDataIndex++;
			}
			else if(stHeadInfoType.lHeadID == 8003) // 삭제
			{
				m_nButtonIndex = AddCellType(&m_ugButtonType);
				cellData[nCol + nCol_Offset].SetText(_T("X"));
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
				cellData[nCol + nCol_Offset].SetCellType(m_nButtonIndex);
				cellData[nCol + nCol_Offset].SetCellTypeEx(UGCT_BUTTONNOFOCUS);
				cellData[nCol + nCol_Offset].SetBackColor(GetSysColor(COLOR_BTNFACE));
				cellData[nCol + nCol_Offset].SetOtherJongCode(stSIGNAL_CONDITION.sCode);
				
				nRealDataIndex++;
			}
			else if(stHeadInfoType.lHeadID == 9033) // 업종구분
			{
				CString strUpjongCode;
				strUpjongCode.Format("%3d", _ttoi(szData[nRealDataIndex]));
				strUpjongCode.Replace(' ', '0');
				
				CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)GetParent();
				CString strMarketName = pwndMainFrm->GetMarketNameToCode(strUpjongCode);
				if(strMarketName != "")
				{
					cellData[nCol + nCol_Offset].SetText((LPCTSTR)strMarketName);
				}
				else
					cellData[nCol + nCol_Offset].SetText((LPCTSTR)strUpjongCode);
				
				nRealDataIndex++;
			}
			else
			{
				if (stHeadInfoType.strUnitText.IsEmpty() == FALSE)
					szData[nRealDataIndex] += " " + stHeadInfoType.strUnitText;  //<= 단위
				
				
				
				if (!szData[nRealDataIndex].IsEmpty())
				{
					szData[nRealDataIndex].TrimRight();
					szData[nRealDataIndex].TrimLeft();
				}
				
				CString strNote;
				if (stHeadInfoType.lHeadID == 8002) // 메모
				{
					strNote = LoadMemo(pstSignalData->strCode.Mid(0, 6));

					cellData[nCol + nCol_Offset].SetText(strNote);
					cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
					cellData[nCol + nCol_Offset].SetReadOnly(FALSE);
					cellData[nCol + nCol_Offset].SetOtherJongCode(pstSignalData->strCode);
					cellData[nCol + nCol_Offset].SetOtherDataCellText((LPCSTR)pstSignalData->strName);
				}
				else 
				{
					if(atof(szData[nRealDataIndex]) == 0)
						cellData[nCol + nCol_Offset].SetText("");
					else
						cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);

					cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
				}
				
				memset(szTemp, NULL, sizeof(szTemp));
				
				char chSign = stSIGNAL_CONDITION.stCommon.sSign;
				if(stHeadInfoType.strDataType == "debi" || stHeadInfoType.strDataType == "debiuel")
				{
					if(chSign == '4' || chSign == '5')
						cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * -100.0));
					else
						cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 100.0));
				}
				else
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atol(szTemp)));
				
				char szSign[2] = {0,};
				szSign[0] = chSign;
				int nSign = atoi(szSign);
				GetSignColor(nSign, clrSignText);
				if (stHeadInfoType.strDataType == "debi")    //## 대비 ##
				{
					cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
					cellData[nCol + nCol_Offset].SetSign(nSign);
				}
				else if (stHeadInfoType.strDataType == "debiuel")   //## 대비율 ##
					cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
				else if(stHeadInfoType.strHeadName == "현재가")
					cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
				
				nRealDataIndex++;
			}
			cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
		}
		
		for(nCol = 0; nCol < ntotCol; nCol++)
		{
			cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
			cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
			cellData[nCol + nCol_Offset].SetBackColor(clrBk);
			
			SetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);
		}
		
		RedrawAll();
	}
	
// 	CUGCell cell;
// 	COLORREF clrBk;
// 	
// 	CString strData;
// 	int nRow, nCol;
// 	if (bReal == TRUE)
// 	{
// 		if (InsertRow(0) == UG_SUCCESS)
// 		{
// 			nRow = 0;
// 			GetEvenOddBkColor(GetNumberRows() - 1, clrBk);  
// 		}
// 		else
// 			return;
// 	}
// 	else if (bReal == FALSE)
// 	{
// 		if (nAddRow == -1)
// 			return;
// 		
// 		nRow = nAddRow;
// 		GetEvenOddBkColor(nRow, clrBk);
// 	}
// 	
// 	nCol = 0;   //# 시간
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	
// 	strData = "";
// 	if (pstSignalData->strTime.IsEmpty() == FALSE)
// 		strData = pstSignalData->strTime.Left(2) + ":" + pstSignalData->strTime.Mid(2, 2);
// 	//pstSignalData->strTime.Right(2);
// 	cell.SetText(strData);
// 	
// 	strData = pstSignalData->strDate + pstSignalData->strTime;
// 	cell.SetOtherDataCellText(strData);
// 	
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	
// 	nCol = 1;   //# 종목
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	
// 	cell.SetText(pstSignalData->strName);
// 	cell.SetOtherJongCode(pstSignalData->strCode);
// 	cell.SetOtherDataInt(pstSignalData->nMarket);
// 	
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	
// 	
// 	nCol = 2;   //# 신호
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	
// 	cell.SetText(pstSignalData->strTitle);
// 	cell.SetTextColor(pstSignalData->clrTitle);
// 	
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	
// 	nCol = 3;   //# 구분
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	
// 	strData = "";
// 	
// 	if (((int)TYPE_SIGNAL_SYS == pstSignalData->nRealType) || ((int)TYPE_SIGNAL_ALERT == pstSignalData->nRealType))
// 		strData = "시스템";
// 	cell.SetText(strData);
// 	
// 	cell.SetOtherDataInt(pstSignalData->nRealType);
// 	cell.SetOtherJongCode(pstSignalData->strCode);
// 	
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	
// 	nCol = 4;   //# 강도
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	
// 	strData.Format("%d", pstSignalData->nPoint);
// 	cell.SetText(strData);
// 	
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	
// 	if (bReal == TRUE)
// 		SetSelectRow(m_nLastSelRow, m_nLastSelRow + 1);
}

void CGrid_Comm::SetHideCol(int nCol, BOOL bShow)
{
	m_bShowCol = bShow;
	m_nHideCol = nCol;
	
	AdjustComponentSizes();
}

void CGrid_Comm::GetAttributeInfo(int nRow, char chAttribute, COLORREF &clrContrastText)
{
	switch (chAttribute)
	{
	case '1':		// 상한(화살표) : RED 배경색에 WHITE TEXT
		clrContrastText = RED;
		break;
	case '2':		// 상승(삼각형) : WHITE 배경색에 RED TEXT
		clrContrastText = RED;
		break;
	case '4':		// 하한(화살표) : BLUE 배경색에 WHITE TEXT
		clrContrastText = BLUE;
		break;
	case '5':		// 하락(삼각형) : WHITE 배경색에 BLUE TEXT
		clrContrastText = BLUE;
		break;
	case '3':		// 보합 : WHITE 배경색에 BLACK TEXT
	default:
		clrContrastText = BLACK;
	}
}

CString CGrid_Comm::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;
	
	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";
		
		strData.Format(strDis, lData / (float)nUnitNum);
	}
	else
	{
		strDis = "%ld";
		strData.Format(strDis, lData);
	}
	
	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;
		
		if (nLength < nLengthFormat)
		{
			int nCaretIndex = 0;
			for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
			{
				if ((CString)strFormat[nIndex] == "X")
				{
					strResult += (CString)strData[nCaretIndex];
					nCaretIndex++;
				}
				else
					strResult += (CString)strFormat[nIndex];
			}
		}
	}
	else
	{
		int nCheckIndex = 0;
		BOOL bDotFlag = FALSE;
		nDotPos = strData.Find(".");
		
		int nStartIndex;
		nLength = strData.GetLength();
		if (nDotPos > 0)
		{
			nStartIndex = nDotPos;
			bDotFlag = TRUE;
		}
		else
		{
			nStartIndex = nLength;
			nDotPos = 0;
		}
		
		int nCaretIndex = 0;
		CString strResultInt;
		
		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
		{
			if (nCheckIndex == 3)
			{
				strResultInt += "," + (CString)strData[nIndex];
				nCheckIndex = 1;
			}
			else
			{
				strResultInt += (CString)strData[nIndex];
				nCheckIndex++;
			}
		}
		
		nLength = strResultInt.GetLength();
		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
			strResult += (CString)strResultInt[nIndex];
		
		if (nDotPos > 0)
			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
	}
	
	return strResult;
}

int CGrid_Comm::OnCanSizeTopHdg()
{
	return FALSE;
}

void CGrid_Comm::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1))
	{
		SetSelectRow(m_nLastSelRow, row);
		//SetCommMenu(point);
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
		if(m_strType != "RESULT")
			return;
		
		CString strCode = "";
		CUGCell cell;
		GetCell(1, m_nLastSelRow, &cell);
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");
		
		CWnd*		pModuleMainWnd;
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
		pModuleMainWnd = pwndMainFrm->m_pModuleMainWnd;
		GetCursorPos(point);
		ScreenToClient(point);
		ClientToScreen(point);
		
		CString strLPARM;
		strLPARM.Format("%s\n%d\t%d\n1301\t%s\n", "IB713300", point->x, point->y, strCode);
		// mapN \n pt.x \t pt.y \n SYM \t DATA \n
		pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<
	}
}

//extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );	-hidden-
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);			-hidden-
void CGrid_Comm::SetCommMenu(POINT *point)
{
	if (m_hChildFrame == NULL)
		return;
	
	long lMenu = 0;	//	::SendMessage(m_hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );	-hidden-
	if ( lMenu > 0 )
	{
		CMenu* pMenu = (CMenu*)lMenu;
		ASSERT ( pMenu );
		
		SetCommMenu_Add(pMenu);
		
		GetCursorPos(point);
		int nIndex_SelMenu = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
			point->x, point->y, this);
		pMenu->DestroyMenu();
		delete pMenu;
		
		if (nIndex_SelMenu > -1)
		{
			HWND hWnd = m_hChildFrame;
			
			CString strScreenNo = "";
			strScreenNo = GetCommMenu_IndexToScreenNo(nIndex_SelMenu);
			
			int nCol = -1;
			if (m_strType == "RESULT")
				nCol = 1;
			
			CString strCode = "";
			if (nCol > -1)
			{
				CUGCell cell;
				GetCell(nCol, m_nLastSelRow, &cell);
				strCode = cell.GetOtherJongCode();
			}
			
			if (strScreenNo.IsEmpty() == TRUE)
			{
				//				if (strCode.IsEmpty() == FALSE)		-hidden-
				//					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
				::PostMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
			}
			else
			{
				//				if (strCode.IsEmpty() == FALSE)		-hidden-	
				//					KSLink_SetLinkTagData("JCODE", strCode);		// 외부화면용
				//				::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo);
			}
		}
	}
	
	RedrawAll();
}

void CGrid_Comm::SetCommMenu_Add(CMenu* pMenu)
{
	//	USE_SS_HTS();	-hidden-
	
	int nMenuCnt;
	UINT nMenuType;
	
	//	pMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)"");
	
	nMenuCnt = _countof(RBTNMENU_NAME);
	CString strScreenInfo;
	for (int nIndex = 0; nIndex < 2; nIndex++)
	{
		if ((CString)RBTNMENU_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else 
			nMenuType = MF_STRING;
		
		//--> HTS구분없앰. 사용하면 추후 다시
		//		if(bIsHTS)
		{
			strScreenInfo = (CString)RBTNMENU_SCREEN_NOMAX[nIndex];
			strScreenInfo = strScreenInfo.Left(4);
		}
		//		else
		//		{
		//			strScreenInfo = (CString)RBTNMENU_SCREEN_NOEMP[nIndex];
		//			strScreenInfo = strScreenInfo.Left(5);
		//		}
		//<--
		strScreenInfo = "[" + strScreenInfo + "] " + (CString)RBTNMENU_NAME[nIndex];
		pMenu->AppendMenu(nMenuType, RBTNMENU_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
	}
}

CString CGrid_Comm::GetCommMenu_IndexToScreenNo(int nIndex_SelMenu)
{
	//	USE_SS_HTS();	-hidden-
	
	CString strScreenNo = "";
	int nIndex;
	
	//--> HTS구분없앰. 사용하면 추후 다시
	//	if(bIsHTS)
	{
		for (nIndex = 0; nIndex < _countof(RBTNMENU_INDEX); nIndex++)
		{
			if ((int)RBTNMENU_INDEX[nIndex] == nIndex_SelMenu)
				strScreenNo = RBTNMENU_SCREEN_NOMAX[nIndex];
		}
	}
	//	else
	//	{
	//		for (nIndex = 0; nIndex < _countof(RBTNMENU_INDEX); nIndex++)
	//		{
	//			if ((int)RBTNMENU_INDEX[nIndex] == nIndex_SelMenu)
	//				strScreenNo = RBTNMENU_SCREEN_NOEMP[nIndex];
	//		}
	//	}
	//<--
	
	return strScreenNo;
}

void CGrid_Comm::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1 && col != -1) && (updn == 1))
	{
		//이전 Focus가 메모Grid일 경우 저장
		CUGCell cell;
		GetCell(m_nLastCol, m_nPrevRow, &cell);
		
		if(!cell.GetReadOnly())
		{
			CString strCode = cell.GetOtherJongCode();
			CString strName = cell.GetOtherDataCellText();
			CString strNote = cell.GetText();
			strName.TrimRight();
			
			if(strNote != "")
				SaveMemo(strCode.Mid(0, 6), strNote);

			StartEdit();
		}
		else
		{
			if (updn == 1)
			{
				SetSelectRow(m_nLastSelRow, row);
				
				if ((m_strType == "RESULT") && (m_bShowCol == TRUE))
				{				
					CMainFrmWnd* pviewTrMain = (CMainFrmWnd*)GetParent();
					CString strData;
					CUGCell cell;
					GetCell(1, row, &cell);   //# Code
					strData = cell.GetOtherJongCode();
					//pviewTrMain->SetSearchData_Chart(strData);
					pviewTrMain->SendTr_ChartData(strData, "");
					
					pviewTrMain->PutLinkData("&STOCK_CODE", strData);
					
					strData = "";
					GetCell(0, row, &cell);   //# Time
					strData = cell.GetOtherDataCellText();
					pviewTrMain->SetSearchData_Time(strData);
					
					this->SetFocus();
				}
			}
		}
		
		/*OnRowChange(m_nLastSelRow, row);*/
		m_nLastCol = col;
		m_nPrevRow = m_nLastSelRow;
		
		
	}
}

void CGrid_Comm::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	if (m_strType == "RESULT")
		return;
	
	if (updn && !processed)
	{		
		QuickSetCellType(m_nSortCol, -1, 0);
		
		if (m_nSortCol == col && m_nSortMethode == UG_SORT_ASCENDING) 
		{
			m_nSortMethode = UG_SORT_DESCENDING;
			m_nArrowType   = UGCT_SORTARROWUP;
		} 
		else 
		{
			m_nSortMethode = UG_SORT_ASCENDING;
			m_nArrowType   = UGCT_SORTARROWDOWN;
		}
		
		m_nSortCol = col;
		SortBy(m_nSortCol, m_nSortMethode);
		QuickSetCellType(m_nSortCol, -1, m_nSortArrow);
		QuickSetCellTypeEx(m_nSortCol, -1, m_nArrowType);
		
		EvenOddColorSetting();
		RedrawAll();
	}
}

int CGrid_Comm::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
	CString strCell1, strCell2;
	
	if (m_nSortCol == 1)
		nResult = cell1->m_string.Compare(cell2->m_string);
	else
	{
		if (cell1->m_lOtherData > cell2->m_lOtherData)
			nResult = 1;
		else if (cell1->m_lOtherData < cell2->m_lOtherData)
			nResult = -1;
	}
	
	nResult *= (-(flags * 2 - 3));
	return nResult;
}

void CGrid_Comm::EvenOddColorSetting()
{
	long nRow, nRowCnt;
	int  nCol, nColCnt;
	
	COLORREF clrBk;
	CUGCell cell;
	
	nRowCnt = GetNumberRows();
	nColCnt = GetNumberCols();
	for (nRow = 0; nRow < nRowCnt; nRow++)
	{
		GetEvenOddBkColor(nRow, clrBk);
		for (nCol = 0; nCol < nColCnt; nCol++)
		{
			GetCell(nCol, nRow, &cell);
			cell.SetBackColor(clrBk);
			
			//## Line Color ##
			cell.SetBorderColor(&m_penGridLine);
			cell.SetBorder(UG_BDR_RTHIN);
			
			SetCell(nCol, nRow, &cell);
		}
	}
}

void CGrid_Comm::OnRowChange(long oldrow, long newrow)
{
	
}


void CGrid_Comm::SetSelectRow(long oldrow, long newrow)
{
	if (GetNumberRows() - 1 < newrow)
		return;
	
	COLORREF clrBk;
	CUGCell	cell;
	
	int nColCnt = GetNumberCols();
	GetEvenOddBkColor(oldrow, clrBk);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
// 		if(nCol == 0 || nCol == 1)
// 			continue;
		
		if (oldrow != -1)
		{
			GetCell(nCol, oldrow, &cell);
			cell.SetBackColor(clrBk);
			SetCell(nCol, oldrow, &cell);
		}
		
		if (newrow != -1)
		{
			GetCell(nCol, newrow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, newrow, &cell);
		}
	}
	
	m_nLastSelRow = newrow;
	RedrawAll();
}

inline void CGrid_Comm::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CGrid_Comm::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(m_clrHeader);
}

void CGrid_Comm::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	if (m_strType == "RESULT")
	{
		CMainFrmWnd *pviewTrMain = (CMainFrmWnd *)GetParent();
		
		CUGCell cell;
		GetCell(3, row, &cell);
		CString strCode = "";
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");
		
		GetCell(3, row, &cell);
		int nRealType;
		nRealType = cell.GetOtherDataInt();
		
		CString strMapNo = "";
		// 		if (((int)TYPE_SIGNAL_NEWS == nRealType) || ((int)TYPE_REAL_NEWS == nRealType))
		// 			strMapNo = "5002";   //# 뉴스분석
		if (((int)TYPE_SIGNAL_SYS == nRealType) || ((int)TYPE_SIGNAL_ALERT == nRealType))
			strMapNo = "7003";   
		
		pviewTrMain->SetLinkCode(strCode, TRUE);
		if(strMapNo != "")
			pviewTrMain->SetOpenScreen(0, strMapNo, strCode);
		
		// 		if (col == 1)
		// 		{
		// 			//20080115 기존에는 종목링크만 했었는데 요청에 의해 화면여는 기능을 추가함.
		// 			pviewTrMain->SetLinkCode(strCode);
		// 			if (((int)TYPE_SIGNAL_NEWS == nRealType) || ((int)TYPE_REAL_NEWS == nRealType))
		// 			{
		// 				CString strMapNo = "5002";   //# 뉴스분석
		// 				pviewTrMain->SetOpenScreen(0, strMapNo, strCode);
		// 			}
		// 		}
		// 		else
		// 		{			
		// 			CString strMapNo = "";
		// 			if (((int)TYPE_SIGNAL_NEWS == nRealType) || ((int)TYPE_REAL_NEWS == nRealType))
		// 			{
		// 				strMapNo = "5002";   //# 뉴스분석
		// 				pviewTrMain->SetLinkCode(strCode, TRUE);
		// 				pviewTrMain->SetOpenScreen(0, strMapNo, strCode);
		// 			}
		// 			else if (((int)TYPE_SIGNAL_SYS == nRealType) || ((int)TYPE_SIGNAL_ALERT == nRealType))
		// 			{
		// 				
		// 			}
		// 		}
	}
}

CString CGrid_Comm::GetRowToCode(int nRow)
{
	CString strResult = "";
	if (GetNumberRows() - 1 < nRow)
		return strResult;
	
	if (m_strType == "RESULT")
	{
		CUGCell cell;
		GetCell(1, nRow, &cell);
		strResult = cell.GetOtherJongCode();
	}
	
	return strResult;
}

extern CString dll_GetStockDetailInfo(CString strCode, int nType);
void CGrid_Comm::SetHint_Code(int nRow)
{
	CString strTip;
	CUGCell cell;
	GetCell(1, nRow, &cell);
	
	if (m_tipInfo.GetSafeHwnd() && m_tipInfo.GetShowState() == FALSE)
	{
		CString strCode;
		strCode = cell.GetOtherJongCode();
		
		strCode.TrimRight(" ");
		if (strCode.IsEmpty() == TRUE)
			return;
		
		//-->@080616-001 종목명에 갔을 때 종목코드가 툴팁으로 보이도록 수정.
		//		int nMarket;
		//		nMarket = cell.GetOtherDataInt();
		//		strTip = dll_GetStockDetailInfo(strCode, nMarket);
		strTip = strCode;
		//<--		
		m_tipInfo.UpdateToolTip(this, strTip, 1);
		m_tipInfo.Show(strTip);
	}
}

int CGrid_Comm::OnHint(int col, long row, int section, CString *string)
{
	if (m_strType == "RESULT")
	{
		if (col == 1)
		{
			SetHint_Code(row);
			return FALSE;
		}
		else
		{
			CUGCell cell;
			GetCell(2, row, &cell);
			*string = cell.GetText();
			return TRUE;
		}
	}
	
	return FALSE;
}

void CGrid_Comm::OnAdjustComponentSizes(RECT *grid, 
										RECT *topHdg,RECT *sideHdg,
										RECT *cnrBtn,RECT *vScroll,
										RECT *hScroll,RECT *tabs)
{
	if (m_bCallAdjustOneTime == TRUE)
		return;
	
	CRect  rtGrid;
	double dWidthGrid = 0.0;
	double dWidthCell = 0.0;
	
	int ntotWidth = 0;
	int nCol = 0, nColCnt = 0;
	if (m_strType == "RESULT")
	{
		nColCnt = _countof(HEADWIDTH_RESULT);
		
		for (nCol = 0; nCol < _countof(HEADWIDTH_RESULT); nCol++)
		{
			if ((m_nHideCol == nCol) && (m_bShowCol == FALSE))
				continue;
			
			if (nCol == 2)
				continue;
			
			ntotWidth += HEADWIDTH_RESULT[nCol];
		}
	}
	
	if (!m_bCallAdjustOneTime)
	{
		rtGrid = CRect(grid);
		
		dWidthGrid = rtGrid.Width() - 1 - ntotWidth;
		if (m_strType == "RESULT")
		{
			// 			for (nCol = 0; nCol < _countof(HEADWIDTH_RESULT) - 1; nCol++)
			// 			{
			// 				if (nCol == 2)
			// 					SetColWidth(nCol, (int)dWidthGrid);
			// 				else if ((m_nHideCol == nCol) && (m_bShowCol == FALSE))
			// 					SetColWidth(nCol, 0);
			// 				else
			// 					SetColWidth(nCol, (int)HEADWIDTH_RESULT[nCol]);
			// 			}
		}
		
		m_bCallAdjustOneTime = TRUE;
		AdjustComponentSizes();
		m_bCallAdjustOneTime = FALSE;
	}
	
	return;
}

//<< 07.07.10 이준옥
void CGrid_Comm::FnLButtonDown(CPoint point)
{
	m_MouseMode = MOUSE_CODE_DRAG;
	
	if(::GetFocus()!=GetSafeHwnd())	SetFocus();
	
	ScreenToClient(&point);
	// ScreenToClient(&point);
	int col;
	
	//	CUGCell cell;
	//	GetCell(point.x, point.y,&cell);
	//	if(cell.IsPropertySet(UGCELL_JOIN_SET))
	//	{
	//		RECT rect;
	//		GetCellRect(&col, &m_lDragRow, &rect);
	//	}
	//	point.y -= GetTH_Height();		
	GetCellFromPoint(point.x, point.y, &col, &m_lDragRow);
	if(col!=m_nDragCol || m_lDragRow == 0) 
	{
		m_MouseMode = MOUSE_NOTHING;
		m_bDragAndDrop = FALSE;
		m_strDragCode = "";
		return;
	}
	m_lDragRow --;	// 실제보다 하나작게
	//TRACE("m_lDragRow=%ld\n", m_lDragRow);
}

//#include "../Builder/LinkInfoMng/KSLinkInfoMng.h"
extern long KSLink_GetEnvInfo(int nType);
void CGrid_Comm::FnLButtonUp(CPoint point)
{
	ScreenToClient(&point);
	
	if ( m_MouseMode == MOUSE_CODE_DRAG )
	{
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		DragImageInfoClear ( );
		
		long lRet(0);
		
		CRect rcFrameRect;
		//::GetClientRect( m_hFrameWnd, &rcFrameRect );
		::GetClientRect( m_hWnd, &rcFrameRect );
		
		HWND hTragetWnd;
		CWnd* pTragetWnd = NULL;
		
		// 다른 윈도우로 drop되는 경우에만 처리해준다;
		if ( !m_strDragCode.IsEmpty() )
		{
			CPoint pt;
			GetCursorPos(&pt);
			
			CPoint ptPoint( pt );
			hTragetWnd = ::WindowFromPoint( pt );
			
			pTragetWnd = CWnd::FromHandle(hTragetWnd);
			TRACE(" WindowFromPoint...hwnd=[0x%08x]\n", hTragetWnd );
			long lData(0);
			if ( hTragetWnd && GetSafeHwnd()  != hTragetWnd )
			{
				lData = ::GetWindowLong ( hTragetWnd, GWL_USERDATA );
				if ( lData > 0 &&  lData == 1013 )
				{
					lRet = ::SendMessage( hTragetWnd, m_uGridNotifyControlMsg , (WPARAM)MAKELONG( JMCOMBO_TO_GRID_CODE_DROP_NOTIFY, 0 ),  (LPARAM)(LPCTSTR)m_strDragCode );
					if ( lRet > 0 )
					{
						m_MouseMode = MOUSE_NOTHING;
						m_bDragAndDrop = FALSE;
						m_strDragCode = "";
						
						return;
					}
				}
			}
			
			if ( lRet == 0 && !rcFrameRect.PtInRect( point) )
			{
			/*	-hidden-
			static DRAG_DATA_INFO DndDataInfo;
			CPoint DropPoint(point);
			ClientToScreen(&DropPoint);
			DndDataInfo.pt	= DropPoint;
			strcpy(DndDataInfo.szCode, (LPCTSTR)m_strDragCode);
			
			  HWND		m_hRunWnd;
			  m_hRunWnd = (HWND)KSLink_GetEnvInfo(0);
			  ::PostMessage( m_hRunWnd, UM_DND_NOTIFY, NULL, (LPARAM)&DndDataInfo);
			  m_MouseMode = MOUSE_NOTHING;
			  m_bDragAndDrop = FALSE;
			  m_strDragCode = "";
			  
				if ( m_bFocus )
				{
				m_bFocus	= FALSE;	
				}
				*/				
				return;
			}
			
			/*
			if( static_cast<CJMComboCtl*>(GetParent()->GetParent())->pFormBase )
			{
			CRect rcFormRect;
			::GetClientRect( static_cast<CJMComboCtl*>(GetParent()->GetParent())->pFormBase->GetSafeHwnd(), &rcFormRect );
			if ( !rcFormRect.PtInRect( point) )
			{
			static DRAG_DATA_INFO DndDataInfo;
			CPoint DropPoint(point);
			ClientToScreen(&DropPoint);
			DndDataInfo.pt	= DropPoint;
			strcpy(DndDataInfo.szCode, (LPCTSTR)m_strDragCode);
			
			  HWND		m_hRunWnd;
			  m_hRunWnd = (HWND)KSLink_GetEnvInfo(0);
			  ::PostMessage( m_hRunWnd, UM_DND_NOTIFY, NULL, (LPARAM)&DndDataInfo);
			  }
			  }
			*/
		}
	}
	
	m_MouseMode = MOUSE_NOTHING;
	m_bDragAndDrop = FALSE;
	m_strDragCode = "";
	
	if ( m_bFocus )
	{
		m_bFocus	= FALSE;	
	}
}

void CGrid_Comm::FnMouseMove(CPoint point)
{
	ScreenToClient(&point);
	
	if ( m_MouseMode == MOUSE_CODE_DRAG && m_bDragAndDrop )
	{
		POINT pt = point;
		ClientToScreen( &pt );
		
		CImageList::DragMove(pt);
		CWnd* pDropWnd = WindowFromPoint(pt);
		CImageList::DragShowNolock(TRUE);
		return;
	}
	
	CRect rcRect;
	GetClientRect( rcRect );
	if(m_MouseMode == MOUSE_CODE_DRAG)// && !rcRect.PtInRect( point) )
	{
		// 종목코드 드래그를 시작한다.
		// 종목코드를 취득한다.
		if(m_lDragRow<0) m_lDragRow = GetCurrentRow();
		if(m_strDragCode.IsEmpty())
		{
			m_strDragCode = GetRowToCode(m_lDragRow);
			if(m_strDragCode.GetLength()<6)
				return;
		}
		
		//display code
		if ( MakeDragImage( m_strDragCode) )
		{
			CPoint	pt;	
			IMAGEINFO ii;
			m_pDragImage->GetImageInfo( 0, &ii );
			
			pt.x = (ii.rcImage.right - ii.rcImage.left) / 2;
			pt.y = (ii.rcImage.bottom - ii.rcImage.top) / 2;
			
			SetCursor( LoadCursor(NULL, IDC_ARROW) );
			m_pDragImage->BeginDrag( 0, pt );
			
			CPoint ptTmp = point;
			ClientToScreen( &ptTmp );
			
			m_pDragImage->DragEnter( NULL,ptTmp );
			m_bDragAndDrop = TRUE;
		}
	}
}


void CGrid_Comm::DragImageInfoClear()
{
	if ( m_pDragImage )
		delete m_pDragImage;
	m_pDragImage = NULL;
	
	if ( m_pDragBitmap )
		delete m_pDragBitmap;
	m_pDragBitmap = NULL;
	
}

BOOL CGrid_Comm::MakeDragImage(CString strText)
{
	strText.TrimRight();
	if(strText.GetLength()==0)	return FALSE;
	
	//CFont *pFont = (CFont*) GetFont(0);
	CFont *pFont = &m_fontDefault;
	if(!pFont)	return FALSE;
	ASSERT(pFont);
	
	CRect	rectImage, 
		rect;
	CDC	*pDC = GetDC();
    if (!pDC)
        return FALSE;
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	
	DragImageInfoClear();
	
	CBitmap *pOldBitmap;
	m_pDragBitmap = new CBitmap();
	m_pDragImage  = new CImageList();
	
	//CFont *pOldFont = MemDC.SelectObject(&font);
	CFont *pOldFont = MemDC.SelectObject(pFont);
	
	rectImage.left	= 0;
	rectImage.top	= 0;
	MemDC.DrawText( strText, rectImage, DT_CALCRECT);
	
	// 256색 설정에서도 drag image를 보여주기 위해서.
	// PLANES	: Number of color planes.
	// BITSPIXEL: Number of adjacent color bits for each pixel.
	int nPlanes = pDC->GetDeviceCaps(PLANES);
	int nColors = pDC->GetDeviceCaps(BITSPIXEL);
	m_pDragBitmap->CreateBitmap( rectImage.Width()+4, 
		rectImage.Height()+4, 
		nPlanes, 
		nColors, 
		NULL);
	
	pOldBitmap = MemDC.SelectObject(m_pDragBitmap);	
	
	CBrush	brush;
	brush.CreateSolidBrush( RGB(0,0,255) );
	
	MemDC.SetBkColor( RGB(0,0,255) );
	MemDC.SetTextColor( RGB(255,255,255) );
	
	rect.SetRect(0, 0, rectImage.Width()+4, rectImage.Height()+4);
	
	MemDC.FillRect(&rect, &brush);
	
	rect.SetRect(2,2,rectImage.Width()+2,rectImage.Height()+2);
	
	MemDC.DrawText( strText, rect,  DT_LEFT | DT_SINGLELINE );
	MemDC.SelectObject(pOldBitmap);
	if(pOldFont)
		MemDC.SelectObject(pOldFont);
	
	brush.DeleteObject();	////[EDIT] CBrush 해제 2007-01-19 김종연 
	MemDC.DeleteDC();		////[EDIT] MemDC 해제 2007-01-11 김종연 
	ReleaseDC( pDC );
	
	m_pDragImage->Create(rectImage.Width()+4, rectImage.Height()+4, ILC_COLORDDB | ILC_MASK, 4, 4 );
	m_pDragImage->Add( m_pDragBitmap, RGB(192,192,192) );
	
	return TRUE;
}

void CGrid_Comm::FnRButtonUp(CPoint point)
{
	SetCommMenu(&point);
}

void CGrid_Comm::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	if(!stGridColor.clrGridHeader)
		return;
	
	m_clrHeader = stGridColor.clrGridHeader;
	m_clrSelRow = stGridColor.clrGridHilight;
	
	int nCurRow = GetCurrentRow();
	CUGCell cell;
	for(int nCol = 0; nCol < GetNumberCols(); nCol++)
	{
		GetCell(nCol, -1, &cell);
		cell.SetBackColor(m_clrHeader);
		SetCell(nCol, -1, &cell);
		
		if(nCurRow > -1)
		{		
			GetCell(nCol, nCurRow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, nCurRow, &cell);	
		}
	}	
}

const		LPCTSTR		HEADNAME_BASE[] = { _T("시간"), _T("종목"), _T("신호"), _T("구분"), _T("강도") };
const		int			HEADWIDTH_BASE[] = { 40, 80, 125, 50, 40};

#include "math.h"
#define COL_CODE      0
#define COL_NAME      1
void CGrid_Comm::SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField)
{
	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
	CMapStringToString UserColWidthsMap;
	CFindData_GridUtil::GetGridAllWidth(this, &UserColWidthsMap);
	
	CUGCell	cell;
	
	ClearAll();
	
	int nColCnt = plistField->GetCount();
	SetNumberCols(nColCnt + 5);
	
	int nCol = 0;
	// 시간
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNCENTER| UG_ALIGNVCENTER);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	// 종목
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	// 신호
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	// 구분
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	// 강도
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	POSITION   pos;
	HeadInfoType    stHeadType;
	for (int nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistField->FindIndex(nIndex);
		stHeadType = m_plistField->GetAt(pos);
		
		TRACE("\n # Count      : %d", nCol);
		TRACE("\n   # HeadName : %s", stHeadType.strHeadName);
		TRACE("\n   # HeadID   : %d", stHeadType.lHeadID);
		
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		QuickSetText(nCol, -1, stHeadType.strHeadName);
		SetColWidth(nCol, (int)(stHeadType.nColWidth * (m_nFontWidth + 1)));
		SetRowHeight(nCol, m_nFontHeight + 4);
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
		
		if ((stHeadType.strDataType == "code") ||  //## 종목 CODE ##
			(stHeadType.strDataType == "candle")	|| //# 캔들
			(stHeadType.strDataType == "pattern") ||	//# 패턴
			(stHeadType.strDataType == "period")	// 기간
			)	
			m_nSortType[nCol] = 1;
		nCol++;
	}
	
	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
	CFindData_GridUtil::SetGridAllWidth(this, &UserColWidthsMap);
	
	RedrawAll();
}

void CGrid_Comm::SetLockColumns(int nCol, BOOL bOffSet)
{
	if (bOffSet == TRUE)
		m_nFixCol = 2 + nCol;
	else
		m_nFixCol = nCol;
	
	LockColumns(m_nFixCol);
}

void CGrid_Comm::SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
						   CString strItemName, char dcolData[960], bool bRedraw)
{
	int nCol;
	CString strData[MAX_COL];
	//	CString strTemp;
	char szTemp[17];
	double dTemp;
	CUGCell cellItemDel, cellItemName, cellItemCode, cellData[MAX_COL];
	
	int nRowIndex = nRow;
	COLORREF clrBk;
	GetEvenOddBkColor(nRowIndex, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;
	
	char szItemCode[7];
	memset(szItemCode, 0x00, sizeof(szItemCode));
	memcpy(szItemCode, strItemCode, 6);	
	
	int nRealDataIndex = 0;
	int nArrIndex = 0;
	if(m_plistField)		//070420금 Null체크루틴 추가.
	{
		for (nCol = 0; nCol < ntotCol; nCol++)
		{
			pos = m_plistField->FindIndex(nCol);
			TempHeadType = m_plistField->GetAt(pos);
			
			int nDataLength = TempHeadType.nDataLength;
			int nUnitNum = TempHeadType.nUnitNum;
			
			
			nArrIndex = nCol * 16;
			
			CString strFormat;
			int nFormatCnt = 0;
			if (TempHeadType.strDataType == "code")  //## CODE ##
				continue;
			else if (TempHeadType.strDataType == "candle")	//# 캔들
			{
				nRealDataIndex++;
				continue;
			}
			else if (TempHeadType.strDataType == "pattern")		//# 패턴
			{
				nRealDataIndex++;
				continue;
			}
			else if (TempHeadType.strDataType == "debi")   //## 대비 ##
			{
				//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);
				
				CString strSign = (dTemp > 0) ? "+" : "-";
				strSign = (dTemp == 0) ? " " : strSign;
				
				//////////////////////////////////////////////////////////////
				// 20100414 박찬모 업종검색에서 전일대비 소수점 이하 2자리 >>
				// 				if(m_nIndustry == 1)
				// 				{
				// 					TempHeadType.strDataType = "double";
				// 					nUnitNum = 100;
				// 				}
				// 20100414 박찬모 업종검색에서 전일대비 소수점 이하 2자리 <<
				//////////////////////////////////////////////////////////////
				
				strData[nRealDataIndex] = /*strSign +*/ GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "debiuel")  //## 대비율 ##
			{
				//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);
				
				CString strSign = _T("");
				strSign = (dTemp == 0) ? " " : strSign;
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "date")  //## 날짜 ##
			{
				//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);
				
				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "time")  //## 시간 ##
			{
				//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);
				
				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
				nRealDataIndex++;
			}
			else		//# int, double
			{
				//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);
				
				CString strSign = (dTemp > 0)?' ':'-';
				strSign = (dTemp == 0) ? " " : strSign;
				
				/////////////////////////////////////////////////////////////////////////////
				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 >> 
				// 				if(m_nIndustry == 1 && !((TempHeadType.strHeadName).Compare("현재가")))
				// 				{
				// 					TempHeadType.strDataType = "double";
				// 					nUnitNum = 100;
				// 				}
				// 				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 <<
				/////////////////////////////////////////////////////////////////////////////
				
				if (TempHeadType.strDataType == "int")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");
				if (TempHeadType.strDataType == "double")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
		}
	}
	
	//GetCell(0, nRowIndex, &cellItemDel);
	GetCell((int)COL_CODE, nRowIndex, &cellItemCode);
	GetCell((int)COL_NAME, nRowIndex, &cellItemName);
	
	int nCol_Offset;
	nCol_Offset = 2;
	
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);
	
	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
	cellItemCode.SetFont(&m_fontDefault);
	cellItemCode.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemCode.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellItemCode.SetText(szItemCode);
	cellItemCode.SetBackColor(clrBk);
	
	cellItemName.SetFont(&m_fontDefault);
	cellItemName.SetText(strItemName);
	cellItemName.SetBackColor(clrBk);
	cellItemName.m_string = strItemName;
	
	cellItemName.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherJongCode(strItemCode);
	cellItemName.SetOtherDataInt(atoi(&chMarkType));
	
	COLORREF clrSignText;
	nRealDataIndex = 0;
	nArrIndex = 0;
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		TempHeadType = m_plistField->GetAt(pos);
		
		nArrIndex = nCol * 16;
		
		if (TempHeadType.strDataType == "char")
			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
		else if ((TempHeadType.strDataType == "double") || 
			(TempHeadType.strDataType == "int"))
			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_NUMBER | UGCELLDATA_OTHER);
		
		if (TempHeadType.strAlignment == "center")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
		else if (TempHeadType.strAlignment == "left")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
		else if (TempHeadType.strAlignment == "right")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNRIGHT);
		
		if (TempHeadType.strDataType == "code")  //## CODE ##
		{
			if (!strItemCode.IsEmpty())
			{
				strItemCode.TrimRight();
				strItemCode.TrimLeft();
			}
			cellData[nCol + nCol_Offset].SetText(strItemCode);
			cellData[nCol + nCol_Offset].m_string = strItemCode;
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strItemCode);
		}
		else if (TempHeadType.strDataType == "candle")  //# 캔들 
		{
			//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
			
			memset(szTemp, NULL, sizeof(szTemp));
			memcpy(szTemp, &dcolData[nArrIndex], 16);
			
			strData[nRealDataIndex] = FindCandleIdFromName((long)atol(szTemp));
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "pattern")  //# 패턴
		{
			//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
			
			memset(szTemp, NULL, sizeof(szTemp));
			memcpy(szTemp, &dcolData[nArrIndex], 16);
			
			strData[nRealDataIndex] = FindPatenIdFromName(atol(szTemp));
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
			nRealDataIndex++;
		}
		else
		{
			if (TempHeadType.strUnitText.IsEmpty() == FALSE)
				strData[nRealDataIndex] += " " + TempHeadType.strUnitText;  //<= 단위
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			
			//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
			
			memset(szTemp, NULL, sizeof(szTemp));
			memcpy(szTemp, &dcolData[nArrIndex], 16);
			
			if(TempHeadType.strDataType == "debi" || TempHeadType.strDataType == "debiuel")
			{
				if(chSign == '4' || chSign == '5')
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * -100.0));
				else
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 100.0));
			}
			else
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atol(szTemp)));
			
			char szSign[2] = {0,};
			szSign[0] = chSign;
			int nSign = atoi(szSign);
			GetSignColor(nSign, clrSignText);
			if (TempHeadType.strDataType == "debi")    //## 대비 ##
			{
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
				cellData[nCol + nCol_Offset].SetSign(nSign);
			}
			else if (TempHeadType.strDataType == "debiuel")   //## 대비율 ##
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
			else if(TempHeadType.strHeadName == "현재가")
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
			
			nRealDataIndex++;
		}
		
		cellData[nCol + nCol_Offset].SetBackColor(clrBk);
		cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
	}
	
	/*
	cellItemDel.SetBackColor(clrBk);
	cellItemDel.SetBorderColor(&m_penGridLine);
	cellItemDel.SetBorder(UG_BDR_RTHIN);
	SetCell(0, nRowIndex, &cellItemDel);
	*/
	
	cellItemCode.SetBackColor(clrBk);
	cellItemCode.SetBorderColor(&m_penGridLine);
	cellItemCode.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_CODE, nRowIndex, &cellItemCode);
	
	cellItemName.SetBackColor(clrBk);
	cellItemName.SetBorderColor(&m_penGridLine);
	cellItemName.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_NAME, nRowIndex, &cellItemName);
	
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
		cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
		SetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);
	}
	
	if (bRedraw == true) 
		RedrawAll();
}

CString CGrid_Comm::FindCandleIdFromName(long lCandleID)
{
	CString strResult = "--";
	
	int i, nCount;
	POSITION pos;
	
	CandleInfo      TempCandleInfo;
	
	if (m_bCandleLoadFalg == FALSE)
		return strResult;
	
	nCount = m_listCnadle.GetCount() - 1;
	for (i = 0; i < nCount; i++)
	{
		pos = m_listCnadle.FindIndex(i);
		TempCandleInfo = m_listCnadle.GetAt(pos);
		
		if (TempCandleInfo.lID == lCandleID)
		{
			strResult = TempCandleInfo.strName;
			break;
		}
	}
	
	return strResult;
}

BOOL CGrid_Comm::LoadCandleIndexToXml()
{
	MSXML::IXMLDOMNodePtr		      pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	CandleInfo      TempCandleInfo;
	
	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;
	CString strIndexFilePath;
	//## Candle Index 파일 LOAD ##
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + CANDLE_INDEX_FILE;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	
	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	
	m_listCnadle.RemoveAll();
	try {
		//# header의 node를 얻는다.
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		int nCount, nLength;
		CString strTemp;
		
		nLength = pNS_FItem->length;
		for(nCount = 0; nCount < nLength; nCount++)
		{
			//# node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//# 속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			//##### Each Filed Information START #####
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempCandleInfo.lID = atol(strTemp);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			TempCandleInfo.strName = CString((PCHAR)pNAttributeTemp->text);
			//## Candle ID insert ##
			m_listCnadle.AddTail(TempCandleInfo);
		}
	}
	catch(...) {
		bResult = FALSE;
		return bResult;
	}
	
	return bResult;
}

CString CGrid_Comm::FindPatenIdFromName(long lPatenID)
{
	CString strResult = "--";
	
	int i, nCount;
	POSITION pos;
	
	PatenInfo      TempPatenInfo;
	
	// 	if (m_bPatenLoadFalg == FALSE)
	// 		return strResult;
	
	nCount = m_listPaten.GetCount() - 1;
	for (i = 0; i < nCount; i++)
	{
		pos = m_listPaten.FindIndex(i);
		TempPatenInfo = m_listPaten.GetAt(pos);
		
		if (TempPatenInfo.lID == lPatenID)
		{
			strResult = TempPatenInfo.strName;
			break;
		}
	}
	
	return strResult;
}

BOOL CGrid_Comm::LoadPatenIndexToXml()
{
	MSXML::IXMLDOMNodePtr		      pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	PatenInfo      TempPatenInfo;
	
	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;
	CString strIndexFilePath;
	//## Candle Index 파일 LOAD ##
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + PATEN_INDEX_FILE;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	
	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	
	m_listPaten.RemoveAll();
	try {
		//# header의 node를 얻는다.
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		int nCount, nLength;
		CString strTemp;
		
		nLength = pNS_FItem->length;
		for(nCount = 0; nCount < nLength; nCount++)
		{
			//# node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//# 속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			//##### Each Filed Information START #####
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempPatenInfo.lID = atol(strTemp);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			TempPatenInfo.strName = CString((PCHAR)pNAttributeTemp->text);
			//## Candle ID insert ##
			m_listPaten.AddTail(TempPatenInfo);
		}
	}
	catch(...) {
		bResult = FALSE;
		return bResult;
	}
	
	return bResult;
}

CSize CGrid_Comm::GetFontSize()
{
	CClientDC	dc(this);
	CFont*	pOldFont = dc.SelectObject(m_GI->m_defFont);
	
	CSize	sizeFont;
	TEXTMETRIC	tm;
	
	dc.GetTextMetrics(&tm);
	sizeFont.cx	=	tm.tmAveCharWidth;
	sizeFont.cy	=	tm.tmHeight;
	
	dc.SelectObject(pOldFont);
	
	return	sizeFont;
}

CString CGrid_Comm::GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;
	
	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";
		strData.Format(strDis, dData);
	}
	else
	{
		strDis = "%.0f";
		strData.Format(strDis, dData);
	}
	
	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;
		
		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
			strData = "0" + strData;
		
		int nCaretIndex = 0;
		for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
		{
			if ((CString)strFormat[nIndex] == "X")
			{
				strResult += (CString)strData[nCaretIndex];
				nCaretIndex++;
			}
			else
				strResult += (CString)strFormat[nIndex];
		}
	}
	else
	{
		int nCheckIndex = 0;
		BOOL bDotFlag = FALSE;
		nDotPos = strData.Find(".");
		
		int nStartIndex;
		nLength = strData.GetLength();
		if (nDotPos > 0)
		{
			nStartIndex = nDotPos;
			bDotFlag = TRUE;
		}
		else
		{
			nStartIndex = nLength;
			nDotPos = 0;
		}
		
		int nCaretIndex = 0;
		CString strResultInt;
		
		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
		{
			if (nCheckIndex == 3)
			{
				strResultInt += "," + (CString)strData[nIndex];
				nCheckIndex = 1;
			}
			else
			{
				strResultInt += (CString)strData[nIndex];
				nCheckIndex++;
			}
		}
		
		nLength = strResultInt.GetLength();
		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
			strResult += (CString)strResultInt[nIndex];
		
		if (nDotPos > 0)
			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
	}
	
	return strResult;
}

// CString CGrid_Comm::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
// {
// 	CString strResult = "";
// 	int nIndex, nLength, nLengthFormat, nDotPos;
// 
// 	CString strData, strDis, strTemp;
// 	if (nUnitNum > 1)
// 	{
// 		strTemp.Format("%d", nUnitNum);
// 		strTemp.Format("%d", strTemp.GetLength() - 1);
// 		strDis = "%." + strTemp + "f";
// 
// 		strData.Format(strDis, lData / (float)nUnitNum);
// 	}
// 	else
// 	{
// 		strDis = "%ld";
// 		strData.Format(strDis, lData);
// 	}
// 
// 	if (bFormatFlag == TRUE)
// 	{
// 		nLength = strData.GetLength() - 1;
// 		nLengthFormat = strFormat.GetLength() - 1;
// 
// 		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
// 			strData = "0" + strData;
// 
// 		int nCaretIndex = 0;
// 		for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
// 		{
// 			if ((CString)strFormat[nIndex] == "X")
// 			{
// 				strResult += (CString)strData[nCaretIndex];
// 				nCaretIndex++;
// 			}
// 			else
// 				strResult += (CString)strFormat[nIndex];
// 		}
// 	}
// 	else
// 	{
// 		int nCheckIndex = 0;
// 		BOOL bDotFlag = FALSE;
// 		nDotPos = strData.Find(".");
// 
// 		int nStartIndex;
// 		nLength = strData.GetLength();
// 		if (nDotPos > 0)
// 		{
// 			nStartIndex = nDotPos;
// 			bDotFlag = TRUE;
// 		}
// 		else
// 		{
// 			nStartIndex = nLength;
// 			nDotPos = 0;
// 		}
// 
// 		int nCaretIndex = 0;
// 		CString strResultInt;
// 
// 		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
// 		{
// 			if (nCheckIndex == 3)
// 			{
// 				if (strData[nIndex] != '-')
// 					strResultInt += "," + (CString)strData[nIndex];
// 				nCheckIndex = 1;
// 			}
// 			else
// 			{
// 				strResultInt += (CString)strData[nIndex];
// 				nCheckIndex++;
// 			}
// 		}
// 
// 		nLength = strResultInt.GetLength();
// 		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
// 			strResult += (CString)strResultInt[nIndex];
// 
// 		if (nDotPos > 0)
// 			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
// 	}
// 
// 	return strResult;
// }

inline void CGrid_Comm::GetSignColor(int nSign, COLORREF &clrSignText)
{
	switch (nSign)
	{
	case 1:		// 상한(화살표)
		clrSignText = RED;
	case 2:		// 상승(삼각형)
		clrSignText = RED;
		break;
	case 4:		// 하한(화살표)
		clrSignText = BLUE;
		break;
	case 5:		// 하락(삼각형)
		clrSignText = BLUE;
		break;
	case 3:		// 보합
	default:
		clrSignText = BLACK;
	}
}

int CGrid_Comm::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if(ID == m_nButtonIndex)	
	{
		if(msg == UGCT_BUTTONUP)
		{
			CUGCell cell;
			GetCell(col, row, &cell);
			
			// 			CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
			// 			pReWnd->m_saDelItemCode.Add(cell.GetOtherJongCode());
			DeleteRow(row);
		}
	}
	
	return TRUE;
}

CString CGrid_Comm::LoadMemo(CString strKey)
{
	if (strKey.IsEmpty())
		return "";
	
	strKey.TrimRight();
	strKey = "A" + strKey;
	
	CString dir, dat;
	char key[12], lBytes[4];
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	dir.Format("%s\\%s\\%s\\memo.mmo", pwndMainFrm->Variant(homeCC), "user", pwndMainFrm->Variant(nameCC));
	CFileFind finder;
	
	if (!finder.FindFile(dir))
		return "";
	
	TRY
	{ 
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}
					
					if (!strKey.Compare(key))
					{
						dat.ReleaseBuffer();
						return dat;
						break;
					}
					//160928 강수현: 0014541: [7127] 항목편집의 "메모" 추가후 현재가 화면에서 종목컨트롤 클릭하여 메모 작성시 조건검색에는 마지막에 추가한 종목의 메모만 나오는현상
					//return "";
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);
		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	return "";
}

void CGrid_Comm::SaveMemo(CString strKey, CString strData)
{
	if (strKey.IsEmpty())
		return;

	strKey = "A" + strKey;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();	
	CString rDir, wDir;
	rDir.Format("%s\\%s\\%s\\memo.mmo", pwndMainFrm->Variant(homeCC), "user", pwndMainFrm->Variant(nameCC));
	wDir.Format("%s\\%s\\%s\\memo.mm2", pwndMainFrm->Variant(homeCC), "user", pwndMainFrm->Variant(nameCC));
	
	char key[12], lBytes[4];
	CString dat;
	CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate);
	
	if (!strData.IsEmpty())
	{
		FillMemory(key, sizeof(key), ' ');
		FillMemory(lBytes, sizeof(lBytes), ' ');
		
		strcpy(key, strKey.operator LPCTSTR());
		itoa(strData.GetLength(), lBytes, 10);
		wFile.Write(key, 12);
		wFile.Write(lBytes, 4);
		wFile.Write(strData, strData.GetLength());
	}
	
	CFileFind finder;
	if (!finder.FindFile(rDir))
	{
		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);
		return;
	}
	
	TRY
	{
		CFile rFile(rDir, CFile::modeRead);
		
		UINT nBytesRead;
		do
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');
			
			nBytesRead = rFile.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
						break;
					
					if (!strKey.Compare(key))
					{
						dat.ReleaseBuffer();
						continue;
					}
					wFile.Write(key, 12);
					wFile.Write(lBytes, 4);
					wFile.Write(dat, nBytesRead);
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);
		rFile.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
		
		wFile.Close();
	CopyFile(wDir, rDir, false);
	DeleteFile(wDir);
	
}