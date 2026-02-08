// Grid_Result.cpp: implementation of the CGrid_Result class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid_Result.h"

#include "../resource.h"

#include  <math.h>
#include  <stdlib.h>

#include "print4Grid.h"
#include "../../Dr_Include/CompanySS.h"
#include "../MainFrmWnd.h"
#include "../ConditionResultWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		Head_Name[] = {_T("분"), _T("신"), _T("∨종목코드"), _T("종목명") };
const		LPCTSTR		Head_Name_Industry[] = { /*_T("지움"),*/ _T("업종코드"), _T("업종명") };
const		int				Head_Width[] = {0, 0,  70, 100 };

const		LPCTSTR		Head_Name_TarGet[] = { _T("코드"), _T("종목명") };
const		int			Head_Width_TarGet[] = { 80, 115 };

const   COLORREF	clrHeader		 = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine	 = COLOR_GRID_LINE;

const   int       MAX_COL = 30;

#define COL_DELBTN    0

#define COL_BUN      0
#define COL_SIN      1
#define COL_CODE      2
#define COL_NAME      3

//# 삼성증권
#define	UM_CHANGE_PARENT			(WM_USER + 450)		// 독립실행시 프레임 윈도우 변경을 통지하기위해...
														// Common\KHResource.h에 정의됨.

BEGIN_MESSAGE_MAP(CGrid_Result,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_COMMAND_RANGE(ID_MENU_START, ID_MENU_END, OnSelectMenu)

	//# 삼성증권, 유진증권
	ON_MESSAGE(UM_CHANGE_PARENT,	OnChangeChildFrameWindowHandle)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid_Result::CGrid_Result()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	
	m_bIndustry = FALSE;
	m_hChildFrame = NULL;
	
	m_nSortCol			= -1;
	m_nLastSelRow		= -1;
	m_nLastCol			= -1;
	m_nPrevRow		    = -1;

	m_pPrint			= new CPrint4Grid;
	m_pPrint->m_ctrl	= this;
	m_pPrint->m_GI		= m_GI;
	m_pBorderPen4Print  = NULL;
	m_nStyle4Print		= 0;
	m_pPrintTailFont	= NULL;

	SetPrintTailFont("굴림체", 600);
	
	//<< 07.07.10 이준옥
	m_pDragBitmap           = NULL;
	m_pDragImage			= NULL;
	m_bDragAndDrop			= FALSE;
	m_MouseMode				= MOUSE_NOTHING;
	m_bFocus				= FALSE;
	m_lDragRow				= -1;
	m_nDragCol				= 0;	// 종목명위치
	m_uGridNotifyControlMsg = ::RegisterWindowMessage("FN_GRID_NOTIFY_CONTROL_MSG");
	//>> 07.07.10 이준옥
	/// 키보드 방향키와 종목연동처리 2009 0423 이민형
	m_bIsKeyDown			= FALSE;
	m_clrHeader  = COLOR_GRID_LINE;
	m_clrSelRow  = COLOR_GRID_SEL_ROW;

	m_bSelectCheckFlag = TRUE;
}

CGrid_Result::~CGrid_Result()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_penGridLine.DeleteObject();

	if(m_pPrint != NULL)
		delete m_pPrint;

	if(m_pBorderPen4Print != NULL)
		delete m_pBorderPen4Print;

	if(m_pPrintTailFont != NULL)
		delete m_pPrintTailFont;

}

void CGrid_Result::OnDestroy()
{
	m_tipInfo.RemoveTool(this);
}

void CGrid_Result::SetInit()
{
	CWnd *pwndChildFrm = GetFrameWnd(this);
	m_hChildFrame = pwndChildFrm->GetSafeHwnd();
}

long CGrid_Result::OnChangeChildFrameWindowHandle(WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;
	
	return 0L;
}

//--> FrameWnd 핸들구하기 수정
CWnd* CGrid_Result::GetFrameWnd(CWnd* pWnd)
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

//CWnd* CGrid_Result::GetFrameWnd(CWnd* pWnd)
//{
//	CWnd* pParentWnd = pWnd->GetParent()->GetParent()->GetParent();
//	pParentWnd->SendMessage(RMSG_KSGETFRAMEWND, 0, 0);
//	return NULL;
//}
//<--

CSize CGrid_Result::GetFontSize()
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

void CGrid_Result::ClearAll()
{
	for (int nCount = 0; nCount < GetNumberRows(); nCount++)
		RemoveDataSource(nCount);
	
	while(GetNumberRows()) 
	{
		m_GI->m_defDataSource->DeleteRow(0);
		m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}
	
	SetNumberRows(0);
	QuickSetCellType(m_nSortCol, -1, 0);
	
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortCol = 0;
	
	m_nLastSelRow	= -1;
	m_bScreenMove = FALSE;
	
	SetLockColumns(0, FALSE);	
	
	m_bSelectCheckFlag = TRUE;

	RedrawAll();
}
const LPCSTR TYPE_MARKETCODE[] = {_T("0001"), _T("1001"), _T("2001"), _T("3001"), _T("4001")};
void CGrid_Result::SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
							 CString strItemName, char dcolData[][16], int nPrice, bool bRedraw)
{
	CString strStatus;
	CStringArray arrData;
	COLORREF clrBunText;
	COLORREF clrBunBack;
	
	strStatus = GetStockStatus(strItemCode, strItemName, clrBunText, clrBunBack);
	strItemName.TrimRight();

	int nCol;
	CString strData[MAX_COL];
//	CString strTemp;
	char szTemp[17];
	double dTemp;
	CUGCell cellItemDel, cellItemName, cellItemCode, cellData[MAX_COL];
	CUGCell cellItemBun, cellItemSin;
	
	int nRowIndex = nRow;
	COLORREF clrBk;
	GetEvenOddBkColor(nRowIndex, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;
	
	int nRealDataIndex = 0;
	if(m_plistField)		//070420금 Null체크루틴 추가.
	{
		for (nCol = 0; nCol < ntotCol; nCol++)
		{
			pos = m_plistField->FindIndex(nCol);
			TempHeadType = m_plistField->GetAt(pos);
			
			int nDataLength = TempHeadType.nDataLength;
			int nUnitNum = TempHeadType.nUnitNum;
			
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
				memcpy(szTemp, dcolData[nRealDataIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = (dTemp > 0) ? "+" : "-";
				strSign = (dTemp == 0) ? " " : strSign;
				strData[nRealDataIndex] = /*strSign +*/ GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "debiuel")  //## 대비율 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, dcolData[nRealDataIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = _T("");
				if(chSign == '4' || chSign == '5')
					strSign = "-";
				else
					strSign = " ";		
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "date")  //## 날짜 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, dcolData[nRealDataIndex], 16);
				dTemp = atof(szTemp);

				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "time")  //## 시간 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, dcolData[nRealDataIndex], 16);
				dTemp = atof(szTemp);

				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
				nRealDataIndex++;
			}
			else		//# int, double
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
	
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, dcolData[nRealDataIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = (dTemp > 0)?' ':'-';
				strSign = (dTemp == 0) ? " " : strSign;
				
				if (TempHeadType.strDataType == "int")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");

				if (TempHeadType.strDataType == "double")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
		}
	}
	
	//GetCell(0, nRowIndex, &cellItemDel);
	GetCell((int)COL_BUN, nRowIndex, &cellItemBun);
	GetCell((int)COL_SIN, nRowIndex, &cellItemSin);

	GetCell((int)COL_CODE, nRowIndex, &cellItemCode);
	GetCell((int)COL_NAME, nRowIndex, &cellItemName);
	
	int nCol_Offset;
	nCol_Offset = 4;
	
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);


	cellItemBun.SetFont(&m_fontDefault);
	cellItemBun.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemBun.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	if(strStatus != "")
	{	
		cellItemBun.SetText(strStatus);
		cellItemBun.SetBackColor(clrBunBack);
		cellItemBun.SetTextColor(clrBunText);
	}
	cellItemBun.SetReadOnly(TRUE);

	cellItemSin.SetFont(&m_fontDefault);
	cellItemSin.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemSin.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellItemSin.SetText(strStatus);
	cellItemSin.SetBackColor(clrBunBack);	
	cellItemSin.SetTextColor(clrBunText);
	cellItemSin.SetReadOnly(TRUE);
	
	cellItemCode.SetFont(&m_fontDefault);
	cellItemCode.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);

	//>>[20110826alzioyes]0000135: 조건검색 결과 리스트에 체크박스 및 기능 추가
	cellItemCode.SetCellType(UGCT_CHECKBOX);
	/*cellItemCode.SetCellTypeEx(UGCT_CHECKBOX3DRAISED);*/
	cellItemCode.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
	cellItemCode.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	//cellItemCode.SetText(strItemCode);
	cellItemCode.SetText("1");
	cellItemCode.SetLabelText(strItemCode);
	cellItemCode.SetBackColor(clrBk);
	//cellItemCode.SetReadOnly(TRUE);	//ReadOnly속성을 빼줘야 체크를 사용할 수 있다.
	//<<0000135

	//cellItemCode.m_string = strItemName;
	//cellItemCode.SetOtherDataCellText(strItemName);
	
	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);

	cellItemName.SetFont(&m_fontDefault);
	cellItemName.SetText(strItemName);
	cellItemName.SetBackColor(clrBk);
	cellItemName.m_string = strItemName;
	
	cellItemName.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherJongCode(strItemCode);
	cellItemName.SetOtherDataInt(atoi(&chMarkType));
	cellItemName.SetReadOnly(TRUE);
	
	COLORREF clrSignText;
	nRealDataIndex = 0;
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		TempHeadType = m_plistField->GetAt(pos);
		
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

		cellData[nCol + nCol_Offset].SetReadOnly(TRUE);
		
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
			memcpy(szTemp, dcolData[nRealDataIndex], 16);

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
			memcpy(szTemp, dcolData[nRealDataIndex], 16);

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
		else if(TempHeadType.lHeadID == 9072 || TempHeadType.lHeadID == 9073) // 일봉, L일봉H
		{
			CString strCandleData;
			CString strTemp;
						
			memset(szTemp, NULL, sizeof(szTemp));	// 전일종가
			memcpy(szTemp, dcolData[56], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;
			
			memset(szTemp, NULL, sizeof(szTemp));	// 시
			memcpy(szTemp, dcolData[57], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 고
			memcpy(szTemp, dcolData[58], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 저
			memcpy(szTemp, dcolData[59], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			strTemp.Format("%d|", nPrice);			// 종(현재가)
			strCandleData += strTemp;

			if(TempHeadType.lHeadID == 9072)
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
		else if(TempHeadType.lHeadID == 8003) // 삭제
		{
			m_nButtonIndex = AddCellType(&m_ugButtonType);
			cellData[nCol + nCol_Offset].SetText(_T("X"));
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
			cellData[nCol + nCol_Offset].SetCellType(m_nButtonIndex);
			cellData[nCol + nCol_Offset].SetCellTypeEx(UGCT_BUTTONNOFOCUS);
			cellData[nCol + nCol_Offset].SetBackColor(GetSysColor(COLOR_BTNFACE));
			cellData[nCol + nCol_Offset].SetOtherJongCode(strItemCode);

			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 9033) // 업종구분
		{
			CString strUpjongCode;
			strUpjongCode.Format("%3d", _ttoi(strData[nRealDataIndex]));
			strUpjongCode.Replace(' ', '0');
			
			CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
			CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
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
			CString strNote;
			if (TempHeadType.strUnitText.IsEmpty() == FALSE)
				strData[nRealDataIndex] += " " + TempHeadType.strUnitText;  //<= 단위
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			if(TempHeadType.lHeadID == 8002) // 메모
			{
				strNote = LoadMemo(strItemCode.Mid(0, 6));

				cellData[nCol + nCol_Offset].SetText(strNote);
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
				cellData[nCol + nCol_Offset].SetReadOnly(FALSE);
				cellData[nCol + nCol_Offset].SetOtherJongCode(strItemCode);
				cellData[nCol + nCol_Offset].SetOtherDataCellText(strItemName);
			}
			else 
			{
				if(atof(strData[nRealDataIndex]) == 0)
					cellData[nCol + nCol_Offset].SetText("");
				else
					cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);

				cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			}

//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

			memset(szTemp, NULL, sizeof(szTemp));
			memcpy(szTemp, dcolData[nRealDataIndex], 16);

			if(TempHeadType.strDataType == "debi" || TempHeadType.strDataType == "debiuel")
			{
				if(chSign == '4' || chSign == '5')
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * -100.0));
				else
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 100.0));
			}
			else
			{
				if(TempHeadType.strDataType == "int")
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atol(szTemp)));
				else
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 100.0));
			}
				
			
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


	cellItemBun.SetBorderColor(&m_penGridLine);
	cellItemBun.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_BUN, nRowIndex, &cellItemBun);


	cellItemSin.SetBorderColor(&m_penGridLine);
	cellItemSin.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_SIN, nRowIndex, &cellItemSin);
	
	cellItemCode.SetBackColor(clrBk);
	cellItemCode.SetBorderColor(&m_penGridLine);
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	if(pReWnd->m_nFixCol == 1)
		cellItemCode.SetBorder(1);
	else
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

CString CGrid_Result::GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
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

CString CGrid_Result::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
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


void CGrid_Result::GotoLastRow()
{
	int nRowIndex = GetNumberRows() - 1;
	GotoRow(nRowIndex);
	GotoRow(nRowIndex - 19);
	
	RedrawAll();
}

// ###############################################################################
// Event를 처리하는 함수들입니다.
void CGrid_Result::OnSetup(BOOL bIndustry, CString strType)
{
	m_bIndustry = bIndustry;
	m_strType = strType;
	
	SetSH_Width(0);
	SetSH_NumberCols(0);
	SetHighlightRow(FALSE);
	//EnableColSwapping(FALSE);
	
	CSize	szFont = GetFontSize();
	SetDefRowHeight(18);
	SetTH_Height(20);
	
	//## Font Size Set ##
	m_nFontHeight = szFont.cy; 
	m_nFontWidth = szFont.cx; 
	
	SetCancelMode(FALSE);
	EnableMenu(TRUE);
	/*
	PrintSetOption(UG_PRINT_TOPHEADING,TRUE);
	PrintSetOption(UG_PRINT_LEFTMARGIN,10);
	PrintSetOption(UG_PRINT_RIGHTMARGIN,10);
	*/
	EnableUpdate(TRUE);
	UseHints(TRUE);
	
	m_tipInfo.Create(this, "BASE", 1);
	m_tipInfo.SetFont(&m_fontDefault);
	m_tipInfo.AddTool(this, (LPCTSTR)" ");
	
	EnableExcelBorders(FALSE);
	
	/*	
	//## Move Head Text Set ##
	HeadMoveAllDelete();
	
	  //## Fix Head Text Set ##
	  HeadFixAllDelete();
	  HeadFixInsert();
	  
		//## Head Text Setting ##
		int nOutFieldCnt;
		nOutFieldCnt = HeaderDataSetup();
		HeaderSetup(nOutFieldCnt);
		
		  LockColumns(1);
		  
			//## Field XML Info Load ##
			LoadMoveSysFiledXML();
			LoadMoveUserFiledXML();
			LoadFixSysFiledXML();
			
	*/
	//## Candle File Load ##
	if(m_strType != "대상변경")
		m_bCandleLoadFalg = LoadCandleIndexToXml();
	//m_bPatenLoadFalg = LoadPatenIndexToXml();
	
	//OnSysColorChange();
}

int CGrid_Result::OnCanSizeTopHdg()
{
	return FALSE;
}

void CGrid_Result::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	// If Mouse LButton Down 
	if (updn && !processed)
	{
		//{
		//	CString szDebug;
		//	szDebug.Format("[SMalzio]OnTH_LClicked. col(%d).", col);
		//	OutputDebugString(szDebug);
		//}
		int nCodeCol = GetCodeColNumber(1);

		if ((row == -1) && (col == nCodeCol) && point->x<=10)
		{
			CUGCell cell;
			m_bSelectCheckFlag = !m_bSelectCheckFlag;

			for (int nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(nCodeCol, nRow, &cell);
				if (cell.GetCellType() == UGCT_CHECKBOX)
				{
					if (m_bSelectCheckFlag == TRUE)
						QuickSetText(nCodeCol, nRow, "1");
					else
						QuickSetText(nCodeCol, nRow, "0");
				}
			}
			RedrawAll();
			return;

			//CWnd* pwndParent = GetParent();       //# CMainWnd
			//pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_HEADERCLK, (LPARAM)NULL);
		}

		// Clear Arrow
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
		
		m_nLastSelRow	= -1;
	}
}

int CGrid_Result::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
	CString strCell1, strCell2;
	
	if (m_nSortType[m_nSortCol] == 1)
	{
		CString text1;
		cell1->GetLabelText(&text1);
		if(text1.GetLength())
		{
			CString text2;
			cell2->GetLabelText(&text2);
			nResult = text1.Compare(text2);
		}
		else
		{
			//# Is Just String
			nResult = cell1->m_string.Compare(cell2->m_string);
		}
	}
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

void CGrid_Result::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor( m_clrHeader );
}

void CGrid_Result::OnRowChange(long oldrow, long newrow)
{
/*
if (m_nLastSelRow == newrow)
return;
	*/
	
	int nCount, nLength;
	COLORREF clrBk;
	CUGCell	cell;
	
	nLength = GetNumberCols();
	for (nCount = 0; nCount < nLength; nCount++)
	{
		if((m_strType != "대상변경") && (nCount == COL_BUN || nCount == COL_SIN))
			continue;

		if (oldrow != -1)
		{
			GetEvenOddBkColor(oldrow, clrBk);
			
			GetCell(nCount, oldrow, &cell);
			cell.SetBackColor(clrBk);
			SetCell(nCount, oldrow, &cell);
		}
		
		if (newrow != -1)
		{
			GetCell(nCount, newrow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCount, newrow, &cell);
		}
	}
	
	m_nLastSelRow = newrow;
	
	if (m_bScreenMove == TRUE)
	{
		Invalidate();
		m_bScreenMove = FALSE;
	}
	else
		RedrawAll();
	
	if(m_bIsKeyDown)
	{/// 키보드 방향키와 종목연동처리 2009 0423 이민형
		m_bIsKeyDown = FALSE;
		SetSharedData(newrow);
	}
}

void CGrid_Result::OnKeyDown(UINT *vcKey,BOOL processed)
{/// 키보드 방향키와 종목연동처리 2009 0423 이민형	

	if(VK_DOWN != *vcKey && VK_UP != *vcKey)		return;
	m_bIsKeyDown = TRUE;
}

void CGrid_Result::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
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
		
 		OnRowChange(m_nLastSelRow, row);
 		m_nLastCol = col;
 		m_nPrevRow = m_nLastSelRow;

		GetCell(COL_NAME, row, &cell);
		CString strCode;
		strCode = cell.GetOtherJongCode();
		
		if(m_strType == "대상변경")
			return;

		CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
		pReWnd->OnDBClickResultGrid(strCode);
		//SetSharedData(row);
	}
}

int CGrid_Result::OnCanViewMove(int oldcol, long oldrow, int newcol, long newrow)
{
	m_bScreenMove = TRUE;
	
	return TRUE;
}

void CGrid_Result::SetSharedData(int nRow)
{
	//# Code 공유 발생!
	ShardData	stShardData;
	stShardData.bOpen = FALSE;
	stShardData.strScreenNo = "종목공유";
	stShardData.strMapNo = "";
	
	CUGCell cell;
	GetCell(COL_NAME, nRow, &cell);
	stShardData.iaMarkType.Add(cell.GetOtherDataInt());
	stShardData.saCode.Add(cell.GetOtherJongCode());
	
	CWnd* pwndParent = GetParent();		//# CSubMainRightView
	pwndParent->SendMessage(WM_NOTIFY_GRIDEVENT, (WPARAM)NM_SENDDATA_SHARD, (LPARAM)&stShardData);
}


//#include "..\..\..\Common\KHResource.h"
//extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );	-hidden-
//extern void    KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);		-hidden-
void CGrid_Result::SetCommMenu(POINT *point)
{
	if (m_hChildFrame == NULL)
		return;
	
	long lMenu = 0; //::SendMessage(m_hChildFrame, UM_GET_SCREENLINKMENU, 0, 0 ); -hidden-
	if ( lMenu > 0 )
	{
		CMenu* pMenu = (CMenu*)lMenu;
		ASSERT ( pMenu );
		
		SetCommMenu_Add(m_bIndustry, pMenu);
		
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
			
			CString strCode = "";
			CUGCell cell;
			GetCell((int)COL_CODE, m_nLastSelRow, &cell);
			strCode = cell.GetText();
			strCode.TrimRight(" ");
			
			if (strScreenNo.IsEmpty() == TRUE)
			{
				//KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
				// 종목코드가 여러개일 경우에는 !를 구분으로 해서 종목리스트를 만든다.
				// 마지막 종목다음에는 !를 넣지않아야 한다.
				// 관심설정창 MenuID : UM_PORTFOLIO_REG = 4316
//-->@메인 관심관련 수정
//				if (strCode.IsEmpty() == FALSE)
//					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
//				::SendMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);

				if(nIndex_SelMenu==4316)
				{
					int nRowCnt = GetNumberRows();
					CString szTmp, strCode = "";
					CUGCell cell;
					for (int nRow = 0; nRow < nRowCnt; nRow++)
					{
						GetCell((int)COL_CODE, nRow, &cell);
						szTmp = cell.GetText();
						if(nRow != nRowCnt-1)
						{
							strCode += szTmp + "!"; 
						}
						else
						{
							strCode += szTmp; 
						}
					}
//					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);		-hidden-
				}
				else
				{
// hidden
//					if (strCode.IsEmpty() == FALSE)
//						KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
				}
				//::SendMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
				::PostMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
//<--
			}
			else
			{
				if (strCode.IsEmpty() == FALSE)
				{
// -hidden-
//					if (m_bIndustry == TRUE)
//						KSLink_SetLinkTagData("UPCODE", strCode);		// 외부화면용
//					else
//						KSLink_SetLinkTagData("JCODE", strCode);		// 외부화면용
				}
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)4, (LPARAM)(LPCTSTR)strScreenNo);
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)strScreenNo.GetLength(), (LPARAM)(LPCTSTR)strScreenNo);
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo); -hidden-
			}
		}
	}
}
	
CString CGrid_Result::GetCommMenu_IndexToScreenNo(int nIndex_SelMenu)
{
//	USE_SS_HTS();
	
	CString strScreenNo = "";
	int nIndex;
	if (m_bIndustry == TRUE)
	{
		for (nIndex = 0; nIndex < _countof(MENU_INDUSTRY_INDEX); nIndex++)
		{
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
			{
				if ((int)MENU_INDUSTRY_INDEX[nIndex] == nIndex_SelMenu)
					strScreenNo = MENU_INDUSTRY_SCREEN_NOMAX[nIndex];
			}
//			else
//			{
//				if ((int)MENU_INDUSTRY_INDEX[nIndex] == nIndex_SelMenu)
//					strScreenNo = MENU_INDUSTRY_SCREEN_NOEMP[nIndex];
//			}
//<--
		}
	}
	else
	{
		for (nIndex = 0; nIndex < _countof(MENU_INDEX); nIndex++)
		{
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
			{
				if ((int)MENU_INDEX[nIndex] == nIndex_SelMenu)
				{
					strScreenNo = MENU_SCREEN_NOMAX[nIndex];
					break;
				}
			}
//			else
//			{
//				if ((int)MENU_INDEX[nIndex] == nIndex_SelMenu)
//				{
//					strScreenNo = MENU_SCREEN_NOEMP[nIndex];
//					break;
//				}
//			}
//<--
		}
	}
	
	return strScreenNo;
}

void CGrid_Result::SetCommMenu_Add(BOOL bIndustry, CMenu* pMenu)
{
//	USE_SS_HTS();

	int nMenuCnt;
	UINT nMenuType;
	
	//pMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)"");
	
	if (bIndustry == TRUE)
	{
		nMenuCnt = _countof(MENU_INDUSTRY_NAME);
		CString strScreenInfo;
		for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
		{
			if (DRMENU_NAME[nIndex] == "")
				nMenuType = MF_SEPARATOR;
			else 
				nMenuType = MF_STRING;
	
//--> HTS구분없앰. 사용하면 추후 다시
//			if(bIsHTS)
			{
				strScreenInfo = (CString)MENU_INDUSTRY_SCREEN_NOMAX[nIndex];
				strScreenInfo = strScreenInfo.Left(4);
			}
//			else
//			{
//				strScreenInfo = (CString)MENU_INDUSTRY_SCREEN_NOEMP[nIndex];
//				strScreenInfo = strScreenInfo.Left(5);
//			}
//<--
			strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_INDUSTRY_NAME[nIndex];
			pMenu->AppendMenu(nMenuType, MENU_INDUSTRY_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
		}
	}
	else
	{
//--> @080610. 유진 추가메뉴 삭제
//		nMenuCnt = _countof(DRMENU_NAME);
//		CString strScreenInfo;
//		for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
//		{
//			if (DRMENU_NAME[nIndex] == "")
//				nMenuType = MF_SEPARATOR;
//			else 
//				nMenuType = MF_STRING;
//
//			if(bIsHTS)
//			{
//				strScreenInfo = (CString)MENU_SCREEN_NOMAX[nIndex];
//				strScreenInfo = strScreenInfo.Left(4);
//			}
//			else
//			{
//				strScreenInfo = (CString)MENU_SCREEN_NOEMP[nIndex];
//				strScreenInfo = strScreenInfo.Left(5);
//			}
//
//			strScreenInfo = "[" + strScreenInfo + "] " + (CString)DRMENU_NAME[nIndex];
//			pMenu->AppendMenu(nMenuType, MENU_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
//		}
	}
}

void CGrid_Result::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1))
	{
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
		//SetCommMenu(point);
		if(m_strType == "대상변경")
			return;

		CWnd*		pModuleMainWnd;
		CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
		pModuleMainWnd = pReWnd->GetModuleMainWnd();
		GetCursorPos(point);
		ScreenToClient(point);
		ClientToScreen(point);
		
		CString strCode = "";
		CUGCell cell;
		GetCell((int)COL_NAME, m_nLastSelRow, &cell);
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");

		CString strLPARM;
		strLPARM.Format("%s\n%d\t%d\n1301\t%s\n", "IB712700", point->x, point->y, strCode);
		// mapN \n pt.x \t pt.y \n SYM \t DATA \n
		pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<
	}
}

void CGrid_Result::OnSelectMenu(UINT nIndex)
{
//	USE_SS_HTS();
	int nItem;
	if ((nIndex >= ID_MENU_START) && (nIndex <= ID_MENU_END))
	{
		for (nItem = 0; nItem < _countof(MENU_INDEX); nItem++)
		{
			if (MENU_INDEX[nItem] == nIndex)
				break;
		}
		
		ShardData	stShardData;
		stShardData.bOpen = TRUE;
//--> HTS구분없앰. 사용하면 추후 다시
//		if(bIsHTS)
		{
			stShardData.strScreenNo = MENU_SCREEN_NOMAX[nItem];
		}
//		else
//		{
//			stShardData.strScreenNo = MENU_SCREEN_NOEMP[nItem];			
//		}
//<--
		stShardData.strMapNo = MENU_MAP_NO[nItem];
		
		CUGCell cell;
		GetCell(1, m_nLastSelRow, &cell);
		stShardData.iaMarkType.Add(cell.GetOtherDataInt());
		stShardData.saCode.Add(cell.GetOtherJongCode());
		
		CWnd* pwndParent = GetParent();		//# CSubMainRightView
		pwndParent->SendMessage(WM_NOTIFY_GRIDEVENT, (WPARAM)NM_SENDDATA_SHARD, (LPARAM)&stShardData);
	}
	/*
	else if (nIndex == 10200)
	SetFinderInternetWord(m_stSelectPosData.strData);
	*/
}

BOOL CGrid_Result::PreTranslateMessage(MSG* pMsg) 
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
//		FnMouseMove(point);

		//D&D할때 스크롤 현상이 있어서 방지하기 위해서 return 1L함.
		if(m_MouseMode == MOUSE_CODE_DRAG)	return 1L;
	}
	//>> 07.07.10 이준옥
	else if( pMsg->message == WM_RBUTTONUP)
	{
		CPoint point = pMsg->pt;
		FnRButtonUp(point);
	}
	else if( pMsg->message == WM_KEYDOWN)
	{
	
		if(VK_RETURN == pMsg->wParam)			
			return TRUE;

		ContinueEdit(m_nLastCol, m_nLastSelRow);
	}
	

	return CWnd::PreTranslateMessage(pMsg);
}

void CGrid_Result::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
// 20110118 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능 >>
//	SetSharedData(row);

	CUGCell cell;
	GetCell(COL_NAME, row, &cell);
	CString strCode;
	strCode = cell.GetOtherJongCode();

	if(m_strType == "대상변경")
		return;

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	pReWnd->OnDBClickResultGrid(strCode);
	// 20110118 박찬모 <<
}

//extern CString dll_GetStockDetailInfo(CString strCode, int nType);	-hidden-
int CGrid_Result::OnHint(int col, long row, int section, CString *string)
{
	if ((int)COL_NAME != col)
		return FALSE;
	
	if (m_bIndustry == FALSE)
		SetHint_Code(row);
	
	return FALSE;
}

//extern CString dll_GetStockDetailInfo(CString strCode, int nType);	-hidden-
void CGrid_Result::SetHint_Code(int nRow)
{
	CString strTip;
	CUGCell cell;
	GetCell((int)COL_NAME, nRow, &cell);
	
	if (m_tipInfo.GetShowState() == FALSE)
	{
		CString strCode;
		strCode = cell.GetOtherJongCode();
		
		strCode.TrimRight(" ");
		if (strCode.IsEmpty() == TRUE)
			return;
		
		int nMarket;
		nMarket = cell.GetOtherDataInt();
		
		strTip = ""; //dll_GetStockDetailInfo(strCode, nMarket); -hidden-
		
		m_tipInfo.UpdateToolTip(this, strTip, 1);
		m_tipInfo.Show(strTip);
	}
}

inline void CGrid_Result::GetSignColor(int nSign, COLORREF &clrSignText)
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

inline void CGrid_Result::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CGrid_Result::EvenOddColorSetting()
{
	long nCount, nRows;
	int  nCount2, nCols;
	
	COLORREF clrBk;
	CUGCell cell;
	
	nRows = GetNumberRows();
	nCols = GetNumberCols();
	
	for (nCount = 0; nCount < nRows; nCount++)
	{
		GetEvenOddBkColor(nCount, clrBk);
		
		for (nCount2 = 0; nCount2 < nCols; nCount2++)
		{
			GetCell(nCount2, nCount, &cell);
			cell.SetBackColor(clrBk);
			
			//## Line Color ##
			cell.SetBorderColor(&m_penGridLine);
			cell.SetBorder(UG_BDR_RTHIN);
			
			SetCell(nCount2, nCount, &cell);
		}
	}
	
	//  SetLockColumns(m_nFixCol, FALSE); 
}

// ###############################################################################

//## RealFinder2 : 2002.05 #######################################################

//##### Fix Head Information 관련 Function #####
BOOL CGrid_Result::HeadFixInsert()
{
	HeadFixInfo       TempHeadFixInfo;
	HeadInfoType      TempHeadType;
	
	MSXML::IXMLDOMNodePtr		      pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;
	
	CString strIndexFilePath;
	//## 사용자 필드 편집 파일 LOAD ##
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));
	
	CString strUserPath;
	strUserPath.Format("%s%s%s", strAppPath, USRDIR, pwndMainFrm->Variant(nameCC));
	
	if(pwndMainFrm->m_nSeparateForm == 0)
		strIndexFilePath = strAppPath + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_FILE;
	else
		strIndexFilePath = strAppPath + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_EXCAVATION;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	
	m_nNumFixField = 0;	//! 2004. 12. 4 추가
	
	// XML 파일을 로드하는데 실패하였다면
	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	
	try {
		// header의 node를 얻는다.
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		int nCount, nLength;
		CString strTemp;
		
		nLength = pNS_FItem->length;
		for(nCount = 0; nCount < nLength; nCount++)
		{
			// node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			// 속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			//##### Each Filed Information START #####
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempHeadFixInfo.stHeadType.lHeadID = atol(strTemp);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			TempHeadFixInfo.stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("VIEW_NAME");
			TempHeadFixInfo.stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT");
			TempHeadFixInfo.stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempHeadFixInfo.stHeadType.nUnitNum = atoi(strTemp);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			TempHeadFixInfo.stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			TempHeadFixInfo.stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempHeadFixInfo.stHeadType.nDataLength = atoi(strTemp);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempHeadFixInfo.stHeadType.nColWidth = atoi(strTemp);
			
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			TempHeadFixInfo.stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//##### Each Filed Information END #####
			
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_NO");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempHeadFixInfo.nColNum = atoi(strTemp);		
			
			pNAttributeTemp = pNA_FItem->getNamedItem("VISIBLE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			if (strTemp == "true") {
				TempHeadFixInfo.bView = TRUE;		
			}
			else if (strTemp == "false")
				TempHeadFixInfo.bView = FALSE;
			
			BOOL bInsertFalg = TRUE;
			/*      if ((TempHeadFixInfo.bView == FALSE) &&
			(TempHeadFixInfo.stHeadType.strDataType == "code"))
			bInsertFalg = FALSE;
			*/      
			if (bInsertFalg == TRUE)
				m_plistFixHead->AddTail(TempHeadFixInfo);
		}
	}
	catch(...) {
		bResult = FALSE;
		return bResult;
	}
	
	return bResult;
}

void CGrid_Result::HeadFixAllDelete()
{
	m_plistFixHead->RemoveAll();
}

BOOL CGrid_Result::HeadFixJongCodeFlag()
{
	BOOL bFlag = FALSE;
	
	POSITION pos;
	HeadFixInfo       TempHeadFixInfo;
	//HeadInfoType      TempHeadType;
	
	int nCount = m_plistFixHead->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pos = m_plistFixHead->FindIndex(i);
		TempHeadFixInfo = m_plistFixHead->GetAt(pos);
		
		if ((TempHeadFixInfo.stHeadType.strDataType == "code")
			&& (TempHeadFixInfo.bView == TRUE))
		{
			bFlag = TRUE;
			break;
		}
	}
	
	return bFlag;
}

int CGrid_Result::HeadFixViewCount()
{
	POSITION pos;
	HeadFixInfo       TempHeadFixInfo;
	
	int nResultCount = 0;
	int nCount = m_plistFixHead->GetCount();
	
	for(int i = 0; i < nCount; i++)
	{
		pos = m_plistFixHead->FindIndex(i);
		TempHeadFixInfo = m_plistFixHead->GetAt(pos);
		
		if ((TempHeadFixInfo.stHeadType.strDataType == "code")
			&& (TempHeadFixInfo.bView == TRUE))
			nResultCount++;
	}
	
	return nResultCount;
}


//##### Move Head Information 관련 Function #####
void CGrid_Result::HeadMoveInsert(HeadMoveInfo stHeadInfo)
{
	m_plistMoveHead->AddTail(stHeadInfo);
}

void CGrid_Result::HeadMoveDelete(CString strElement)
{/*
 POSITION posElement;
 
   posElement = GetElementToMovePosition(strElement);
   m_plistMoveHead->RemoveAt(posElement);
   
	 int nCount = m_plistMoveHead->GetCount();
	 
	   POSITION pos;
	   HeadMoveInfo  TempHeadMoveInfo;
	   for(int i=0; i<nCount; i++)
	   {
	   pos = m_plistMoveHead->FindIndex(i);
	   TempHeadMoveInfo = m_plistMoveHead->GetAt(pos);
	   
		 if (TempHeadMoveInfo.nIndex == nResultGridIndex)
		 {
		 m_plistMoveHead->RemoveAt(pos);
		 break;
		 }
		 }
	*/
}

POSITION CGrid_Result::GetElementToMovePosition(CString strElement)
{
	POSITION posResult = NULL;
	
	/*
	POSITION pos;
	HeadMoveInfo  tempHeadMoveInfo;
	
	  strElement.TrimLeft();
	  strElement.TrimRight();
	  strElement.MakeUpper();
	  
		int nCount = m_plistMoveHead->GetCount();
		for(int i = 0; i < nCount; i++)
		{
		pos = m_plistMoveHead->FindIndex(i);
		tempHeadMoveInfo = m_plistMoveHead->GetAt(pos);
		
		  tempHeadMoveInfo.strElement.TrimLeft();
		  tempHeadMoveInfo.strElement.TrimRight();
		  tempHeadMoveInfo.strElement.MakeUpper();
		  
			if (tempHeadMoveInfo.strElement == strElement)
			{
			posResult = pos;
			break;
			}
			}
	*/
	
	return posResult;
}



void CGrid_Result::HeadMoveAllDelete()
{
	m_plistMoveHead->RemoveAll(); 
}

void CGrid_Result::HeadMoveChange(int FrontIndex, int AfterIndex)

{
	//## BOOL bState Information ##
	//  FrontIndex > AfterIndex => TRUE
	//  FrontIndex < AfterIndex => FALSE
	
	/*
	HeadMoveInfo  TempHeadMoveInfo;
	TempHeadMoveInfo = m_plistMoveHead->GetAt(posFront);
	
	  if (bState == TRUE)//  (FrontIndex > AfterIndex)
	  //기준(AfterIndex) 정보 뒤에 
	  m_plistMoveHead->InsertBefore(posAfter, TempHeadMoveInfo);
	  else if (bState == FALSE)//(FrontIndex < AfterIndex)
	  //기준(AfterIndex) 정보 뒤에 
	  m_plistMoveHead->InsertAfter(posAfter, TempHeadMoveInfo);
	  
		m_plistMoveHead->RemoveAt(posFront);
	*/
	
	
	int nCount = m_plistMoveHead->GetCount();
	TRACE("\n# Grid_Result MoveHead Count : %d", nCount);
	
	int i;
	POSITION posFront, posAfter;
	HeadMoveInfo  TempFront, TempAfter;
	
	posFront = m_plistMoveHead->FindIndex(FrontIndex);
	TempFront = m_plistMoveHead->GetAt(posFront);
	
	if (FrontIndex > AfterIndex)
	{
		for(i = FrontIndex; i > AfterIndex; i--)
		{
			posFront = m_plistMoveHead->FindIndex(i - 1);
			TempAfter = m_plistMoveHead->GetAt(posFront);
			
			posAfter = m_plistMoveHead->FindIndex(i);
			m_plistMoveHead->SetAt(posAfter, TempAfter);
		}
	}
	else if (FrontIndex < AfterIndex)
	{
		for(i = FrontIndex; i < AfterIndex; i++)
		{
			posFront = m_plistMoveHead->FindIndex(i + 1);
			TempAfter = m_plistMoveHead->GetAt(posFront);
			
			posAfter = m_plistMoveHead->FindIndex(i);
			m_plistMoveHead->SetAt(posAfter, TempAfter);
		}
	}
	
	posAfter = m_plistMoveHead->FindIndex(AfterIndex);
	m_plistMoveHead->SetAt(posAfter, TempFront);
}


int CGrid_Result::HeaderDataSetup(BOOL bBackTest, int nSearchType, int nStepCnt)
{
	//## Real View Field Count ##
	
	POSITION        pos;
	HeadInfoType    TempHeadType;
	CList<HeadInfoType, HeadInfoType&> tempListHeadInfoType;
	
	HeadMoveInfo    TempHeadMoveInfo;
	HeadFixInfo     TempHeadFixInfo;
	
	int			    nCol, nCount;
	CUGCell	    cell;
	
	m_plistField->RemoveAll();
	
	//# Add Back Test Val View Files
	m_nNumFixField = (bBackTest == TRUE) ? m_plistFixHead->GetCount() : m_plistFixHead->GetCount() - 1;
	//##### Fix Header Info Set #####
	int nFixFieldCnt = 0;
	TRACE("\n# View Fix Head Total Data View");
	for (nCount = 0; nCount < m_nNumFixField; nCount++)
	{
		pos = m_plistFixHead->FindIndex(nCount);
		TempHeadFixInfo = m_plistFixHead->GetAt(pos);
		
		if (TempHeadFixInfo.bView == TRUE)
		{
			TRACE("\n   # Index : %d", nFixFieldCnt);
			TRACE("\n   # Head Name : %s", TempHeadFixInfo.stHeadType.strHeadName);
			
			TempHeadType.lHeadID      = TempHeadFixInfo.stHeadType.lHeadID;
			TempHeadType.nColWidth    = TempHeadFixInfo.stHeadType.nColWidth;
			TempHeadType.nDataLength  = TempHeadFixInfo.stHeadType.nDataLength;
			TempHeadType.nUnitNum     = TempHeadFixInfo.stHeadType.nUnitNum;
			TempHeadType.strAlignment = TempHeadFixInfo.stHeadType.strAlignment;
			TempHeadType.strDataType  = TempHeadFixInfo.stHeadType.strDataType;
			TempHeadType.strHeadName  = TempHeadFixInfo.stHeadType.strHeadName;
			TempHeadType.strUnitText  = TempHeadFixInfo.stHeadType.strUnitText;
			
			m_plistField->AddTail(TempHeadType);
			nFixFieldCnt++;
		}
	}
	
	//##### Move Header Info Set #####
	nCol = m_plistMoveHead->GetCount();
	
	TRACE("\n# View Move Head Total Data View");
	for (nCount = 0; nCount < nCol; nCount++)
	{
		pos = m_plistMoveHead->FindIndex(nCount);
		TempHeadMoveInfo = m_plistMoveHead->GetAt(pos);
		
		//TRACE("\n   # Field View : %d", TempHeadMoveInfo.bView);
		TRACE("\n   # Field Name (1): %s", TempHeadMoveInfo.stHeadType[0].strHeadName);
		TRACE("\n   # Field Name (2): %s", TempHeadMoveInfo.stHeadType[1].strHeadName);
		TRACE("\n   # Col Cnt : %d\n", TempHeadMoveInfo.nColCnt);
	}
	
	
	//## 일괄 검색 ##
	if (nStepCnt == -1)
		nStepCnt = nCol;
	
	TRACE("\n# View Move Head Total Data View");
	
	int nDataIndex;
	int nHeadIndex = 0;
	int nMoveFieldCnt = 0;
	for (nCount = 0; nCount < nCol; nCount++)
	{
		if (nSearchType == 0) //<= 기본검색 
			nDataIndex = nCount;
		else if (nSearchType == 1) //<= 단계검색 
			nDataIndex = (nCol - 1) - nCount;
		
		pos = m_plistMoveHead->FindIndex(nDataIndex);
		TempHeadMoveInfo = m_plistMoveHead->GetAt(pos);
		
		if (TempHeadMoveInfo.bCondition == TRUE)
			nHeadIndex++;
		
		if (nStepCnt >= nHeadIndex)
		{
			if ((TempHeadMoveInfo.bView == TRUE) &&
				(TempHeadMoveInfo.nColCnt > 0))
			{
				//TRACE("\n   # Index : %d", TempHeadMoveInfo.nIndex);
				//TRACE("\n   # Element : %s", TempHeadMoveInfo.strElement);
				
				for (int nCol = 0; nCol < TempHeadMoveInfo.nColCnt; nCol++)
				{
					TempHeadType.lHeadID      = TempHeadMoveInfo.stHeadType[nCol].lHeadID;
					TempHeadType.nColWidth    = TempHeadMoveInfo.stHeadType[nCol].nColWidth;
					TempHeadType.nDataLength  = TempHeadMoveInfo.stHeadType[nCol].nDataLength;
					TempHeadType.nUnitNum     = TempHeadMoveInfo.stHeadType[nCol].nUnitNum;
					TempHeadType.strAlignment = TempHeadMoveInfo.stHeadType[nCol].strAlignment;
					TempHeadType.strDataType  = TempHeadMoveInfo.stHeadType[nCol].strDataType;
					
					TempHeadType.strHeadName  = TempHeadMoveInfo.stHeadType[nCol].strHeadName + 
						" (" + TempHeadMoveInfo.strElement + ")";
					TRACE("\n   # Move Field Head (%d) : %s", nCol, TempHeadType.strHeadName);
					
					TempHeadType.strUnitText  = TempHeadMoveInfo.stHeadType[nCol].strUnitText;
					
					if (nSearchType == 0) //<= 기본검색 
						m_plistField->AddTail(TempHeadType);
					else if (nSearchType == 1) //<= 단계검색 
						tempListHeadInfoType.AddTail(TempHeadType);
					
					nMoveFieldCnt++;
				}
				
				TRACE("\n   # Condition View : %d", TempHeadMoveInfo.bCondition);
				TRACE("\n   # Field View : %d", TempHeadMoveInfo.bView);
				TRACE("\n   # Field Col Count : %d\n", TempHeadMoveInfo.nColCnt);
			}
		}
		else
			break;
	}
	
	if (nSearchType == 1) //<= 단계검색 
	{
		int nHeadCnt = tempListHeadInfoType.GetCount();
		for (nCount = 0; nCount < nHeadCnt; nCount++)
		{
			pos = tempListHeadInfoType.FindIndex((nHeadCnt - 1) - nCount);
			TempHeadType = tempListHeadInfoType.GetAt(pos);
			m_plistField->AddTail(TempHeadType);
		}
	}
	
	TRACE("\n## * Fix Head Field Count : %d, * Move Head Field Count : %d", 
		nFixFieldCnt, nMoveFieldCnt);
	
	//## 종목 코드 포함 ##
	int nRealViewFieldCnt = nFixFieldCnt + nMoveFieldCnt;
	TRACE("\n## * Total Head Field Count : %d", nRealViewFieldCnt);
	return nRealViewFieldCnt;
}

void CGrid_Result::SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField)
{
	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
	CMapStringToString UserColWidthsMap;
	CFindData_GridUtil::GetGridAllWidth(this, &UserColWidthsMap);

	CUGCell	cell;

	ClearAll();
	ZeroMemory(&m_nSortType, sizeof(int) * RESULT_FIELD_MAX);
	
	int nColCnt = plistField->GetCount();
	SetNumberCols(_countof(Head_Name) + plistField->GetCount());
	
	int nCol = 0;
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	POSITION   pos;
	HeadInfoType    stHeadType;

	BOOL bExistBun = FALSE;
	int nColBun = 0;
	BOOL bExistSin = FALSE;
	CString strHeadName;

	for (int nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistField->FindIndex(nIndex);
		stHeadType = m_plistField->GetAt(pos);
		
		TRACE("\n # Count      : %d", nCol);
		TRACE("\n   # HeadName : %s", stHeadType.strHeadName);
		TRACE("\n   # HeadID   : %d", stHeadType.lHeadID);

		if(stHeadType.strHeadName == "분")
		{
			bExistBun = TRUE;
			nColBun = nCol;
			stHeadType.strHeadName = "분HIDE";
			strHeadName = "분HIDE";
		}
		else if(stHeadType.strHeadName == "신")
		{
			
		}

		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		QuickSetText(nCol, -1, stHeadType.strHeadName);
		SetColWidth(nCol, (int)(stHeadType.nColWidth * (m_nFontWidth + 1)));
		SetRowHeight(nCol, m_nFontHeight + 4);
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	
		
		if(stHeadType.strHeadName == "업종구분")
			m_nSortType[nCol] = 1;

		if ((stHeadType.strDataType == "code") ||  //## 종목 CODE ##
			(stHeadType.strDataType == "candle")	|| //# 캔들
			(stHeadType.strDataType == "pattern"))		//# 패턴
			m_nSortType[nCol] = 1;
		nCol++;
	}

	if(bExistBun)
	{
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, Head_Name[0], COL_BUN, 20);
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, strHeadName, nColBun, 0);
	}
	else
	{
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, Head_Name[0], COL_BUN, 0);
	}

	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
	CFindData_GridUtil::SetGridAllWidth(this, &UserColWidthsMap);

	SetHighlightRow(TRUE);
	RedrawAll();
}

void CGrid_Result::SetHeader_Init(CList <HeadFixInfo, HeadFixInfo&>	*plistFixHead)
{
	CUGCell	cell;
	POSITION   pos;
	HeadFixInfo		stHeadFixInfo;
	
	ClearAll();
	ZeroMemory(&m_nSortType, sizeof(int) * RESULT_FIELD_MAX);
	
	int nColCnt = plistFixHead->GetCount();
	int nIndex, nViewColCnt = 0;
	for (nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistFixHead->FindIndex(nIndex);
		stHeadFixInfo = plistFixHead->GetAt(pos);
		
		if (stHeadFixInfo.bView == TRUE)
			nViewColCnt++;
	}
	SetNumberCols(nViewColCnt + 4);
	
	int nCol = 0;
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	if (m_bIndustry == TRUE)
		QuickSetText(nCol, -1, Head_Name_Industry[nCol]);
	else
		QuickSetText(nCol, -1, Head_Name[nCol]);
	SetColWidth(nCol, Head_Width[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	TRACE("\n## Grid_Result HeaderSetup ##");
	for (nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistFixHead->FindIndex(nIndex);
		stHeadFixInfo = plistFixHead->GetAt(pos);
		
		TRACE("\n # Count      : %d", nCol);
		TRACE("\n   # HeadName : %s", stHeadFixInfo.stHeadType.strHeadName);
		TRACE("\n   # HeadID   : %d", stHeadFixInfo.stHeadType.lHeadID);
		
		if (stHeadFixInfo.bView == TRUE)
		{
			QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
			QuickSetText(nCol, -1, stHeadFixInfo.stHeadType.strHeadName);
			SetColWidth(nCol, (int)(stHeadFixInfo.stHeadType.nColWidth * (m_nFontWidth + 1)));
			SetRowHeight(nCol, m_nFontHeight + 4);
			QuickSetBackColor(nCol, -1, m_clrHeader);
			QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
			QuickSetFont(nCol, -1, &m_fontDefault);
			
			if ((stHeadFixInfo.stHeadType.strDataType == "code") ||  //## 종목 CODE ##
				(stHeadFixInfo.stHeadType.strDataType == "candle")	|| //# 캔들
				(stHeadFixInfo.stHeadType.strDataType == "pattern"))		//# 패턴
				m_nSortType[nCol] = 1;
			nCol++;
		}
	}
	
	RedrawAll();
}

void CGrid_Result::HeadMoveUpdate()
{
	POSITION pos;
	HeadMoveInfo  stHeadMoveInfo;
	CList<HeadMoveInfo, HeadMoveInfo&>  templistHeadMoveInfo;
	
	int nCount = m_plistMoveHead->GetCount();
	if (nCount > 0)
	{
		int i;
		//# Data Copy
		for(i = 0; i < nCount; i++)
		{
			pos = m_plistMoveHead->FindIndex(i);
			stHeadMoveInfo = m_plistMoveHead->GetAt(pos);
			templistHeadMoveInfo.AddTail(stHeadMoveInfo);
		}
		
		m_plistMoveHead->RemoveAll(); 
		
		//# Data Change 
		for(i = 0; i < nCount; i++)
		{
			pos = templistHeadMoveInfo.FindIndex(i);
			stHeadMoveInfo = templistHeadMoveInfo.GetAt(pos);
			
			CString strHeadID = "";
			CString strtempHeadID;
			//for (int nCnt=0; nCnt<HEAD_OTHER_CNT; nCnt++)
			for (int nCnt = 0; nCnt < stHeadMoveInfo.nColCnt; nCnt++)
			{
				strtempHeadID.Format("%d", stHeadMoveInfo.stHeadType[nCnt].lHeadID);
				strHeadID += strtempHeadID + "/";
			}
			
			HeadMoveSearch(stHeadMoveInfo.nIndex, stHeadMoveInfo.strElement, strHeadID, 
				stHeadMoveInfo.bView, stHeadMoveInfo.bCondition);
		}
	}
}



BOOL CGrid_Result::HeadMoveSetStandLine(CList<OneElementInfo, OneElementInfo> &listElement)
{
	BOOL bResult = FALSE;
	
	POSITION posElement, posMoveHead;
	HeadMoveInfo stHeadMoveInfo;
	OneElementInfo   stElementString;
	
	CList<HeadMoveInfo, HeadMoveInfo&>  tempHeadMoveInfo;
	tempHeadMoveInfo.RemoveAll();
	
	int nCheckCnt = 0;
	int nConditionCnt = listElement.GetCount();
	int nHeadCount = m_plistMoveHead->GetCount();
	int nIndex = 0;
	for (nIndex = 0; nIndex < nConditionCnt; nIndex++)
	{
		posElement = listElement.FindIndex(nIndex);
		stElementString = listElement.GetAt(posElement);
		stElementString.strElement.MakeUpper();
		
		for(int nHead = 0; nHead < nHeadCount; nHead++)
		{
			posMoveHead = m_plistMoveHead->FindIndex(nHead);
			stHeadMoveInfo = m_plistMoveHead->GetAt(posMoveHead);
			
			if (stElementString.strElement == stHeadMoveInfo.strElement)
			{
				stHeadMoveInfo.bCondition = stElementString.bView;
				
				if (stElementString.bView == FALSE)
					stHeadMoveInfo.bView = FALSE;
				
				tempHeadMoveInfo.AddTail(stHeadMoveInfo);
				nCheckCnt++;
			}
		}
	}
	
	if (nHeadCount == nCheckCnt)
	{
		m_plistMoveHead->RemoveAll();
		
		for (nIndex = 0; nIndex < nHeadCount; nIndex++)
		{
			posMoveHead = tempHeadMoveInfo.FindIndex(nIndex);
			stHeadMoveInfo = tempHeadMoveInfo.GetAt(posMoveHead);
			
			m_plistMoveHead->AddTail(stHeadMoveInfo);
		}
		bResult = TRUE;
	}
	
	return bResult;
}


void CGrid_Result::LoadMoveUserFiledXML()
{
	CString strIndexFilePath;
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + MOVEFIELD_USER_FILE;
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	//m_bJFieldLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	m_bMoveUserLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	m_pMoveUserDOM = pDOMConditionIndex;
	//pJFieldDOMConditionIndex = pDOMConditionIndex;
}

void CGrid_Result::LoadMoveSysFiledXML()
{
	CString strIndexFilePath;
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + MOVEFIELD_SYS_FILE;
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	m_bMoveSysLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	m_pMoveSysDOM = pDOMConditionIndex;
}


void CGrid_Result::LoadFixSysFiledXML()
{
	CString strIndexFilePath;
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + FIXFIELD_SYS_FILE;
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	m_bFixSysLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	m_pFixSysDOM = pDOMConditionIndex;
}


BOOL CGrid_Result::HeadMoveSearch(int nIndexCount, CString strElement, CString strHeadID,
								  BOOL bView, BOOL bCondition)
{
	BOOL bResult = TRUE;
	BOOL bHaveFieldID = FALSE;
	
	HeadInfoType      stHeadType;
	HeadMoveInfo		  stHeadMoveInfo;
	
	MSXML::IXMLDOMNodePtr		      pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	// XML 파일을 로드하는데 실패하였다면
	if( m_bMoveSysLoadedXMLFile == VARIANT_FALSE || 
		m_bMoveUserLoadedXMLFile == VARIANT_FALSE || 
		m_bFixSysLoadedXMLFile == VARIANT_FALSE ) 
	{
		bResult = FALSE;
		return bResult;
	}
	
	int nColCount = 0;
	int nFindPos = 0;
	CString tmpStr = strHeadID;
	CString ExtractStr;
	
	if ( !tmpStr.IsEmpty() ) 
	{
		while( nFindPos >=0 ) 
		{
			nFindPos = tmpStr.Find("/");
			if (nFindPos < 0)
				break;
			ExtractStr = tmpStr.Mid(0, nFindPos);
			tmpStr = tmpStr.Mid(nFindPos+1, tmpStr.GetLength());
			
			try {
				// header의 node를 얻는다.
				if (!ExtractStr.Left(1).Compare("5")) 
					pNIndex = m_pMoveUserDOM->selectSingleNode("INDEX");
				else if (!ExtractStr.Left(1).Compare("9")) 
					pNIndex = m_pFixSysDOM->selectSingleNode("INDEX");
				
				pNS_FItem  = pNIndex->selectNodes("FITEM");
				
				int nCount, nLength;
				CString strTemp;
				nLength = pNS_FItem->length;
				for(nCount = 0; nCount < nLength; nCount++)
				{
					// node를 얻어온다.
					pN_FItem = pNS_FItem->item[nCount];	
					
					// 속성을 얻어온다.
					MSXML::IXMLDOMNodePtr	pNAttributeTemp;
					pNA_FItem= pN_FItem->attributes;
					
					pNAttributeTemp = pNA_FItem->getNamedItem("ID");
					strTemp = CString((PCHAR)pNAttributeTemp->text);
					
					if ( !strTemp.Compare(ExtractStr) )
					{
						pNAttributeTemp = pNA_FItem->getNamedItem("ID");
						strTemp = CString((PCHAR)pNAttributeTemp->text);
						stHeadType.lHeadID = atol(strTemp);
						
						if (!ExtractStr.Left(1).Compare("5")) 
							pNAttributeTemp = pNA_FItem->getNamedItem("VIEW_NAME");
						else if (!ExtractStr.Left(1).Compare("9")) 
							pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
						stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
						strTemp = CString((PCHAR)pNAttributeTemp->text);
						stHeadType.nUnitNum = atoi(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
						stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
						stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
						strTemp = CString((PCHAR)pNAttributeTemp->text); 
						stHeadType.nDataLength = atoi(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
						strTemp = CString((PCHAR)pNAttributeTemp->text);
						stHeadType.nColWidth = atoi(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
						stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
						
						stHeadMoveInfo.stHeadType[nColCount] = stHeadType;
						
						bHaveFieldID = TRUE;
						break;
					}
				}
			}
			catch (...)
			{
				bResult = FALSE;
				return bResult;
			}
			
			//## Not Have Move Field => Search from Move System File ##
			if ((!ExtractStr.Left(1).Compare("5")) && (bHaveFieldID == FALSE))
			{
				bHaveFieldID = HeadMoveSysSearch(ExtractStr, stHeadType);
				stHeadMoveInfo.stHeadType[nColCount] = stHeadType;
			}
			
			nColCount++;
			bHaveFieldID = FALSE;
		}
	}
	
	stHeadMoveInfo.nIndex = nIndexCount;
	stHeadMoveInfo.strElement = strElement;
	
	stHeadMoveInfo.nColCnt = nColCount;
	
	stHeadMoveInfo.bView = bView;
	stHeadMoveInfo.bCondition = bCondition;   //## 조건포함
	
	HeadMoveInsert(stHeadMoveInfo);
	
	return bResult;
}


BOOL CGrid_Result::HeadMoveSysSearch(CString ExtractStr, HeadInfoType &stHeadType)
{
	BOOL bHaveFieldID = FALSE;
	
	MSXML::IXMLDOMNodePtr		    pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
// 	try {
// 		pNIndex = m_pMoveSysDOM->selectSingleNode("INDEX");
// 		
// 		pNS_FItem  = pNIndex->selectNodes("FITEM");
// 		
// 		int nCount, nLength;
// 		CString strTemp;
// 		nLength = pNS_FItem->length;
// 		for(nCount = 0; nCount < nLength; nCount++)
// 		{
// 			// node를 얻어온다.
// 			pN_FItem = pNS_FItem->item[nCount];	
// 			
// 			// 속성을 얻어온다.
// 			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
// 			pNA_FItem= pN_FItem->attributes;
// 			
// 			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
// 			strTemp = CString((PCHAR)pNAttributeTemp->text);
// 			
// 			if ( !strTemp.Compare(ExtractStr) )
// 			{
// 				pNAttributeTemp = pNA_FItem->getNamedItem("ID");
// 				strTemp = CString((PCHAR)pNAttributeTemp->text);
// 				stHeadType.lHeadID = atol(strTemp);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
// 				stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
// 				strTemp = CString((PCHAR)pNAttributeTemp->text);
// 				stHeadType.nUnitNum = atoi(strTemp);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
// 				stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
// 				stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
// 				strTemp = CString((PCHAR)pNAttributeTemp->text); 
// 				stHeadType.nDataLength = atoi(strTemp);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
// 				strTemp = CString((PCHAR)pNAttributeTemp->text);
// 				stHeadType.nColWidth = atoi(strTemp);
// 				
// 				pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
// 				stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
// 				
// 				bHaveFieldID = TRUE;
// 				break;
// 			}
// 		}
// 	}
// 	catch ( CFileException *e )
	// 	{
	// 		TRACE("CGrid_Result::HeadMoveSysSearch Exception(%ld)", e);	// Warnning 보기 싫어서..
	// 	}

	pNIndex = m_pMoveSysDOM->selectSingleNode("INDEX");

	pNS_FItem  = pNIndex->selectNodes("FITEM");

	int nCount, nLength;
	CString strTemp;
	nLength = pNS_FItem->length;
	for(nCount = 0; nCount < nLength; nCount++)
	{
		// node를 얻어온다.
		pN_FItem = pNS_FItem->item[nCount];	

		// 속성을 얻어온다.
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNA_FItem= pN_FItem->attributes;

		pNAttributeTemp = pNA_FItem->getNamedItem("ID");
		strTemp = CString((PCHAR)pNAttributeTemp->text);

		if ( !strTemp.Compare(ExtractStr) )
		{
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stHeadType.lHeadID = atol(strTemp);

			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stHeadType.nUnitNum = atoi(strTemp);

			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			strTemp = CString((PCHAR)pNAttributeTemp->text); 
			stHeadType.nDataLength = atoi(strTemp);

			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stHeadType.nColWidth = atoi(strTemp);

			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);

			bHaveFieldID = TRUE;
			break;
		}
	}
	
	return bHaveFieldID;
}


//########################################

BOOL CGrid_Result::LoadCandleIndexToXml()
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
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

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


BOOL CGrid_Result::LoadPatenIndexToXml()
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
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));

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

CString CGrid_Result::FindCandleIdFromName(long lCandleID)
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


CString CGrid_Result::FindPatenIdFromName(long lPatenID)
{
	CString strResult = "--";
	
	int i, nCount;
	POSITION pos;
	
	PatenInfo      TempPatenInfo;
	
	if (m_bPatenLoadFalg == FALSE)
		return strResult;
	
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


BOOL CGrid_Result::GetRegStockItem(CStringArray &saRegStockItem)
{
	BOOL bResult = FALSE;
	
	CUGCell cell;
	CString strStockItemCode, strStockItemName;
	
	if (GetNumberRows() > 0)
	{
		for (int nRow = 0; nRow < GetNumberRows(); nRow++)
		{
			GetCell(0, nRow, &cell);
			//!strStockItemCode = cell.m_strJongCode;
			strStockItemCode = cell.GetOtherJongCode();
			strStockItemName = cell.GetText();
			saRegStockItem.Add(strStockItemCode + "|" + strStockItemName);
		}
		
		bResult = TRUE;
	}
	
	return bResult;
}


BOOL CGrid_Result::GetRegStockItem(CPtrArray &paRegStockItem)
{
	BOOL bResult = FALSE;
	
	/*
	CUGCell cell;
	CString strStockItemCode, strStockItemName;
	
	  if (GetNumberRows() > 0)
	  {
	  for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	  {
	  GetCell(0, nRow, &cell);
	  strStockItemCode = cell.GetOtherJongCode();
	  strStockItemName = cell.GetText();
	  saRegStockItem.Add(strStockItemCode + "|" + strStockItemName);
	  }
	  
		bResult = TRUE;
		}
	*/
	
	return bResult;
}


void CGrid_Result::GetRowCode(int nRow, CString &strCode, int &nMarkType)
{
	CUGCell cell;
	if(m_strType == "대상변경")
		GetCell(1, nRow, &cell);
	else
		GetCell(COL_NAME, nRow, &cell);
	strCode = cell.GetOtherJongCode();
	nMarkType = cell.GetOtherDataInt();
	
	strCode.TrimLeft(" ");
	strCode.TrimRight(" ");
}

BOOL CGrid_Result::IsRowCodeChecked(int nRow)
{
	LPCSTR strCheck = QuickGetText(COL_CODE, nRow);
	return (BOOL)atoi(strCheck);
}

CString CGrid_Result::GetCellData(int nRow, int nCol)
{
	CString strResult = "";
	
	CUGCell cell;
	GetCell(nCol, nRow, &cell);
	strResult = cell.GetLabelText();
	if(strResult.GetLength()>0)
		return strResult;

	strResult = cell.GetText();	
	return strResult;
}

void CGrid_Result::GetCellColor(int nRow, int nCol, COLORREF &clrText, COLORREF &clrBack)
{
	CUGCell cell;
	GetCell(nCol, nRow, &cell);
	
	clrText = cell.GetTextColor();
	clrBack = cell.GetBackColor();
}

void CGrid_Result::GetCellSignText(int nRow, int nCol, CString &strSignText)
{
	CUGCell cell;
	GetCell(nCol, nRow, &cell);
	
	strSignText = GetSignText(cell.GetSign());
}

CString CGrid_Result::GetSignText(int nSign)
{
	CString strSignText;
	
	switch (nSign)
	{
	case 1:		// 상한(화살표)
		strSignText = "↑";
	case 2:		// 상승(삼각형)
		strSignText = "▲";
		break;
	case 4:		// 하한(화살표)
		strSignText = "↓";
		break;
	case 5:		// 하락(삼각형)
		strSignText = "▼";
		break;
	case 3:		// 보합
	default:
		strSignText = "";
	}
	
	return strSignText;
}

void CGrid_Result::GetRowData(int nRow, BOOL bBackTest, CString &strCode, CString &strNowPrice)
{
	CUGCell cell;
	GetCell(0, nRow, &cell);
	strCode = cell.GetOtherJongCode();
	
	if (bBackTest == TRUE)			//# Back Test!
		GetCell(6, nRow, &cell);
	else
		GetCell(2, nRow, &cell);
	
	strNowPrice.Format("%d", cell.GetOtherDataLong());
}

void CGrid_Result::SetLockColumns(int nCol, BOOL bOffSet)
{
	if (bOffSet == TRUE)
		m_nFixCol = 2 + nCol;
	else
		m_nFixCol = nCol;
	
	LockColumns(m_nFixCol);
	RedrawAll();
}

int CGrid_Result::PrintingInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow, int endCol,long endRow)
{
	return m_pPrint->PrintInit(pDC,pPD,startCol,startRow,endCol,endRow);
}


int CGrid_Result::PrintingPage(CDC * pDC, int pageNum)
{
	return m_pPrint->PrintPage(pDC,pageNum);
}

void CGrid_Result::PrintingTail(CDC * pDC, CPrintInfo * pInfo)
{
	CRect rect = pInfo->m_rectDraw;
	//여백적용
	if(m_pPrint)	
	{
		rect.left  = m_pPrint->m_printLeftMarginPX;	
		rect.right = m_pPrint->m_printRightMarginPX;
	}

	CFont * pOldFont = NULL;
	if(m_pPrintTailFont)
		pOldFont = pDC->SelectObject(m_pPrintTailFont);
	else
		pOldFont = pDC->SelectObject(m_GI->m_defFont);

	CSize sz;
	TEXTMETRIC	tm;	
	pDC->GetTextMetrics(&tm);
	sz.cx =	tm.tmAveCharWidth;
	sz.cy =	tm.tmHeight;

	rect.top = rect.bottom - (sz.cy + 5);

	//시간
	CTime time(NULL);
	time = CTime::GetCurrentTime();
	CString strYear, strTime;
	strYear.Format("%d", time.GetYear());
	strYear = strYear.Right(2);
	strTime.Format("%02d/%02d/%s %02d:%02d:%02d", 
		time.GetMonth(), time.GetDay(), strYear, 
		time.GetHour(),  time.GetMinute(), time.GetSecond());

	//페이지
	CString strPage;
	strPage.Format("%d/%d", pInfo->m_nCurPage, pInfo->GetMaxPage());
	
	int nMode = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(strTime, rect, DT_LEFT | DT_TOP);
	pDC->DrawText(strPage, rect, DT_RIGHT| DT_TOP);

	pDC->SetBkMode(nMode);
	pDC->SelectObject(pOldFont);	
}

int CGrid_Result::PrintingSetMargin(int whichMargin,int size)
{
	return m_pPrint->PrintSetMargin(whichMargin,size);
}


int CGrid_Result::PrintingSetScale(double scale)
{	
	return m_pPrint->PrintSetScale(scale);
}


int CGrid_Result::PrintingSetOption(int option,long param)
{	
	return m_pPrint->PrintSetOption(option,param);
}


int CGrid_Result::PrintingGetOption(int option,long *param)
{	
	return m_pPrint->PrintGetOption(option,param);
}

int CGrid_Result::GetTHRowHeight(int row)
{
	row = (row * -1) -1;
	if(row <0 || row > m_GI->m_numberTopHdgRows)
		return 0;

	return m_GI->m_topHdgHeights[row];
}

void CGrid_Result::SetCellBorderColor4Print(COLORREF clrBorder)
{
	if(m_pBorderPen4Print)
		delete m_pBorderPen4Print;

	m_pBorderPen4Print = new CPen(PS_SOLID, 1, clrBorder);	
}

void CGrid_Result::SetBorderStyle4Print(short nStyle)
{
	m_nStyle4Print = nStyle;
}

short CGrid_Result::GetBorderStyle4Print()
{
	return m_nStyle4Print;
}

void CGrid_Result::SetPrintTailFont(CString strFontName, int nFontSize)
{
	if(m_pPrintTailFont)
		delete m_pPrintTailFont;

	m_pPrintTailFont = new CFont;
	m_pPrintTailFont->CreatePointFont( nFontSize, strFontName );
}

CFont * CGrid_Result::GetPrintTailFont()
{
	return m_pPrintTailFont;
}


int CGrid_Result::GetCodeColNumber(int nType)
{
	if(nType==1)	return 2;

	return 0;
}

BOOL CGrid_Result::GetRowToCode(int nRow, CString &strCode, int &nMarketType)
{
	BOOL bResult = FALSE;
	
	if (nRow < GetNumberRows())
	{
		int nCodeCol = GetCodeColNumber();
		CUGCell cell;
		GetCell(nCodeCol, nRow, &cell);
		strCode = cell.m_string;		
		bResult = TRUE;
	}
	else 
		bResult = FALSE;
	
	return bResult;
}

//<< 07.07.10 이준옥
void CGrid_Result::FnLButtonDown(CPoint point)
{
//	m_MouseMode = MOUSE_CODE_DRAG;

	if(::GetFocus()!=GetSafeHwnd())	SetFocus();

	ScreenToClient(&point);
	int col;
	GetCellFromPoint(point.x, point.y, &col, &m_lDragRow);
	if((col!=m_nDragCol && col!=m_nDragCol+1) || m_lDragRow == 0 ) 
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
void CGrid_Result::FnLButtonUp(CPoint point)
{
	ScreenToClient(&point);
// hidden 
/*	if ( m_MouseMode == MOUSE_CODE_DRAG )
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
				
				return;
			}
		}
	}

	m_MouseMode = MOUSE_NOTHING;
	m_bDragAndDrop = FALSE;
	m_strDragCode = "";
	m_lDragRow = -1;

	if ( m_bFocus )
	{
		m_bFocus	= FALSE;	
	}
*/
}

void CGrid_Result::FnMouseMove(CPoint point)
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
		int nMarketType;
		if(m_strDragCode.IsEmpty())
		{
			if (GetRowToCode(m_lDragRow, m_strDragCode, nMarketType) == FALSE)
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

void CGrid_Result::DragImageInfoClear()
{
	if ( m_pDragImage )
		delete m_pDragImage;
	m_pDragImage = NULL;

	if ( m_pDragBitmap )
		delete m_pDragBitmap;
	m_pDragBitmap = NULL;

}

BOOL CGrid_Result::MakeDragImage(CString strText)
{
	strText.TrimRight();
	if(strText.GetLength()==0)	return FALSE;

	//CFont *pFont = (CFont*) GetFont(0);
	CFont *pFont = &m_fontDefault;
	if(!pFont) return FALSE;
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

//>> 07.07.10 이준옥

void CGrid_Result::FnRButtonUp(CPoint point)
{
	SetCommMenu(&point);
}

void CGrid_Result::ChangeGridColor(ST_GRIDCOLOR stGridColor)
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

int CGrid_Result::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if(ID == m_nButtonIndex)	
	{
		if(msg == UGCT_BUTTONUP)
		{
			CUGCell cell;
			GetCell(col, row, &cell);
			
			CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
			pReWnd->m_saDelItemCode.Add(cell.GetOtherJongCode());
			DeleteRow(row);
		}
	}
	
	return TRUE;
}

CString CGrid_Result::GetStockStatus(CString strItemCode, CString strItemName, COLORREF& clrbunText, COLORREF& clrbunBack)
{
	CString strStockStatus;
	
	CString strData;
	strData = strItemName.GetAt(0);
	
	if(strData != " ")
	{
		if(strData == "-")
		{
			strData = "불";				// 불성실 공시
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "/")
		{
			strData = "액";				// 액면분할, 액면병합
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "#")
		{
			strData = "투";				// 투자유의
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "!")
		{
			strData = "이";				// 이상급등
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "%")
		{
			strData = "관";				// 관리
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "X")
		{
			strData = "거";				// 거래정지
			clrbunBack = RGB(152, 82, 40);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "&")
		{
			strData = "기";				// 기준가 발생
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
			
		}
			
	}
	else
	{
		clrbunBack = RGB(255, 255, 255);
	}
	return strData;
}

BOOL CGrid_Result::GetRowToCodeName(int nRow, CString& strCode, CString& strName, int& nMarketType)
{
	BOOL bResult = FALSE;
	
	CUGCell cell;
	GetCell(COL_NAME, nRow, &cell);
	strCode = cell.GetOtherJongCode();
	nMarketType = cell.GetOtherDataInt();
	strName = cell.GetText();
	
	strCode.TrimLeft(" ");
	strCode.TrimRight(" ");
	strName.TrimLeft(" ");
	strName.TrimRight(" ");
	
	return bResult;
}

CString CGrid_Result::LoadMemo(CString strKey)
{
	if (strKey.IsEmpty())
		return "";
	
	strKey.TrimRight();
	strKey = "A" + strKey;
	
	CString dir, dat;
	char key[12], lBytes[4];
	
	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;
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

void CGrid_Result::SaveMemo(CString strKey, CString strData)
{
	if (strKey.IsEmpty())
		return;

	strKey = "A" + strKey;

	CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)pReWnd->m_pwndMain;

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

void CGrid_Result::SetHeader()
{
	CUGCell	cell;

	ClearAll();
	ZeroMemory(&m_nSortType, sizeof(int) * 2);
	
	SetNumberCols(_countof(Head_Name_TarGet));
	
	int nCol = 0;
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
	QuickSetText(nCol, -1, Head_Name_TarGet[nCol]);
	SetColWidth(nCol, Head_Width_TarGet[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;
	
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
	QuickSetText(nCol, -1, Head_Name_TarGet[nCol]);
	SetColWidth(nCol, Head_Width_TarGet[nCol]);
	QuickSetBackColor(nCol, -1, m_clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;

	SetHighlightRow(TRUE);
	RedrawAll();
}

void CGrid_Result::SetAddRow(int nCol, int nRow, CString strCode, CString strName, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	GetEvenOddBkColor(nRow, clrBk);
	
	/*AppendRow();*/
	
	// 종목코드
	nCol = 0;
 	GetCell(nCol, nRow, &cell);	// 가로 세로
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(strCode);
	cell.SetLabelText(strCode);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	SetCell(nCol, nRow, &cell);

	// 종목명
	nCol = 1;
	GetCell(nCol, nRow, &cell);	// 가로 세로
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(strName);
	cell.SetBackColor(clrBk);
	cell.m_string = strName;
	cell.SetOtherDataCellText(strName);
	cell.SetOtherJongCode(strCode);
	cell.SetOtherDataInt(0);
	cell.SetReadOnly(TRUE);
	cell.SetBackColor(clrBk);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);	
	
	SetCell(nCol, nRow, &cell);
	
	if(bRedraw)
		RedrawAll();
}