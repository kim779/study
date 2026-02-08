#include "stdafx.h"
#include "ExCommaEdit.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_LENGTH            100
#define INT_KEY_DELETE        46
#define INT_KEY_INSERT        45
#define MINUS_SIGN            '-'
#define PLUS_SIGN             '+'
#define DEFAULT_COMMA         ','
#define DEFAULT_DECIMAL_POINT '.'

#define RETRIEVE_BUFFER_SPACE 100


CCommaEdit::CCommaEdit()
{
	m_bCommas = TRUE;
	m_cComma = DEFAULT_COMMA;
	m_cDecimalPoint = DEFAULT_DECIMAL_POINT; 
	m_nDecimals = 0;
	m_dblCurrentValue = 0.0;
}

CCommaEdit::~CCommaEdit()
{
}


BEGIN_MESSAGE_MAP(CCommaEdit, CEdit)
	//{{AFX_MSG_MAP(CCommaEdit)
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

double CCommaEdit::GetValue(void)
{
	return m_dblCurrentValue;
}

void CCommaEdit::SetValue(double dblValue)
{
	m_dblCurrentValue = dblValue;
	CString strCurrent = _T("");
	toString(m_dblCurrentValue, strCurrent);
	setStringAndSelection(strCurrent, 0);
	SetSel(0, 0);
}

void CCommaEdit::setStringAndSelection(CString strNew, int nSelection)
{
	SetWindowText(strNew.GetBuffer(1));
	nSelection = nSelection < 0 ? 0 : nSelection;
	nSelection = nSelection > strNew.GetLength() ? strNew.GetLength() : nSelection;
	SetSel(nSelection, nSelection);
	CString strWork = strNew;
	CString strInteger, strDecimal;
	removeCommas(strWork);
	getIntegerAndDecimal(strWork, strInteger, strDecimal);
	strWork = strInteger + CString(".") + strDecimal;
	m_dblCurrentValue = atof(strWork.GetBuffer(1));
}

int CCommaEdit::GetDecimals(void)
{
	return m_nDecimals;
}

void CCommaEdit::SetDecimals(int nDecimals)
{
	m_nDecimals = nDecimals;
	SetValue(m_dblCurrentValue);
}

void CCommaEdit::EnableCommas(BOOL bNewHasCommas)
{
	m_bCommas = bNewHasCommas;
	SetValue(m_dblCurrentValue);
}

void CCommaEdit::SetComma(char cNewComma)
{
	m_cComma = cNewComma;
	SetValue(m_dblCurrentValue);
}

char CCommaEdit::GetComma(void)
{
	return m_cComma;
}

void CCommaEdit::SetDecimalPoint(char cNewDecimal)
{
	m_cDecimalPoint = cNewDecimal;
	SetValue(m_dblCurrentValue);
}

char CCommaEdit::GetDecimalPoint(void)
{
	return m_cDecimalPoint;
}


void CCommaEdit::toString(double dblValue, CString & strCurrent)
{
	CString strNumber;
	CString strInteger, strDecimal; 
	CString strWork;

	int nDecimalPosition, nSign;

	strNumber = CString(_ecvt(dblValue, MAX_LENGTH, &nDecimalPosition, &nSign));
	strInteger = strNumber.Left(nDecimalPosition);
	strDecimal = midString(strNumber, nDecimalPosition, m_nDecimals);
	if(m_nDecimals != 0)
		strWork = strInteger + CString(m_cDecimalPoint) + strDecimal;
	else
		strWork = strInteger;

  
	putCommas(strWork, strCurrent);

	if(nSign == 1)
		strCurrent = CString(MINUS_SIGN) + strCurrent;

	
}

void CCommaEdit::putCommas(CString strSource, CString & strDestination)
{
	CString strWork = strSource;
	removeCommas(strWork);
	strDestination = strWork;

	if(m_bCommas)
	{
		CString strInteger, strDecimal;
		getIntegerAndDecimal(strWork, strInteger, strDecimal);

		int nNumber = 0;
		CString strCommaInteger = CString("");
		for(int nCount = 0; nCount < strInteger.GetLength(); nCount ++)
		{
			strCommaInteger = midString(strInteger, strInteger.GetLength() - nCount - 1, 1) + strCommaInteger;

			nNumber ++;
			nNumber %= 3;
			if(nNumber == 0)
			{
				strCommaInteger = CString(m_cComma) + strCommaInteger;
			}
		}
    
		if(m_nDecimals == 0)
			strDestination = strCommaInteger;
		else
			strDestination = strCommaInteger + CString(m_cDecimalPoint) + strDecimal;

		removeFirstComma(strDestination);
	}
}

void CCommaEdit::removeCommas( CString & strSource)
{
	int nCount;
	CString strWork = strSource;
	CString strResult = CString("");

	for(nCount = 0; nCount < strSource.GetLength(); nCount ++)
	{
		if(strWork.GetAt(nCount) != m_cComma)
		{
			strResult += CString(strWork.GetAt(nCount));
		}
	} 

	strSource = strResult;
}


void CCommaEdit::deleteCharLeft(CString &strSource, int nSelection, int nNumber)
{
	const int nCurrentSelection  = nSelection - nNumber;
	deleteCharRight(strSource, nCurrentSelection, nNumber);
}

void CCommaEdit::deleteCharRight(CString &strSource, int nSelection, int nNumber)
{
	CString str1, str3;
	str1 = strSource.Left(nSelection);
	str3 = midString(strSource, nSelection + nNumber, strSource.GetLength());
	strSource = str1 + str3;
}


BOOL CCommaEdit::bIsLeftDecimal(CString strSource, int nSelection)
{
	BOOL bResult = TRUE;

	const int nPos = strSource.Find(m_cDecimalPoint);
	if (nPos != -1)
	{
		if(nSelection > nPos)
			bResult = FALSE;
	} 

	return bResult;
}


BOOL CCommaEdit::bIsLeftCharacterComma(CString strSource, int nSelection)
{
	BOOL bResult = FALSE;

	if(nSelection > 0)
	{
		if(strSource.GetAt(nSelection - 1) == m_cComma)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

BOOL CCommaEdit::bIsRightCharacterComma(CString strSource, int nSelection)
{
	BOOL bResult = FALSE;

	if(nSelection < strSource.GetLength())
	{
		if(strSource.GetAt(nSelection) == m_cComma)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}


BOOL CCommaEdit::bIsLeftCharacterDecimal(CString strSource, int nSelection)
{
	BOOL bResult = FALSE;

	if(nSelection > 0)
	{
		if(strSource.GetAt(nSelection - 1) == m_cDecimalPoint)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

BOOL CCommaEdit::bIsRightCharacterDecimal(CString strSource, int nSelection)
{
	BOOL bResult = FALSE;

	if(nSelection < strSource.GetLength())
	{
		if(strSource.GetAt(nSelection) == m_cDecimalPoint)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}


void CCommaEdit::getStringAndSelection(CString& strCurrent, int & nSelection)
{
	GetWindowText(strCurrent);

	int nStartSel, nEndSel;
	GetSel(nStartSel, nEndSel);

	nSelection = nStartSel;
}




void CCommaEdit::getIntegerAndDecimal(CString strCurrent, 
                                      CString & strInteger, 
                                      CString & strDecimal)
{
	CString strWork;
	strWork = strCurrent;

	removeCommas(strWork);

	int nPos = strWork.Find(CString(m_cDecimalPoint));
	if(nPos == -1)
	{
		strInteger = strWork;
		strDecimal = CString("");
	}
	else
	{
		strInteger = strWork.Left(nPos);
		strDecimal = midString(strWork, nPos + 1, strWork.GetLength());
	}
}

void CCommaEdit::saveStringAndSelection(void)
{
	CString strCurrent;
	int nStartSel, nEndSel;

	GetSel(nStartSel, nEndSel);
	GetWindowText(strCurrent);

	m_strSavedString = strCurrent;
	m_nSavedSelection = nStartSel;
}


void CCommaEdit::deleteSelection(void)
{
	
	int nSelStart, nSelEnd;
	GetSel(nSelStart, nSelEnd);

	
	if(nSelEnd > nSelStart)
	{
		ReplaceSel("");
	}
  
	CString strControl;
	GetWindowText(strControl);

	setStringAndSelection(strControl, nSelStart);
	SetValue(m_dblCurrentValue);

	SetSel(nSelStart, nSelStart);
}

void CCommaEdit::processCharacter( UINT cCurrent)
{
	if(cCurrent == MINUS_SIGN || cCurrent == PLUS_SIGN)
	{
		deleteSelection();
		processSign(cCurrent);
	}


	if(cCurrent >= '0' && cCurrent <= '9')
	{
		deleteSelection();
		processDigit(cCurrent);
	}


	if(cCurrent == 8)
	{
		if(hasSelection())
			deleteSelection();
		else
			processBackspace();
	}

  
	if(cCurrent == (UINT) m_cDecimalPoint)
	{
		deleteSelection();
		processDecimalPoint();
	}

}


void CCommaEdit::processDigit(UINT cCurrent)
{
	CString strCurrent;
	int nSelection;

	getStringAndSelection(strCurrent, nSelection);

	if(bIsLeftDecimal(strCurrent, nSelection))
	{ 
	int nDelta;
	CString strDestination;

	nDelta = strCurrent.GetLength() - nSelection;

	strCurrent = strCurrent.Left(nSelection) + 
		CString((char) cCurrent) + 
		midString(strCurrent, nSelection, strCurrent.GetLength());

	putCommas(strCurrent, strDestination);
	strCurrent = strDestination;
	nSelection = strCurrent.GetLength() - nDelta;

	}
  
	else
	{ 
		if(nSelection < strCurrent.GetLength())
		{
			strCurrent.SetAt(nSelection, (char) cCurrent);
			nSelection++;

		} 
	}

  
	if(bIsRightCharacterComma(strCurrent, nSelection))
	{
		nSelection ++;
	}

  
	setStringAndSelection(strCurrent, nSelection);
}

void CCommaEdit::processDelete()
{
	CString strCurrent;
	int nSelection;

	getStringAndSelection(strCurrent, nSelection);

	if(bIsRightCharacterDecimal(strCurrent, nSelection))
		nSelection ++;

	if(bIsLeftDecimal(strCurrent, nSelection))
	{   
		int nDelta = strCurrent.GetLength() - nSelection;
    
		if(bIsRightCharacterComma(strCurrent, nSelection))
		{
			deleteCharRight(strCurrent, nSelection, 2);
			nDelta -= 2;
		}
		else
		{
			deleteCharRight(strCurrent, nSelection, 1);
			nDelta -= 1;
		}
    
		CString strDestination;
		putCommas(strCurrent, strDestination);
		strCurrent = strDestination;
		nSelection = strCurrent.GetLength() - nDelta;
	}
	else 
	{
		if(nSelection < strCurrent.GetLength())
		{
			strCurrent = strCurrent.Left(nSelection) + 
				midString(strCurrent, nSelection + 1, strCurrent.GetLength()) +
				CString("0");
		} 
	}

	setStringAndSelection(strCurrent, nSelection);
}

void CCommaEdit::processBackspace()
{
	CString strCurrent;
	int nSelection;

	getStringAndSelection(strCurrent, nSelection);

	if(bIsLeftCharacterDecimal(strCurrent, nSelection))
	nSelection --;

	if(bIsLeftDecimal(strCurrent, nSelection))
	{ 
		const int nDelta = strCurrent.GetLength() - nSelection;
    
		if(bIsLeftCharacterComma(strCurrent, nSelection))
		{
			deleteCharLeft(strCurrent, nSelection, 2);
		}
		else
		{
			deleteCharLeft(strCurrent, nSelection, 1);
		}
    
		CString strDestination;
		putCommas(strCurrent, strDestination);
		strCurrent = strDestination;
		nSelection = strCurrent.GetLength() - nDelta;
	}
	else 
	{
		strCurrent = strCurrent.Left(nSelection-1) + 
			midString(strCurrent, nSelection, strCurrent.GetLength()) +
			CString("0");

		nSelection--;
	}

	setStringAndSelection(strCurrent, nSelection);
}

void CCommaEdit::processDecimalPoint()
{
	CString strCurrent;
	int nSelection;

	getStringAndSelection(strCurrent, nSelection);

	int nPos = strCurrent.Find(CString(m_cDecimalPoint));
	nPos = nPos == -1 ? strCurrent.GetLength() : nPos;

	if(bIsLeftDecimal(strCurrent, nSelection))
	{
		strCurrent = strCurrent.Left(nSelection) + midString(strCurrent, nPos, strCurrent.GetLength());
	}

	CString strDestination;
	putCommas(strCurrent, strDestination);
	strCurrent = strDestination;

	nPos = strCurrent.Find(CString(m_cDecimalPoint));
	nPos = nPos == -1 ? strCurrent.GetLength() : nPos;
	nSelection = nPos + 1;
	nSelection = 
		nSelection > strCurrent.GetLength() ? strCurrent.GetLength() : nSelection;

	setStringAndSelection(strCurrent, nSelection);
}

void CCommaEdit::processSign(UINT nCharacter)
{
  
	CString strCurrent;
	int nSelection;

	getStringAndSelection(strCurrent, nSelection);

	if(nCharacter == MINUS_SIGN)
	{

		if(bIsRightCharacterSign(strCurrent, 0))
		{
			strCurrent = strCurrent.Right(strCurrent.GetLength() - 1);
			nSelection --;
		}
		else
		{
			strCurrent = CString(MINUS_SIGN) + strCurrent;
			nSelection ++;
		}
	}

	if(nCharacter == PLUS_SIGN)
	{  

		if(bIsRightCharacterSign(strCurrent, 0))
		{
			strCurrent = strCurrent.Right(strCurrent.GetLength() - 1);
			nSelection --;
		}
	}

	setStringAndSelection(strCurrent, nSelection);
}

void CCommaEdit::OnChar(UINT nChar, UINT, UINT) 
{
	processCharacter(nChar);
}

BOOL CCommaEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == INT_KEY_DELETE)
	{
		if(hasSelection())
			deleteSelection();
		else
			processDelete();

		return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == INT_KEY_INSERT)
	{
		return TRUE;
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}


void CCommaEdit::removeFirstComma(CString & strDestination)
{
	BOOL bSign = FALSE;
	CString strWork = strDestination;

	if(strWork.GetLength() > 0)
	{
		if(strWork.GetAt(0) == MINUS_SIGN)
		{
			bSign = TRUE;
			strWork = strWork.Right(strWork.GetLength() - 1);
		}
	}

	if(strWork.Left(1) == CString(m_cComma))
		strWork = strWork.Right(strWork.GetLength() - 1);

	if(bSign)
		strWork = CString(MINUS_SIGN) + strWork;

	strDestination = strWork;
}


BOOL CCommaEdit::bIsRightCharacterSign(CString strCurrent, int nSelection)
{
	BOOL bResult = FALSE; 

	if(nSelection < strCurrent.GetLength() && nSelection >= 0)
	{
		if(strCurrent.GetAt(nSelection) == MINUS_SIGN)
			bResult = TRUE;
	}

	return bResult;
}

void CCommaEdit::OnRButtonDown(UINT, CPoint) 
{
  
}

CString CCommaEdit::midString(CString strIn, int nStart, int nLength)
{
  
	CString strResult = CString("");

	if(nStart >= 0 && nStart < strIn.GetLength())
	{
		strResult = strIn.Mid(nStart, nLength);
	}

	return strResult;
}


BOOL CCommaEdit::hasSelection(void)
{
	BOOL bResult = FALSE;


	int nSelStart, nSelEnd;
	GetSel(nSelStart, nSelEnd);


	if(nSelEnd > nSelStart)
		bResult = TRUE;


	return bResult;
}

void CCommaEdit::getLocaleInformation(char & cDecimal, char & cComma)
{
	struct lconv * pLocalStructure;
	pLocalStructure = localeconv();

	cDecimal = * (pLocalStructure->decimal_point);
	cComma = *( pLocalStructure->currency_symbol);
}

void CCommaEdit::LocalSettings(void)
{
	char cDecimal[RETRIEVE_BUFFER_SPACE];
	char cComma[RETRIEVE_BUFFER_SPACE];

	::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SDECIMAL, cDecimal, RETRIEVE_BUFFER_SPACE);
	::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_STHOUSAND, cComma, RETRIEVE_BUFFER_SPACE);

	m_cDecimalPoint = *cDecimal;


	if(*cDecimal != *cComma)
		m_cComma = *cComma;
}

