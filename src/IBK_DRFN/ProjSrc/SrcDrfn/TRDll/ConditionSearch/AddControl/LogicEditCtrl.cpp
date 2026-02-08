// LogicEditCtrl.cpp : implementation file
//
/**********************************************************************/
/*																																		*/
/*										논리 연산자 조합 Edit Control										*/
/*											(Mouse/KeyIn Control)													*/
/*																																		*/
/*												 작성일 : 2002. 7~													*/
/*							       작성자 : 우동우 (Doori)												*/
/*																																		*/
/**********************************************************************/


//#### History ###########################################################
//#  1. 2002. 7     초기 설계/제작															(우동우)
//#  2. 2002. 10    key In 기능 추가														(우동우)
//#  3. 2003. 1     Key In 삭제 / 구분(One, Group) control 제작 (우동우)

#include "stdafx.h"
#include "../resource.h"		//<= Cursor
#include "LogicEditCtrl.h"

#include "../MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogicEditCtrl

/*# [TEMP] #########################################################
const		LPCTSTR		CONDITION_INDEX[] = { "A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","O","P","Q","R","S","T",
"U","V","W","X","Y","Z" };

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define PROGRAM_TITLE						"MyNetFind"
//#################################################################*/

const		LPCTSTR		ALPHA_UPPER[] = { "A","B","C","D","E","F","G","H","I","J",
"K","L","M","N","O","P","Q","R","S","T",
"U","V","W","X","Y","Z" };


const LPCSTR	CONDITION_OPT[]		 = {_T("and"), _T("or")};

#define	FONT_STYLE								"굴림체"

#define COLOR_NORMAL           				RGB(0, 0, 0)
#define COLOR_ELEMENT  				        RGB(0, 0, 255)
#define COLOR_OPERATION				        RGB(0, 0, 0)
#define COLOR_OPERATION_SELECT				RGB(255, 0, 0)


const   int OPERATION_SEARCH_COUNT    = 5;  //#<= 반드시 홀수

const   COLORREF  clrNormal           = COLOR_NORMAL;
const   COLORREF  clrElement          = COLOR_ELEMENT;
const   COLORREF  clrOperation        = COLOR_OPERATION;
const   COLORREF  clrOperationSelect  = COLOR_OPERATION_SELECT;

CLogicEditCtrl::CLogicEditCtrl()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	//## System Valuse ##
	m_bAutoElemet = FALSE;

	//## Values Create ##
	m_bSelectDuring = FALSE;
	m_bMouseOver = FALSE;

	m_bCaptureEnabled = FALSE;

	m_bSelectState = FALSE;
	m_nSelectStartPosition = -1;
	m_nSelectEndPosition = -1;

	//#### 검색 구분 #####
	m_bUseGroup = FALSE;

	m_bHaveBracket = FALSE;
	m_bHaveCaret = FALSE;
}

CLogicEditCtrl::~CLogicEditCtrl()
{
	m_fontDefault.DeleteObject();
}

BEGIN_MESSAGE_MAP(CLogicEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CLogicEditCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogicEditCtrl message handlers

int CLogicEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

//	SetReadOnly(FALSE);
	//## Cursor ##
	m_hOperationSelectCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_ANDOR);
	m_hSelectDuringCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_SELECT);

	DeleteAll();

	SetFont(&m_fontDefault);

	return 0;
}


void CLogicEditCtrl::OnDestroy() 
{
	CRichEditCtrl::OnDestroy();

	// TODO: Add your message handler code here

}

BOOL CLogicEditCtrl::SetErrorMessageBox(CString strErrorMsg, BOOL bFocus)
{
	BOOL bState = TRUE;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent()->GetParent();
	if (bFocus == TRUE)
	{
		strErrorMsg += "\n 재작성 하시겠습니까?";
		if (pwndMainFrm->SetMessageBox(strErrorMsg, PROGRAM_TITLE, MB_ICONQUESTION | MB_YESNO) == IDYES)
			SendMessage(WM_SETFOCUS, 0, 0);
		else
			bState = FALSE;
	}
	else
		pwndMainFrm->SetMessageBox(strErrorMsg, PROGRAM_TITLE, MB_OK);
	ShowCursor(TRUE);

	return bState;
}

//## 2002. 10 ~
void CLogicEditCtrl::AddOneElement(int nElementIndex, int nOperation, BOOL bNot)
{
	CString strOperation;
	OneElementInfo stOneElement;

	if (nOperation == 0)		//# Opt : and
		strOperation = "and";
	else if (nOperation == 1)	//# Opt : or
		strOperation = "or";
	else if (nOperation == 2)	//# Opt : next
		strOperation = "->";


	stOneElement.strOperation = strOperation;

	if (bNot == TRUE)
		stOneElement.strNot = "^";
	else
		stOneElement.strNot = "";

	stOneElement.strElement = CONDITION_INDEX[nElementIndex];

	stOneElement.strBracket = "";
	stOneElement.lBracketCount = 0;
	stOneElement.bView = TRUE;

	m_listOneElement.AddTail(stOneElement);

	TRACE(_T("\n# Element Insert => ") + stOneElement.strNot + CONDITION_INDEX[nElementIndex] + "\n");

	ReLoadAll();
}

void CLogicEditCtrl::InsertOneElement(CString strElement, int nElementIndex, int nOperation, BOOL bNot)
{
	CString strOperation;
	OneElementInfo stOneElement;

	if (nOperation == 0)		//# Opt : and
		strOperation = "and";
	else if (nOperation == 1)	//# Opt : or
		strOperation = "or";
	else if (nOperation == 2)	//# Opt : next
		strOperation = "->";


	stOneElement.strOperation = strOperation;

	if (bNot == TRUE)
		stOneElement.strNot = "^";
	else
		stOneElement.strNot = "";

	stOneElement.strElement = CONDITION_INDEX[nElementIndex];

	stOneElement.strBracket = "";
	stOneElement.lBracketCount = 0;
	stOneElement.bView = TRUE;

	POSITION pos;
	OneElementInfo stTempOneElement;
	for (int nIndex = 0; nIndex < m_listOneElement.GetCount(); nIndex++)
	{
		pos = m_listOneElement.FindIndex(nIndex);
		stTempOneElement = m_listOneElement.GetAt(pos);

		if (strElement == stTempOneElement.strElement)
			break;
	}
	m_listOneElement.InsertBefore(pos, stOneElement);

	TRACE(_T("\n# Element Insert => ") + stOneElement.strNot + CONDITION_INDEX[nElementIndex] + "\n");

	ReLoadAll();
}

void CLogicEditCtrl::AddGroupElement(int nGroup, int nElementIndex, int nOperation, BOOL bNot)
{
	GroupElementInfo  stGroupElement;

	stGroupElement.nGroup = nGroup;

	if (nOperation == 0)		//# Opt : and
		stGroupElement.stOneElementInfo.strOperation = "and";
	else if (nOperation == 1)	//# Opt : or
		stGroupElement.stOneElementInfo.strOperation = "or";

	if (bNot == TRUE)
		stGroupElement.stOneElementInfo.strNot = "^";
	else
		stGroupElement.stOneElementInfo.strNot = "";

	stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];

	stGroupElement.stOneElementInfo.strBracket = "";
	stGroupElement.stOneElementInfo.lBracketCount = 0;
	stGroupElement.stOneElementInfo.bView = TRUE;

	// 2009.11.10 -hidden-	
	//	m_listGroupElement.AddTail(stGroupElement);

	BOOL bFind = FALSE;
	int nSize = m_arrGroupElement.GetSize();
	GROUP_ELEMINFO* pGroupElement = NULL;

	for(int i = 0; i < nSize; ++i)
	{
		pGroupElement = m_arrGroupElement.GetAt(i);
		if(pGroupElement->nGroup == nGroup)
		{
			pGroupElement->arrGroupElemInfo.Add(stGroupElement);
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		pGroupElement = new GROUP_ELEMINFO;
		pGroupElement->nGroup = nGroup;
		pGroupElement->arrGroupElemInfo.Add(stGroupElement);
		m_arrGroupElement.Add(pGroupElement);
	}

	ReLoadAll(); 
}

// 2009.12.04 -hidden-
void CLogicEditCtrl::AddGroupElement(GroupElementInfo& stNewInfo)
{
	BOOL bFind = FALSE;
	int nSize = m_arrGroupElement.GetSize();
	GROUP_ELEMINFO* pGroupElement = NULL;

	for(int i = 0; i < nSize; ++i)
	{
		pGroupElement = m_arrGroupElement.GetAt(i);
		if(pGroupElement->nGroup == stNewInfo.nGroup)
		{
			pGroupElement->arrGroupElemInfo.Add(stNewInfo);
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		pGroupElement = new GROUP_ELEMINFO;
		pGroupElement->nGroup = stNewInfo.nGroup;
		pGroupElement->arrGroupElemInfo.Add(stNewInfo);
		m_arrGroupElement.Add(pGroupElement);
	}

	ReLoadAll();
}

void CLogicEditCtrl::InsertGroupElement(CString strElement, int nGroup, int nElementIndex, int nOperation, BOOL bNot)
{
	GroupElementInfo  stGroupElement;

	stGroupElement.nGroup = nGroup;

	if (nOperation == 0)		//# Opt : and
		stGroupElement.stOneElementInfo.strOperation = "and";
	else if (nOperation == 1)	//# Opt : or
		stGroupElement.stOneElementInfo.strOperation = "or";

	if (bNot == TRUE)
		stGroupElement.stOneElementInfo.strNot = "^";
	else
		stGroupElement.stOneElementInfo.strNot = "";

	stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];

	stGroupElement.stOneElementInfo.strBracket = "";
	stGroupElement.stOneElementInfo.lBracketCount = 0;
	stGroupElement.stOneElementInfo.bView = TRUE;

	POSITION pos;
	GroupElementInfo  stTempGroupElement;
	for (int nIndex = 0; nIndex < m_listGroupElement.GetCount(); nIndex++)
	{
		pos = m_listGroupElement.FindIndex(nIndex);
		stTempGroupElement = m_listGroupElement.GetAt(pos);

		if (strElement == stTempGroupElement.stOneElementInfo.strElement)
			break;
	}
	m_listGroupElement.InsertBefore(pos, stGroupElement);

	TRACE(_T("\n# Group Element Add => ") + stGroupElement.stOneElementInfo.strNot +
		CONDITION_INDEX[nElementIndex] + "\n");

	ReLoadAll(); 
}


BOOL CLogicEditCtrl::DeleteElement(CString strElement, BOOL bDelete)
{
	BOOL bResult = FALSE;

	if (m_bUseGroup == TRUE)
	{
		bResult = DeleteGroupElement(strElement, bDelete);
		bResult = DeleteOneElement(strElement, bDelete);// 20110623 박찬모 순차검색 저장 오류 >>
	}
	else
		bResult = DeleteOneElement(strElement, bDelete);

	return bResult;
}

BOOL CLogicEditCtrl::DeleteGroupElement(CString strElement, BOOL bDelete)
{
	BOOL bResult = FALSE;

	/*	GroupElementInfo stGroupElement;
	CString newText;
	int nCount = m_listGroupElement.GetCount();

	POSITION pos;
	strElement.TrimLeft();
	strElement.TrimRight();
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
	pos = m_listGroupElement.FindIndex(nIndex);
	stGroupElement = m_listGroupElement.GetAt(pos);

	stGroupElement.stOneElementInfo.strElement.TrimLeft();
	stGroupElement.stOneElementInfo.strElement.TrimRight();

	if (stGroupElement.stOneElementInfo.strElement.Compare(strElement) == 0)
	{
	if (bDelete == TRUE)
	m_listGroupElement.RemoveAt(pos);
	else
	{
	stGroupElement.stOneElementInfo.bView = FALSE;
	m_listGroupElement.SetAt(pos, stGroupElement);
	}

	bResult = TRUE;
	ReLoadAll();
	break;
	}
	}
	*/

	// 2009.11.12 -hidden-
	GroupElementInfo stGroupElement;
	CString newText;
	int nElemSize = 0;
	int nSize = m_arrGroupElement.GetSize();

	strElement.TrimLeft();
	strElement.TrimRight();

	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(nIndex);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();

		for(int j = 0; j < nElemSize; ++j)
		{
			stGroupElement = pGroup->arrGroupElemInfo.GetAt(j);

			stGroupElement.stOneElementInfo.strElement.TrimLeft();
			stGroupElement.stOneElementInfo.strElement.TrimRight();

			if (stGroupElement.stOneElementInfo.strElement.Compare(strElement) == 0)
			{
				if (bDelete == TRUE)
					pGroup->arrGroupElemInfo.RemoveAt(j);
				else
				{
					stGroupElement.stOneElementInfo.bView = FALSE;
					pGroup->arrGroupElemInfo.SetAt(j, stGroupElement);
				}

				bResult = TRUE;
				ReLoadAll();
				return bResult;
			}
		}
	}

	return bResult;
}

BOOL CLogicEditCtrl::DeleteOneElement(CString strElement, BOOL bDelete)
{
	BOOL bResult = FALSE;

	OneElementInfo stOneElement;
	CString newText;
	int i, nCount;

	nCount = m_listOneElement.GetCount();

	POSITION pos;
	strElement.TrimLeft();
	strElement.TrimRight();
	for(i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);

		stOneElement.strElement.TrimLeft();
		stOneElement.strElement.TrimRight();

		if(stOneElement.strElement == strElement)
		{
			bResult = TRUE;

			POSITION posBracket;
			long tempBracket = 0; //<- ((AandB)and(CandD)andE)
			//    A delete
			//    (CandD) 짝괄호의 개수 
			BOOL bDeleteSkip = FALSE;

			//## 괄호 수정 ##
			if (stOneElement.lBracketCount != 0)
			{
				LONG ltempBracketCount; 
				ltempBracketCount = stOneElement.lBracketCount;

				//######### "("인 경우 #############################################
				if(ltempBracketCount > 0) 
				{
					for(int nBracket = i + 1; nBracket < nCount; nBracket++)
					{
						posBracket = m_listOneElement.FindIndex(nBracket);
						stOneElement = m_listOneElement.GetAt(posBracket);

						//## "(" 짝 괄호 개수 Count ##
						if (stOneElement.lBracketCount < 0)//<- 검색 중 ")"를 찾음
						{
							if(tempBracket == 0)
								bDeleteSkip = FALSE;
							else
							{
								tempBracket += stOneElement.lBracketCount;
								if(tempBracket < 0)
									bDeleteSkip = FALSE;
							}
						}
						else if (stOneElement.lBracketCount > 0) //<- 검색 중 "("를 찾음
						{
							tempBracket += stOneElement.lBracketCount;
							bDeleteSkip = TRUE;
						}

						if (bDeleteSkip != TRUE)
						{
							//## ")"의 개수 ##
							int tempCount = stOneElement.lBracketCount * -1;

							for(int nBracketCount=0; nBracketCount<tempCount; nBracketCount++)
							{
								CString tempBracket;
								int tempBracketCount;

								tempBracket = stOneElement.strBracket;
								tempBracketCount = tempBracket.GetLength();
								stOneElement.strBracket = tempBracket.Left(tempBracketCount-1);

								ltempBracketCount--;
								stOneElement.lBracketCount++;
								//## DATA 반영##
								m_listOneElement.SetAt(posBracket, stOneElement);

								if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
									break;
							}
						}
						if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
							break;
						//## ")"를 찾음 END ##
					}
				}
				//######### ")"인 경우 #############################################
				else if(ltempBracketCount < 0) 
				{
					for(int nBracket = i - 1; nBracket >= 0; nBracket--)
					{
						posBracket = m_listOneElement.FindIndex(nBracket);
						stOneElement = m_listOneElement.GetAt(posBracket);

						//## ")" 짝 괄호 개수 Count ##
						if (stOneElement.lBracketCount > 0)//<- 검색 중 "("를 찾음
						{
							if(tempBracket == 0)
								bDeleteSkip = FALSE;
							else
							{
								tempBracket += stOneElement.lBracketCount;
								if(tempBracket > 0)
									bDeleteSkip = FALSE;
							}
						}
						else if (stOneElement.lBracketCount < 0) //<- 검색 중 ")"를 찾음
						{
							tempBracket += stOneElement.lBracketCount;
							bDeleteSkip = TRUE;
						}

						if (bDeleteSkip != TRUE)
						{
							//## "("의 개수 ##
							int tempCount = stOneElement.lBracketCount;

							for(int nBracketCount = 0; nBracketCount < tempCount; nBracketCount++)
							{
								CString tempBracket;
								int tempBracketCount;

								tempBracket = stOneElement.strBracket;
								tempBracketCount = tempBracket.GetLength();
								stOneElement.strBracket = tempBracket.Left(tempBracketCount - 1);

								ltempBracketCount++;
								stOneElement.lBracketCount++;
								//## DATA 반영##
								m_listOneElement.SetAt(posBracket, stOneElement);

								if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
									break;
							}
						}
						if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
							break;
						//## ")"를 찾음 END ##
					}
				}
			}

			if (bDelete == TRUE)
			{
				m_listOneElement.RemoveAt(pos);
				ReLoadAll();
			}
			break;
		}
	}


	// 2009.11.13 -hidden-
	/*	OneElementInfo stOneElement;
	CString newText;
	int i, nCount, nElemSize;

	nCount = m_arrGroupElement.GetSize();

	int nFindBracket = 0;

	strElement.TrimLeft();
	strElement.TrimRight();
	for(i = 0; i < nCount; ++i)
	{
	GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
	nElemSize = pGroup->arrConGrpInfo.GetSize();
	for(int j = 0; j < nElemSize; ++j)
	{
	stOneElement = pGroup->arrConGrpInfo.GetAt(j);

	stOneElement.strElement.TrimLeft();
	stOneElement.strElement.TrimRight();

	if(stOneElement.strElement == strElement)
	{
	bResult = TRUE;

	POSITION posBracket;
	long tempBracket = 0;
	BOOL bDeleteSkip = FALSE;

	if (stOneElement.lBracketCount != 0)
	{
	LONG ltempBracketCount; 
	ltempBracketCount = stOneElement.lBracketCount;

	if(ltempBracketCount > 0) 
	{
	for(int nBracket = j + 1; nBracket < nCount; nBracket++)
	{
	nFindBracket = nBracket;
	stOneElement = pGroup->arrConGrpInfo.GetAt(nBracket);

	if (stOneElement.lBracketCount < 0)//<- 검색 중 ")"를 찾음
	{
	if(tempBracket == 0)
	bDeleteSkip = FALSE;
	else
	{
	tempBracket += stOneElement.lBracketCount;
	if(tempBracket < 0)
	bDeleteSkip = FALSE;
	}
	}
	else if (stOneElement.lBracketCount > 0) //<- 검색 중 "("를 찾음
	{
	tempBracket += stOneElement.lBracketCount;
	bDeleteSkip = TRUE;
	}

	if (bDeleteSkip != TRUE)
	{
	//## ")"의 개수 ##
	int tempCount = stOneElement.lBracketCount * -1;

	for(int nBracketCount=0; nBracketCount<tempCount; nBracketCount++)
	{
	CString tempBracket;
	int tempBracketCount;

	tempBracket = stOneElement.strBracket;
	tempBracketCount = tempBracket.GetLength();
	stOneElement.strBracket = tempBracket.Left(tempBracketCount-1);

	ltempBracketCount--;
	stOneElement.lBracketCount++;
	//## DATA 반영##
	pGroup->arrConGrpInfo.SetAt(nFindBracket, stOneElement);

	if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
	break;
	}
	}
	if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
	break;
	//## ")"를 찾음 END ##
	}
	}
	//######### ")"인 경우 #############################################
	else if(ltempBracketCount < 0) 
	{
	for(int nBracket = j - 1; nBracket >= 0; nBracket--)
	{
	nFindBracket = nBracket;
	stOneElement = pGroup->arrConGrpInfo.GetAt(nBracket);

	if (stOneElement.lBracketCount > 0)//<- 검색 중 "("를 찾음
	{
	if(tempBracket == 0)
	bDeleteSkip = FALSE;
	else
	{
	tempBracket += stOneElement.lBracketCount;
	if(tempBracket > 0)
	bDeleteSkip = FALSE;
	}
	}
	else if (stOneElement.lBracketCount < 0) //<- 검색 중 ")"를 찾음
	{
	tempBracket += stOneElement.lBracketCount;
	bDeleteSkip = TRUE;
	}

	if (bDeleteSkip != TRUE)
	{
	//## "("의 개수 ##
	int tempCount = stOneElement.lBracketCount;

	for(int nBracketCount = 0; nBracketCount < tempCount; nBracketCount++)
	{
	CString tempBracket;
	int tempBracketCount;

	tempBracket = stOneElement.strBracket;
	tempBracketCount = tempBracket.GetLength();
	stOneElement.strBracket = tempBracket.Left(tempBracketCount - 1);

	ltempBracketCount++;
	stOneElement.lBracketCount++;
	//## DATA 반영##
	pGroup->arrConGrpInfo.SetAt(posBracket, stOneElement);

	if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
	break;
	}
	}
	if (ltempBracketCount == 0)//<--Delete Bracket Index Count 0
	break;
	//## ")"를 찾음 END ##
	}
	}
	}

	if (bDelete == TRUE)
	{
	pGroup->arrConGrpInfo.RemoveAt(nFindBracket);
	ReLoadAll();
	}
	break;
	}
	}
	}
	*/
	return bResult;
}


BOOL CLogicEditCtrl::BracketSelectMake()
{
	
	OneElementInfo stOneElement;
	BOOL bFalg = TRUE;

	int i;
	long nStart, nEnd;
	GetSel(nStart, nEnd);

	CString strWholeText;
	CString strTemp;	
	GetWindowText(strTemp);
	strWholeText = strTemp;

	strTemp = strTemp.Mid(nStart, nEnd - nStart);
	//## 선택된 String ##
	TRACE0("## selected string :" + strTemp +"\n");

	//## Grammar Check! START ##
	int nGrammarOK;
	nGrammarOK = CheckMakeBracketOK(strTemp);

	if (nGrammarOK > 0)
	{
		if (nGrammarOK == 1)
			SetErrorMessageBox("구분자는 2개 이상을 선택하시오.");
		else if (nGrammarOK == 2)
			SetErrorMessageBox("선택영역이 연산자 '~'(next)포함합니다.\n 선택영역을 확인하시오.");
		else if (nGrammarOK == 3)
			SetErrorMessageBox("괄호의 선택이 잘못 되었습니다.\n 선택영역을 확인 하시오.");
		else if (nGrammarOK == 4)
			SetErrorMessageBox("선택역역의 괄호는 '('로 시작할수 없습니다. \n선택영역을 확인 하시오.");

		BrecketSelectCreate();
		return FALSE;
	}
	//## Grammar Check! END ##

	if ((nEnd-nStart) < 4)
		return FALSE;
	//## 선택된 영역 문법 Check ##
	//## chaeck -> 괄호, 연산자

	//20100719 User입력
	
	CUIntArray arrElementIndex;
	CStringArray arrInputText;

	
	int nChoiceElement = 0;
	CStringArray saElement;
	for (i = 0; i < strTemp.GetLength(); i++)
	{
		//# 대소 문자 구분 #
		//0000141: 조건 id 가 소문자일때 조건을 이동시켜도 하단 에디트창에 적용이 안되는 현상 개선
		//0000143: 조건검색, 일반검색의 논리 Logic식에 소문자를 사용시 ()-괄호 기능이 동작하지 않는 오류 개선
		if( ((strTemp[i] >= 'A') && (strTemp[i] <= 'Z')) || ((strTemp[i] >= 'a') && (strTemp[i] <= 'z')) )
		{
			saElement.Add(strTemp[i]);
			nChoiceElement++;
			arrElementIndex.Add(i + nStart);//Element인덱스
		}
	}

	//수식Text 전체를 Array에저장 
	int nIndex;
	CString strCharacter;
	CString strResult;
	for(nIndex = 0; nIndex < strWholeText.GetLength(); nIndex++)
	{
		strCharacter.Format("%c", strWholeText.GetAt(nIndex));
		arrInputText.Add(strCharacter);
	}

	arrInputText.InsertAt(arrElementIndex.GetAt(0), _T("("));
	arrInputText.InsertAt(arrElementIndex.GetAt(arrElementIndex.GetSize() - 1) + 2, _T(")"));
	
	for(nIndex = 0; nIndex < arrInputText.GetSize(); nIndex++)
		strResult += arrInputText.GetAt(nIndex);


	//## 선택된 Element ##
	for (i = 0; i < saElement.GetSize(); i++)
		TRACE0("-> choice Element :" + saElement.GetAt(i)+"\n");

	//## Data Insert (괄호) ##
	int nCount;
	POSITION pos;
	nCount = m_listOneElement.GetCount();

	for (i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);
		if(stOneElement.strElement == saElement.GetAt(0))
		{
			//## first Bracket ##
			stOneElement.strBracket += "("; 
			stOneElement.lBracketCount++;

			m_listOneElement.SetAt(pos, stOneElement);
		}
		else if( stOneElement.strElement == saElement.GetAt(saElement.GetSize()-1) )
		{
			//## Last Bracket ##
			stOneElement.strBracket += ")"; 
			stOneElement.lBracketCount--;

			m_listOneElement.SetAt(pos, stOneElement);
		}
	}

	SetWindowText(strResult);
	BrecketSelectCreate();
	SetCStringColor();
//	ReLoadAll();

	return bFalg;
}


BOOL CLogicEditCtrl::BracketSystemMake()
{
	BOOL bFlag = TRUE;
	//## 현재 설정된 괄호를 지운다.
	BracketDelete();

	//## 규칙을 따라 괄호 만들기 
	//## 규칙 ##
	//-> 시스템 괄호는 괄호안에서 동일하지 않은 연산자가 연속 되었을 때
	//   먼저 나온 연산자를 기준으로 괄호를 묶는다.
	OneElementInfo stOneElement, structElementFront;

	int i;
	POSITION pos_now, pos_front;
	CString strOperation;  
	BOOL bBracket;   //# ( : TRUE
	//# ) : FALSE 

	//## View Element Total Count ##
	int nViewElementCount = 0;  //<= View Element Total Count 
	for(i = 0; i < m_listOneElement.GetCount(); i++)
	{
		pos_now = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos_now);

		if (stOneElement.bView == TRUE)
			nViewElementCount++;
	}

	//## Need Not Make Brecket Element Count! ##
	if (nViewElementCount < 2)
	{
		bFlag = FALSE;
		return bFlag;
	}

	int nElementIndex = -1;  //<= View Element Index Count 
	for(i = 0; i < m_listOneElement.GetCount(); i++)
	{
		pos_now = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos_now);

		if (stOneElement.bView == FALSE)
			continue;

		nElementIndex++;  //<= View Element Index Count 

		if (nElementIndex > 0)
		{
			//## 이전 정보가 bView = TRUE 를 찾음 
			BOOL bFrontInfo = FALSE;
			for (int j = i - 1; j >= 0; j--)
			{
				pos_front = m_listOneElement.FindIndex(j);
				structElementFront = m_listOneElement.GetAt(pos_front);

				if (structElementFront.bView == TRUE)
				{
					bFrontInfo = TRUE;
					break;
				}
			}

			//## i-1의 bView == TRUE가 없으면 skip
			if (bFrontInfo == FALSE)
				continue;
		}
		else if (nElementIndex == 0)
		{
			stOneElement.strBracket = "(";
			stOneElement.lBracketCount = 1;
			m_listOneElement.SetAt(pos_now, stOneElement);

			bBracket = TRUE;
			continue;
		}

		if (nElementIndex == nViewElementCount-1) //## ->String 마지막 괄호 처리 START ##
		{
			if (bBracket == TRUE)
			{
				if (stOneElement.strOperation == structElementFront.strOperation)
				{
					stOneElement.strBracket = ")";
					stOneElement.lBracketCount = -1;
					m_listOneElement.SetAt(pos_now, stOneElement);
				}
				else if (stOneElement.strOperation != structElementFront.strOperation)
				{  
					structElementFront.strBracket = ")";
					structElementFront.lBracketCount = -1;
					m_listOneElement.SetAt(pos_front, structElementFront);
				}
			}
			else if (bBracket == FALSE)
			{
				structElementFront.strBracket = "(";
				structElementFront.lBracketCount = 1;
				m_listOneElement.SetAt(pos_front, structElementFront);

				stOneElement.strBracket = ")";
				stOneElement.lBracketCount = -1;
				m_listOneElement.SetAt(pos_now, stOneElement);
			}
		}
		else  //## -> String 중간 괄호 처리 START ##
		{
			if (stOneElement.strOperation == structElementFront.strOperation)
			{
				if (bBracket == FALSE)
				{
					structElementFront.strBracket = "(";
					structElementFront.lBracketCount = 1;
					m_listOneElement.SetAt(pos_front, structElementFront);

					bBracket = TRUE;
				}
			}
			else if (stOneElement.strOperation != structElementFront.strOperation)
			{
				if ((bBracket == TRUE) && (nElementIndex > 1))
				{
					structElementFront.strBracket = ")";
					structElementFront.lBracketCount = -1;
					m_listOneElement.SetAt(pos_front, structElementFront);

					bBracket = FALSE;
				}
				else if ((bBracket == FALSE) && (nElementIndex > 2))
				{
					structElementFront.strBracket = "(";
					structElementFront.lBracketCount = 1;
					m_listOneElement.SetAt(pos_front, structElementFront);

					bBracket = TRUE;
				}
			} 
		} //## -> String 중간 괄호 처리 END ##
	} //#-> for END 

	ReLoadAll();

	return bFlag;
}

BOOL CLogicEditCtrl::BracketDelete()
{
	OneElementInfo  stOneElement;
	BOOL bFalg = TRUE;

	//## Data Insert (괄호) ##
	int nCount;
	POSITION pos;
	nCount = m_listOneElement.GetCount();

	for(int i=0; i<nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);

		if(stOneElement.strBracket.IsEmpty() == FALSE)
		{
			stOneElement.strBracket = "";
			stOneElement.lBracketCount = 0;
			m_listOneElement.SetAt(pos, stOneElement);
		}
	}

	ReLoadAll();

	return bFalg;
}

void CLogicEditCtrl::AllAndOrChange(BOOL bAnd)
{
	OneElementInfo  stOneElement;
	POSITION pos;
	int nCount = m_listOneElement.GetCount();

	CString tempAnd;
	if (bAnd == TRUE)
		tempAnd = "and";
	else
		tempAnd = "or";

	for (int i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);

		if (stOneElement.bView == TRUE)
		{
			stOneElement.strOperation = tempAnd;
			m_listOneElement.SetAt(pos, stOneElement);
		}
	}

	ReLoadAll();
}

void CLogicEditCtrl::ReLoadAll()
{
	int nTempGroup = 0;

	OneElementInfo		stOneElement;
	GroupElementInfo  stGroupElement;

	CString newText = "";
	POSITION pos;
	int nElementIndex = 0;
	CString strElement;
	int nCount;

	if (m_bUseGroup == FALSE)
	{
		//## Have Bracket ##
		m_bHaveBracket = FALSE;

		nCount = m_listOneElement.GetCount();
		if (nCount > 0)
			EnableWindow(TRUE);

		for (int i = 0; i < nCount; i++)
		{
			pos = m_listOneElement.FindIndex(i);
			stOneElement = m_listOneElement.GetAt(pos);

			if (stOneElement.bView == TRUE)
			{
				if (m_bAutoElemet == TRUE)
				{
					stOneElement.strElement = CONDITION_INDEX[nElementIndex];
					m_listOneElement.SetAt(pos, stOneElement);
				}

				//TRACE("\n#    LogicEditCtrl Element : %s", stOneElement.strElement);
				nElementIndex++;
			}
			else if (stOneElement.bView == FALSE)
				continue;

			if (stOneElement.bView == TRUE)
			{
				if (nElementIndex == 1)
					newText += stOneElement.strBracket
					//+ stOneElement.strNot
					+ stOneElement.strElement;
				else
				{
					CString strOpt = "";

					if (stOneElement.strBracket.IsEmpty() == TRUE)
					{
						if (strOpt.IsEmpty() == FALSE)
							newText += " " + strOpt + " " 
							//+ stOneElement.strNot
							+ stOneElement.strElement;
						else
							newText += " " + stOneElement.strOperation 
							+ " " //+ stOneElement.strNot
							+ stOneElement.strElement;
					}
					else if (stOneElement.strBracket[0] == '(')
					{
						if (strOpt.IsEmpty() == FALSE)
						{
							newText += " " + strOpt + " " 
								+ stOneElement.strBracket 
								//+ stOneElement.strNot
								+ stOneElement.strElement;
							m_bHaveBracket = TRUE;
						}
						else
						{
							newText += " " + stOneElement.strOperation 
								+ " " + stOneElement.strBracket 
								//+ stOneElement.strNot
								+ stOneElement.strElement;
							m_bHaveBracket = TRUE;
						}
					}
					else if (stOneElement.strBracket[0] == ')')
					{
						if (strOpt.IsEmpty() == FALSE)
						{
							newText += " " + strOpt + " " 
								//+ stOneElement.strNot
								+ stOneElement.strElement 
								+ stOneElement.strBracket;
							m_bHaveBracket = TRUE;
						}
						else
						{
							newText += " " + stOneElement.strOperation 
								+ " " //+ stOneElement.strNot
								+ stOneElement.strElement 
								+ stOneElement.strBracket;
							m_bHaveBracket = TRUE;
						}
					}
				}
			}
		}
	}
	else if (m_bUseGroup == TRUE)
	{
		/*		int nAfterGroup, nMaxGroup;

		nMaxGroup = GetMaxGroup();
		nCount = m_listGroupElement.GetCount();
		if (nCount > 0)
		EnableWindow(TRUE);

		for (int nNowGroup = 1; nNowGroup <= nMaxGroup; nNowGroup++)
		{
		for(int i = 0; i < nCount; i++)
		{
		pos = m_listGroupElement.FindIndex(i);
		stGroupElement = m_listGroupElement.GetAt(pos);

		//## Group 구분 ##
		if (nNowGroup == stGroupElement.nGroup)
		{
		if (stGroupElement.stOneElementInfo.bView == TRUE)
		{
		if (m_bAutoElemet == TRUE)
		{
		stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];
		m_listGroupElement.SetAt(pos, stGroupElement);
		}

		//TRACE("\n#    LogicEditCtrl Element : %s", stOneElement.strElement);
		nElementIndex++;
		}
		else if (stGroupElement.stOneElementInfo.bView == FALSE)
		continue;

		if (stGroupElement.stOneElementInfo.bView == TRUE)
		{
		if (nElementIndex == 1)
		newText += stGroupElement.stOneElementInfo.strBracket 
		//+ stGroupElement.stOneElementInfo.strNot
		+ stGroupElement.stOneElementInfo.strElement;
		else
		{
		CString strOpt = "";
		if (stGroupElement.nGroup != nAfterGroup)
		strOpt = "->";

		if (stGroupElement.stOneElementInfo.strBracket.IsEmpty() == TRUE)
		{
		if (strOpt.IsEmpty() == FALSE)
		newText += " " + strOpt + " " 
		//+ stOneElement.strNot +
		+ stGroupElement.stOneElementInfo.strElement;
		else
		newText += " " + stGroupElement.stOneElementInfo.strOperation 
		+ " " //+ stOneElement.strNot
		+ stGroupElement.stOneElementInfo.strElement;
		}
		else if (stGroupElement.stOneElementInfo.strBracket[0] == '(' )
		{
		if (strOpt.IsEmpty() == FALSE)
		{
		newText += " " + strOpt + " " 
		+ stGroupElement.stOneElementInfo.strBracket 
		//+ stOneElement.strNot
		+ stGroupElement.stOneElementInfo.strElement;
		m_bHaveBracket = TRUE;
		}
		else
		{
		newText += " " + stGroupElement.stOneElementInfo.strOperation 
		+ " " + stGroupElement.stOneElementInfo.strBracket 
		//+ stOneElement.strNot
		+ stGroupElement.stOneElementInfo.strElement;
		m_bHaveBracket = TRUE;
		}
		}
		else if (stGroupElement.stOneElementInfo.strBracket[0] == ')')
		{
		if (strOpt.IsEmpty() == FALSE)
		{
		newText += " " + strOpt + " " 
		//+ stOneElement.strNot
		+ stGroupElement.stOneElementInfo.strElement 
		+ stGroupElement.stOneElementInfo.strBracket;
		m_bHaveBracket = TRUE;
		}
		else
		{
		newText += " " + stGroupElement.stOneElementInfo.strOperation 
		+ " " //+ stOneElement.strNot
		+ stGroupElement.stOneElementInfo.strElement 
		+ stGroupElement.stOneElementInfo.strBracket;
		m_bHaveBracket = TRUE;
		}
		}
		}

		nAfterGroup = nNowGroup;
		}
		}
		}
		}
		*/
		int nAfterGroup;
		int nGroupSize = m_arrGroupElement.GetSize();

		nCount = 0;
		for(int nSize = 0; nSize < nGroupSize; ++nSize)
		{
			GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(nSize);
			nCount += pGroup->arrGroupElemInfo.GetSize();
		}

		if (nCount > 0)
			EnableWindow(TRUE);

		for(int i = 0; i < nGroupSize; ++i)
		{
			GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
			int nElemSize = pGroup->arrGroupElemInfo.GetSize();

			for(int j = 0; j < nElemSize; ++j)
			{
				stGroupElement = pGroup->arrGroupElemInfo.GetAt(j);
				if (stGroupElement.stOneElementInfo.bView == TRUE)
				{
					if (m_bAutoElemet == TRUE)
					{
						stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];
						pGroup->arrGroupElemInfo.SetAt(j, stGroupElement);
					}
					nElementIndex++;
				}
				else if (stGroupElement.stOneElementInfo.bView == FALSE)
					continue;

				if (stGroupElement.stOneElementInfo.bView == TRUE)
				{
					if (nElementIndex == 1)
						newText += stGroupElement.stOneElementInfo.strBracket + stGroupElement.stOneElementInfo.strElement;
					else
					{
						CString strOpt = "";
						if (stGroupElement.nGroup != nAfterGroup)
							strOpt = "->";

						if (stGroupElement.stOneElementInfo.strBracket.IsEmpty() == TRUE)
						{
							if (strOpt.IsEmpty() == FALSE)
								newText += " " + strOpt + " " + stGroupElement.stOneElementInfo.strElement;
							else
								newText += " " + stGroupElement.stOneElementInfo.strOperation 
								+ " " + stGroupElement.stOneElementInfo.strElement;
						}
						else if (stGroupElement.stOneElementInfo.strBracket[0] == '(' )
						{
							if (strOpt.IsEmpty() == FALSE)
							{
								newText += " " + strOpt + " " 
									+ stGroupElement.stOneElementInfo.strBracket 
									//+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement;
								m_bHaveBracket = TRUE;
							}
							else
							{
								newText += " " + stGroupElement.stOneElementInfo.strOperation 
									+ " " + stGroupElement.stOneElementInfo.strBracket 
									//+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement;
								m_bHaveBracket = TRUE;
							}
						}
						else if (stGroupElement.stOneElementInfo.strBracket[0] == ')')
						{
							if (strOpt.IsEmpty() == FALSE)
							{
								newText += " " + strOpt + " " 
									//+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement 
									+ stGroupElement.stOneElementInfo.strBracket;
								m_bHaveBracket = TRUE;
							}
							else
							{
								newText += " " + stGroupElement.stOneElementInfo.strOperation 
									+ " " //+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement 
									+ stGroupElement.stOneElementInfo.strBracket;
								m_bHaveBracket = TRUE;
							}
						}
					}
					nAfterGroup = pGroup->nGroup;
				}
			}
		}
	}

	SetWindowText(newText + " ");

	//## Element Color Setting ##
	SetCStringColor();

	Invalidate();

	TRACE("\n# Logic Edit Reload All!");
}


void CLogicEditCtrl::RollBack()
{
	TRACE("\n# LogicEditCtrl RollBack!");

	ReLoadAll();

	//## Values Create ##

	m_bSelectDuring = FALSE;
	m_bMouseOver = FALSE;

	m_bSelectState = FALSE;
	m_nSelectStartPosition = -1;
	m_nSelectEndPosition = -1;
}


int CLogicEditCtrl::GetMaxGroup()
{
	int nResult = -1;

	POSITION pos;
	int nCount = m_listGroupElement.GetCount();
	GroupElementInfo stGroupElementInfo;

	for (int i = 0; i < nCount; i++)
	{
		pos = m_listGroupElement.FindIndex(i);
		stGroupElementInfo = m_listGroupElement.GetAt(pos);

		if (nResult < stGroupElementInfo.nGroup)
			nResult = stGroupElementInfo.nGroup;
	}

	return nResult;
}


int CLogicEditCtrl::GetGroupToConditionCnt(int nGroup)
{
	int nResult = 0;

	/*	POSITION pos;
	int nCount = m_listGroupElement.GetCount();
	GroupElementInfo stGroupElementInfo;

	for (int i = 0; i < nCount; i++)
	{
	pos = m_listGroupElement.FindIndex(i);
	stGroupElementInfo = m_listGroupElement.GetAt(pos);

	if ((nGroup == stGroupElementInfo.nGroup) &&
	(stGroupElementInfo.stOneElementInfo.bView == TRUE))
	nResult++;
	}
	*/	


	// 2009.11.13 -hidden-
	GroupElementInfo stGroupElementInfo;
	int nElemSize = 0;

	int nSize = m_arrGroupElement.GetSize();	
	for (int i = 0; i < nSize; ++i)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();
		for(int j = 0; j < nElemSize; ++j)
		{
			stGroupElementInfo = pGroup->arrGroupElemInfo.GetAt(j);

			if ((nGroup == stGroupElementInfo.nGroup) && (stGroupElementInfo.stOneElementInfo.bView == TRUE))
				nResult++;
		}
	}

	return nResult;
}

int CLogicEditCtrl::GetElementCount()
{
	int nCount = 0;

	if (m_bUseGroup == TRUE)
	{
		//		nCount = m_listGroupElement.GetCount();
		// 2009.11.13 -hidden-
		int nSize = m_arrGroupElement.GetSize();	
		for (int i = 0; i < nSize; ++i)
		{
			GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
			nCount += pGroup->arrGroupElemInfo.GetSize();
		}
	}
	else
		nCount = m_listOneElement.GetCount();

	return nCount;
}

BOOL CLogicEditCtrl::GetNowElement(CStringArray &saElement)
{
	BOOL bResult = FALSE;

	POSITION posElement;
	OneElementInfo stElementString;

	int nElementCnt = m_listOneElement.GetCount();

	if (nElementCnt > 0)
		bResult = TRUE;

	for (int nIndex = 0; nIndex < nElementCnt; nIndex++)
	{
		posElement = m_listOneElement.FindIndex(nIndex);
		stElementString = m_listOneElement.GetAt(posElement);

		saElement.Add(stElementString.strElement);
	}

	return bResult;
}

CHARFORMAT CLogicEditCtrl::GetFontFormat(COLORREF clrColor)
{
	CHARFORMAT cfResult;
	cfResult.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR;
	cfResult.dwMask ^= CFM_ITALIC ^ CFM_BOLD ^ CFM_STRIKEOUT ^ CFM_UNDERLINE;

	cfResult.dwEffects = 0;
	cfResult.yHeight = 180;   //# 10pts * 20 twips/point = 200 twips
	cfResult.bPitchAndFamily = FIXED_PITCH | FF_MODERN;
	cfResult.yOffset = 0;
	cfResult.crTextColor = clrColor;

	cfResult.bCharSet = DEFAULT_CHARSET;

	strcpy(cfResult.szFaceName, (const char*)FONT_STYLE);
	cfResult.cbSize = sizeof(cfResult);

	return cfResult;
}

void CLogicEditCtrl::SetCStringColor()
{
	COLORREF  clrValuse;

	CString strLogic;
	GetWindowText(strLogic);
	strLogic.TrimLeft();
	strLogic.TrimRight();

	int bState;
	int i = 0;
	while(i < strLogic.GetLength())
	{
		bState = 0;
		for(int j = 0; j < _countof(CONDITION_INDEX); j++)
		{
			if ((CString)strLogic[i] == ' ')
				break;
			else if ((CString)strLogic[i] == '-')
			{
				if (strLogic.GetLength() >= i + 1)
				{
					if ((CString)strLogic[i + 1] == '>')
					{
						bState = 3;
						break;
					}
				}
			}

			if (CONDITION_INDEX[j] == (CString)strLogic[i])
			{
				//## Not 'and' or 'or' ## 
				if ((CString)strLogic[i] == 'a')
				{
					if (strLogic.GetLength() >= i + 3)
					{
						if ( ((CString)strLogic[i + 1] == 'n') && 
							((CString)strLogic[i + 2] == 'd') )
						{
							bState = 1;
							break;
						}
					}
				}
				else if ((CString)strLogic[i] == 'o')
				{
					if (strLogic.GetLength() >= i + 2)
					{
						if ((CString)strLogic[i + 1] == 'r')
						{
							bState = 2;
							break;
						}
					}
				}
				else
				{
					bState = 0;
					break;
				}
			}
		}

		if (bState == 0)
		{
			clrValuse = clrElement;
			SetSel(i, i + 1);
			i++;
		}
		else 
		{
			clrValuse = clrNormal;
			if (bState == 1)
			{
				SetSel(i, i + 3);
				i = i + 3;
			}
			else if ((bState == 2) || (bState == 3))
			{
				SetSel(i, i + 2);
				i = i + 2;
			}
		}

		Invalidate(TRUE);
		SetSelectionCharFormat(GetFontFormat(clrValuse));
	}

	SendMessage(WM_KILLFOCUS, 0, 0);
}


BOOL CLogicEditCtrl::ModifyElementView(CString strElement, BOOL bView)
{
	BOOL bResult = FALSE;

	strElement.TrimLeft();
	strElement.TrimRight();

	if (m_bUseGroup == TRUE)
		bResult = ModifyGroupElementView(strElement, bView);
	else if (m_bUseGroup == FALSE)
		bResult = ModifyOneElementView(strElement, bView);

	if (bResult == TRUE)
		ReLoadAll();

	return bResult;
}

BOOL CLogicEditCtrl::ModifyOneElementView(CString strElement, BOOL bView)
{
	BOOL bResult = FALSE;

	POSITION posElement = NULL;
	OneElementInfo  stOneElement;
	posElement = GetOneElementToPosition(strElement);
	stOneElement = m_listOneElement.GetAt(posElement);

#ifdef _DEBUG
	if (posElement == NULL)
		SetErrorMessageBox("# [LogicEditCtrl] Not Have View Change Search Element!");
#endif

	if (posElement != NULL)
	{
		DeleteElement(strElement, FALSE);

		if (bView == FALSE)
		{
			stOneElement.strBracket = "";
			stOneElement.lBracketCount = 0;
		}
		stOneElement.bView = bView;
		m_listOneElement.SetAt(posElement, stOneElement);

		bResult = TRUE;
	}

	return bResult;
}

BOOL CLogicEditCtrl::ModifyGroupElementView(CString strElement, BOOL bView)
{
	/*	BOOL bResult = FALSE;

	POSITION posElement = NULL;
	GroupElementInfo  stGroupElement;
	posElement = GetGroupElementToPosition(strElement);
	stGroupElement = m_listGroupElement.GetAt(posElement);

	#ifdef _DEBUG
	if (posElement == NULL)
	SetErrorMessageBox("# [LogicEditCtrl] Not Have View Change Search Element!");
	#endif

	if (posElement != NULL)
	{
	if (bView == FALSE)
	{
	stGroupElement.stOneElementInfo.strBracket = "";
	stGroupElement.stOneElementInfo.lBracketCount = 0;
	}
	stGroupElement.stOneElementInfo.bView = bView;
	m_listGroupElement.SetAt(posElement, stGroupElement);

	bResult = TRUE;
	}
	*/

	// 2009.11.13 -hidden-

	return GetGroupElementToPosition(strElement, bView);
}



BOOL CLogicEditCtrl::ModifyElementNot(CString strElement, BOOL bNot)
{
	
	BOOL bResult = FALSE;

	strElement.TrimLeft();
	strElement.TrimRight();

	if (m_bUseGroup == TRUE)
		bResult = ModifyGroupElementNot(strElement, bNot);
	else if (m_bUseGroup == FALSE)
		bResult = ModifyOneElementNot(strElement, bNot);

	if (bResult == TRUE)
		ReLoadAll();

	return bResult;
}

BOOL CLogicEditCtrl::ModifyOneElementNot(CString strElement, BOOL bNot)
{
	BOOL bResult = FALSE;

	POSITION posElement = NULL;
	OneElementInfo  stOneElement;
	posElement = GetOneElementToPosition(strElement);
	stOneElement = m_listOneElement.GetAt(posElement);

#ifdef _DEBUG
	if (posElement == NULL)
		SetErrorMessageBox("# [LogicEditCtrl] Not Have View Change Search Element!");
#endif

	if (posElement != NULL)
	{
		CString tempNot;
		if (bNot == TRUE)
			tempNot = "^";
		else
			tempNot = "";

		stOneElement.strNot = tempNot;
		m_listOneElement.SetAt(posElement, stOneElement);

		bResult = TRUE;
	}

	return bResult;
}

BOOL CLogicEditCtrl::ModifyGroupElementNot(CString strElement, BOOL bNot)
{
	/*	BOOL bResult = FALSE;

	POSITION posElement = NULL;
	GroupElementInfo  stGroupElement;
	posElement = GetGroupElementToPosition(strElement);
	stGroupElement = m_listGroupElement.GetAt(posElement);

	#ifdef _DEBUG
	if (posElement == NULL)
	SetErrorMessageBox("# [LogicEditCtrl] Not Have View Change Search Element!");
	#endif

	if (posElement != NULL)
	{
	CString tempNot;
	if (bNot == TRUE)
	tempNot = "^";
	else
	tempNot = "";

	stGroupElement.stOneElementInfo.strNot = tempNot;
	m_listGroupElement.SetAt(posElement, stGroupElement);

	bResult = TRUE;
	}
	*/

	// 2009.11.13 -hidden-
	CString tempNot;
	GroupElementInfo  stGroupElement;

	strElement.TrimLeft();
	strElement.TrimRight();

	int nElemSize = 0;
	int nSize = m_arrGroupElement.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();
		for(int j = 0; j < nElemSize; ++j)
		{
			stGroupElement = pGroup->arrGroupElemInfo.GetAt(j);

			stGroupElement.stOneElementInfo.strElement.TrimLeft();
			stGroupElement.stOneElementInfo.strElement.TrimRight();

			if (stGroupElement.stOneElementInfo.strElement.Compare(strElement) == 0)
			{
				if (bNot == TRUE)
					tempNot = "^";
				else
					tempNot = "";

				stGroupElement.stOneElementInfo.strNot = tempNot;
				pGroup->arrGroupElemInfo.SetAt(j, stGroupElement);
				return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL CLogicEditCtrl::ModifyElementInsertGroup(int nGroup)
{
	BOOL bResult = FALSE;

	POSITION posElement;
	GroupElementInfo  stGroupElement;
	int nCount = m_listGroupElement.GetCount();

	if (nCount == 0)
		bResult = TRUE;
	else
	{
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			posElement = m_listGroupElement.FindIndex(nIndex);
			stGroupElement = m_listGroupElement.GetAt(posElement);

			if (nGroup <= stGroupElement.nGroup)
			{
				stGroupElement.nGroup++;
				m_listGroupElement.SetAt(posElement, stGroupElement);

				bResult = TRUE;
			}
		}
	}

	return bResult;
}

// 2009.11.10 -hidden-	
BOOL CLogicEditCtrl::ModifyElementInsertUpGroup(int nIndex, int nGroup)
{
	GROUP_ELEMINFO* pGroupEmlement = new GROUP_ELEMINFO;
	pGroupEmlement->nGroup = nGroup;
	m_arrGroupElement.InsertAt(nIndex-1, pGroupEmlement);

	return TRUE;
}

// 2009.11.10 -hidden-	
BOOL CLogicEditCtrl::ModifyElementInsertDownGroup(int nIndex, int nGroup)
{
	GROUP_ELEMINFO* pGroupEmlement = new GROUP_ELEMINFO;
	pGroupEmlement->nGroup = nGroup;

	int nSize = m_arrGroupElement.GetSize();
	if(nSize >= nIndex)
		m_arrGroupElement.InsertAt(nIndex-1, pGroupEmlement);
	else
		m_arrGroupElement.Add(pGroupEmlement);

	return TRUE;
}


void CLogicEditCtrl::MakeGroupStruct(int nIndex, int nGroup)
{
	if(nIndex < 0)
		return;

	int nSize = m_arrGroupElement.GetSize();
	GROUP_ELEMINFO* pGroupElement = NULL;

	for(int i = 0; i < nSize; ++i)
	{
		pGroupElement = m_arrGroupElement.GetAt(i);
		if(pGroupElement->nGroup == nGroup)
			return;
	}

	pGroupElement = new GROUP_ELEMINFO;
	pGroupElement->nGroup = nGroup;

	if(!m_arrGroupElement.GetSize())
	{
		m_arrGroupElement.Add(pGroupElement);
		return;
	}

	if(nGroup == 0)
		m_arrGroupElement.InsertAt(0, pGroupElement);
	else if(1 == nIndex)
	{
		if(!m_arrGroupElement.GetSize())
			m_arrGroupElement.Add(pGroupElement);
		else
		{
			if(0 == m_arrGroupElement.GetAt(0)->nGroup)
				m_arrGroupElement.InsertAt(nIndex, pGroupElement);
			else
				m_arrGroupElement.InsertAt(nIndex - 1, pGroupElement);
		}
	}
	else
		m_arrGroupElement.InsertAt(nIndex - 1, pGroupElement);

	//	m_arrGroupElement.InsertAt(nIndex, pGroupElement);
}

BOOL CLogicEditCtrl::ModifyElementDeleteGroup(int nGroup)
{
	/*	BOOL bResult = FALSE;

	POSITION posElement;
	GroupElementInfo  stGroupElement;
	int nCount = m_listGroupElement.GetCount();
	int nCountIndex = 0;

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	posElement = m_listGroupElement.FindIndex(nCountIndex);
	stGroupElement = m_listGroupElement.GetAt(posElement);

	if (nGroup > stGroupElement.nGroup)
	nCountIndex++;
	else if (nGroup == stGroupElement.nGroup)
	{
	m_listGroupElement.RemoveAt(posElement);
	bResult = TRUE;
	}
	else if (nGroup < stGroupElement.nGroup)
	{
	stGroupElement.nGroup--;
	m_listGroupElement.SetAt(posElement, stGroupElement);

	nCountIndex++;
	}
	}

	//## Group => Not Condition
	if (nCountIndex == nCount)
	bResult = TRUE;

	if ((nCount > 0) && (bResult == TRUE))
	ReLoadAll();

	return bResult;
	*/

	int nSize = m_arrGroupElement.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		if(pGroup->nGroup == nGroup)
		{
			pGroup->arrGroupElemInfo.RemoveAll();
			delete pGroup;

			m_arrGroupElement.RemoveAt(i);
			break;
		}
	}

	ReLoadAll();

	return TRUE;
}

POSITION CLogicEditCtrl::GetOneElementToPosition(CString strElement)
{
	POSITION pos, posElement = NULL;
	OneElementInfo  stOneElement;

	strElement.TrimLeft();
	strElement.TrimRight();

	int nCount = m_listOneElement.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);

		stOneElement.strElement.TrimLeft();
		stOneElement.strElement.TrimRight();
		if (stOneElement.strElement.Compare(strElement) == 0)
		{
			posElement = pos;
			break;
		}
	}

	return posElement;
}


//POSITION CLogicEditCtrl::GetGroupElementToPosition(CString strElement)
// 2009.11.13 -hidden-
BOOL CLogicEditCtrl::GetGroupElementToPosition(CString strElement, BOOL bView)
{
	GroupElementInfo  stGroupElement;

	strElement.TrimLeft();
	strElement.TrimRight();

	int nElemSize = 0;
	int nSize = m_arrGroupElement.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();
		for(int j = 0; j < nElemSize; ++j)
		{
			stGroupElement = pGroup->arrGroupElemInfo.GetAt(j);

			stGroupElement.stOneElementInfo.strElement.TrimLeft();
			stGroupElement.stOneElementInfo.strElement.TrimRight();

			if (stGroupElement.stOneElementInfo.strElement.Compare(strElement) == 0)
			{
				if (bView == FALSE)
				{
					stGroupElement.stOneElementInfo.strBracket = "";
					stGroupElement.stOneElementInfo.lBracketCount = 0;
				}
				stGroupElement.stOneElementInfo.bView = bView;
				pGroup->arrGroupElemInfo.SetAt(j, stGroupElement);
				return TRUE;
			}
		}
	}

	return FALSE;
}


void CLogicEditCtrl::DeleteAll()
{
	if (m_bUseGroup == TRUE)
	{
		//		m_listGroupElement.RemoveAll(); 

		int nSize = m_arrGroupElement.GetSize();
		for(int i = 0; i < nSize; ++i)
		{
			GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
			pGroup->arrGroupElemInfo.RemoveAll();
			delete pGroup;
		}
		m_arrGroupElement.RemoveAll();
	}
	else
	{
		m_listOneElement.RemoveAll(); 
		m_listOneElementUser.RemoveAll();
	}

	BrecketSelectCreate();
	m_bHaveBracket = FALSE;
	SetWindowText("");

	EnableWindow(FALSE);
}

void CLogicEditCtrl::DeleteGroupAll(int nGroup)
{
	/*
	POSITION posElement = NULL;
	GroupElementInfo  stGroupElement;

	BOOL bState = TRUE;
	while (bState == TRUE)
	{
	bState = FALSE;
	for (int nIndex = 0; nIndex < m_listGroupElement.GetCount(); nIndex++)
	{
	posElement = m_listGroupElement.FindIndex(nIndex);
	stGroupElement = m_listGroupElement.GetAt(posElement);

	if (stGroupElement.nGroup == nGroup)
	{
	m_listGroupElement.RemoveAt(posElement);
	bState = TRUE;
	break;
	}
	}
	}

	if (m_listGroupElement.GetCount() == 0)
	DeleteAll();
	else
	ReLoadAll();
	*/


	// 2009.11.13 -hidden-
	GroupElementInfo  stGroupElement;
	BOOL bState = TRUE;

	int nSize = m_arrGroupElement.GetSize();
	int i = 0;
	for(i = 0; i < nSize; ++i)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		if(pGroup->nGroup == nGroup)
		{
			pGroup->arrGroupElemInfo.RemoveAll();
			delete pGroup;
			m_arrGroupElement.RemoveAt(i);
			break;
		}
	}

	int nRestCnt = 0;

	nSize = m_arrGroupElement.GetSize();
	for(i = 0; i < nSize; ++i)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		nRestCnt += pGroup->arrGroupElemInfo.GetSize();
	}

	if (nRestCnt == 0)
		DeleteAll();
	else
		ReLoadAll();
}


POSITION CLogicEditCtrl::ListSearchPosition(CList<OneElementInfo, OneElementInfo> &beforList, 
											CString strElement)
{
	OneElementInfo stOneElement;
	BOOL bFlag = FALSE;
	POSITION pos, resultPos;
	int nCount;

	nCount = beforList.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		pos = beforList.FindIndex(i);
		stOneElement = beforList.GetAt(pos);

		if(stOneElement.strElement == strElement)
		{
			resultPos = pos;
			bFlag = TRUE;
			break;
		}
	}

	if(bFlag == TRUE)
		return resultPos;
	else
		return NULL;
}


void CLogicEditCtrl::ElementPositionMove(int FrontIndex, int AfterIndex)
{
	OneElementInfo  structMove, structCopy, structSet;
	POSITION pos_move, pos_copy, pos_set;

	//## 이동 데이터 ##
	pos_copy = m_listOneElement.FindIndex(FrontIndex);
	structCopy = m_listOneElement.GetAt(pos_copy);

	TRACE("\n# LogicEditCtrl Drag&Drop Element(START) : %s", structCopy.strElement);

	if (FrontIndex > AfterIndex)  
	{
		//for(int i = nOldIndex - 1; i >= nNewIndex; i--)
		for(int nIndex = FrontIndex; nIndex > AfterIndex; nIndex--)
		{
			//pos_move = m_listOneElement.FindIndex(i);
			pos_move = m_listOneElement.FindIndex(nIndex - 1);
			structMove = m_listOneElement.GetAt(pos_move);

			TRACE("\n# LogicEditCtrl Drag&Drop Element : %s", structMove.strElement);

			//## Data Move ##
			structSet.bView = structMove.bView;
			structSet.strOperation = structMove.strOperation;
			structSet.strNot = structMove.strNot;
			structSet.strElement = structMove.strElement;

			//## Bracket Info 위치 고정 ##
			pos_set = m_listOneElement.FindIndex(nIndex);
			structMove = m_listOneElement.GetAt(pos_set);
			structSet.strBracket = structMove.strBracket;
			structSet.lBracketCount = structMove.lBracketCount;

			m_listOneElement.SetAt(pos_set, structSet);
		}
	}
	else if (FrontIndex < AfterIndex)
	{
		//for(int i = nOldIndex + 1; i <= nNewIndex; i++)
		for(int nIndex = FrontIndex; nIndex < AfterIndex; nIndex++)
		{
			//pos_move = m_listOneElement.FindIndex(i);
			pos_move = m_listOneElement.FindIndex(nIndex + 1);
			structMove = m_listOneElement.GetAt(pos_move);

			TRACE("\n# LogicEditCtrl Drag&Drop Element : %s", structMove.strElement);

			//## Data Move ##
			structSet.bView = structMove.bView;
			structSet.strOperation = structMove.strOperation;
			structSet.strNot = structMove.strNot;
			structSet.strElement = structMove.strElement;

			//## Bracket Info 위치 고정 ##
			pos_set = m_listOneElement.FindIndex(nIndex);
			structMove = m_listOneElement.GetAt(pos_set);
			structSet.strBracket = structMove.strBracket;
			structSet.lBracketCount = structMove.lBracketCount;

			m_listOneElement.SetAt(pos_set, structSet);
		}
	}

	structSet.bView = structCopy.bView;
	structSet.strOperation = structCopy.strOperation;
	structSet.strNot = structCopy.strNot;
	structSet.strElement = structCopy.strElement;

	TRACE("\n# LogicEditCtrl Drag&Drop Element(END) : %s", structMove.strElement);

	//## Bracket Info 위치 고정 
	pos_set = m_listOneElement.FindIndex(AfterIndex);
	structMove = m_listOneElement.GetAt(pos_set);
	structSet.strBracket = structMove.strBracket;
	structSet.lBracketCount = structMove.lBracketCount;

	m_listOneElement.SetAt(pos_set, structSet);

	ReLoadAll();
}


CSize CLogicEditCtrl::GetFontSize()
{
	CClientDC	dc(this);
	CFont*	pOldFont = dc.SelectObject(&m_fontDefault);

	CSize	sizeFont;
	TEXTMETRIC	tm;

	dc.GetTextMetrics(&tm);
	sizeFont.cx	=	tm.tmAveCharWidth;
	sizeFont.cy	=	tm.tmHeight;

	dc.SelectObject(pOldFont);

	return	sizeFont;
}


void CLogicEditCtrl::BrecketSelectCreate()
{
	if (m_bUseGroup == TRUE) //# <= 순차
		return;    

	//## Operation Color restoration ##
	if ((m_noldStartOperation > -1) || (m_noldEndOperation > -1))
	{
		SetSel(m_noldStartOperation, m_noldEndOperation);
		Invalidate(TRUE);
		SetSelectionCharFormat(GetFontFormat(clrOperation));

		m_noldStartOperation = -1;
		m_noldEndOperation = -1;
	}

	m_nSelectStartPosition = -1;
	m_nSelectEndPosition = -1;
	m_bSelectState = FALSE;
	m_bSelectDuring = FALSE;
	Invalidate();

	//TRACE("\n## LogicEditCtrl => BrecketSelectCreate");
}


int CLogicEditCtrl::CheckMakeBracketOK(CString strSelect)
{
	OneElementInfo stOneElement;

	int i;
	int nCount = strSelect.GetLength();

	int nChoiceElement = 0;
	CStringArray saElement;

	long lBracketCount = 0;
	int nBracketIndex = 0;
	CString strFirstBracket = "";

	int nHaveNext = 0;
	for(i = 0; i < nCount; i++)
	{

#ifdef _DEBUG
		CString strTemp;
		strTemp = strSelect[i];
#endif

		//0000141: 조건 id 가 소문자일때 조건을 이동시켜도 하단 에디트창에 적용이 안되는 현상 개선
		//0000143: 조건검색, 일반검색의 논리 Logic식에 소문자를 사용시 ()-괄호 기능이 동작하지 않는 오류 개선
		if( ( (strSelect[i] >= 'A') && (strSelect[i] <= 'Z') ) || ((strSelect[i] >= 'a') && (strSelect[i] <= 'z')) )
		{
			saElement.Add(strSelect[i]);
			nChoiceElement++;
		}
		else if ( strSelect[i] == '(' )
		{
			lBracketCount++;

			if (nBracketIndex == 0)
				strFirstBracket = strSelect[i];

			nBracketIndex++;
		}
		else if ( strSelect[i] == ')' )
		{
			lBracketCount--;

			if (nBracketIndex == 0)
				strFirstBracket = strSelect[i];

			nBracketIndex++;
		}
		else if ( strSelect[i] == '~' )
			nHaveNext++;
	}

	int nResult = 0;

	//## 선택된 Element의 수가 적음 ## 
	if (nChoiceElement < 2)
		return 1;

	if (nHaveNext > 0)
		return 2;

	//## 괄호 검색 (0이 아니면 Error) ##
	if (lBracketCount != 0)
		return 3;
	else
	{
		//## 첫번째 괄호가 ')' 인경우 ##
		if (strFirstBracket == ")")
			return 4;
	}

	return 0;
}


int CLogicEditCtrl::PointToStrPosition(CPoint point)
{
	CString strLogic;
	GetWindowText(strLogic);

	int nPositionChar = -1;

	CSize	szFont;
	if (point.x == 1)
		nPositionChar = 0;
	else
	{
		szFont = GetFontSize();

		int nFontWidth = 0;
		for(int i = 0; i < strLogic.GetLength(); i++)
		{
			nFontWidth += szFont.cx;

			if(nFontWidth >= point.x)
			{
				nPositionChar = i;
				break;
			}
		}
	}

	int nLineCnt = GetLineCount();
	for (int nLineIndex = 0; nLineIndex < nLineCnt; nLineIndex++)
	{
		if (point.y >= szFont.cy * nLineIndex)
		{
			if (LineIndex(nLineIndex) != -1)
				nPositionChar += LineIndex(nLineIndex);
		}
	}

	return nPositionChar;
}

void CLogicEditCtrl::GetLogicElement(CStringArray& arrElement, CStringArray& arrOpt)
{
	COLORREF clrError = RGB(0, 255, 0);
	int nFindCondition = -1; //조건
	int nFindSpecial = -1;   //특수문자
	int nFindOpt = -1;       //로직
	int nFindTemp = -1;

	int nIndex; 

	CString strUserBackup;
	CString strUserInput;
	CString strUserInputConvert;
	GetWindowText(strUserInput);

	if(strUserInput == " " || strUserInput == "")
		return;
	//특수문자 및 숫자 체크
	CString strData;
	for(nIndex = 0; nIndex < strUserInput.GetLength(); nIndex++)
	{
		if( (isalpha(strUserInput[nIndex]) == FALSE && !(strUserInput[nIndex] == '(' || strUserInput[nIndex] == ')' ) ) ||
			isdigit(strUserInput[nIndex]))
		{
			strData.Format("%c", strUserInput[nIndex]);
			strUserInput.Replace(strData, " ");
		}
	}

	strUserBackup = strUserInput;
	strUserInput.MakeLower();	

	strUserInput.Replace(CONDITION_OPT[0], "&");
	strUserInput.Replace(CONDITION_OPT[1], "|");

	strUserInput.Replace(" ", "");
	strUserInput.MakeUpper();

	strUserInput += "!";

	int nSel;
	if( !CheckIsValidLogic(strUserInput, nSel) )
	{
		// 		SetSel(nSel, nSel+1);
		// 		Invalidate(TRUE);
		// 		SetSelectionCharFormat(GetFontFormat(clrError));
		::MessageBox(m_hWnd, "형식에 맞지 않는 조건식이 포함되어 있습니다.", PROGRAM_TITLE, MB_OK);
		return;	
	}
	strUserInput.Replace("!", "");

	//첫 기호 인덱스
	arrOpt.Add("1");
	for(nIndex = 0; nIndex < strUserInput.GetLength(); nIndex++)
	{
		if(isalpha(strUserInput[nIndex]))
		{	
			//엘레멘트 추가
			strData.Format("%c", strUserInput[nIndex]);
			arrElement.Add(strData);
		}
		else
		{
			strData.Format("%c", strUserInput[nIndex]);
			if(strData.CompareNoCase("&") == 0)
				arrOpt.Add("1");
			else if(strData.CompareNoCase("|") == 0)
				arrOpt.Add("0");
		}
	}

}

void CLogicEditCtrl::GetLogicPacketOneString(CString &strLogic)
{
	COLORREF clrError = RGB(0, 255, 0);
	int nFindCondition = -1; //조건
	int nFindSpecial = -1;   //특수문자
	int nFindOpt = -1;       //로직
	int nFindTemp = -1;

	int nIndex; 

	CString strUserBackup;
	CString strUserInput;
	CString strUserInputConvert;
	GetWindowText(strUserInput);
	
	if(strUserInput == " " || strUserInput == "")
		return;
//특수문자 및 숫자 체크
	CString strData;
	for(nIndex = 0; nIndex < strUserInput.GetLength(); nIndex++)
	{
		if( (isalpha(strUserInput[nIndex]) == FALSE && !(strUserInput[nIndex] == '(' || strUserInput[nIndex] == ')' ) ) ||
			isdigit(strUserInput[nIndex]))
		{
			strData.Format("%c", strUserInput[nIndex]);
			strUserInput.Replace(strData, " ");
		}
	}
		
	strUserBackup = strUserInput;
	strUserInput.MakeLower();	

	strUserInput.Replace(CONDITION_OPT[0], "&");
	strUserInput.Replace(CONDITION_OPT[1], "|");

	strUserInput.Replace(" ", "");
	strUserInput.MakeUpper();

	strUserInput += "!";

	int nSel;
	if( !CheckIsValidLogic(strUserInput, nSel) )
	{
// 		SetSel(nSel, nSel+1);
// 		Invalidate(TRUE);
// 		SetSelectionCharFormat(GetFontFormat(clrError));
		::MessageBox(m_hWnd, "형식에 맞지 않는 조건식이 포함되어 있습니다.", PROGRAM_TITLE, MB_OK);
		return;	
	}
	strUserInput.Replace("!", "");

	strLogic = strUserInput;
	return;
	

	//엘레멘트 갯수 파악
	CStringArray arrElement;
	CStringArray arrLogic;
	CStringArray arrBracket;
	CUIntArray   arrBracketCount;
	CString strBracket;
	int		nBracketCount;
	int nTemp = 0;

	for(nIndex = 0; nIndex < strUserInput.GetLength(); nIndex++)
	{
		if(isalpha(strUserInput[nIndex]))
		{
			strBracket = "";
			nBracketCount = 0;
			nTemp = 0;
			//엘레멘트 추가
			strData.Format("%c", strUserInput[nIndex]);
			arrElement.Add(strData);
			//연산자 확인
			if(strUserInput.GetLength() == nIndex+1)
				strData = "";
			else
				strData.Format("%c", strUserInput[nIndex+1]);
			arrLogic.Add(strData);

			//좌측(시작) 괄호확인
			while(TRUE)
			{
				if(nIndex + (nTemp - 1) < 0)
					break;

				strData.Format("%c", strUserInput[nIndex + (--nTemp)]);
				if (strData.CompareNoCase("(") == 0)
				{
					strBracket += strData;
					nBracketCount++;
				}
				else
					break;
			}
			//우측(시작) 괄호확인
			nTemp = 0;
			while(TRUE)
			{
				if(nIndex + (nTemp + 1) > strUserInput.GetLength())
					break;

				strData.Format("%c", strUserInput[nIndex + (++nTemp)]);
				if (strData.CompareNoCase(")") == 0)
				{
					strBracket += strData;
					nBracketCount--;
				}
				else
					break;
			}
			arrBracket.Add(strBracket);
			arrBracketCount.Add(nBracketCount);
		}
	}
	
	CString strOpt;
	for(nIndex = 0; nIndex < arrElement.GetSize(); nIndex++)
	{
		OneElementInfo stOneElementUser;
		if(arrLogic.GetAt(nIndex).CompareNoCase("&") == 0)
			strOpt = _T("and");
		else if(arrLogic.GetAt(nIndex).CompareNoCase("|") == 0)
			strOpt = _T("or");

		stOneElementUser.lBracketCount = arrBracketCount.GetAt(nIndex);
		stOneElementUser.strBracket = arrBracket.GetAt(nIndex);
		stOneElementUser.strElement = arrElement.GetAt(nIndex);
		stOneElementUser.strNot = "";
		stOneElementUser.strOperation = strOpt;
		stOneElementUser.strTag = "";
		stOneElementUser.bView = TRUE;

		m_listOneElementUser.AddTail(stOneElementUser);
	}
	

	

	OneElementInfo  stOneElement;
	int nCount = m_listOneElement.GetCount();

	POSITION pos;
	int nElementIndex = 0;
	int nViewCount = 0;
	for(nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = m_listOneElement.FindIndex(nIndex);

		//## Element 정리 ##
		stOneElement = m_listOneElement.GetAt(pos);

		if (stOneElement.bView == FALSE)
			continue;
		else
			nElementIndex++;

		CString strOperation;
		if (stOneElement.bView == TRUE)
		{
			//if (nStepCnt >= nViewCount)  //## nStepIndex
			//{
			if (stOneElement.strOperation == "and")
			{
				//strarrayOperater.Add("1");
				strOperation = "&";
			}
			else if (stOneElement.strOperation == "or")
			{
				//strarrayOperater.Add("0");
				strOperation = "|";
			}
			else if (stOneElement.strOperation == "next")  //## 순차검색
			{
				//strarrayOperater.Add("1");
				strOperation = "~";
			}

			if(nElementIndex == 1)
				strLogic += stOneElement.strBracket + stOneElement.strElement;
			else
			{
				if(stOneElement.strBracket.IsEmpty() == TRUE)
					strLogic += strOperation + stOneElement.strElement;
				else if( stOneElement.strBracket[0] == '(' )
					strLogic += strOperation + stOneElement.strBracket + stOneElement.strElement;
				else if( stOneElement.strBracket[0] == ')' )
					strLogic += strOperation + stOneElement.strElement + stOneElement.strBracket;
			}

			nViewCount++;
			//}
		}
	}
}


void CLogicEditCtrl::GetLogicPacketGroupString(CString &strGroupLogic)
{
	/*	strGroupLogic = "";

	int nElementIndex = 0;
	int nAfterGroup, nMaxGroup;
	nMaxGroup = GetMaxGroup();
	int nCount = m_listGroupElement.GetCount();
	if (nCount > 0)
	EnableWindow(TRUE);

	POSITION pos;
	GroupElementInfo  stGroupElement;
	for (int nNowGroup = 1; nNowGroup <= nMaxGroup; nNowGroup++)
	{
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	pos = m_listGroupElement.FindIndex(nIndex);
	stGroupElement = m_listGroupElement.GetAt(pos);

	//## Group 구분 ##
	if (nNowGroup == stGroupElement.nGroup)
	{
	if (stGroupElement.stOneElementInfo.bView == TRUE)
	{
	if (m_bAutoElemet == TRUE)
	{
	stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];
	m_listGroupElement.SetAt(pos, stGroupElement);
	}

	//TRACE("\n#    LogicEditCtrl Element : %s", stOneElement.strElement);
	nElementIndex++;
	}
	else if (stGroupElement.stOneElementInfo.bView == FALSE)
	continue;

	if (stGroupElement.stOneElementInfo.bView == TRUE)
	{
	if (nElementIndex == 1)
	strGroupLogic += stGroupElement.stOneElementInfo.strBracket 
	//+ stGroupElement.stOneElementInfo.strNot
	+ stGroupElement.stOneElementInfo.strElement;
	else
	{
	CString strOpt = "";
	if (stGroupElement.nGroup != nAfterGroup)
	strOpt = "~";
	else
	{
	if (stGroupElement.stOneElementInfo.strOperation == "and")
	strOpt = "&";
	else if (stGroupElement.stOneElementInfo.strOperation == "or")
	strOpt = "|";
	}

	if(stGroupElement.stOneElementInfo.strBracket.IsEmpty() == TRUE)
	{
	if (strOpt.IsEmpty() == FALSE)
	strGroupLogic += " " + strOpt + " " 
	//+ stOneElement.strNot +
	+ stGroupElement.stOneElementInfo.strElement;
	else
	strGroupLogic += " " + strOpt 
	+ " " //+ stOneElement.strNot
	+ stGroupElement.stOneElementInfo.strElement;
	}
	else if (stGroupElement.stOneElementInfo.strBracket[0] == '(')
	{
	if (strOpt.IsEmpty() == FALSE)
	{
	strGroupLogic += " " + strOpt + " " 
	+ stGroupElement.stOneElementInfo.strBracket 
	//+ stOneElement.strNot
	+ stGroupElement.stOneElementInfo.strElement;
	}
	else
	{
	strGroupLogic += " " + strOpt
	+ " " + stGroupElement.stOneElementInfo.strBracket 
	//+ stOneElement.strNot
	+ stGroupElement.stOneElementInfo.strElement;
	}
	}
	else if (stGroupElement.stOneElementInfo.strBracket[0] == ')')
	{
	if (strOpt.IsEmpty() == FALSE)
	{
	strGroupLogic += " " + strOpt + " " 
	//+ stOneElement.strNot
	+ stGroupElement.stOneElementInfo.strElement 
	+ stGroupElement.stOneElementInfo.strBracket;
	}
	else
	{
	strGroupLogic += " " + strOpt 
	+ " " //+ stOneElement.strNot
	+ stGroupElement.stOneElementInfo.strElement 
	+ stGroupElement.stOneElementInfo.strBracket;
	}
	}
	}

	nAfterGroup = nNowGroup;
	}
	}
	}
	}

	strGroupLogic.Replace(" ", "");
	*/

	// 2009.11.12 -hidden-
	strGroupLogic = "";

	int nElementIndex = 0;
	int nAfterGroup, nCount = 0;

	int nGroupSize = m_arrGroupElement.GetSize();

	for(int nSize = 0; nSize < nGroupSize; ++nSize)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(nSize);
		nCount += pGroup->arrGroupElemInfo.GetSize();
	}

	if (nCount > 0)
		EnableWindow(TRUE);

	GroupElementInfo  stGroupElement;
	for(int i = 0; i < nGroupSize; ++i)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		int nElemSize = pGroup->arrGroupElemInfo.GetSize();

		for (int nIndex = 0; nIndex < nElemSize; nIndex++)
		{
			stGroupElement = pGroup->arrGroupElemInfo.GetAt(nIndex);

			if (stGroupElement.stOneElementInfo.bView == TRUE)
			{
				if (m_bAutoElemet == TRUE)
				{
					stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];
					pGroup->arrGroupElemInfo.SetAt(nIndex, stGroupElement);
				}
				nElementIndex++;
			}
			else if (stGroupElement.stOneElementInfo.bView == FALSE)
				continue;

			if (stGroupElement.stOneElementInfo.bView == TRUE)
			{
				if (nElementIndex == 1)
					strGroupLogic += stGroupElement.stOneElementInfo.strBracket 
					+ stGroupElement.stOneElementInfo.strElement;
				else
				{
					CString strOpt = "";
					if (stGroupElement.nGroup != nAfterGroup)
						strOpt = "~";
					else
					{
						if (stGroupElement.stOneElementInfo.strOperation == "and")
							strOpt = "&";
						else if (stGroupElement.stOneElementInfo.strOperation == "or")
							strOpt = "|";
					}

					if(stGroupElement.stOneElementInfo.strBracket.IsEmpty() == TRUE)
					{
						if (strOpt.IsEmpty() == FALSE)
							strGroupLogic += " " + strOpt + " " 
							//+ stOneElement.strNot +
							+ stGroupElement.stOneElementInfo.strElement;
						else
							strGroupLogic += " " + strOpt 
							+ " " //+ stOneElement.strNot
							+ stGroupElement.stOneElementInfo.strElement;
					}
					else if (stGroupElement.stOneElementInfo.strBracket[0] == '(')
					{
						if (strOpt.IsEmpty() == FALSE)
						{
							strGroupLogic += " " + strOpt + " " 
								+ stGroupElement.stOneElementInfo.strBracket 
								//+ stOneElement.strNot
								+ stGroupElement.stOneElementInfo.strElement;
						}
						else
						{
							strGroupLogic += " " + strOpt
								+ " " + stGroupElement.stOneElementInfo.strBracket 
								//+ stOneElement.strNot
								+ stGroupElement.stOneElementInfo.strElement;
						}
					}
					else if (stGroupElement.stOneElementInfo.strBracket[0] == ')')
					{
						if (strOpt.IsEmpty() == FALSE)
						{
							strGroupLogic += " " + strOpt + " " 
								//+ stOneElement.strNot
								+ stGroupElement.stOneElementInfo.strElement 
								+ stGroupElement.stOneElementInfo.strBracket;
						}
						else
						{
							strGroupLogic += " " + strOpt 
								+ " " //+ stOneElement.strNot
								+ stGroupElement.stOneElementInfo.strElement 
								+ stGroupElement.stOneElementInfo.strBracket;
						}
					}
				}

				nAfterGroup = pGroup->nGroup;
			}
		}
	}

	strGroupLogic.Replace(" ", "");
}


void CLogicEditCtrl::GetLogicString(CString &strLogic, int nStepCnt, CStringArray &strarrayOperater)
{
	OneElementInfo  stOneElement;
	int nCount, nViewCount;
	nCount = m_listOneElement.GetCount();

	nStepCnt = nStepCnt - 1; //<= Index Change

	POSITION pos;
	int nElementIndex = 0;
	nViewCount = 0;
	for(int i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);

		//## Element 정리 ##
		stOneElement = m_listOneElement.GetAt(pos);

		if (stOneElement.bView == FALSE)
			continue;
		else
			nElementIndex++;

		CString strOperation;
		if (stOneElement.bView == TRUE)
		{
			if (nStepCnt >= nViewCount)  //## nStepIndex
			{
				if (stOneElement.strOperation == "and")
				{
					strarrayOperater.Add("1");
					strOperation = "&";
				}
				else if (stOneElement.strOperation == "or")
				{
					strarrayOperater.Add("0");
					strOperation = "|";
				}
				else if (stOneElement.strOperation == "next")  //## 순차검색
				{
					strarrayOperater.Add("1");
					strOperation = "~";
				}

				if(nElementIndex == 1)
					strLogic += stOneElement.strBracket + stOneElement.strElement;
				else
				{
					if(stOneElement.strBracket.IsEmpty() == TRUE)
						strLogic += strOperation + stOneElement.strElement;
					else if( stOneElement.strBracket[0] == '(' )
						strLogic += strOperation + stOneElement.strBracket + stOneElement.strElement;
					else if( stOneElement.strBracket[0] == ')' )
						strLogic += strOperation + stOneElement.strElement + stOneElement.strBracket;
				}

				nViewCount++;
			}
		}
	}
}


BOOL CLogicEditCtrl::GetHaveBracket()
{
	BOOL bFlag = FALSE;

	POSITION pos;
	OneElementInfo stOneElement;

	int nCount = m_listOneElement.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);

		if (stOneElement.lBracketCount > 0)
		{
			bFlag = TRUE;
			break;
		}
	}

	return bFlag;
}


BOOL CLogicEditCtrl::GetElementOverlap()
{
	BOOL bResult = FALSE;

	POSITION pos;
	OneElementInfo stOneElement;

	CString strTempElement;
	int nCount = m_listOneElement.GetCount();

	for (int i = 0; i < nCount - 1; i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElement = m_listOneElement.GetAt(pos);

		strTempElement = stOneElement.strElement;
		for (int j = i + 1; j < nCount; j++)
		{
			pos = m_listOneElement.FindIndex(j);
			stOneElement = m_listOneElement.GetAt(pos);

			if(strTempElement == stOneElement.strElement)
			{
				bResult = TRUE;
				break;
			}
		}
	}

	return bResult;
}


int CLogicEditCtrl::SetOneElementStandLine()
{
	int nResult = 0;

	POSITION	pos;
	OneElementInfo	stOneElement;

	int nCount = m_listOneElement.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = m_listOneElement.FindIndex(nIndex);
		stOneElement = m_listOneElement.GetAt(pos);

		stOneElement.strElement = CONDITION_INDEX[nIndex];
		m_listOneElement.SetAt(pos, stOneElement);

		nResult++;
	}

	return nResult;
}


int CLogicEditCtrl::SetGroupElementStandLine(int nElementIndex)
{
	int nResult = nElementIndex;

	/*	POSITION	pos;
	GroupElementInfo	stGroupElement;
	int nMaxGroup = GetMaxGroup();

	int nCount = m_listGroupElement.GetCount();
	for (int nGroupIndex = 1; nGroupIndex <= nMaxGroup; nGroupIndex++)
	{
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	pos = m_listGroupElement.FindIndex(nIndex);
	stGroupElement = m_listGroupElement.GetAt(pos);

	if (stGroupElement.nGroup == nGroupIndex)
	{
	stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];
	m_listGroupElement.SetAt(pos, stGroupElement);

	nElementIndex++;
	nResult++;
	}
	}
	}
	*/

	// 2009.11.13 -hidden-
	GroupElementInfo	stGroupElement;
	int nElemSize = 0;	

	int nCount = m_arrGroupElement.GetSize();
	for (int i = 0; i < nCount; ++i)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(i);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();
		for (int j = 0; j < nElemSize; ++j)
		{
			stGroupElement = pGroup->arrGroupElemInfo.GetAt(j);
			stGroupElement.stOneElementInfo.strElement = CONDITION_INDEX[nElementIndex];
			pGroup->arrGroupElemInfo.SetAt(j, stGroupElement);

			nElementIndex++;
			nResult++;
		}
	}

	return nResult;
}


//## Mouse #######################################################


void CLogicEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("\n# LButton Db Click!");

	CRichEditCtrl::OnLButtonDblClk(nFlags, point);

	long nStart, nEnd;
	GetSel(nStart, nEnd);
	m_nSelectEndPosition = nStart;
	m_nSelectEndPosition = nEnd;

	if (m_bUseGroup == TRUE) //# <= 순차
		return;

	CString strLogic;
	GetWindowText(strLogic);

	int nPositionChar = -1;
	nPositionChar = PointToStrPosition(point);

	TRACE(_T("* Move Position => %d\n"), nPositionChar);

	int nSearchResult, nSearchStart;
	int nAndOrTextCount;  //# 0: None
	//# 3: and
	//# 2: or
	CString strAndOrText;

	if (nPositionChar == -1)
	{
		BrecketSelectCreate();
		return;
	}

	nSearchStart = nPositionChar - (int)OPERATION_SEARCH_COUNT / 2;
	if(nSearchStart < 0)
		nSearchStart = 0;

	if (strLogic.GetLength() < nSearchStart)
	{
		BrecketSelectCreate();
		return;
	}

	CString strTempLogic;
	strTempLogic = strLogic;
	strTempLogic = strTempLogic.Mid(nSearchStart, OPERATION_SEARCH_COUNT);
	TRACE("* Move Search String => " + strTempLogic + "\n");

	nAndOrTextCount = 0;
	strAndOrText = "";
	if (strTempLogic.Find("and", 0) != -1)
	{
		nSearchResult = strTempLogic.Find("and", 0);
		nAndOrTextCount = 3; //<- and
		strAndOrText = "or";
	}
	else if (strTempLogic.Find("or", 0) != -1)
	{
		nSearchResult = strTempLogic.Find("or", 0);
		nAndOrTextCount = 2; //<- or
		strAndOrText = "and";
	}
	else
	{
		BrecketSelectCreate();
		return;
	}

	//## 전체 논리 조합에서 연산자위치 복귀
	nSearchResult = nSearchStart + nSearchResult;
	CString strCopy;
	if (nAndOrTextCount != 0)
	{
		//##### List Data Change START #####
		int nSelectElementIndex = 0;
		CString strSelectedEdlement;

		//## Selected Operation => Element Index Search
		for(int i = nSearchResult + nAndOrTextCount - 1; i < strLogic.GetLength(); i++)
		{  
			if( (strLogic[i] >= 'A') && (strLogic[i] <= 'Z') )
			{
				nSelectElementIndex = i;
				strSelectedEdlement = strLogic[i];
				break;
			}
		}

		OneElementInfo  stOneElement;
		POSITION pos;
		for(int nElementIndex = 0; nElementIndex < m_listOneElement.GetCount(); nElementIndex++)
		{
			pos = m_listOneElement.FindIndex(nElementIndex);
			stOneElement = m_listOneElement.GetAt(pos);

			if (stOneElement.strElement == strSelectedEdlement)
				break;
		}

		
		CStringArray saEditData;		//<= Change Info
		//# => 0 : Element
		//# => 1 : Operation
		saEditData.Add(stOneElement.strElement);

		if (stOneElement.strOperation == "or")
		{
			stOneElement.strOperation = "and";
			saEditData.Add("1");
		}
		else if (stOneElement.strOperation == "and")
		{
			stOneElement.strOperation = "or";
			saEditData.Add("0");	
		}

		m_listOneElement.SetAt(pos, stOneElement);

		//## User_define => Opt 수정 ##
		CWnd* pParentWnd = this;
		//# CSubMainRightView
		pParentWnd = pParentWnd->GetParent();

		pParentWnd->SendMessage(WM_LOGIC_EDIT, (WPARAM)NM_EDIT_OPTCHANG, (LPARAM)&saEditData);

		//##### List Data Change END #####
	}

	BrecketSelectCreate();	
//	ReLoadAll();	

	//20100716 로직 User에디터 작업

	CString strOpt = strCopy = strLogic.Mid(nStart, nEnd - nStart );
	CString strHead = strLogic.Left(nStart);
	CString strEnd  = strLogic.Mid(nEnd + 1);

	strCopy.TrimLeft();
	strCopy.TrimRight();

	if(strCopy.CompareNoCase("and") == 0)
	{
		strOpt = _T("or ");
	//	nEnd -= 1;
	}
	else if(strCopy.CompareNoCase("or") == 0)
	{
		strOpt = _T("and ");
	//	nEnd += 1;
	}
	else
		return;

	CString strResult = strHead;
	strResult += strOpt;
	strResult += strEnd;
	SetWindowText(strResult);

	SetSel(nStart, nEnd - 1);	
	Invalidate(TRUE);
	SetSelectionCharFormat(GetFontFormat(clrOperationSelect));

}


void CLogicEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("\n# LButton Down!");

	if (m_bUseGroup == TRUE) //# <= 순차
		return;

	BrecketSelectCreate();
	m_nSelectStartPosition = PointToStrPosition(point);
	if (m_nSelectStartPosition > -1)
	{
		m_bSelectDuring = TRUE;

		m_bCaptureEnabled = TRUE;
		if (m_bCaptureEnabled == TRUE)
			SetCapture();
	}

	TRACE("\n# Mouse Down => *Position : %d, \n     *Select During Flag : %d", 
		m_nSelectStartPosition, m_bSelectDuring);

	CRichEditCtrl::OnLButtonDown(nFlags, point);
}

void CLogicEditCtrl::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTCAPTION)
		m_bCaptureEnabled = m_bCaptureEnabled ? FALSE : TRUE;

	CRichEditCtrl::OnNcLButtonDown(nHitTest, point);
}

void CLogicEditCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default 
	TRACE("\n# LButton Up!");

	if (m_bUseGroup == TRUE) //# <= 순차
		return;

	if (m_bSelectDuring == TRUE)
	{
		SelectFieldCheck();

		if (GetCapture() == this)
			::ReleaseCapture();
	}

	CRichEditCtrl::OnLButtonUp(nFlags, point);
}


void CLogicEditCtrl::SelectFieldCheck()
{
	long lStartChar, lEndChar;
	GetSel(lStartChar, lEndChar);

	m_nSelectStartPosition = (int)lStartChar;
	m_nSelectEndPosition = (int)lEndChar;

	if (((m_nSelectEndPosition > -1) && (m_nSelectStartPosition > -1)) &&
		(m_nSelectEndPosition != m_nSelectStartPosition) )
	{
		m_bSelectState = TRUE;
		m_bSelectDuring = FALSE;
		TRACE("\n# Mouse Up Position : %d", m_nSelectEndPosition);
	}
	else if (m_nSelectEndPosition == m_nSelectStartPosition)
		BrecketSelectCreate();
}


void CLogicEditCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	if (m_bUseGroup == TRUE) //# <= 순차
 		return;

	if (!m_bMouseOver)
	{
		m_bMouseOver = TRUE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;// | TME_HOVER;
		tme.hwndTrack = m_hWnd;
		//tme.dwHoverTime = HOVER_DEFAULT;
		::_TrackMouseEvent(&tme);
	}

	//## Have Selected Area ##
	if (m_bSelectState == TRUE) 
	{
		TRACE("\n# Mouse Move SelectState!");
	}
	//## Selecting ##
	else if (m_bSelectDuring == TRUE)  
	{
// 		if (m_bHaveCaret == FALSE)
 			SendMessage(WM_SETFOCUS, 0, 0);
// 		else
// 			SendMessage(WM_KILLFOCUS, 0, 0);
		

		SetCursor(m_hSelectDuringCursor);

		m_nSelectEndPosition = PointToStrPosition(point);
		SetSel(m_nSelectStartPosition, m_nSelectEndPosition);


		//TRACE("\n# Mouse Move Selecting!");
	}
	//## Mouse Move ##
	else
	{
		CString strLogic;
		int nPositionChar = -1;
		GetWindowText(strLogic);

		nPositionChar = PointToStrPosition(point);
		//TRACE(_T("\n# Now Caret Position => %d"), nPositionChar);

		int nSearchResult, nSearchStart;
		int nAndOrTextCount;  //# 0: None
		//# 3: and
		//# 2: or

		if (nPositionChar == -1)
			return;

		nSearchStart = nPositionChar - (int)OPERATION_SEARCH_COUNT / 2;
		if(nSearchStart < 0)
			nSearchStart = 0;

		if (strLogic.GetLength() < nSearchStart)
			return;

		CString strTempLogic;
		strTempLogic = strLogic.Mid(nSearchStart, OPERATION_SEARCH_COUNT);

		nAndOrTextCount = 0;
		if (strTempLogic.Find("and", 0) != -1)
		{
			nSearchResult = strTempLogic.Find("and", 0);
			nAndOrTextCount = 3; //<- and
		}
		else if (strTempLogic.Find("or", 0) != -1)
		{
			nSearchResult = strTempLogic.Find("or", 0);
			nAndOrTextCount = 2; //<- or
		}
		else  //<= Not Have Operation
			return;

		//## 전체 논리 조합에서 연산자위치 복귀
		nSearchResult = nSearchStart + nSearchResult;

		//## Operation Color Setting START ##
		GetWindowText(strLogic);
		if (nAndOrTextCount != 0) 
		{
			if ((m_nSelectEndPosition > -1) && (m_nSelectStartPosition > -1))
			{
				if ((m_nSelectEndPosition != m_nSelectStartPosition) 
					&& (m_bSelectState == TRUE))
					SetSel(m_nSelectStartPosition, m_nSelectEndPosition);
			}
			else
			{
				//## Operation Color Setting START ##
				if (m_bSelectState == FALSE)
				{
					//## Front Select Operation Position ##
					SetSel(m_noldStartOperation, m_noldEndOperation);
					Invalidate();
					SetSelectionCharFormat(GetFontFormat(clrOperation));

					//## Now Select Operation Position ##
					SetSel(nSearchResult, nSearchResult + nAndOrTextCount);
					Invalidate();

					SetSelectionCharFormat(GetFontFormat(clrOperationSelect));
					//## Operation Color Setting END ##

					//## Now Select Operation Position Save ##
					m_noldStartOperation = nSearchResult;
					m_noldEndOperation = nSearchResult + nAndOrTextCount;

					TRACE(_T("* Now Position START: %d, END: %d \n"), 
						nSearchResult, nSearchResult+nAndOrTextCount);

					SetCursor(m_hOperationSelectCursor);
				}

				//## Kill Focus ##
 				if (m_bHaveCaret == TRUE) 
 					SendMessage(WM_KILLFOCUS, 0, 0);

				TRACE("\n# Mouse Move KillFocus!");
			}
		}
		CRichEditCtrl::OnMouseMove(nFlags, point);
	}
}

LRESULT CLogicEditCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//TRACE("\n# Mouse Leave");

	if (m_bUseGroup == TRUE) //# <= 순차
		return 0;

	//## Operation Color restoration ##
	if ((m_noldStartOperation > -1) || (m_noldEndOperation > -1))
	{
		SetSel(m_noldStartOperation, m_noldEndOperation);
		Invalidate();
		SetSelectionCharFormat(GetFontFormat(clrOperation));

		m_noldStartOperation = -1;
		m_noldEndOperation = -1;
	}

	if ( ((m_nSelectEndPosition > -1) && (m_nSelectStartPosition > -1)) &&
		(m_nSelectEndPosition != m_nSelectStartPosition) )
	{
		SetSel(m_nSelectStartPosition, m_nSelectEndPosition);
		TRACE("\n## Mouse Leave => START:%d, END:%d \n", 
			m_nSelectStartPosition, m_nSelectEndPosition);
	}

	m_bMouseOver = FALSE;

	return 0;
}

LRESULT CLogicEditCtrl::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	/*
	TRACE(_T("############### Mouse Hover (X=%d, Y=%d) \n"), 
	LOWORD(lParam), HIWORD(lParam));

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER|TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	::_TrackMouseEvent(&tme);
	*/

	return 0;
}

BOOL CLogicEditCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//TRACE("\n# OnSetCursor!");

	// TODO: Add your message handler code here and/or call default
	/*
	if ((m_bHaveCaret == TRUE) && (m_bSelectDuring == FALSE) && (m_bSelectState == FALSE))
	{
	CWnd *pParentWnd = this;
	//## ConditionView ##
	pParentWnd = pParentWnd->GetParent();
	pParentWnd->SendMessage(WM_LOGIC_EDIT, (WPARAM)NM_EDIT_SETFOCUS, (LPARAM)0);
	}
	*/

	return CRichEditCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CLogicEditCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditCtrl::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here

	//## 절대 작업 하지 마시오! ##
	TRACE("\n# Set Focus!");
	ShowCaret();
	m_bHaveCaret = TRUE;

	ShowCursor(TRUE);
	//Invalidate();
}

void CLogicEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	// TODO: Add your message handler code here
	TRACE("\n# Kill Focus!");
	HideCaret();
	m_bHaveCaret = FALSE;
	
	CRichEditCtrl::OnKillFocus(pNewWnd);
}


void CLogicEditCtrl::DebugViewElement(OneElementInfo  &stElement)
{
	TRACE("\n* Element  : %s, View : %d", stElement.strElement, stElement.bView);
	TRACE("\n#  => Operater : %s", stElement.strOperation);
	TRACE("\n#  => Bracket  : %s", stElement.strBracket);
	TRACE("\n#  => Bracket Count : %ld", stElement.lBracketCount);

	TRACE("\n#  => Not : %s", stElement.strNot);
}

void CLogicEditCtrl::AddElementOnMinDownBtn(CString strElement, CString strElementAfter)
{
	OneElementInfo stOneElement;
	OneElementInfo stOneElementAfter;

	POSITION posOrigin = NULL;
	POSITION posChange = NULL;
	POSITION posCopy = NULL;
	POSITION pos = m_listOneElement.GetHeadPosition();
	while(pos && !(posOrigin && posChange))
	{	
		posCopy = pos;
		stOneElement = m_listOneElement.GetNext(pos);
		if(strElement.Compare(stOneElement.strElement) == 0)	//이슈[0000141,0000143] if(strElement.CompareNoCase(stOneElement.strElement) == 0)
			posOrigin = posCopy;
		else if(strElementAfter.Compare(stOneElement.strElement) == 0)	//이슈[0000141,0000143] else if(strElementAfter.CompareNoCase(stOneElement.strElement) == 0)
			posChange = posCopy;			
	}

	stOneElement	  = m_listOneElement.GetAt(posOrigin);
	stOneElementAfter = m_listOneElement.GetAt(posChange);

	m_listOneElement.AddTail(stOneElement);
	m_listOneElement.RemoveAt(posOrigin);	

	ReLoadAll();
}

void CLogicEditCtrl::InsertElementOnMaxUpBtn(CString strElement, CString strElementAfter)
{
	OneElementInfo stOneElement;
	OneElementInfo stOneElementAfter;

	POSITION posOrigin = NULL;
	POSITION posChange = NULL;
	POSITION posCopy = NULL;
	POSITION pos = m_listOneElement.GetHeadPosition();
	while(pos && !(posOrigin && posChange))
	{	
		posCopy = pos;
		stOneElement = m_listOneElement.GetNext(pos);
		if(strElement.Compare(stOneElement.strElement) == 0)		//이슈[0000141,0000143] if(strElement.CompareNoCase(stOneElement.strElement) == 0)
			posOrigin = posCopy;
		else if(strElementAfter.Compare(stOneElement.strElement) == 0)	//이슈[0000141,0000143] else if(strElementAfter.CompareNoCase(stOneElement.strElement) == 0)
			posChange = posCopy;			
	}

	stOneElement	  = m_listOneElement.GetAt(posOrigin);
	stOneElementAfter = m_listOneElement.GetAt(posChange);

	m_listOneElement.InsertBefore(posChange, stOneElement);
	m_listOneElement.RemoveAt(posOrigin);	

	ReLoadAll();
}

void CLogicEditCtrl::SwitchElement(CString strElement, CString strElementAfter)
{
	OneElementInfo stOneElement;
	OneElementInfo stOneElementAfter;

	POSITION posOrigin = NULL;
	POSITION posChange = NULL;
	POSITION posCopy = NULL;
	POSITION pos = m_listOneElement.GetHeadPosition();

	while(pos && !(posOrigin && posChange))
	{	
		posCopy = pos;
		stOneElement = m_listOneElement.GetNext(pos);
		if(strElement.Compare(stOneElement.strElement) == 0)	//이슈[0000141,0000143]if(strElement.CompareNoCase(stOneElement.strElement) == 0)
			posOrigin = posCopy;
		else if(strElementAfter.Compare(stOneElement.strElement) == 0)	//이슈[0000141,0000143]else if(strElementAfter.CompareNoCase(stOneElement.strElement) == 0)
			posChange = posCopy;			
	}

	stOneElement	  = m_listOneElement.GetAt(posOrigin);
	stOneElementAfter = m_listOneElement.GetAt(posChange);
		
	//>>0000398: 조건검색 화면의 로직창에서 괄호로 묶은 조건을 서로 위치 변경했을때 괄호가 깨지는 현상
	CString strBracket, strBracketAfter;
	strBracket = stOneElementAfter.strBracket;
	stOneElementAfter.strBracket = stOneElement.strBracket;
	stOneElement.strBracket = strBracket;
	//<<0000398

	m_listOneElement.SetAt(posChange, stOneElement);
	m_listOneElement.SetAt(posOrigin, stOneElementAfter);	

	ReLoadAll();
}

// 2009.11.13 -hidden-
GROUP_ELEMINFO* CLogicEditCtrl::FindGroupInfoWithGroupNumber(int nGroup)
{
	GROUP_ELEMINFO* pFindGroup = NULL;

	for(int i = 0; i < m_arrGroupElement.GetSize(); ++i)
	{
		pFindGroup = m_arrGroupElement.GetAt(i);
		if(pFindGroup->nGroup == nGroup)
			break;
	}

	return pFindGroup;
}

void CLogicEditCtrl::AddGroupElementOnMinDownBtn(CString strElement, CString strElementAfter)
{
	GroupElementInfo stGroupElementTemp;
	GroupElementInfo stGroupElement;
	GroupElementInfo stGroupElementAfter;

	GROUP_ELEMINFO* pGroupTemp;
	CString newText;
	int nElemSize = 0;
	int nSize = m_arrGroupElement.GetSize();

	int nEleInx, nEleInxAfter;
	int nIndex;
	for(nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(nIndex);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();

		int j;
		for(j = 0; j < nElemSize; ++j)
		{
			stGroupElementTemp = pGroup->arrGroupElemInfo.GetAt(j);

			stGroupElementTemp.stOneElementInfo.strElement.TrimLeft();
			stGroupElementTemp.stOneElementInfo.strElement.TrimRight();

			if (stGroupElementTemp.stOneElementInfo.strElement.Compare(strElement) == 0)
			{
				nEleInx = j;
				stGroupElement = stGroupElementTemp;
			}
			else if (stGroupElementTemp.stOneElementInfo.strElement.Compare(strElementAfter) == 0)
			{
				nEleInxAfter = j;
				stGroupElementAfter = stGroupElementTemp;
				pGroupTemp = pGroup;
			}
		}
	}

	if(!pGroupTemp)
		return;

	pGroupTemp->arrGroupElemInfo.RemoveAt(nEleInx);
	pGroupTemp->arrGroupElemInfo.Add(stGroupElement);
	

	ReLoadAll();
}

void CLogicEditCtrl::InsertGroupElementOnMaxUpBtn(CString strElement, CString strElementAfter)
{
	GroupElementInfo stGroupElementTemp;
	GroupElementInfo stGroupElement;
	GroupElementInfo stGroupElementAfter;

	GROUP_ELEMINFO* pGroupTemp;
	CString newText;
	int nElemSize = 0;
	int nSize = m_arrGroupElement.GetSize();

	int nEleInx, nEleInxAfter;
	int nIndex;
	for(nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(nIndex);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();

		int j;
		for(j = 0; j < nElemSize; ++j)
		{
			stGroupElementTemp = pGroup->arrGroupElemInfo.GetAt(j);

			stGroupElementTemp.stOneElementInfo.strElement.TrimLeft();
			stGroupElementTemp.stOneElementInfo.strElement.TrimRight();

			if (stGroupElementTemp.stOneElementInfo.strElement.Compare(strElement) == 0)
			{
				nEleInx = j;
				stGroupElement = stGroupElementTemp;
			}
			else if (stGroupElementTemp.stOneElementInfo.strElement.Compare(strElementAfter) == 0)
			{
				nEleInxAfter = j;
				stGroupElementAfter = stGroupElementTemp;
				pGroupTemp = pGroup;
			}
		}
	}

	if(!pGroupTemp)
		return;

	pGroupTemp->arrGroupElemInfo.RemoveAt(nEleInx);
	pGroupTemp->arrGroupElemInfo.InsertAt(nEleInxAfter, stGroupElement);
	

	ReLoadAll();
}

void CLogicEditCtrl::SwitchGroupElement(CString strElement, CString strElementAfter)
{	
	GroupElementInfo stGroupElementTemp;
	GroupElementInfo stGroupElement;
	GroupElementInfo stGroupElementAfter;

	GROUP_ELEMINFO* pGroupTemp;
	CString newText;
	int nElemSize = 0;
	int nSize = m_arrGroupElement.GetSize();
	
	int nEleInx, nEleInxAfter;
	int nIndex;
	for(nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_ELEMINFO* pGroup = m_arrGroupElement.GetAt(nIndex);
		nElemSize = pGroup->arrGroupElemInfo.GetSize();

		int j;
		for(j = 0; j < nElemSize; ++j)
		{
			stGroupElementTemp = pGroup->arrGroupElemInfo.GetAt(j);

			stGroupElementTemp.stOneElementInfo.strElement.TrimLeft();
			stGroupElementTemp.stOneElementInfo.strElement.TrimRight();

			if (stGroupElementTemp.stOneElementInfo.strElement.Compare(strElement) == 0)
			{
				nEleInx = j;
				stGroupElement = stGroupElementTemp;
			}
			else if (stGroupElementTemp.stOneElementInfo.strElement.Compare(strElementAfter) == 0)
			{
				nEleInxAfter = j;
				stGroupElementAfter = stGroupElementTemp;
				pGroupTemp = pGroup;
			}
		}
	}

	if(!pGroupTemp)
		return;
	
	pGroupTemp->arrGroupElemInfo.SetAt(nEleInxAfter, stGroupElement);
	pGroupTemp->arrGroupElemInfo.SetAt(nEleInx,		 stGroupElementAfter);

	ReLoadAll();
}


BOOL CLogicEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN)
	{
		if(VK_RETURN == pMsg->wParam)			
			return TRUE;

	}


	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

BOOL CLogicEditCtrl::CheckIsValidLogic(CString strLogic, int& nSel)
{
	int i, liFlag=0, liOpe = 1;

	if(strLogic[0] == '!')	return FALSE; 

	for(i=0; i<EXPLEN; i++)
	{
		nSel = i;
		if(liFlag < 0)	return FALSE;

		switch(strLogic[i])
		{
		case '(' :
			liFlag++;
			if(liOpe < 0)			return FALSE;
			liOpe = 1;
			break;
		case ')' :
			liFlag--;
			liOpe = -1;
			break;
		case '&' :
		case '|' :
		case '~' :
			liOpe = 1;
			break;

		case '!' :
			if(liFlag == 0) 	return TRUE;
			else 				return FALSE;
			break;

		default : 
			break;
		}

	} 

	return FALSE;
}

// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 >>
CString CLogicEditCtrl::GetElementOperation(CString strElement)
{
	POSITION pos  = NULL;
	OneElementInfo stOneElementInfo;
	for(int i = 0; i < m_listOneElement.GetCount(); i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElementInfo = m_listOneElement.GetAt(pos);

		if(strElement == stOneElementInfo.strElement)
			return stOneElementInfo.strOperation;
	}

	return "";
}
// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 <<

void CLogicEditCtrl::DeleteAllBracket()
{
	POSITION pos  = NULL;
	OneElementInfo stOneElementInfo;
	
	CList<OneElementInfo, OneElementInfo&> listOneElement;
	int i = 0;
	for( i = 0; i < m_listOneElement.GetCount(); i++)
	{
		pos = m_listOneElement.FindIndex(i);
		stOneElementInfo = m_listOneElement.GetAt(pos);
		stOneElementInfo.strBracket = "";
		stOneElementInfo.lBracketCount = 0;
		
		listOneElement.AddTail(stOneElementInfo);
	}

	m_listOneElement.RemoveAll();
	
	for(i = 0; i < listOneElement.GetCount(); i++)
	{
		pos = listOneElement.FindIndex(i);
		stOneElementInfo = listOneElement.GetAt(pos);
		
		m_listOneElement.AddTail(stOneElementInfo);
	}

	ReLoadAll();
}
