// LogicEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LogicEditCtrl.h"
#include "../../Common_ST/STControllerDef.h"
#include "../StrategyConfigureWizardDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogicEditCtrl

CLogicEditCtrl::CLogicEditCtrl()
{
	m_chLastElement = NULL;
	m_nBeginSelected = -1;
	m_nEndSelected = -1;
	//Font Create
	LOGFONT	  LogFont;	
	LogFont.lfHeight	=	12;    
	LogFont.lfWidth	=	0;    
	LogFont.lfEscapement=	0;    
	LogFont.lfOrientation=	0; 
	LogFont.lfWeight	=	400;
	LogFont.lfItalic	=	0;    
	LogFont.lfUnderline	=	0;        
	LogFont.lfStrikeOut	=	0;     
	LogFont.lfCharSet	=	129;        
	LogFont.lfOutPrecision	=	3;        
	LogFont.lfClipPrecision	=	2;     
	LogFont.lfQuality	=	1;        
	LogFont.lfPitchAndFamily=	49;        
	strcpy( LogFont.lfFaceName, (const char*)_T("굴림체") );
	m_fontDefault.CreateFontIndirect( &LogFont );
}

CLogicEditCtrl::~CLogicEditCtrl()
{
	m_fontDefault.DeleteObject();
}


BEGIN_MESSAGE_MAP(CLogicEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CLogicEditCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogicEditCtrl message handlers

void CLogicEditCtrl::AddElement(char chElement)
{
	if(chElement<=m_chLastElement)
	{
		return;
	}
	m_chLastElement = chElement;
	int nSize = m_arrayElement.GetSize();
	if(nSize)
	{
		CElementInLogic Operation;
		Operation.m_nType = OPERATIONINLOGIC;
		Operation.m_strValue = STRANDINLOGIC;
		m_arrayElement.SetAtGrow(nSize,Operation);
		nSize++;
	}
	CElementInLogic Element;
	Element.m_nType = ELEMENTINLOGIC;
	Element.m_strValue = chElement;
	m_arrayElement.SetAtGrow(nSize,Element);
	UpdateLogic();
}

void CLogicEditCtrl::DeleteElement(char chElement)
{

	// find Parenthsis to be removed..
	CUIntArray	arrayStackParenthsis;
	BOOL	bAppendMode = FALSE;
	UINT	nCntRemainedToAppendParenthsis = 0;
	CElementInLogic Element;
	int nSize = m_arrayElement.GetSize();
	int nPos = 0;
	for( nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		if(Element.m_nType==ELEMENTINLOGIC&&Element.m_strValue==chElement)
		{
			bAppendMode = TRUE;
			nCntRemainedToAppendParenthsis = arrayStackParenthsis.GetSize();
		}
		else if(Element.m_nType==PARENTHESISINLOGIC)
		{
			if(Element.m_strValue==STROPENPARENTHESISINLOGIC&&!bAppendMode)
			{
				arrayStackParenthsis.Add(nPos);
			}
			else if(Element.m_strValue==STRCLOSEPARENTHESISINLOGIC)
			{	
				if(!bAppendMode)
				{
					arrayStackParenthsis.RemoveAt(arrayStackParenthsis.GetSize()-1);
				}
				else
				{
					if(nCntRemainedToAppendParenthsis)
					{
						arrayStackParenthsis.Add(nPos);
						nCntRemainedToAppendParenthsis--;
					}					
				}				
			}
		}
	}

	// find Parenthsis fined..
	int nSizeOfStackParenthsis = arrayStackParenthsis.GetSize();
	for(nPos=nSizeOfStackParenthsis-1;nPos>=0;nPos--)
	{
		m_arrayElement.RemoveAt(arrayStackParenthsis.GetAt(nPos));
	}


	// Delete element and operation related
	for(nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		if(Element.m_nType==ELEMENTINLOGIC&&Element.m_strValue==chElement)
		{
			m_arrayElement.RemoveAt(nPos);
			if(nSize!=1)
			{
				if(nPos==0)
				{
					m_arrayElement.RemoveAt(nPos);
				}
				else
				{
					m_arrayElement.RemoveAt(nPos-1);
				}				
			}
			UpdateLogic();
			break;
		}
	}
}

void CLogicEditCtrl::UpdateLogic()
{
	if(!GetSafeHwnd())	return;
	// reorder element
	SetFont(&m_fontDefault);
	long nCntStackedOfParenthesis = 0;
	long nCntStackedOfParenthesisAtSelectionStart = -1;
	int nSize = m_arrayElement.GetSize();
	long lOffset = 0;
	long lLength = 0;
	CString strElement;
	CString strText;
	CElementInLogic Element;
	int nPos = 0;
	// make logic sentence and calculate position...
	for( nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);

		if(!Element.m_bTRUE)
		{
			strElement = STRNOTINLOGIC + Element.m_strValue;
		}
		else
		{
			strElement = Element.m_strValue;
		}

		lLength = strElement.GetLength();

		if(nPos)	
		{
			strText += " ";
			lOffset += 1;
		}

		Element.m_nStartChar = lOffset;
		Element.m_nEndChar = lOffset + lLength;
		m_arrayElement.SetAt(nPos,Element);

		strText += strElement;
		lOffset += lLength;		
	}
	SetWindowText(strText);


	// Set Color under sentence.. and Make Selection info
	long nPosStart = -1;
	long nPosEnd = -1;
	long nStartChar = -1;
	long nEndChar = -1;
	CHARFORMAT cf;
	memset(&cf,0x00,sizeof(cf));
	for(nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		
		if(Element.m_strValue==STROPENPARENTHESISINLOGIC)
		{
			nCntStackedOfParenthesis++;
		}
		else if(Element.m_strValue==STRCLOSEPARENTHESISINLOGIC)
		{
			nCntStackedOfParenthesis--;
		}
		
		if(Element.m_nType==ELEMENTINLOGIC)
		{
			SetSel(Element.m_nStartChar,Element.m_nEndChar);
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = RGB(0,0,255);
			SetSelectionCharFormat(cf);
		}
		if(m_nBeginSelected<=nPos&&m_nEndSelected>=nPos)
		{
			if(nStartChar==-1)
			{
				nStartChar = Element.m_nStartChar;
				nPosStart = nPos;
				nCntStackedOfParenthesisAtSelectionStart = nCntStackedOfParenthesis;
			}			
			nEndChar = Element.m_nEndChar;
			nPosEnd = nPos;
		}
	}


	// check selection and send message
	if(m_nBeginSelected == -1||m_nEndSelected == -1)
	{
		SetSel(0,0);
	}
	else
	{
		SetSel(nStartChar,nEndChar);
		if(nPosStart==nPosEnd)		// one element..
		{
			Element = m_arrayElement.GetAt(nPosStart);
			if(!Element.m_bTRUE)
			{
				strText = STRNOTINLOGIC + Element.m_strValue;
			}
			else
			{
				strText = Element.m_strValue;
			}			
			GetParent()->SendMessage(UWM_SETSEL,(WPARAM)(LPCTSTR)strText,nCntStackedOfParenthesisAtSelectionStart>0?0:NEXTISPOSSIBLE);
		}
		else
		{
			CElementInLogic ElementSt;
			CElementInLogic ElementEn;

			ElementSt = m_arrayElement.GetAt(nPosStart);
			ElementEn = m_arrayElement.GetAt(nPosEnd);
			if(ElementSt.m_nType==PARENTHESISINLOGIC&&ElementSt.m_strValue==STROPENPARENTHESISINLOGIC
				&&ElementEn.m_nType==PARENTHESISINLOGIC&&ElementEn.m_strValue==STRCLOSEPARENTHESISINLOGIC
				&&ElementSt.m_nStartChar>=nStartChar&&ElementEn.m_nEndChar<=nEndChar)
			{
				GetParent()->SendMessage(UWM_SETSEL,(WPARAM)(LPCTSTR)"",MULTISELECTED|INCLUDEPARENTHESIS);// With including parenthesis
			}
			else
			{
				GetParent()->SendMessage(UWM_SETSEL,(WPARAM)(LPCTSTR)"",MULTISELECTED);// With no including parenthesis
			}
		}
		
	}
	
}

void CLogicEditCtrl::ChangeOperation(LPCTSTR lpOperation)
{
	long nStartChar = 0;
	long nEndChar = 0;
	GetSel(nStartChar,nEndChar);
	CElementInLogic Element;
	int nSize = m_arrayElement.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		if(Element.m_nStartChar==nStartChar&&Element.m_nEndChar==nEndChar&&Element.m_nType==OPERATIONINLOGIC)
		{
			Element.m_strValue = lpOperation;
			m_arrayElement.SetAt(nPos,Element);
			UpdateLogic();
			break;
		}
	}	
}

void CLogicEditCtrl::ChangeElement(BOOL bTRUE)
{
	long nStartChar = 0;
	long nEndChar = 0;
	GetSel(nStartChar,nEndChar);
	CElementInLogic Element;
	int nSize = m_arrayElement.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		if(Element.m_nStartChar==nStartChar&&Element.m_nEndChar==nEndChar&&Element.m_nType==ELEMENTINLOGIC)
		{
			Element.m_bTRUE = bTRUE;
			m_arrayElement.SetAt(nPos,Element);
			UpdateLogic();
			break;
		}
	}	

}

void CLogicEditCtrl::Unparenthesize()
{
	long nStartChar = 0;
	long nEndChar = 0;
	GetSel(nStartChar,nEndChar);
	CElementInLogic Element;
	int nSize = m_arrayElement.GetSize();
	int nBeginPosParenthesize = -1;
	int nEndPosParenthesize = -1;
	int nCntStackedOfParenthesis = 0;
	for(int nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		if(nBeginPosParenthesize==-1&&Element.m_nStartChar>=nStartChar&&Element.m_nType==PARENTHESISINLOGIC)
		{
			if(Element.m_strValue==STROPENPARENTHESISINLOGIC)
			{
				nBeginPosParenthesize = nPos;
			}			
		}
		else if(nBeginPosParenthesize!=-1&&Element.m_nEndChar<=nEndChar&&Element.m_nType==PARENTHESISINLOGIC)
		{
			if(Element.m_strValue==STROPENPARENTHESISINLOGIC)
			{
				nCntStackedOfParenthesis++;
			}				
			else if(Element.m_strValue==STRCLOSEPARENTHESISINLOGIC&&nCntStackedOfParenthesis)
			{
				nCntStackedOfParenthesis--;
			}
			else if(Element.m_strValue==STRCLOSEPARENTHESISINLOGIC&&!nCntStackedOfParenthesis)
			{
				nEndPosParenthesize = nPos;
			}
		}
	}
	if(nBeginPosParenthesize!=-1&&nEndPosParenthesize!=-1)
	{
		m_arrayElement.RemoveAt(nEndPosParenthesize);
		m_arrayElement.RemoveAt(nBeginPosParenthesize);

		m_nBeginSelected = nBeginPosParenthesize;
		m_nEndSelected = nEndPosParenthesize - 2;

		UpdateLogic();
	}
}

void CLogicEditCtrl::Parenthesize()
{
	long nStartCharSelected = 0;
	long nEndCharSelected = 0;
	GetSel(nStartCharSelected,nEndCharSelected);
	CElementInLogic Element;
	int nSize = m_arrayElement.GetSize();
	int nBeginPosParenthesize = -1;
	int nEndPosParenthesize = -1;
	long lCntOfStackInParenthsis = 0;
	for(int nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);
		if(nBeginPosParenthesize==-1&&Element.m_nStartChar>=nStartCharSelected&&Element.m_nType==ELEMENTINLOGIC)
		{
			nBeginPosParenthesize = nPos;
		}
		else if(nBeginPosParenthesize!=-1&&Element.m_nEndChar<=nEndCharSelected&&Element.m_nType==ELEMENTINLOGIC)
		{
			nEndPosParenthesize = nPos;
		}
		else if(Element.m_nStartChar>=nStartCharSelected&&Element.m_nEndChar<=nEndCharSelected&&Element.m_nType==PARENTHESISINLOGIC)
		{
			if(Element.m_strValue==STROPENPARENTHESISINLOGIC)
			{
				lCntOfStackInParenthsis += 1;
				if(nBeginPosParenthesize==-1&&lCntOfStackInParenthsis==1)
				{
					nBeginPosParenthesize = nPos;
				}
			}
			else if(Element.m_strValue==STRCLOSEPARENTHESISINLOGIC)
			{
				lCntOfStackInParenthsis -= 1;
				if(nBeginPosParenthesize!=-1&&lCntOfStackInParenthsis==0)
				{
					nEndPosParenthesize = nPos;
				}
			}
			if(lCntOfStackInParenthsis<0) return;
		}
	}
	if(nBeginPosParenthesize!=-1&&nEndPosParenthesize!=-1&&lCntOfStackInParenthsis==0)
	{
		CElementInLogic ElementParenthsis;
		ElementParenthsis.m_nType = PARENTHESISINLOGIC;
		ElementParenthsis.m_strValue = STRCLOSEPARENTHESISINLOGIC;
		m_arrayElement.InsertAt(nEndPosParenthesize+1,ElementParenthsis);
		ElementParenthsis.m_nType = PARENTHESISINLOGIC;
		ElementParenthsis.m_strValue = STROPENPARENTHESISINLOGIC;
		m_arrayElement.InsertAt(nBeginPosParenthesize,ElementParenthsis);

		m_nBeginSelected = nBeginPosParenthesize;
		m_nEndSelected = nEndPosParenthesize + 2;
		UpdateLogic();
	}
}

BOOL CLogicEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	HideCaret();		
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CLogicEditCtrl::DeleteAllElement()
{
	m_chLastElement = NULL;
	m_arrayElement.RemoveAll();
	m_nBeginSelected = -1;
	m_nEndSelected = -1;
	UpdateLogic();
}

void CLogicEditCtrl::SwapElement(char chElement1, char chElement2)
{
	CElementInLogic Element1;
	CElementInLogic Element2;
	CElementInLogic ElementTemp;
	int nPos1 = -1;
	int nPos2 = -1;
	int nSize = m_arrayElement.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		ElementTemp = m_arrayElement.GetAt(nPos);
		if(ElementTemp.m_nType==ELEMENTINLOGIC)
		{
			if(ElementTemp.m_strValue==chElement1)
			{
				Element1 = ElementTemp;
				nPos1 = nPos;
			}
			else if(ElementTemp.m_strValue==chElement2)
			{
				Element2 = ElementTemp;
				nPos2 = nPos;
			}
		}
	}
	if(nPos1!=-1&&nPos2!=-1)
	{
		m_arrayElement.SetAt(nPos2,Element1);
		m_arrayElement.SetAt(nPos1,Element2);
		UpdateLogic();
	}
}

void CLogicEditCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CString strText;
	long nCntStackedOfParenthesisInSelection = 0;
	long nCntNextInSelection = 0;
	long nStartChar = 0;
	long nEndChar = 0;
	int nBeginSelected = -1;
	int nEndSelected = -1;
	int nTypeSelection = 0; // 1: element, 2:parenthesis
	GetSel(nStartChar,nEndChar);
	CElementInLogic Element;
	CElementInLogic ElementSt;
	CElementInLogic ElementEn;
	CElementInLogic ElementStBefore;
	CElementInLogic ElementEnAfter;
	int nSize = m_arrayElement.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);

		if(Element.m_nStartChar>=nStartChar&&Element.m_nEndChar<=nEndChar)
		{
			if(Element.m_strValue==STROPENPARENTHESISINLOGIC)
			{
				nCntStackedOfParenthesisInSelection++;
			}
			else if(Element.m_strValue==STRCLOSEPARENTHESISINLOGIC)
			{
				nCntStackedOfParenthesisInSelection--;
				if(nCntStackedOfParenthesisInSelection<0)
				{
					nCntStackedOfParenthesisInSelection = 1000;
				}
			}
			else if(Element.m_strValue==STRNEXTINLOGIC)
			{
				nCntNextInSelection++;
			}
		}

		// one Element & operation
		if(nStartChar==nEndChar&&Element.m_nStartChar<=nStartChar&&Element.m_nEndChar>=nEndChar&&Element.m_nType!=PARENTHESISINLOGIC)
		{
			nBeginSelected = nPos;
			nEndSelected = nPos;
		}
		// one or more elements or parenthesis
		else if(nStartChar!=nEndChar&&Element.m_nStartChar>=nStartChar&&Element.m_nEndChar<=nEndChar&&Element.m_nType!=OPERATIONINLOGIC)
		{
			if(nCntNextInSelection>0)
			{
				nEndSelected = -1;
				CString strMessage;
				strMessage.Format("( )안에 %s을 사용할수 없습니다.",STRNEXTINLOGIC);
				MessageBox(strMessage);
				break;
			}
			if(nBeginSelected==-1)
			{
				nTypeSelection = Element.m_nType;
				nBeginSelected = nPos;
			}
			if(!nCntStackedOfParenthesisInSelection)
			{
				nEndSelected = nPos;
			}
		}
	}
	if(nEndSelected==-1)
	{
		m_nBeginSelected = -1;
	}
	else
	{
		m_nBeginSelected = nBeginSelected;
		m_nEndSelected = nEndSelected;	
	}		
	UpdateLogic();
}

void CLogicEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	long nStartCharSelected = 0;
	long nEndCharSelected = 0;
	GetSel(nStartCharSelected,nEndCharSelected);
	SetSel(nStartCharSelected,nStartCharSelected);
	CRichEditCtrl::OnLButtonDown(nFlags, point);
}

CString CLogicEditCtrl::GetLogic()
{
	CString strLogic;
	CString strAllLogic;
	CElementInLogic Element;
	int nValue;
	int nSize = m_arrayElement.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		Element = m_arrayElement.GetAt(nPos);

		switch(Element.m_nType)
		{
		case OPERATIONINLOGIC:
			if(Element.m_strValue==STRORINLOGIC)
			{
				nValue = ORINLOGIC;
			}
			else if(Element.m_strValue==STRNEXTINLOGIC)
			{
				nValue = NEXTINLOGIC;
			}
			else //STRANDINLOGIC
			{
				nValue = ANDINLOGIC;
			}
			break;
		case PARENTHESISINLOGIC:
			if(Element.m_strValue==STROPENPARENTHESISINLOGIC)
			{
				nValue = OPENPARENTHESISINLOGIC;
			}
			else	// STRCLOSEPARENTHESISINLOGIC
			{
				nValue = CLOSEPARENTHESISINLOGIC;
			}
			break;
		default: //ELEMENTINLOGIC
			nValue = Element.m_bTRUE;
			break;
		}
		strLogic.Format("%d%d",Element.m_nType,nValue);		

		strAllLogic += strLogic;
	}	
	return strAllLogic;
}

void CLogicEditCtrl::SetLogic(LPCTSTR lpAllLogic)
{
	CString strAllLogic = lpAllLogic;
	char szType[2];
	char szValue[2];
	int nType = 0;
	int nValue = 0;
	int nOffsetOfAllLogic = 0;
	int nLengthOfAlllogic = strAllLogic.GetLength();	
	int nPosInArray = 0;
	int nSize = m_arrayElement.GetSize();
	CElementInLogic Element;
	memset(szType,0x00,sizeof(szType));
	memset(szValue,0x00,sizeof(szValue));
	while(1)
	{
		if(nOffsetOfAllLogic>=nLengthOfAlllogic)	break;
		szType[0] = strAllLogic[nOffsetOfAllLogic];
		nType = atol(szType);
		nOffsetOfAllLogic++;

		if(nOffsetOfAllLogic>=nLengthOfAlllogic)	break;
		szValue[0] = strAllLogic[nOffsetOfAllLogic];
		nValue = atol(szValue);
		nOffsetOfAllLogic++;

		switch(nType)
		{
		case OPERATIONINLOGIC:
			Element = m_arrayElement.GetAt(nPosInArray);
			if(Element.m_nType!=OPERATIONINLOGIC)	break;
			switch(nValue)
			{
			case ORINLOGIC:
				Element.m_strValue = STRORINLOGIC;
				break;
			case NEXTINLOGIC:
				Element.m_strValue = STRNEXTINLOGIC;
				break;
			default:	// ANDINLOGIC
				Element.m_strValue = STRANDINLOGIC;
				break;
			}
			m_arrayElement.SetAt(nPosInArray,Element);			
			nPosInArray++;
			break;
		case PARENTHESISINLOGIC:
			Element.Init();
			Element.m_nType = PARENTHESISINLOGIC;
			if(nValue==OPENPARENTHESISINLOGIC)
			{
				Element.m_strValue=STROPENPARENTHESISINLOGIC;
			}
			else
			{
				Element.m_strValue=STRCLOSEPARENTHESISINLOGIC;
			}
			m_arrayElement.InsertAt(nPosInArray,Element);
			nPosInArray++;
			break;
		default:	//ELEMENTINLOGIC
			Element = m_arrayElement.GetAt(nPosInArray);
			if(Element.m_nType!=ELEMENTINLOGIC)	break;
			Element.m_bTRUE = nValue;			
			m_arrayElement.SetAt(nPosInArray,Element);	
			nPosInArray++;
			break;
		}		
	}	
	UpdateLogic();
}
