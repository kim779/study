// ConditionGrid.cpp: implementation of the CConditionGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConditionGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*####################################################################################
##																																									##
##															Condition Make Grid Control													##
##																																									## 
##																																									##
##																작성일 : 2003. 7 ~																##
##																작성자 : 우동우 (TEXEN SYSTEM)										##
##																																									##  
######################################################################################*/
const		LPCTSTR		Head_Name[] = { _T("-"), _T(""), _T(""), 
_T("대상"), _T("조건 추가지표"), _T("값"), 
_T("수정"), _T("삭제"), _T("▲"), _T("▼"), _T("↑"), _T("↓") };

const		int				COL_MAX_MIN						= 0;
const		int				COL_CONTITOTN_CHECK			= 1;
const		int				COL_ELEMENT_TEXT			= 2;
const		int				COL_TARGET_LIST				= 3;
const		int				COL_CONDITION_TEXT			= 4;
const		int				COL_FIELD_CHECK				= 5;
const		int				COL_MODIFY_BTN				= 6;
const		int				COL_DELETE_BTN				= 7;
const		int				COL_UP_BTN					= 8;
const		int				COL_DOWN_BTN				= 9;
const		int				COL_MAXUP_BTN				= 10;
const		int				COL_MINDOWN_BTN				= 11;

const		int				Head_Width[] = {0, 18, 18, 
50, 295, 40, //18,
0, 35, 15, 15, 15, 15};

const		int				Head_Width_Common[] = {0, 0, 18, 
0, 300, 0,
0, 35, 15, 15, 15, 15};


const		LPCTSTR		CONDITION_INDEX[] = { "A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","O","P","Q","R","S","T",
"U","V","W","X","Y","Z",
"a","b","c","d","e","f","g","h","i","j",
"k","l","m","n","o","p","q","r","s","t",
"u","v","w","x","y","z"};

/* 0:일간, 1:주간, 2:월간, 3:1분, 4:3분	*/
/* 5:5분,  6:10분, 7:15분, 8:30분, 9:60분*/
const		LPCTSTR		TARGETDATA_TYPE1 = " 일간\n 주간\n 월간\n  1분\n  2분\n  3분\n  5분\n 10분\n 15분\n 20분\n 30분\n 60분\n120분\n";
const		LPCTSTR		TARGETLIST_TYPE1[] = { " 일간", " 주간", " 월간", "  1분", "  2분", "  3분", "  5분", 
" 10분", " 15분", " 20분", " 30분", " 60분", "120분" };
const		int		TARGETINDEX_TYPE1[] = {0, 1, 2, 3, 20, 4, 5, 6, 7, 21, 8, 9, 22};
const		LPCTSTR		TARGETDATA_TYPE2 = "일간\n";
const		LPCTSTR		TARGETLIST_TYPE2[] = { "일간" };
const		int		TARGETINDEX_TYPE2[] = { 0 };

//--> 0015707: [7127] "당일내발생" 내 조건에 2분, 20분, 120분 주기 추가요청
const		LPCTSTR		TARGETDATA_TYPE3 = "  1분\n  2분\n  3분\n  5분\n 10분\n 15분\n 20분\n 30분\n 60분\n120분\n";
const		LPCTSTR		TARGETLIST_TYPE3[] = { "  1분", "  2분", "  3분", "  5분", 
" 10분", " 15분", " 20분", " 30분", " 60분", "120분" };
const		int		TARGETINDEX_TYPE3[] = { 3, 20, 4, 5, 6, 7, 21, 8, 9, 22};
//<--
/*
const		LPCTSTR		TARGETDATA_TYPE3 = " 1분\n 3분\n 5분\n10분\n15분\n30분\n60분\n";
const		LPCTSTR		TARGETLIST_TYPE3[] = { " 1분", " 3분", " 5분", 
"10분", "15분", "30분", "60분" };
const		int		TARGETINDEX_TYPE3[] = { 3, 4, 5, 
6, 7, 8, 9 };

const		LPCTSTR		TARGETDATA_TYPE1 = "일간\n주간\n월간\n 1분\n 3분\n 5분\n10분\n15분\n60분\n";
const		LPCTSTR		TARGETLIST_TYPE1[] = { "일간", "주간", "월간", " 1분", " 3분", " 5분", 
"10분", "15분", "60분" };
const		int		TARGETINDEX_TYPE1[] = { 0, 1, 2, 3, 4, 5, 
6, 7, 9 };

const		LPCTSTR		TARGETDATA_TYPE2 = "일간\n";
const		LPCTSTR		TARGETLIST_TYPE2[] = { "일간" };
const		int		TARGETINDEX_TYPE2[] = { 0 };

const		LPCTSTR		TARGETDATA_TYPE3 = " 1분\n 3분\n 5분\n10분\n15분\n60분\n";
const		LPCTSTR		TARGETLIST_TYPE3[] = { " 1분", " 3분", " 5분", 
"10분", "15분", "60분" };
const		int		TARGETINDEX_TYPE3[] = { 3, 4, 5, 
6, 7, 9 };
*/

const	int ROW_DEF_HEIGHT			= 20;
const	int ROW_HEAD_DEF_HEIGHT = 20;

#define COL_MAXWIDTH    500
#define COL_MINWIDTH    100

#define _countof(array) (sizeof(array)/sizeof(array[0]))

const   COLORREF	clrHeader					= COLOR_HEADERFACE;
const   COLORREF	clrHeader_Focus		= COLOR_HEADERFACE_FOCUS;

const   COLORREF  clrSelRow					= COLOR_SEL_ROW;
const   COLORREF  clrEvenRow				= COLOR_EVEN_ROW;
const   COLORREF  clrOddRow					= COLOR_ODD_ROW;

const   COLORREF  clrSelRow_Focus    = COLOR_SEL_ROW_FOCUS;
const   COLORREF  clrEvenRow_Focus   = COLOR_EVEN_ROW_FOCUS;
const   COLORREF  clrOddRow_Focus	   = COLOR_ODD_ROW_FOCUS;

const   COLORREF  clrBtn						= COLOR_BUTTON;
const   COLORREF  clrBtnText				= COLOR_BUTTON_TEXT;
const   COLORREF  clrBtnSearch			= COLOR_BUTTON_SEARCH;

BEGIN_MESSAGE_MAP(CConditionGrid,CUGCtrl)		//CUGCtrl => CWnd From cyberahn. 2006.06.19. VC++ => .Net포팅.
	//{{AFX_MSG_MAP(ShowGrid)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConditionGrid::CConditionGrid()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	m_bSearchMode = TRUE;
	m_bUserDragDrop = FALSE;

	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);

	m_nSortCol			= -1;
	m_bProcessSelectChange = true;
	m_nLastSelRow		= 0;

	//## Mouse Left Button State ##  
	m_bLeftBtnState = FALSE;
	//m_bDuringDrag = FALSE;

	//## col/row Size Flag ##
	m_allowColSizing = FALSE;
	m_allowRowSizing = FALSE;

	m_bCallAdjustOneTime = FALSE;

	m_minWidth = COL_MINWIDTH;
	m_maxWidth = COL_MAXWIDTH;

	m_nDefRowHeight = ROW_DEF_HEIGHT;
	m_nDefRowHeadHeight = ROW_HEAD_DEF_HEIGHT;


	m_bDetailView = FALSE;
}

CConditionGrid::~CConditionGrid()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
}

CSize CConditionGrid::GetFontSize()
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

void CConditionGrid::ClearAll()
{
	int nCount;

	for (nCount = 0; nCount < GetNumberRows(); nCount++)
		RemoveDataSource(nCount);

	/*
	// 모든 Row를 삭제합니다.
	while(GetNumberRows()) 
	{
	m_GI->m_defDataSource->DeleteRow(0);
	m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}
	*/

	SetNumberRows(0);

	// 모든 Row가 지워지면 SORTING정보도 초기화 하여 줍니다.
	QuickSetCellType(m_nSortCol, -1, 0);
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortCol = 0;

	m_nLastSelRow = -1;

	//## Mouse Left Button State ##  
	m_bLeftBtnState = FALSE;
	//m_bDuringDrag = FALSE;

	m_bViewAllField = FALSE;
	SetHeadColor(FALSE);

	RedrawAll();
}

void CConditionGrid::SetModifyRow(int nRow,
								  BOOL bNot, 
								  CString strItem, 
								  BOOL bFieldVal,
								  BOOL bRedraw)
{
	CUGCell cellText;
	int nModifyRow = nRow;

	//## 조건 완성 구문 ##
	GetCell(COL_CONDITION_TEXT, nModifyRow, &cellText);
	strItem.TrimLeft();
	strItem.TrimRight();
	cellText.SetText(strItem);
	cellText.SetOtherDataInt((int)bNot);
	SetCell(COL_CONDITION_TEXT, nModifyRow, &cellText);

	BOOL bFieldState;
	GetCell(COL_FIELD_CHECK, nModifyRow, &cellText);
	bFieldState = (BOOL)cellText.GetOtherDataInt();

	CString strTemp;
	QuickGetText(COL_CONTITOTN_CHECK, nModifyRow, &strTemp);
	BOOL bState = (strTemp[0] == '1') ? TRUE : FALSE;

	//## 값보기 CheckBox Enable ##
	if ((bFieldState == TRUE) && (bState == TRUE) && (bNot == FALSE))
	{
		QuickSetCellType(COL_FIELD_CHECK, nModifyRow, UGCT_CHECKBOX);
		QuickSetCellTypeEx(COL_FIELD_CHECK, nModifyRow, UGCT_CHECKBOXCHECKMARK);

		//## 값보기 Defult Values Set ##
		if (bFieldVal == TRUE)
			QuickSetText(COL_FIELD_CHECK, nModifyRow, "1");
		else
			QuickSetText(COL_FIELD_CHECK, nModifyRow, "0");
	}
	else
	{
		QuickSetCellType(COL_FIELD_CHECK, nModifyRow, UGCT_NORMAL);
		QuickSetCellTypeEx(COL_FIELD_CHECK, nModifyRow, UGCT_NORMAL);

		QuickSetText(COL_FIELD_CHECK, nModifyRow, " ");
	}

	if (bRedraw == TRUE) 
		RedrawAll();
}


void CConditionGrid::SetModifyRow(int nRow,
								  BOOL		bState,					//# Condition View
								  CString	strElement,			//# Element Index
								  BOOL		bNot,						//# Not State 
								  int			nTargetType,		//# Target Type
								  int			nTargetIndex,		//# Target Index
								  BOOL		bTargetEnable,	//# Target Enable
								  CString strItem,				//# 완성 구문 
								  BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
								  BOOL		bFieldVal)			//# 값보기
{
	int nElementIndex = GetElementIndex(strElement);
	SetRow(nRow,
		bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,	//# Target Enable
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기 

	SetSelectRow(nRow);
}

BOOL CConditionGrid::SetDeleteRow(int nRow)
{
	BOOL bResult = FALSE;

	if (DeleteRow(nRow) == UG_SUCCESS)
	{
		RedrawAll();
		bResult = TRUE;
	}

	return bResult;
}

void CConditionGrid::SetAddRow(BOOL			bState,					//# Condition View
							   int			nElementIndex,	//# Element Index
							   BOOL		bNot,						//# Not State 
							   int			nTargetType,		//# Target Type
							   int			nTargetIndex,		//# Target Index
							   BOOL		bTargetEnable,	//# Target Enable
							   CString strItem,				//# 완성 구문 
							   BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
							   BOOL		bFieldVal)			//# 값보기 
{
	int nAddRow;
	AppendRow();
	nAddRow = GetNumberRows() - 1;
	SetRow(nAddRow,
		bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,	//# Target Enable
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기  
}


void CConditionGrid::SetInsertRow(int nRow,
								  BOOL			bState,					//# Condition View
								  int			nElementIndex,	//# Element Index
								  BOOL		bNot,						//# Not State 
								  int			nTargetType,		//# Target Type
								  int			nTargetIndex,		//# Target Index
								  BOOL		bTargetEnable,	//# Target Enable
								  CString strItem,				//# 완성 구문 
								  BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
								  BOOL		bFieldVal)			//# 값보기 
{
	InsertRow(nRow);

	SetRow(nRow,
		bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,	//# Target Enable
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기 

	SetEvenOddColor();
	SetUnSelectRow(nRow + 1);
	m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_SELECT, (LPARAM)nRow);
}

void CConditionGrid::SetRow(int nRow,
							BOOL		bState,					//# Condition View
							int			nElementIndex,	//# Element Index
							BOOL		bNot,						//# Not State 
							int			nTargetType,		//# Target Type
							int			nTargetIndex,		//# Target Index
							BOOL		bTargetEnable,	//# Target Enable
							CString strItem,				//# 완성 구문 
							BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
							BOOL		bFieldVal)			//# 값보기 
{
	CUGCell cellText;
	COLORREF clrBk;
	GetEvenOddBkColor(nRow, clrBk);

	QuickSetBackColor(COL_MAX_MIN, nRow, clrBk);

	//## Check Box ##
	QuickSetCellType(COL_CONTITOTN_CHECK, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(COL_CONTITOTN_CHECK, nRow, UGCT_CHECKBOXCHECKMARK);

	//## Condition Values Set ##
	if (bState == TRUE)
		QuickSetText(COL_CONTITOTN_CHECK, nRow, "1");
	else
		QuickSetText(COL_CONTITOTN_CHECK, nRow, "0");

	QuickSetFont(COL_CONTITOTN_CHECK, nRow, &m_fontBold);
	QuickSetBackColor(COL_CONTITOTN_CHECK, nRow, clrBk);

	//## Element Text ##
	GetCell(COL_ELEMENT_TEXT, nRow, &cellText);
	cellText.SetDataType(UGCELLDATA_CURRENCY);
	cellText.SetAlignment(UG_ALIGNLEFT | UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellText.SetFont(&m_fontBold);

	/*
	CString temp;
	if (bNot == TRUE)
	temp = "^" + (CString)CONDITION_INDEX[nRowIndex];
	else
	temp = (CString)CONDITION_INDEX[nRowIndex];
	*/

	cellText.SetText(CONDITION_INDEX[nElementIndex]);
	cellText.SetBackColor(clrBk);
	SetCell(COL_ELEMENT_TEXT, nRow, &cellText);


	//## 대상 ListBox ##
	GetCell(COL_TARGET_LIST, nRow, &cellText);
	cellText.SetOtherDataInt(nTargetType);
	SetCell(COL_TARGET_LIST, nRow, &cellText);

	QuickSetCellType(COL_TARGET_LIST, nRow, UGCT_DROPLIST);
	//QuickSetCellTypeEx(COL_TARGET_LIST, nRow, UGCT_DROPLISTHIDEBUTTON);
	QuickSetAlignment(COL_TARGET_LIST, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);

	//## ListBox Type 구분 ##
	if (nTargetType == 1)
	{
		QuickSetLabelText(COL_TARGET_LIST, nRow, TARGETDATA_TYPE1);

		if ((TARGETINDEX_TYPE1[0] <= nTargetIndex) &&
			(TARGETINDEX_TYPE1[_countof(TARGETINDEX_TYPE1) - 1] >= nTargetIndex))
		{
			int nIndex = 0;
			for (nIndex = 0; nIndex < _countof(TARGETINDEX_TYPE1); nIndex++)
			{
				if (TARGETINDEX_TYPE1[nIndex] == nTargetIndex)
					break;
			}
			nTargetIndex = nIndex;
		}
		else
			nTargetIndex = 0;

		QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE1[nTargetIndex]);
	}
	else if (nTargetType == 2)
	{
		QuickSetLabelText(COL_TARGET_LIST, nRow, TARGETDATA_TYPE2);

		if ((TARGETINDEX_TYPE2[0] <= nTargetIndex) &&
			(TARGETINDEX_TYPE2[_countof(TARGETINDEX_TYPE2) - 1] >= nTargetIndex))
		{

		}
		else
			nTargetIndex = 0;

		QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE2[nTargetIndex]);
	}
	else if (nTargetType == 3)
	{
		QuickSetLabelText(COL_TARGET_LIST, nRow, TARGETDATA_TYPE3);
		/* 0015707: -hidden-
		if ((TARGETINDEX_TYPE3[0] <= nTargetIndex) &&
			(TARGETINDEX_TYPE3[_countof(TARGETINDEX_TYPE3) - 1] >= nTargetIndex))
		{
			int nIndex = 0;
			for (nIndex = 0; nIndex < _countof(TARGETINDEX_TYPE3); nIndex++)
			{
				if (TARGETINDEX_TYPE3[nIndex] == nTargetIndex)
					break;
			}
			nTargetIndex = nIndex;
		}
		else
			nTargetIndex = 0;
		*/

		//--> 0015707: [7127] "당일내발생" 내 조건에 2분, 20분, 120분 주기 추가요청
		if (nTargetIndex >= 3)
			nTargetIndex -= 3;
		else
			nTargetIndex = 0;
		//<--
		QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE3[nTargetIndex]);
	}

	if (bTargetEnable == TRUE)
	{
		CString strListText;
		if (nTargetType == 1)
			strListText = (CString)TARGETLIST_TYPE1[nTargetIndex];
		else if (nTargetType == 2)
		{
			/*
			int nListCnt = _countof(TARGETLIST_TYPE2);
			if (nListCnt < nTargetIndex)
			strListText = (CString)TARGETLIST_TYPE2[0];
			else
			strListText = (CString)TARGETLIST_TYPE2[nTargetIndex];
			*/

			strListText = (CString)TARGETLIST_TYPE2[0];
		}
		else if (nTargetType == 3)
		{/* 0015707: -hidden-
			int nListCnt = _countof(TARGETINDEX_TYPE3);
			if (nListCnt - 3 <= nTargetIndex + 1)
				strListText = (CString)TARGETLIST_TYPE3[nTargetIndex - 3];
		*/
			strListText = (CString)TARGETLIST_TYPE3[nTargetIndex];
		}

		GetCell(COL_TARGET_LIST, nRow, &cellText);
		cellText.SetText(strListText);
		SetCell(COL_TARGET_LIST, nRow, &cellText);

		QuickSetCellType(COL_TARGET_LIST, nRow, UGCT_NORMAL);
		QuickSetCellTypeEx(COL_TARGET_LIST, nRow, UGCT_NORMAL);
		QuickSetLabelText(COL_TARGET_LIST, nRow, strListText);
	}
	QuickSetBackColor(COL_TARGET_LIST, nRow, clrBk);


	//## 조건 완성 구문 ##
	GetCell(COL_CONDITION_TEXT, nRow, &cellText);
	cellText.SetDataType(UGCELLDATA_CURRENCY);
	cellText.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cellText.SetFont(&m_fontDefault);

	strItem.TrimLeft();
	strItem.TrimRight();
	cellText.SetText(strItem);							//#<= 조건 완성 구문
	cellText.SetOtherDataInt((int)bNot);		//#<= Not Info
	cellText.SetBackColor(clrBk);
	SetCell(COL_CONDITION_TEXT, nRow, &cellText);

	//## 값보기 Check Box ##
	GetCell(COL_FIELD_CHECK, nRow, &cellText);
	cellText.SetText("");
	cellText.SetOtherDataInt((int)bFieldState);  //<= FALSE : Not have FieldView
	SetCell(COL_FIELD_CHECK, nRow, &cellText);

	if ((bFieldState == TRUE) && (bState == TRUE) && (bNot == FALSE))
	{
		QuickSetCellType(COL_FIELD_CHECK, nRow, UGCT_CHECKBOX);
		QuickSetCellTypeEx(COL_FIELD_CHECK, nRow, UGCT_CHECKBOXCHECKMARK);
		QuickSetLabelText(COL_FIELD_CHECK, nRow, "값");

		//## 값보기 Defult Values Set ##
		if (bFieldVal == TRUE)
			QuickSetText(COL_FIELD_CHECK, nRow, "1");
		else
			QuickSetText(COL_FIELD_CHECK, nRow, "0");
	}
	else
	{
		QuickSetCellType(COL_FIELD_CHECK, nRow, UGCT_NORMAL);
		QuickSetCellTypeEx(COL_FIELD_CHECK, nRow, UGCT_NORMAL);
		QuickSetLabelText(COL_FIELD_CHECK, nRow, " ");
	}
	QuickSetBackColor(COL_FIELD_CHECK, nRow, clrBk);

	//## 수정 Button ##
	int buttonIndex = AddCellType(&m_button);
	QuickSetAlignment(COL_MODIFY_BTN, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(COL_MODIFY_BTN, nRow, buttonIndex);
	QuickSetCellTypeEx(COL_MODIFY_BTN, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(COL_MODIFY_BTN, nRow, "설정");
	QuickSetBackColor(COL_MODIFY_BTN, nRow, clrBtn);
	//QuickSetFont(COL_MODIFY_BTN, nRow, &m_font2);
	QuickSetTextColor(COL_MODIFY_BTN, nRow, clrBtnText);
	QuickSetAlignment(COL_MODIFY_BTN, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

	//## 삭제 Button ##
	buttonIndex = AddCellType(&m_button);
	QuickSetAlignment(COL_DELETE_BTN, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(COL_DELETE_BTN, nRow, buttonIndex);
	QuickSetCellTypeEx(COL_DELETE_BTN, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(COL_DELETE_BTN, nRow, "삭제");
	QuickSetBackColor(COL_DELETE_BTN, nRow, clrBtn);
	QuickSetTextColor(COL_DELETE_BTN, nRow, clrBtnText);
	QuickSetAlignment(COL_DELETE_BTN, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

//	if(m_bSearchMode == FALSE)
//		return;
	//## UP Button ##
	buttonIndex = AddCellType(&m_buttonUp);
	QuickSetAlignment(COL_UP_BTN, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(COL_UP_BTN, nRow, buttonIndex);
	QuickSetCellTypeEx(COL_UP_BTN, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(COL_UP_BTN, nRow, Head_Name[COL_UP_BTN]);
	QuickSetBackColor(COL_UP_BTN, nRow, clrBtn);
	QuickSetTextColor(COL_UP_BTN, nRow, clrBtnText);
	QuickSetAlignment(COL_UP_BTN, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

	//## DOWN Button ##
	buttonIndex = AddCellType(&m_buttonDown);
	QuickSetAlignment(COL_DOWN_BTN, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(COL_DOWN_BTN, nRow, buttonIndex);
	QuickSetCellTypeEx(COL_DOWN_BTN, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(COL_DOWN_BTN, nRow, Head_Name[COL_DOWN_BTN]);
	QuickSetBackColor(COL_DOWN_BTN, nRow, clrBtn);
	QuickSetTextColor(COL_DOWN_BTN, nRow, clrBtnText);
	QuickSetAlignment(COL_DOWN_BTN, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

	//## MAX UP Button ##
	buttonIndex = AddCellType(&m_buttonMaxUp);
	QuickSetAlignment(COL_MAXUP_BTN, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(COL_MAXUP_BTN, nRow, buttonIndex);
	QuickSetCellTypeEx(COL_MAXUP_BTN, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(COL_MAXUP_BTN, nRow, Head_Name[COL_MAXUP_BTN]);
	QuickSetBackColor(COL_MAXUP_BTN, nRow, clrBtn);
	QuickSetTextColor(COL_MAXUP_BTN, nRow, clrBtnText);
	QuickSetAlignment(COL_MAXUP_BTN, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

	//## MAX DOWN Button ##
	buttonIndex = AddCellType(&m_buttonMinDown);
	QuickSetAlignment(COL_MINDOWN_BTN, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(COL_MINDOWN_BTN, nRow, buttonIndex);
	QuickSetCellTypeEx(COL_MINDOWN_BTN, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(COL_MINDOWN_BTN, nRow, Head_Name[COL_MINDOWN_BTN]);
	QuickSetBackColor(COL_MINDOWN_BTN, nRow, clrBtn);
	QuickSetTextColor(COL_MINDOWN_BTN, nRow, clrBtnText);
	QuickSetAlignment(COL_MINDOWN_BTN, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

	//RedrawAll();
}

int CConditionGrid::GetElementIndex(CString strElement)
{
	int nResult = -1;

//	int nElementCnt = _countof(CONDITION_INDEX) - 1;
	int nElementCnt = _countof(CONDITION_INDEX);

	for (int nIndex = 0; nIndex < nElementCnt; nIndex++)
	{
		if (strElement == CONDITION_INDEX[nIndex])
		{
			nResult = nIndex;
			break;
		}
	}

	return nResult;
}


int CConditionGrid::GetListBoxType(int nTagetType)
{
	int nResult = 1;

	if (nTagetType == 3254)
		return nResult;
	else if ((nTagetType >= 3200) && (nTagetType <= 3270))
		nResult = 2;

	return nResult;
}

int CConditionGrid::OnCellTypeNotify(long ID, int col, long row, long msg, long param)
{
	//	TRACE("\n# [Condition Make Grid] : CConditionGrid - col :%d, row : %d", col, row);

	if (col == COL_CONDITION_TEXT)
		return FALSE;

	CString strTemp;
	int ans;
	CWnd *pParentWnd = this;
	pParentWnd = pParentWnd->GetParent();
	if (row != -1 && col != -1)
	{
		if (col == COL_CONTITOTN_CHECK)  //<= CheckBox
		{  
			//## View TRUE/FALSE
			if(msg == UGCT_CHECKBOXSET)
			{
				//## Selected Row Struct Delete ##
				CStringArray saData;

				strTemp = "";
				ans = QuickGetText(COL_ELEMENT_TEXT, row, &strTemp);
				if (ans != UG_SUCCESS)
				{
					if (strTemp.IsEmpty() == TRUE)
						return -1;
				}

				saData.Add(strTemp);  //<= Element

				strTemp.Format("%d", row);	
				saData.Add(strTemp);		//<= Select Row

				strTemp.Format("%d", param);
				saData.Add(strTemp);  //<- View Info

				CUGCell cellText;
				GetCell(COL_FIELD_CHECK, row, &cellText);
				BOOL bFielsState = (BOOL)cellText.GetOtherDataInt();
				CString strFlag = QuickGetText(COL_FIELD_CHECK, row);

				if(strFlag.CompareNoCase("0") == 0 && (int)param != 0)
					param = 0;
				else if(strFlag.CompareNoCase("1") == 0 && (int)param != 1)
					param = 1;

				//## Field View State ##
				if ((int)param == 1) //# View : TRUE
				{
					if (bFielsState == FALSE)
					{
						QuickSetCellType(COL_FIELD_CHECK, row, UGCT_NORMAL);
						QuickSetCellTypeEx(COL_FIELD_CHECK, row, UGCT_NORMAL); 

						QuickSetText(COL_FIELD_CHECK, row, " ");
					}
					else 
					{
						QuickSetCellType(COL_FIELD_CHECK, row, UGCT_CHECKBOX);
						QuickSetCellTypeEx(COL_FIELD_CHECK, row, UGCT_CHECKBOXCHECKMARK);

						QuickSetText(COL_FIELD_CHECK, row, "0");
					}
				}
				else if ((int)param == 0) //# View : FALSE
				{
					//## View Field Checkbox State Check! ##
					CString strFiledView = "";
					ans = QuickGetText(COL_FIELD_CHECK, row, &strFiledView);
					if (ans != UG_SUCCESS)
					{
						if (strFiledView.IsEmpty() == TRUE)
							return -1;
					}

					if (strFiledView == "1")
						QuickSetText(COL_FIELD_CHECK, row, "0");

					//## View Field Checkbox Hide ##
					if (bFielsState == TRUE)
					{
						QuickSetAlignment(COL_FIELD_CHECK, row, UG_ALIGNCENTER | UG_ALIGNVCENTER);
						QuickSetCellType(COL_FIELD_CHECK, row, UGCT_NORMAL);
						QuickSetCellTypeEx(COL_FIELD_CHECK, row, UGCT_NORMAL); 

						QuickSetText(COL_FIELD_CHECK, row, " ");
					}
				}			
		
				//## Selected Row Struct Insert ##
				pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_CONDITION_CHECK, (LPARAM)&saData);
				Invalidate();
			}
		}
		else if (col == COL_TARGET_LIST) //<= 검색 대상 
		{
			if (msg == UGCT_DROPLISTSTART)
			{
				if (m_bGridType == TRUE)
					pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_TARGET_LIST_START, (LPARAM)NULL);
			}
			else if (msg == UGCT_DROPLISTSELECT)
			{
				CStringArray saData;

				strTemp = "";
				ans = QuickGetText(COL_ELEMENT_TEXT, row, &strTemp);
				if (ans != UG_SUCCESS)
				{
					if (strTemp.IsEmpty() == TRUE)
						return -1;
				}

				saData.Add(strTemp);  //<= Element

				strTemp.Format("%d", row);
				saData.Add(strTemp);  //<= Select Row 

				CString strListText = *((CString *)param);

				CUGCell cellText;
				GetCell(COL_TARGET_LIST, row, &cellText);
				int nTargetType = cellText.GetOtherDataInt();
				strTemp.Format("%d", nTargetType);
				saData.Add(strTemp);  //<= Target Type

				int nTargetIndex;
				if (nTargetType == 1)
				{
					for (nTargetIndex = 0; nTargetIndex < _countof(TARGETLIST_TYPE1); nTargetIndex++)
					{
						if (TARGETLIST_TYPE1[nTargetIndex] == strListText)
						{
							strTemp.Format("%d", TARGETINDEX_TYPE1[nTargetIndex]);
							saData.Add(strTemp);  //<= ListBox Index Val
							break;
						}
					}
				}
				else if (nTargetType == 2)
				{
					for (nTargetIndex = 0; nTargetIndex < _countof(TARGETLIST_TYPE2); nTargetIndex++)
					{
						if (TARGETLIST_TYPE2[nTargetIndex] == strListText)
						{
							strTemp.Format("%d", TARGETINDEX_TYPE2[nTargetIndex]);
							saData.Add(strTemp);  //<= ListBox Index Val
							break;
						}
					}
				}
				else if (nTargetType == 3)
				{
					for (nTargetIndex = 0; nTargetIndex < _countof(TARGETLIST_TYPE3); nTargetIndex++)
					{
						if (TARGETLIST_TYPE3[nTargetIndex] == strListText)
						{
							strTemp.Format("%d", TARGETINDEX_TYPE3[nTargetIndex]);
							saData.Add(strTemp);  //<= ListBox Index Val
							break;
						}
					}
				}

				//## 순차검색 : List 통일 ##
				if (m_bSearchMode == FALSE)  //## 순차검색 
					SetModifyAllTargetCol(nTargetIndex);

				//## Selected Row Struct Insert ##
				pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_TARGET_LIST, (LPARAM)&saData);
			}
		}
		else if (col == COL_CONDITION_TEXT) //<= Condition Title 
		{

		}
		else if (col == COL_FIELD_CHECK) //<= 값보기 CheckBox 
		{
			if (msg == UGCT_CHECKBOXSET)
			{
				//## Selected Row Struct Delete ##
				CStringArray saData;

				strTemp = "";
				ans = QuickGetText(COL_ELEMENT_TEXT, row, &strTemp);
				if (ans != UG_SUCCESS)
				{
					if (strTemp.IsEmpty() == TRUE)
						return -1;
				}

				saData.Add(strTemp);  //<= Element

				strTemp.Format("%d", row);
				saData.Add(strTemp);  //<= Select Row

				strTemp.Format("%d", param);
				saData.Add(strTemp);  //<= Field View Info

				//## Selected Row Struct Insert ##
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_VALVIEW_CHECK, (LPARAM)&saData);
			}
		}
		else if (col == COL_MODIFY_BTN) //<= Modify Button
		{
			if (msg == UGCT_BUTTONCLICK)
			{
				//## Selected Row Struct Delete ##
				CStringArray saData;

				strTemp = "";
				ans = QuickGetText(COL_ELEMENT_TEXT, row, &strTemp);  
				if (ans == UG_SUCCESS)
				{
					if (strTemp.IsEmpty() == TRUE)
						return -1;
				}

				saData.Add(strTemp);  //<= Element

				strTemp.Format("%d", row);
				saData.Add(strTemp);      //<= Select Row

				int nDisplayRow;
				int nRowCnt = GetNumberRows() - 1;
				if (nRowCnt == 0)
					nDisplayRow = 0;
				else 
				{
					if (nRowCnt > row)
						nDisplayRow = row;
					else if (nRowCnt == row)
						nDisplayRow = row - 1;
				}
				strTemp.Format("%d", nDisplayRow);
				saData.Add(strTemp);	//<= Display Row

				CRect reCell;
				GetCellRect(COL_MODIFY_BTN, nDisplayRow, reCell);
				ClientToScreen(reCell);

				strTemp.Format("%d", reCell.top);
				saData.Add(strTemp);	//<= Display Row => Top Screen Pos

				//## Selected Row Struct Delete ##
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_MODIFY_BTN, (LPARAM)&saData);
			}
		}
		else if (col == COL_DELETE_BTN) //<= Delete Button
		{
			if (msg == UGCT_BUTTONCLICK)
			{
				CStringArray saData;
				strTemp = "";
				ans = QuickGetText(COL_ELEMENT_TEXT, row, &strTemp);
				if (ans == UG_SUCCESS)
				{
					if (strTemp.IsEmpty() == TRUE)
						return -1;
				}
				int nRow_Delete = row;

				saData.Add(strTemp);  //<= Element

				if (GetNumberRows() - 1 == row)
					row--;
				strTemp.Format("%d", row);
				saData.Add(strTemp);		//<= After Select Row

				if (row == -1)
				{
					ClearAll();
					//## Selected Row Struct All Delete ##
					//m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_HEAD_DELETEALL, (LPARAM)0);
					m_pParentWnd->PostMessage(WM_GRID_EVENT, (WPARAM)NM_HEAD_DELETEALL, (LPARAM)0);  //Modified by cyberahn@KOSCOM 2006.07.13. 죽는버그..수정.
				}
				else
				{
					//## Selected Row delete ##
					RemoveDataSource(nRow_Delete);
					DeleteRow(nRow_Delete);

					//## Selected Row Struct Delete ##
					m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_DELETE_BTN, (LPARAM)&saData);
				}
			}
		}
		else if (col == COL_UP_BTN) 
		{
			if(msg == UGCT_BUTTONUP && row > 0)
			{
				m_loldRow = row;
				m_lnewRow = row - 1;
				ConditionRowChange(row, m_lnewRow);

				CUIntArray* iaData = new CUIntArray;
				iaData->Add(0);
				iaData->Add(m_loldRow);
				iaData->Add(m_lnewRow);
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_UP_BTN, (LPARAM)iaData);
				iaData->RemoveAll();
				iaData->FreeExtra();
				delete iaData;
			}			
		}
		else if (col == COL_DOWN_BTN) 
		{
			if(msg == UGCT_BUTTONUP && row < GetNumberRows() - 1 && row >= 0)
			{
				m_loldRow = row;
				m_lnewRow = row + 1;
				ConditionRowChange(row, m_lnewRow);

				CUIntArray* iaData = new CUIntArray;
				iaData->Add(0);
				iaData->Add(m_loldRow);
				iaData->Add(m_lnewRow);
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_DOWN_BTN, (LPARAM)iaData);	
				iaData->RemoveAll();
				iaData->FreeExtra();
				delete iaData;
			}	
			
		}
		else if (col == COL_MAXUP_BTN) 
		{
			if(msg == UGCT_BUTTONUP && row > 0)
			{
				m_loldRow = row;
				m_lnewRow = 0;

				CStringArray saData;
				CString strTemp = "";
				saData.Add("0");
				QuickGetText(COL_ELEMENT_TEXT, m_loldRow, &strTemp); 
				saData.Add(strTemp);
				QuickGetText(COL_ELEMENT_TEXT, m_lnewRow, &strTemp);  
				saData.Add(strTemp);

				ConditionRowChange(row, m_lnewRow);				
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_MAXUP_BTN, (LPARAM)&saData);			
			}	
			
		}
		else if (col == COL_MINDOWN_BTN) 
		{
			if(msg == UGCT_BUTTONUP &&row < GetNumberRows() - 1 && row >= 0)
			{
				m_loldRow = row;
				m_lnewRow = GetNumberRows() - 1;

				CStringArray saData;
				CString strTemp = "";
				saData.Add("0");
				QuickGetText(COL_ELEMENT_TEXT, m_loldRow, &strTemp); 
				saData.Add(strTemp);
				QuickGetText(COL_ELEMENT_TEXT, m_lnewRow, &strTemp);  
				saData.Add(strTemp);

				ConditionRowChange(row, m_lnewRow);				
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_MINDOWN_BTN, (LPARAM)&saData);		
			}				
		}
	}

	m_bLeftBtnState = FALSE;
	m_bDuringDrag = FALSE;

	return TRUE;
}


void CConditionGrid::SetModifyAllTargetCol(int nTargetIndex, int nOutType, BOOL bEnable)
{
	CString strListText, strOutTypeText;
	COLORREF clrBk;

	CUGCell cellText;
	int nGetTargetType;
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetCell(COL_TARGET_LIST, nRow, &cellText);
		nGetTargetType = cellText.GetOtherDataInt();

		if (bEnable == TRUE)
		{
			if (nGetTargetType == 1)
			{
				strListText = (CString)TARGETLIST_TYPE1[nTargetIndex];

				if (nOutType == 0)		//# 일간 고정
				{
					strOutTypeText = cellText.GetText();
					if (strOutTypeText == (CString)TARGETLIST_TYPE1[0])
						strListText = (CString)TARGETLIST_TYPE1[0];
				}		
			}
			else if (nGetTargetType == 2)
				strListText = (CString)TARGETLIST_TYPE2[0];
			else if (nGetTargetType == 3)
			{
				int nListCnt = _countof(TARGETINDEX_TYPE3) - 1;

				if ((TARGETINDEX_TYPE3[0] <= nTargetIndex) &&
					(TARGETINDEX_TYPE3[nListCnt] >= nTargetIndex))
					strListText = (CString)TARGETLIST_TYPE3[nTargetIndex - 3];
				else
					cellText.GetText(&strListText);
			}

			cellText.SetText(strListText);
			SetCell(COL_TARGET_LIST, nRow, &cellText);

			QuickSetCellType(COL_TARGET_LIST, nRow, UGCT_NORMAL);
			QuickSetCellTypeEx(COL_TARGET_LIST, nRow, UGCT_NORMAL);
			QuickSetLabelText(COL_TARGET_LIST, nRow, strListText);
		}
		else 
		{
			//# 2004.09.23 => 개별 적용시 현상태 유지!

			QuickSetCellType(COL_TARGET_LIST, nRow, UGCT_DROPLIST);
			QuickSetAlignment(COL_TARGET_LIST, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);

			if (nGetTargetType == 1)
			{
				QuickSetLabelText(COL_TARGET_LIST, nRow, TARGETDATA_TYPE1);

				if (nTargetIndex != -1)
				{
					QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE1[nTargetIndex]);
				}
				else if ((int)(_countof(TARGETLIST_TYPE1) - 1) < nTargetIndex)
					QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE1[0]);
			}
			else if (nGetTargetType == 2)		//# 일간 고정
			{
				QuickSetCellType(COL_TARGET_LIST, nRow, UGCT_NORMAL);
				QuickSetCellTypeEx(COL_TARGET_LIST, nRow, UGCT_NORMAL);

				if ((int)(_countof(TARGETLIST_TYPE2) - 1) < nTargetIndex)
					QuickSetLabelText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE2[0]);
			}
			else if (nGetTargetType == 3)
			{
				QuickSetLabelText(COL_TARGET_LIST, nRow, TARGETDATA_TYPE3);

				if (nTargetIndex != -1)
					QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE3[nTargetIndex]);
				else if ((int)(_countof(TARGETLIST_TYPE3) - 1) < nTargetIndex - 3)
					QuickSetText(COL_TARGET_LIST, nRow, TARGETLIST_TYPE3[0]);
			}

			GetEvenOddBkColor(nRow, clrBk);
			QuickSetBackColor(COL_TARGET_LIST, nRow, clrBk);
		}
	}

	if (m_nLastSelRow > -1)
	{
		CUGCell	cell;
		for (int nCol = 0; nCol < GetNumberCols(); nCol++)
		{
			GetCell(nCol, m_nLastSelRow, &cell);
			SetBackColorIfNotBtnStep(&cell, clrSelRow);
			SetCell(nCol, m_nLastSelRow, &cell);
		}

		RedrawRow(m_nLastSelRow);
	}
	RedrawRange(COL_TARGET_LIST, 0, COL_TARGET_LIST, GetNumberRows());

	SetSelectRow(m_nLastSelRow);
}

void CConditionGrid::GotoLastRow()
{
	int nRowIndex = GetNumberRows() - 1;
	GotoRow(nRowIndex);

	RedrawAll();
}

BOOL CConditionGrid::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}

int CConditionGrid::SetAlignElement(int nRowIndex)
{
	//## 조건명 Index 재배치 ##
	CUGCell cell;

	CString strTemp;
	int nElementIndex = nRowIndex;
	int nTest = GetNumberRows();
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		int ans = QuickGetText(COL_CONTITOTN_CHECK, nRow, &strTemp);
		if (ans != UG_SUCCESS)
		{
			if (strTemp.IsEmpty() == TRUE)
				return -1;
		}

		BOOL bView = atol(strTemp);
//		if (bView == TRUE)
		{
			QuickSetText(COL_ELEMENT_TEXT, nRow, CONDITION_INDEX[nElementIndex]);
			nElementIndex++;
		}
//		else
//			QuickSetText(COL_ELEMENT_TEXT, nRow, "-"); 
	}

	RedrawAll();

	return nElementIndex;
}


CString CConditionGrid::GetCellDate(int col, int row)
{
	CUGCell cell;
	CString temp;
	GetCell(col, row, &cell);
	cell.GetText(&temp);

	return temp;
}


void CConditionGrid::SetCellDate(int col, int row, CString strData)
{
	CUGCell cellText;
	GetCell(col, row, &cellText);

	strData.TrimLeft();
	strData.TrimRight();
	cellText.SetText(strData);
	SetCell(col, row, &cellText);
}



void CConditionGrid::GetInsertElement(CStringArray &saInsetedElement)
{
	CUGCell cell;
	CString tempElement;
	if(saInsetedElement.GetSize()) 
		saInsetedElement.RemoveAll();

	int nRowCnt = GetNumberRows();
	if (nRowCnt > 0)
	{
		for (int nRow = 0; nRow < nRowCnt; nRow++)
		{
			GetCell(COL_ELEMENT_TEXT, nRow, &cell);  //# Element
			cell.GetText(&tempElement);

			tempElement.TrimLeft();
			tempElement.TrimRight();
			tempElement.MakeLower();

			saInsetedElement.Add(tempElement);
		}
	}
}

CString CConditionGrid::GetRowToElement(int nRow)
{
	CString strMsg;
	CString strResult = "";

	if (GetNumberRows() < nRow)
	{
		strMsg.Format("\n# CConditionGrid::GetRowToElement => nRow Error!");
		TRACE(strMsg);
		return strResult;
	}

	CString strElement;
	int ans = QuickGetText(COL_ELEMENT_TEXT, nRow, &strElement);
	if (ans == UG_SUCCESS)
		strResult = strElement;

	TRACE("\n# CConditionGrid::GetRowToElement => Row : %d, Element : %s", nRow, strElement);

	return strResult;
}

void CConditionGrid::OnSetFocus(int section)
{
	m_bSetFocus = TRUE;
}

void CConditionGrid::OnKillFocus(int section)
{
	m_bSetFocus = FALSE;

	m_bLeftBtnState = FALSE;
	m_bDuringDrag = FALSE;

	//SetUnSelectRow();
}

void CConditionGrid::SetSelectRow(int nRow)
{
	if (!m_bProcessSelectChange) 
		return;

	COLORREF clrBk;
	CUGCell	cell;
	for (int nCol = 0; nCol < GetNumberCols(); nCol++)
	{
		if (m_nLastSelRow != -1)
		{
			GetEvenOddBkColor(m_nLastSelRow, clrBk);

			GetCell(nCol, m_nLastSelRow, &cell);
			SetBackColorIfNotBtnStep(&cell, clrBk);
			SetCell(nCol, m_nLastSelRow, &cell);
		}

		if (nRow != -1)
		{
			GetCell(nCol, nRow, &cell);
			SetBackColorIfNotBtnStep(&cell, clrSelRow);
			SetCell(nCol, nRow, &cell);
		}
	}

	m_nLastSelRow = nRow;
	RedrawAll();

	//## LButton flag Create ##
	if ((m_bDuringDrag == FALSE) && (m_bLeftBtnState == TRUE))
		m_bLeftBtnState = FALSE;
}

void CConditionGrid::SetSelectRow(int nOldRow, int nNewRow)
{
	if (!m_bProcessSelectChange) 
		return;

	COLORREF clrBk;
	CUGCell	cell;
	for (int nCol = 0; nCol < GetNumberCols(); nCol++)
	{
		if (nOldRow != -1)
		{
			GetEvenOddBkColor(nOldRow, clrBk);

			GetCell(nCol, nOldRow, &cell);
			SetBackColorIfNotBtnStep(&cell, clrBk);
			SetCell(nCol, nOldRow, &cell);
		}

		if (nNewRow != -1)
		{
			GetCell(nCol, nNewRow, &cell);
			SetBackColorIfNotBtnStep(&cell, clrSelRow);
			SetCell(nCol, nNewRow, &cell);
		}
	}

	// 마지막으로 선택한 ROW를 기억하도록 합니다.
	m_nLastSelRow = nNewRow;
	RedrawAll();

	//## LButton flag Create ##
	if ((m_bDuringDrag == FALSE) && (m_bLeftBtnState == TRUE))
		m_bLeftBtnState = FALSE;
}

void CConditionGrid::SetUnSelectRow(int nRow, BOOL bDel)
{
	int nCount, nLength;
	COLORREF clrBk = 0;
	CUGCell	cell;

	if (nRow == -1)
		nRow = m_nLastSelRow;
	
	if(bDel)
		nRow--; // 20100821 박찬모 현재 선택되어 있는 row보다 낮은 row로 선택될시 m_nLastSelRow값과 실제 선택 색이 칠해져 있는 row값이 틀림
	
	nLength = GetNumberCols();
	for (nCount = 0; nCount < nLength; nCount++)
	{
		if (nRow != -1)
		{
			GetEvenOddBkColor(nRow, clrBk);

			GetCell(nCount, nRow, &cell);
			SetBackColorIfNotBtnStep(&cell, clrBk);
			SetCell(nCount, nRow, &cell);
		}
	}
	RedrawAll();

	m_nLastSelRow = -1;
}


void CConditionGrid::SetDetailViewState(BOOL bDetail)
{
	m_bDetailView = bDetail;

	AdjustComponentSizes();
	RedrawAll();
}

void CConditionGrid::SetHeadColor(BOOL bFocus)
{
	m_bSetFocus = bFocus;

	//CUGCell cell;
	int nLength = _countof(Head_Width);

	for (int nCount = 0; nCount < nLength; nCount++ )
	{
		//GetColDefault(nCount, &cell);
		//cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		//SetColDefault(nCount, &cell);
		//QuickSetText(nCount, -1, Head_Name[nCount]);
		//SetColWidth(nCount, Head_Width[nCount]);
		if (bFocus == TRUE)
		{
			QuickSetBackColor(nCount, -1, clrHeader_Focus);
			QuickSetTextColor(nCount, -1, COLOR_HEADERTEXT_FOCUS);
		}
		else if (bFocus == FALSE)
		{
			QuickSetBackColor(nCount, -1, m_clrHeader);
			QuickSetTextColor(nCount, -1, COLOR_HEADERTEXT);
		}
	}

	QuickSetText(COL_CONTITOTN_CHECK, -1, "구분");
	JoinCells(COL_CONTITOTN_CHECK, -1, COL_ELEMENT_TEXT, -1);

	Invalidate();
}


void CConditionGrid::OnSetup()
{
	int	nLength;
	CUGCell	cell;

	nLength = _countof(Head_Width);
	SetNumberCols(nLength);

	SetSH_Width(0);
	SetSH_NumberCols(0);

	SetHighlightRow(FALSE);
	EnableColSwapping(FALSE);

	CString strHeadName;
	for (int nCol = 0; nCol < nLength; nCol++ )
	{
		GetColDefault(nCol, &cell);
		cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		SetColDefault(nCol, &cell);

		//# 2005.07.28 => Danny
		if (COL_CONDITION_TEXT == nCol)
		{
			if (m_bGridType == TRUE)
				strHeadName = "순차 " + (CString)Head_Name[nCol];
			else
				strHeadName = "일반 " + (CString)Head_Name[nCol];
		}
		else 
			strHeadName = Head_Name[nCol];

		QuickSetText(nCol, -1, strHeadName);

		//QuickSetText(nCol, -1, Head_Name[nCol]);

		if ((nCol == COL_MAX_MIN) && (m_bGridType == FALSE))
		{
			QuickSetText(nCol, -1, "");
			SetColWidth(nCol, -1);
		}
		else
			SetColWidth(nCol, Head_Width[nCol]);

		QuickSetBackColor(nCol, -1, clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_HEADERTEXT);
	}

	QuickSetText(COL_CONTITOTN_CHECK, -1, "구분");
	JoinCells(COL_CONTITOTN_CHECK, -1, COL_ELEMENT_TEXT, -1);

	/*
	CSize	szFont = GetFontSize();
	SetDefRowHeight( szFont.cy + 2);
	SetTH_Height( szFont.cy + 4 );
	*/

	SetDefRowHeight(m_nDefRowHeight);
	SetTH_Height(m_nDefRowHeadHeight);

	SetCancelMode(FALSE);
	EnableMenu(TRUE);

	/*
	PrintSetOption(UG_PRINT_TOPHEADING,TRUE);
	PrintSetOption(UG_PRINT_LEFTMARGIN,10);
	PrintSetOption(UG_PRINT_RIGHTMARGIN,10);
	*/

	EnableUpdate(TRUE);
	UseHints(TRUE);

	DragDropTarget(TRUE);
	SetMultiSelectMode(FALSE);

	SetUserSizingMode(FALSE);
	AdjustComponentSizes();

	SetHS_Height(0);   //# 가로

	m_bViewAllField = FALSE;
	m_bViewState = TRUE;

	SetHighlightRow(TRUE);
	//SetPopUpMenu();

	m_pParentWnd = this;
	m_pParentWnd = m_pParentWnd->GetParent();
}


void CConditionGrid::SetPopUpMenu()
{
	EnableMenu(TRUE);
	AddMenuItem(1001,"그룹 추가");
	AddMenuItem(1002,"그룹 삽입(위)");
	AddMenuItem(1003,"그룹 삽입(아래)");
	AddMenuItem(1004,"그룹 삭제");
}

void CConditionGrid::OnMenuCommand(int col,long row,int section,int item)
{
	// if right mouse click outside grid area, exit
	if (section != UG_GRID)
		return;

	switch (item) 
	{
	case 1001:		//# 그룹 추가
		m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_GROUP_ADD, (LPARAM)0);
		break;
	case 1002:		//# 그룹 삽입(위)
		m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_GROUP_INSERTUP, (LPARAM)0);
		break;
	case 1003:		//# 그룹 삽입(아래)
		m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)0);
		break;
	case 1004:		//# 그룹 삭제
		m_pParentWnd->PostMessage(WM_GRID_EVENT, (WPARAM)NM_GROUP_DELETE, (LPARAM)0);
		break;
	}

	RedrawAll();
}
/***************************************************
return UG_SUCCESS to allow the menu to appear
return 1 to not allow the menu to appear
****************************************************/
int CConditionGrid::OnMenuStart(int col,long row,int section)
{
	if (section != UG_GRID)
		return FALSE;
	return TRUE;
}

int CConditionGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

void CConditionGrid::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	// If Mouse LButton Down 
	if (updn && !processed)
	{		
		if (row == -1)
		{
			if ((col == COL_MAX_MIN) && (m_bGridType == TRUE))
			{
				//## Case Order Grid ##
				if (SetMakeRowVisible(m_bViewState) == TRUE)
				{
					m_bViewState = !m_bViewState;
					RedrawAll();
					m_pParentWnd->PostMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_MAXMIN, (BOOL)!m_bViewState);
				}
			}
			else if ((col == COL_CONTITOTN_CHECK) || (col == COL_ELEMENT_TEXT))
			{
				//# 각 그룹의 Element Align!
				//m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ALIGN_ELEMENT, (LPARAM)0);
			}
			else if (col == COL_FIELD_CHECK)		//# 값보기
			{
				if (GetNumberRows() <= 0)
					return;

				if (m_bViewAllField == TRUE)
					m_bViewAllField = FALSE;
				else
					m_bViewAllField = TRUE;

				SetFieldAllCheck(m_bViewAllField);

				//## 값보기 모두 Check! ##
				m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_HEAD_VALVIEWALL_CHECK, (BOOL)m_bViewAllField);
			}
			else if (col == COL_DELETE_BTN)	//# All Delete
			{
				if (GetNumberRows() <= 0)
					return;

				//if (MessageBox("전체 삭제하시겠습니까?", "전체 삭제", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
				//{
				ClearAll();
				//## Selected Row Struct All Delete ##
				m_pParentWnd->PostMessage(WM_GRID_EVENT, (WPARAM)NM_HEAD_DELETEALL, (LPARAM)0);
				//}
			}

			m_pParentWnd->PostMessage(WM_GRID_EVENT, NM_GRID_SETFOCUS, (LPARAM)0);
		}
	}
}

BOOL CConditionGrid::SetMakeRowVisible(BOOL bViewState)
{
	int nRow;
	int nRowCnt = GetNumberRows();

	if (nRowCnt <= 0)
		return FALSE;

	if (bViewState == TRUE) //# MAX
	{
		for(nRow = 0; nRow < nRowCnt; nRow++)
			SetRowHeight(nRow, m_nDefRowHeight);

		QuickSetText(COL_MAX_MIN, -1, "-");
	}
	else if (m_bViewState == FALSE) //# MIN
	{
		for (nRow = 0; nRow < nRowCnt; nRow++)
			SetRowHeight(nRow, 0);

		QuickSetText(COL_MAX_MIN, -1, "+");
	}

	return TRUE;
}


void CConditionGrid::OnTH_RClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed) 
{
	/*
	// If Mouse RButton Down 
	if( updn && !processed)
	{		
	if (row == -1)
	{
	if (col == 4)
	{

	}
	}
	}
	*/
}


void CConditionGrid::SetFieldAllCheck(BOOL nView)
{
	CUGCell cell;

	for (int nRowIndex = 0; nRowIndex < GetNumberRows(); nRowIndex++)
	{
		GetCell(COL_FIELD_CHECK, nRowIndex, &cell);
		if (cell.GetCellType() == UGCT_CHECKBOX)
		{
			//## 값보기 Defult Values Set ##
			if (nView == TRUE)
				QuickSetText(COL_FIELD_CHECK, nRowIndex, "1");
			else
				QuickSetText(COL_FIELD_CHECK, nRowIndex, "0");
		}
	}
	RedrawAll();
}

int CConditionGrid::OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags)
{
	int  nResult = 0;

	if (m_nSortCol == 0)
		// Is Just String
	{
		nResult = cell1->m_string.Compare(cell2->m_string);
	}
	else 
		// Is Numeral
	{
		if ( cell1->m_lOtherData > cell2->m_lOtherData )
		{
			nResult = 1;
		}
		else if ( cell1->m_lOtherData < cell2->m_lOtherData )
		{
			nResult = -1;
		}
	}

	// 내림차순일경우에는 반대로 적용해 줍니다.
	nResult *= (-(flags * 2 - 3));
	return nResult;
}

void CConditionGrid::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(clrHeader);
}

void CConditionGrid::OnRowChange(long oldrow, long newrow)
{
	/*
	if (!m_bProcessSelectChange) 
	return;

	int nCount, nLength;
	COLORREF clrBk;
	CUGCell	cell;

	nLength = GetNumberCols();
	for (nCount = 0; nCount < nLength; nCount++)
	{
	if (oldrow != -1)
	{
	GetEvenOddBkColor(oldrow, clrBk);

	GetCell(nCount, oldrow, &cell);
	SetBackColorIfNotBtnStep(&cell, clrBk);
	SetCell(nCount, oldrow, &cell);
	}

	if (m_nLastSelRow != -1 && m_nLastSelRow != oldrow)
	{
	GetEvenOddBkColor( m_nLastSelRow, clrBk );

	GetCell(nCount, m_nLastSelRow, &cell);
	SetBackColorIfNotBtnStep(&cell, clrBk);
	SetCell(nCount, m_nLastSelRow, &cell);
	}

	if (newrow != -1)
	{
	GetCell(nCount, newrow, &cell);
	SetBackColorIfNotBtnStep(&cell, clrSelRow);
	SetCell(nCount, newrow, &cell);
	}
	}

	// 마지막으로 선택한 ROW를 기억하도록 합니다.
	m_nLastSelRow = newrow;
	RedrawAll();

	//## LButton flag Create ##
	if ((m_bDuringDrag == FALSE) && (m_bLeftBtnState == TRUE))
	m_bLeftBtnState = FALSE;
	*/
}

void CConditionGrid::SetStepColor(BOOL bStepFlag, int nSearchType, int nStepCnt)
{
	CUGCell cell;

	int nSetStepCnt = 0;
	if (bStepFlag == TRUE)  //# 단계검색 
	{
		long nCount, nRows;
		nRows = GetNumberRows();

		int ans;
		CString strCheck;
		int nDataIndex;
		for (nCount = 0; nCount < nRows; nCount++)
		{
			if (nSearchType == 0)
				nDataIndex = nCount;
			else if (nSearchType == 1)
				nDataIndex = (nRows - 1) - nCount;

			ans = QuickGetText(COL_CONTITOTN_CHECK, nDataIndex, &strCheck);
			if (ans == UG_SUCCESS)
			{
				if (atoi(strCheck) == 1)
				{
					GetCell(COL_ELEMENT_TEXT, nDataIndex, &cell);
					SetBackColorIfNotBtnStep(&cell, clrBtnSearch);
					SetCell(COL_ELEMENT_TEXT, nDataIndex, &cell);
					nSetStepCnt++;
				}

				if (nStepCnt == nSetStepCnt)
					break;
			}
			else
				break;
		}
	}
	else if (bStepFlag == FALSE)  //# 일괄검색 
		SetEvenOddColor();

	RedrawAll();
}

inline void CConditionGrid::SetBackColorIfNotBtnStep(CUGCell *pCell, COLORREF clrSet)
{
	COLORREF clrCurrentBack;
	clrCurrentBack = pCell->GetBackColor();

	if ((clrCurrentBack != clrBtn) && (clrCurrentBack != clrBtnSearch))
		pCell->SetBackColor(clrSet);
}

inline void CConditionGrid::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;

	if( nRow % 2 ) 
		clrBk = clrOddRow;
}

inline void CConditionGrid::SetBackColorBtn(CUGCell *pCell, COLORREF clrSet)
{
	COLORREF clrCurrentBack;
	clrCurrentBack = pCell->GetBackColor();

	if (clrCurrentBack != clrBtn)
		pCell->SetBackColor(clrSet);
}


void CConditionGrid::SetEvenOddColor()
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
			SetBackColorBtn(&cell, clrBk);
			SetCell(nCount2, nCount, &cell);
		}
	}

	m_nLastSelRow = -1;
}

void CConditionGrid::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if (row != -1 && col != -1)
	{

	}
}

void CConditionGrid::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if (m_bSetFocus == FALSE)
		return;

	CUGCell cellText;
	GetCell(COL_CONDITION_TEXT, row, &cellText);
	CString strText = cellText.GetText();

	if(strText.IsEmpty())
		return;

	if (updn)
	{
		if(col == 4)		// 마우스로 조건을 선택했을때만 업데이트
			m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_UPDATE, NULL);

		if (row != -1 && col == COL_CONDITION_TEXT)		//# 지표 명
		{
			m_loldRow = -1;
			m_lnewRow = -1;

			m_bLeftBtnState = TRUE;
			m_bDuringDrag = FALSE;

			m_loldRow = row;  //<= Drag&Drop Old Row

			m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_SELECT, (LPARAM)row);
		}
	}
	else if (!updn)
	{
		if( row != -1 && col == COL_CONDITION_TEXT)
		{
			if (m_bUserDragDrop == TRUE)
			{
				if (m_bDuringDrag == TRUE)  //<= DragDrop During End
				{
					if (OnDragDropRowChange(m_loldRow, m_lnewRow) == TRUE)
					{
						if ((m_loldRow - m_lnewRow) != 0)
						{
							CUIntArray iaData;
							iaData.Add(m_loldRow);
							iaData.Add(m_lnewRow);

							m_pParentWnd->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ROW_DRAGDROP, (LPARAM)&iaData);
						}
					}
				}
			}
		}

		/*
		if (col == COL_CONDITION_TEXT)
		m_pParentWnd->PostMessage(WM_GRID_EVENT, NM_GRID_SETFOCUS, (LPARAM)0);
		*/

		m_bLeftBtnState = FALSE;
		m_bDuringDrag = FALSE;
	}
}

void CConditionGrid::OnMouseMove(int col, long row, POINT *point,UINT nFlags, BOOL processed)
{
	if (row != -1 && col == COL_CONDITION_TEXT)
	{
		if (m_bLeftBtnState == TRUE) //<= DragDrop During
		{
			m_bDuringDrag = TRUE;
			m_lnewRow = row;  //<= Drag&Drop Now Row

			SetSelectRow(row);
		}
		else if (m_bLeftBtnState == FALSE)
			m_bDuringDrag = FALSE;
	}
}


void	CConditionGrid::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	if (col != -1 && row != -1)
	{

	}
}

int CConditionGrid::OnHint(int col, long row, int section, CString *string)
{
	if (section == UG_GRID)
	{
		if (col == COL_CONDITION_TEXT)
		{		
			CUGCell cellText;
			GetCell(COL_CONDITION_TEXT, row, &cellText);

			CString strTemp;
			strTemp = cellText.GetText();
			if (strTemp.GetLength() > 86)
			{
				if (cellText.GetOtherDataInt() == 1)
					strTemp = strTemp.Left(75) + "...(결과제외)";
				else 
					strTemp = strTemp.Left(86) + "...";

				*string = strTemp;
			}
			else
				*string = strTemp;
		}
	}
	return TRUE;
}


//## Grid Drag & Drop Function ################################
BOOL CConditionGrid::OnDragDropRowChange(long nOldRow, long nNewRow)
{
	BOOL bResultFlag = FALSE;

	if ((nOldRow == nNewRow) || (nOldRow == -1) || (nNewRow == -1))
		return bResultFlag;

	long lnewRow;
	if (nOldRow - nNewRow < 0)
		lnewRow = nNewRow + 1;
	else if (m_loldRow - nNewRow > 0)
	{
		lnewRow = nNewRow;
		nOldRow = nOldRow + 1; 
	}

	//## OldRow와 Row 자리 바꿈 
	int ans;
	ans = InsertRow(lnewRow);
	if (ans ==  UG_SUCCESS)
	{
		CUGCell cellText;

		//## Check Box ##
		CString strTemp;
		QuickGetText(COL_CONTITOTN_CHECK, nOldRow, &strTemp);  
		BOOL bConditionView = (atoi(strTemp) == 1)?TRUE:FALSE;

		QuickSetCellType(COL_CONTITOTN_CHECK, lnewRow, UGCT_CHECKBOX);
		QuickSetCellTypeEx(COL_CONTITOTN_CHECK, lnewRow, UGCT_CHECKBOXCHECKMARK);
		QuickSetText(COL_CONTITOTN_CHECK, lnewRow, strTemp);
		QuickSetFont(COL_CONTITOTN_CHECK, lnewRow, &m_fontBold);
		//QuickSetLabelText(0, nRowIndex, CONDITION_INDEX[nRowIndex]);
		//QuickSetBackColor(nCols, row, clrBk);

		GetCell(COL_ELEMENT_TEXT, nOldRow, &cellText);
		strTemp = cellText.GetText();
		GetCell(COL_ELEMENT_TEXT, lnewRow, &cellText);
		cellText.SetDataType(UGCELLDATA_CURRENCY);
		cellText.SetAlignment(UG_ALIGNLEFT | UG_ALIGNCENTER | UG_ALIGNVCENTER);
		cellText.SetFont(&m_fontBold);
		cellText.SetText(strTemp);
		//cellText.SetBackColor(clrBk);
		SetCell(COL_ELEMENT_TEXT, lnewRow, &cellText);


		//## 대상 ListBox ##
		QuickGetText(COL_TARGET_LIST, nOldRow, &strTemp);

		QuickSetCellType(COL_TARGET_LIST, lnewRow, UGCT_DROPLIST);
		//    QuickSetCellTypeEx(nCols, lnewRow, UGCT_DROPLISTHIDEBUTTON);
		QuickSetText(COL_TARGET_LIST, lnewRow, strTemp);
		QuickSetAlignment(COL_TARGET_LIST, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);

		//## ListBox Type 구분 ##
		GetCell(COL_TARGET_LIST, nOldRow, &cellText);
		int nListTypeIndex = cellText.GetOtherDataInt();		//#<= Target Type Info
		if (nListTypeIndex == 1)
			QuickSetLabelText(COL_TARGET_LIST, lnewRow, TARGETDATA_TYPE1);
		else if (nListTypeIndex == 2)
			QuickSetLabelText(COL_TARGET_LIST, lnewRow, TARGETDATA_TYPE2);
		else if (nListTypeIndex == 3)
			QuickSetLabelText(COL_TARGET_LIST, lnewRow, TARGETDATA_TYPE3);

		GetCell(COL_TARGET_LIST, lnewRow, &cellText);
		cellText.SetOtherDataInt(nListTypeIndex);
		SetCell(COL_TARGET_LIST, lnewRow, &cellText);


		//## 조건 완성 구문 ##
		GetCell(COL_CONDITION_TEXT, nOldRow, &cellText);
		BOOL bNot = (BOOL)cellText.GetOtherDataInt();		//#<= Not Info

		strTemp = cellText.GetText();
		GetCell(COL_CONDITION_TEXT, lnewRow, &cellText);
		cellText.SetDataType(UGCELLDATA_CURRENCY);
		cellText.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		cellText.SetText(strTemp);
		cellText.SetOtherDataInt((int)bNot);
		//cellText.SetBackColor(clrBk);
		SetCell(COL_CONDITION_TEXT, lnewRow, &cellText);


		//## 값보기 Check Box ##
		GetCell(COL_FIELD_CHECK, nOldRow, &cellText);
		BOOL bFieldState = (BOOL)cellText.GetOtherDataInt();		//#<= Field State Info

		GetCell(COL_FIELD_CHECK, lnewRow, &cellText);
		cellText.SetOtherDataInt((int)bFieldState);
		SetCell(COL_FIELD_CHECK, lnewRow, &cellText);

		if ((bFieldState == TRUE) && (bConditionView == TRUE) && (bNot == FALSE))
		{
			QuickGetText(COL_FIELD_CHECK, nOldRow, &strTemp);

			QuickSetAlignment(COL_FIELD_CHECK, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 
			QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_CHECKBOX);
			QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_CHECKBOXCHECKMARK);
			QuickSetLabelText(COL_FIELD_CHECK, lnewRow, "값");

			QuickSetText(COL_FIELD_CHECK, lnewRow, strTemp);
		}
		else
		{
			QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL);
			QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL); 

			QuickSetText(COL_FIELD_CHECK, lnewRow, " ");
		}

		/*
		if (bViewState == FALSE)
		{
		QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL);
		QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL); 

		QuickSetText(COL_FIELD_CHECK, lnewRow, " ");
		}
		else 
		{
		if (bConditionView == TRUE)
		{
		if (bNot == TRUE)
		{
		QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL);
		QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL); 

		QuickSetText(COL_FIELD_CHECK, lnewRow, " ");
		}
		else
		{
		QuickGetText(COL_FIELD_CHECK, nOldRow, &strTemp);

		QuickSetAlignment(COL_FIELD_CHECK, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 
		QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_CHECKBOX);
		QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_CHECKBOXCHECKMARK);

		QuickSetText(COL_FIELD_CHECK, lnewRow, strTemp);
		}
		}
		else
		{
		QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL);
		QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL); 

		QuickSetText(COL_FIELD_CHECK, lnewRow, " ");
		}
		}
		*/

		//## 삭제 Button ##
		int buttonIndex = AddCellType(&m_button);
		QuickSetAlignment(COL_MODIFY_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_MODIFY_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_MODIFY_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_MODIFY_BTN, lnewRow, "설정");
		QuickSetBackColor(COL_MODIFY_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_MODIFY_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_MODIFY_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);


		buttonIndex = AddCellType(&m_button);
		QuickSetAlignment(COL_DELETE_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_DELETE_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_DELETE_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_DELETE_BTN, lnewRow, "삭제");
		QuickSetBackColor(COL_DELETE_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_DELETE_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_DELETE_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);

	//	if(m_bSearchMode == TRUE)
		{		
			//## UP Button ##
			buttonIndex = AddCellType(&m_buttonUp);
			QuickSetAlignment(COL_UP_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
			QuickSetCellType(COL_UP_BTN, lnewRow, buttonIndex);
			QuickSetCellTypeEx(COL_UP_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
			QuickSetText(COL_UP_BTN, lnewRow, Head_Name[COL_UP_BTN]);
			QuickSetBackColor(COL_UP_BTN, lnewRow, clrBtn);
			QuickSetTextColor(COL_UP_BTN, lnewRow, clrBtnText);
			QuickSetAlignment(COL_UP_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

			//## DOWN Button ##
			buttonIndex = AddCellType(&m_buttonDown);
			QuickSetAlignment(COL_DOWN_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
			QuickSetCellType(COL_DOWN_BTN, lnewRow, buttonIndex);
			QuickSetCellTypeEx(COL_DOWN_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
			QuickSetText(COL_DOWN_BTN, lnewRow, Head_Name[COL_DOWN_BTN]);
			QuickSetBackColor(COL_DOWN_BTN, lnewRow, clrBtn);
			QuickSetTextColor(COL_DOWN_BTN, lnewRow, clrBtnText);
			QuickSetAlignment(COL_DOWN_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

			//## MAX UP Button ##
			buttonIndex = AddCellType(&m_buttonMaxUp);
			QuickSetAlignment(COL_MAXUP_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
			QuickSetCellType(COL_MAXUP_BTN, lnewRow, buttonIndex);
			QuickSetCellTypeEx(COL_MAXUP_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
			QuickSetText(COL_MAXUP_BTN, lnewRow, Head_Name[COL_MAXUP_BTN]);
			QuickSetBackColor(COL_MAXUP_BTN, lnewRow, clrBtn);
			QuickSetTextColor(COL_MAXUP_BTN, lnewRow, clrBtnText);
			QuickSetAlignment(COL_MAXUP_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

			//## MAX DOWN Button ##
			buttonIndex = AddCellType(&m_buttonMinDown);
			QuickSetAlignment(COL_MINDOWN_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
			QuickSetCellType(COL_MINDOWN_BTN, lnewRow, buttonIndex);
			QuickSetCellTypeEx(COL_MINDOWN_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
			QuickSetText(COL_MINDOWN_BTN, lnewRow, Head_Name[COL_MINDOWN_BTN]);
			QuickSetBackColor(COL_MINDOWN_BTN, lnewRow, clrBtn);
			QuickSetTextColor(COL_MINDOWN_BTN, lnewRow, clrBtnText);
			QuickSetAlignment(COL_MINDOWN_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 
		}
		//## 이전 정보를 지움
		DeleteRow(nOldRow);

		SetEvenOddColor();
		SetSelectRow(nOldRow, nNewRow);
		RedrawAll();

		bResultFlag = TRUE;
	}

	TRACE("\n << Mouse Drag&Drop! >>\n");
	return bResultFlag;
}


int CConditionGrid::OnCanSizeCol(int col)
{
	/*
	if(m_allowColSizing == FALSE)
	{
	if (col != COL_CONDITION_TEXT)
	return FALSE;
	else
	return TRUE;
	}

	return TRUE;
	*/
	return m_allowColSizing; 
}

void CConditionGrid::OnColSizing(int col,int *width)
{
	if (*width <	m_minWidth)
		*width = m_minWidth;

	if (*width > m_maxWidth)
		*width = m_maxWidth;
}

int CConditionGrid::OnCanSizeRow(long row)
{
	/*
	if(m_allowRowSizing == FALSE)
	return FALSE;

	return TRUE;
	*/
	return m_allowRowSizing;
}

void CConditionGrid::OnAdjustComponentSizes(RECT *grid, 
											RECT *topHdg,RECT *sideHdg,
											RECT *cnrBtn,RECT *vScroll,
											RECT *hScroll,RECT *tabs)
{
	CRect  rtGrid;
	double dWidthGrid = 0.0;
	double dWidthCell = 0.0;

	int ntotWidth = 0;
	for (int nCol = 0; nCol < _countof(Head_Width); nCol++)
	{
		if (nCol != COL_CONDITION_TEXT)
		{
			if ((m_bGridType != FALSE) || (nCol != COL_MAX_MIN))
			{
				if (m_bDetailView == TRUE)
					ntotWidth += Head_Width[nCol];
				else
					ntotWidth += Head_Width_Common[nCol];
			}
		}
	}

	int nWidthTitle;
	if (!m_bCallAdjustOneTime)
	{
		rtGrid = CRect(grid);

		dWidthGrid  = rtGrid.Width() - 1;

		/*
		if( dWidthGrid < (SIZE_SUBDLGBAR - 32) )
		nWidthTitle = (SIZE_SUBDLGBAR - 32) - ntotWidth;
		else
		*/
		nWidthTitle = (int)dWidthGrid - ntotWidth;

		if (m_bGridType == TRUE)
			SetColWidth(COL_MAX_MIN, Head_Width[0]);
		else
			SetColWidth(COL_MAX_MIN, 0);

		if (m_bDetailView == TRUE)
		{
			SetColWidth(COL_CONTITOTN_CHECK, Head_Width[COL_CONTITOTN_CHECK]);
			SetColWidth(COL_ELEMENT_TEXT, Head_Width[COL_ELEMENT_TEXT]);
			SetColWidth(COL_TARGET_LIST, Head_Width[COL_TARGET_LIST]);
			SetColWidth(COL_CONDITION_TEXT, nWidthTitle);
			SetColWidth(COL_FIELD_CHECK, Head_Width[COL_FIELD_CHECK]);
			SetColWidth(COL_MODIFY_BTN, Head_Width[COL_MODIFY_BTN]);
			SetColWidth(COL_DELETE_BTN, Head_Width[COL_DELETE_BTN]);
		}
		else
		{
			SetColWidth(COL_CONTITOTN_CHECK, Head_Width_Common[COL_CONTITOTN_CHECK]);
			SetColWidth(COL_ELEMENT_TEXT, Head_Width_Common[COL_ELEMENT_TEXT]);
			SetColWidth(COL_TARGET_LIST, Head_Width_Common[COL_TARGET_LIST]);
			SetColWidth(COL_CONDITION_TEXT, nWidthTitle);
			SetColWidth(COL_FIELD_CHECK, Head_Width_Common[COL_FIELD_CHECK]);
			SetColWidth(COL_MODIFY_BTN, Head_Width_Common[COL_MODIFY_BTN]);
			SetColWidth(COL_DELETE_BTN, Head_Width_Common[COL_DELETE_BTN]);
		}

		m_bCallAdjustOneTime = TRUE;
		AdjustComponentSizes();
		m_bCallAdjustOneTime = FALSE;
	}
}

void CConditionGrid::ChangeGridColor(ST_GRIDCOLOR stGridColor)
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

		if(m_nLastSelRow > -1)
		{		
			GetCell(nCol, m_nLastSelRow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, m_nLastSelRow, &cell);	
		}
	}	
}

BOOL CConditionGrid::ConditionRowChange(long nOldRow, long nNewRow)
{
	BOOL bResultFlag = FALSE;

	if ((nOldRow == nNewRow) || (nOldRow == -1) || (nNewRow == -1))
		return bResultFlag;

	long lnewRow = nNewRow;
	if (nOldRow - nNewRow < 0)
		lnewRow = nNewRow + 1;
	else if (m_loldRow - nNewRow > 0)
	{
		lnewRow = nNewRow;
		nOldRow = nOldRow + 1; 
	}

	//## OldRow와 Row 자리 바꿈 
	int ans;
	ans = InsertRow(lnewRow);
	if (ans ==  UG_SUCCESS)
	{
		CUGCell cellText;

		//## Check Box ##
		CString strTemp;
		QuickGetText(COL_CONTITOTN_CHECK, nOldRow, &strTemp);  
		BOOL bConditionView = (atoi(strTemp) == 1)?TRUE:FALSE;

		QuickSetCellType(COL_CONTITOTN_CHECK, lnewRow, UGCT_CHECKBOX);
		QuickSetCellTypeEx(COL_CONTITOTN_CHECK, lnewRow, UGCT_CHECKBOXCHECKMARK);
		QuickSetText(COL_CONTITOTN_CHECK, lnewRow, strTemp);
		QuickSetFont(COL_CONTITOTN_CHECK, lnewRow, &m_fontBold);
		GetCell(COL_ELEMENT_TEXT, nOldRow, &cellText);
		strTemp = cellText.GetText();
		GetCell(COL_ELEMENT_TEXT, lnewRow, &cellText);
		cellText.SetDataType(UGCELLDATA_CURRENCY);
		cellText.SetAlignment(UG_ALIGNLEFT | UG_ALIGNCENTER | UG_ALIGNVCENTER);
		cellText.SetFont(&m_fontBold);
		cellText.SetText(strTemp);
		//cellText.SetBackColor(clrBk);
		SetCell(COL_ELEMENT_TEXT, lnewRow, &cellText);


		//## 대상 ListBox ##
		QuickGetText(COL_TARGET_LIST, nOldRow, &strTemp);
		QuickSetCellType(COL_TARGET_LIST, lnewRow, UGCT_DROPLIST);
		//    QuickSetCellTypeEx(nCols, lnewRow, UGCT_DROPLISTHIDEBUTTON);
		QuickSetText(COL_TARGET_LIST, lnewRow, strTemp);
		QuickSetAlignment(COL_TARGET_LIST, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);

		//## ListBox Type 구분 ##
		GetCell(COL_TARGET_LIST, nOldRow, &cellText);
		int nListTypeIndex = cellText.GetOtherDataInt();		//#<= Target Type Info
		if (nListTypeIndex == 1)
			QuickSetLabelText(COL_TARGET_LIST, lnewRow, TARGETDATA_TYPE1);
		else if (nListTypeIndex == 2)
		{
			QuickSetCellType(COL_TARGET_LIST, lnewRow, UGCT_NORMAL);
			QuickSetCellTypeEx(COL_TARGET_LIST, lnewRow, UGCT_NORMAL);
			QuickSetLabelText(COL_TARGET_LIST, lnewRow, TARGETDATA_TYPE2);
		}
		else if (nListTypeIndex == 3)
			QuickSetLabelText(COL_TARGET_LIST, lnewRow, TARGETDATA_TYPE3);

		GetCell(COL_TARGET_LIST, lnewRow, &cellText);
		cellText.SetOtherDataInt(nListTypeIndex);
		SetCell(COL_TARGET_LIST, lnewRow, &cellText);


		//## 조건 완성 구문 ##
		GetCell(COL_CONDITION_TEXT, nOldRow, &cellText);
		BOOL bNot = (BOOL)cellText.GetOtherDataInt();		//#<= Not Info

		strTemp = cellText.GetText();
		GetCell(COL_CONDITION_TEXT, lnewRow, &cellText);
		cellText.SetDataType(UGCELLDATA_CURRENCY);
		cellText.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		cellText.SetText(strTemp);
		cellText.SetOtherDataInt((int)bNot);
		//cellText.SetBackColor(clrBk);
		SetCell(COL_CONDITION_TEXT, lnewRow, &cellText);


		//## 값보기 Check Box ##
		GetCell(COL_FIELD_CHECK, nOldRow, &cellText);
		BOOL bFieldState = (BOOL)cellText.GetOtherDataInt();		//#<= Field State Info

		GetCell(COL_FIELD_CHECK, lnewRow, &cellText);
		cellText.SetOtherDataInt((int)bFieldState);
		SetCell(COL_FIELD_CHECK, lnewRow, &cellText);

		if ((bFieldState == TRUE) && (bConditionView == TRUE) && (bNot == FALSE))
		{
			QuickGetText(COL_FIELD_CHECK, nOldRow, &strTemp);

			QuickSetAlignment(COL_FIELD_CHECK, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 
			QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_CHECKBOX);
			QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_CHECKBOXCHECKMARK);
			QuickSetLabelText(COL_FIELD_CHECK, lnewRow, "값");

			QuickSetText(COL_FIELD_CHECK, lnewRow, strTemp);
		}
		else
		{
			QuickSetCellType(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL);
			QuickSetCellTypeEx(COL_FIELD_CHECK, lnewRow, UGCT_NORMAL); 

			QuickSetText(COL_FIELD_CHECK, lnewRow, " ");
		}		

		//## 삭제 Button ##
		int buttonIndex = AddCellType(&m_button);
		QuickSetAlignment(COL_MODIFY_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_MODIFY_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_MODIFY_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_MODIFY_BTN, lnewRow, "설정");
		QuickSetBackColor(COL_MODIFY_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_MODIFY_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_MODIFY_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);


		buttonIndex = AddCellType(&m_button);
		QuickSetAlignment(COL_DELETE_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_DELETE_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_DELETE_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_DELETE_BTN, lnewRow, "삭제");
		QuickSetBackColor(COL_DELETE_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_DELETE_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_DELETE_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);

		//## UP Button ##
		buttonIndex = AddCellType(&m_buttonUp);
		QuickSetAlignment(COL_UP_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_UP_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_UP_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_UP_BTN, lnewRow, Head_Name[COL_UP_BTN]);
		QuickSetBackColor(COL_UP_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_UP_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_UP_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

		//## DOWN Button ##
		buttonIndex = AddCellType(&m_buttonDown);
		QuickSetAlignment(COL_DOWN_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_DOWN_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_DOWN_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_DOWN_BTN, lnewRow, Head_Name[COL_DOWN_BTN]);
		QuickSetBackColor(COL_DOWN_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_DOWN_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_DOWN_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

		//## MAX UP Button ##
		buttonIndex = AddCellType(&m_buttonMaxUp);
		QuickSetAlignment(COL_MAXUP_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_MAXUP_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_MAXUP_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_MAXUP_BTN, lnewRow, Head_Name[COL_MAXUP_BTN]);
		QuickSetBackColor(COL_MAXUP_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_MAXUP_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_MAXUP_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

		//## MAX DOWN Button ##
		buttonIndex = AddCellType(&m_buttonMinDown);
		QuickSetAlignment(COL_MINDOWN_BTN, lnewRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
		QuickSetCellType(COL_MINDOWN_BTN, lnewRow, buttonIndex);
		QuickSetCellTypeEx(COL_MINDOWN_BTN, lnewRow, UGCT_BUTTONNOFOCUS);
		QuickSetText(COL_MINDOWN_BTN, lnewRow, Head_Name[COL_MINDOWN_BTN]);
		QuickSetBackColor(COL_MINDOWN_BTN, lnewRow, clrBtn);
		QuickSetTextColor(COL_MINDOWN_BTN, lnewRow, clrBtnText);
		QuickSetAlignment(COL_MINDOWN_BTN, lnewRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

		//## 이전 정보를 지움
		DeleteRow(nOldRow);
		SetEvenOddColor();
		SetSelectRow(nOldRow, nNewRow);
		RedrawAll();

		bResultFlag = TRUE;
	}	
	return bResultFlag;
}
