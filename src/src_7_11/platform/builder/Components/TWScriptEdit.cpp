/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Juraj Rojko jrojko@twist.cz
// All rights reserved
//
// TWScriptEdit.cpp : implementation file
//
#include "stdafx.h"
#include "TWScriptEdit.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IS_NOSYMBOL	9999

#define	pOFFSET		(pPtr - pBuffer)

/////////////////////////////////////////////////////////////////////////////
// CTWScriptEdit

CTWScriptEdit::CTWScriptEdit()
{
	m_nEndLineLen    = 0;
	m_chComment      = 1;
	m_bCaseSensitive = FALSE;
	m_bChangeCase    = TRUE;
	m_bShowList      = false;
	m_bUndoLock      = false;

	SetStringQuotes(_T("\""));

	SetKeywordColor(RGB(0, 0, 255), FALSE);
	SetConstantColor(RGB(0, 0, 0), TRUE);
	SetCommentColor(RGB(0, 128, 0), FALSE);
	SetNumberColor(RGB(0, 0, 0), FALSE);
	SetStringColor(RGB(0, 0, 0), FALSE);

	m_bInForcedChange = FALSE;
	m_changeType = ctUndo;
	m_crOldSel.cpMin = m_crOldSel.cpMax = 0;

	m_bActivate = false;			// for cursor position by mouse click
	m_pAutoDlg  = NULL;
	m_pAutoTip  = NULL;

	m_sControlName.Empty();
}

CTWScriptEdit::~CTWScriptEdit()
{
	if (m_pAutoDlg)
	{
		m_pAutoDlg->DestroyWindow();
		delete m_pAutoDlg;
	}
	if (m_pAutoTip)
	{
		m_pAutoTip->DestroyWindow();
		delete m_pAutoTip;
	}
}

void CTWScriptEdit::Initialize() 
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = MAX_TAB_STOPS;
	for( int itab = 0 ; itab < pf.cTabCount  ; itab++ )
		pf.rgxTabs[itab] = (itab + 1) * 1540/5;

	SetParaFormat( pf );

	CHARFORMAT cfDefault;
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED; 
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE /*| CFM_CHARSET*/ | CFM_PROTECTED;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = 0xEE; 
	strcpy_s(cfDefault.szFaceName, _T("")); 

	SetDefaultCharFormat(cfDefault);
	
	SetEventMask(ENM_CHANGE | ENM_SELCHANGE | ENM_PROTECTED | ENM_SCROLL | ENM_SCROLLEVENTS);

	m_pAutoDlg = new CAutoCmplDlg(this);
	m_pAutoDlg->Create(IDD_AUTOCOMPLETE, this);
	m_pAutoDlg->ModifyStyleEx(NULL, WS_EX_TOOLWINDOW, 0);
	m_pAutoDlg->SetOwner(this);
	m_pAutoDlg->ShowWindow(SW_HIDE);

	m_pAutoTip = new CAutoToolTip(this);
	m_pAutoTip->Create(IDD_AUTOTIP, this);
	m_pAutoTip->ModifyStyleEx(NULL, WS_EX_TOOLWINDOW, 0);
	m_pAutoTip->SetOwner(this);
	m_pAutoTip->ShowWindow(SW_HIDE);
	SetFocus();
}

void CTWScriptEdit::SetSLComment(TCHAR chComment, TCHAR chComment2)
{
	m_chComment = chComment;
	m_chComment2 = chComment2;
}

void CTWScriptEdit::SetSLComment(LPCTSTR lpszComment)
{
	m_strComment = lpszComment;
}

void CTWScriptEdit::AddKeywords(LPCTSTR lpszKwd)
{
	m_strKeywords = m_strKeywords + lpszKwd;
	m_strKeywordsLower = m_strKeywords;
	if (!m_bCaseSensitive)
		m_strKeywordsLower.MakeLower();
}

void CTWScriptEdit::ClearKeywords()
{
	m_strKeywords.Empty();
	m_strKeywordsLower.Empty();
}							  

void CTWScriptEdit::AddConstants(LPCTSTR lpszConst)
{
	m_strConstants = m_strConstants + lpszConst;
	m_strConstantsLower = m_strConstants;
	if (!m_bCaseSensitive)
		m_strConstantsLower.MakeLower();
}

void CTWScriptEdit::ClearConstants()
{
	m_strConstants.Empty();
	m_strConstantsLower.Empty();
}							  

void CTWScriptEdit::SetCaseSensitive(BOOL bSensitive)
{
	m_bCaseSensitive = bSensitive;
}

void CTWScriptEdit::SetChangeCase(BOOL bChange)
{
	m_bChangeCase = bChange;
}

void CTWScriptEdit::SetStringQuotes(LPCTSTR lpszStrQ)
{
	m_strStringQuotes = lpszStrQ;
}

void CTWScriptEdit::SetKeywordColor(COLORREF clr, BOOL bBold)
{
	m_icKeyword.clrColor = clr;
	m_icKeyword.bBold = bBold;
}

void CTWScriptEdit::SetConstantColor(COLORREF clr, BOOL bBold)
{
	m_icConstant.clrColor = clr;
	m_icConstant.bBold = bBold;
}

void CTWScriptEdit::SetCommentColor(COLORREF clr, BOOL bBold)
{
	m_icComment.clrColor = clr;
	m_icComment.bBold = bBold;
}

void CTWScriptEdit::SetNumberColor(COLORREF clr, BOOL bBold)
{
	m_icNumber.clrColor = clr;
	m_icNumber.bBold = bBold;
}

void CTWScriptEdit::SetStringColor(COLORREF clr, BOOL bBold)
{
	m_icString.clrColor = clr;
	m_icString.bBold = bBold;
}

BEGIN_MESSAGE_MAP(CTWScriptEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CTWScriptEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(EN_VSCROLL, OnVscroll)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_PROTECTED, OnProtected)
	ON_NOTIFY_REFLECT(EN_SELCHANGE, OnSelChange)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTWScriptEdit message handlers

UINT CTWScriptEdit::OnGetDlgCode() 
{
	UINT uCode = CRichEditCtrl::OnGetDlgCode();

	uCode = DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTMESSAGE | DLGC_WANTTAB;

	return uCode;
}

void CTWScriptEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (isalnum(nChar))
		TRACE("char %c\n", nChar);
	if (nChar == '\r')
	{
		if (!m_nEndLineLen)
		{
			ReplaceFTab();
			return;
		}

		CHARRANGE curSel;
		GetSel(curSel);

		HideSelection(TRUE, FALSE);
		SetSel(LineIndex(), LineIndex() + m_nEndLineLen - 4);
		CString strText = GetSelText();
		ReplaceSel(strText.Mid(1));
		CUndoAction action;
		action.sRemoveText = "\t";
		action.beforeRange.cpMin = LineIndex() - 1;
		action.beforeRange.cpMax = LineIndex();
		action.afterRange.cpMax = action.afterRange.cpMin = action.beforeRange.cpMax;
		action.sAddText    = "";
		action.nType       = CUndoAction::eCut;
		m_pUndoArr->Add(action);
		m_pRedoArr->RemoveAll();

		curSel.cpMin--;
		curSel.cpMax--;
		SetSel(curSel);

		HideSelection(FALSE, FALSE);
		ReplaceFTab();
		m_nEndLineLen = 0;
		return;
	}
	m_nEndLineLen = 0;
	if (nChar == '\t') 
	{
		ReplaceTab();
		return;
	}
	if (nChar == '.')
	{
		CHARRANGE curSel;
		GetSel(curSel);
		m_curPos = curSel.cpMax;
		m_bShowList = true;
	}
	else
		m_bShowList = false;

	if (nChar == '(' || nChar == ' ')
	{
		CHARRANGE curSel;
		GetSel(curSel);
		m_curPos = curSel.cpMax;
		m_bShowTip = true;
	}
	else
		m_bShowTip = false;

	
	if (nChar == '\'')
	{
		if (ReplaceComment())
			return;
	}

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	
	CString strText;
	int nLineLength = LineLength(-1) + 4;
	int nLen = GetLine(LineFromChar(-1), strText.GetBuffer(nLineLength), nLineLength);
	strText.ReleaseBuffer(nLineLength - 4);
	int nTab = GetTabCount(strText);
	strText.Replace("\t", "");

	if ((((nChar == 'f' || nChar == 'F') && !strText.CompareNoCase("End if")) ||
		((nChar == 't' || nChar == 'T') && !strText.CompareNoCase("End Select")) ||
		((nChar == 'r' || nChar == 'R') && !strText.CompareNoCase("End for")) ||
		((nChar == 'e' || nChar == 'E') && !strText.CompareNoCase("End while")))
		&& nTab)
	{
		m_nEndLineLen = nLineLength;
	}
}


int CTWScriptEdit::IsKeyword(LPCTSTR lpszSymbol)
{
	CString strSymbol; strSymbol.Format(" %s ", lpszSymbol);
	if (!m_bCaseSensitive) 
		strSymbol.MakeLower();
				
	return m_strKeywordsLower.Find(strSymbol);
}

int CTWScriptEdit::IsConstant(LPCTSTR lpszSymbol)
{
	CString strSymbol; strSymbol.Format(" %s ", lpszSymbol);
	if (!m_bCaseSensitive) 
		strSymbol.MakeLower();
				
	return m_strConstantsLower.Find(strSymbol);
}

BOOL CTWScriptEdit::IsStringQuote(TCHAR ch)
{
	return (m_strStringQuotes.Find(ch) >= 0);
}


void CTWScriptEdit::SetFormatRange(int nStart, int nEnd, BOOL bBold, COLORREF clr)
{
	if (nStart >= nEnd)
		return;

	SetSel(nStart, nEnd);

	DWORD dwEffects = bBold?CFE_BOLD:0;

	CHARFORMAT cfm;
	cfm.cbSize = sizeof(cfm);
	GetSelectionCharFormat(cfm);
	
	if ((cfm.dwMask & CFM_COLOR)  && cfm.crTextColor == clr && 
		(cfm.dwMask & CFM_BOLD) && (cfm.dwEffects & CFE_BOLD) == dwEffects)
		return;

	cfm.dwEffects = dwEffects;
	cfm.crTextColor = clr;
	cfm.dwMask = CFM_BOLD | CFM_COLOR;

	SetSelectionCharFormat(cfm);
}

void CTWScriptEdit::ChangeCase(int nStart, int nEnd, LPCTSTR lpsz)
{
	ASSERT((nEnd - nStart) == (int)_tcslen(lpsz));

	if (!m_bCaseSensitive && m_bChangeCase) {
		SetSel(nStart, nEnd);
		ReplaceSel(lpsz);
	}
}

void CTWScriptEdit::FormatTextRange(int nStart, int nEnd)
{
	if (nStart >= nEnd)
		return;

	CHARRANGE crOldSel;

	/*while (nEnd - nStart > 3000)
	{
		crOldSel.cpMax = nEnd;
		nEnd -= 3000;
		crOldSel.cpMin = nEnd;
		m_rangeArr.InsertAt(0, crOldSel);
		nEnd--;
	}
	if (m_rangeArr.GetSize())
		SetTimer(100, 500, NULL);	// 3000 char 이상인경우 분할처리*/

	//AfxGetApp()->DoWaitCursor(1);
	m_bInForcedChange = TRUE;

	GetSel(crOldSel);
	LockWindowUpdate();
	HideSelection(TRUE, FALSE);

	TCHAR *pBuffer = NULL;
	try {
		SetSel(nStart, nEnd);
		pBuffer = new TCHAR[nEnd - nStart + 1];

		CString sTxt = GetSelText();
		long nLen = sTxt.GetLength();
		_tcscpy_s(pBuffer, nEnd - nStart + 1, sTxt);
		
		ASSERT(nLen <= nEnd - nStart);
		pBuffer[nLen] = 0;

		TCHAR *pStart, *pPtr;
		pStart = pPtr = pBuffer;

		TCHAR* pSymbolStart = NULL;
		SymbolColor ic;

		while (*pPtr != 0) 
		{
			TCHAR ch = *pPtr;
			if (ch == '\n' || ch == ' ' || ch == '\t' || ch == '\r' || ch == NULL)
			{
				pPtr++;
				continue;
			}
			if (pPtr == pBuffer + nLen)
				break;

			if (ch == m_chComment && (m_chComment2 == 0 || pPtr[1] == m_chComment2)) 
			{
				pSymbolStart = pPtr;
				pPtr += (LineIndex(LineFromChar(nStart + pOFFSET) + 1) - (nStart + pOFFSET + 1));
				ic = m_icComment;
			} 
			else if (IsStringQuote(ch)) // Process strings
			{ 
				pSymbolStart = pPtr;
				TCHAR ch1 = ch;
				do {
					ch = *(++pPtr);
				} while (ch != 0 && ch != ch1 && ch != '\n');
				if (ch == ch1) pPtr++;
				ic = m_icString;
			} 
			else if (_istalpha(ch) || ch == '_') // Process keywords
			{
				bool bShow = false, bShowTip = false;
				pSymbolStart = pPtr;
				do {
					ch = *(++pPtr);
				} while (_istalnum(ch) || ch == '_');
				
				if (*pPtr == '.')
					bShow = true;
				else if (*pPtr == '(' || *pPtr == ' ')
					bShowTip = true;

				*pPtr = 0;
				long nPos = IsKeyword(pSymbolStart);
				if (nPos >= 0) {
					ChangeCase(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, 
								m_strKeywords.Mid(nPos+1, pPtr - pSymbolStart));
					if (_tcsicmp(m_strComment, pSymbolStart) == 0) {
						*pPtr = ch;
						*pSymbolStart = m_chComment;
						if (pSymbolStart[1] != 0 && m_chComment2 != 0)
							pSymbolStart[1] = m_chComment2;
						pPtr = pSymbolStart;
						pSymbolStart = NULL;
						continue;
					}
					ic = m_icKeyword;
				} 
				else 
				{
					nPos = IsConstant(pSymbolStart);
					if (nPos >= 0) 
					{
						ChangeCase(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, 
							m_strConstants.Mid(nPos+1, pPtr - pSymbolStart));
						ic = m_icConstant;
					} 
					else 
					{
						CString sym;
						sym.Format("%s", pSymbolStart);

						nPos = IsSymbol(sym);
						
						if (LOWORD(nPos) < IS_NOSYMBOL) 
						{
							ChangeCase(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, sym);
							ic = m_icKeyword;
							if (m_bShowList && bShow && (nStart + pPtr - pBuffer == m_curPos))
							{
								showList(nPos);
								m_bShowList = false;
							}
						}

						if (m_bShowTip && bShowTip && (nStart + pPtr - pBuffer == m_curPos))
						{
							if (pBuffer < pSymbolStart && *(pSymbolStart - 1) == '.')
							{
								pSymbolStart--;
								do {
									ch = *(--pSymbolStart);
								} while (_istalnum(ch) || ch == '_');
								pSymbolStart++;
							}
							sym.Format("%s", pSymbolStart);
							showTip(sym);
							m_bShowTip = false;
						}
						if (LOWORD(nPos) >= IS_NOSYMBOL)
							pSymbolStart = NULL;
					}
				}
				*pPtr = ch;
			} 
			else 
				pPtr++;

			if (pSymbolStart != NULL) 
			{
				ASSERT(pSymbolStart < pPtr);
				SetFormatRange(nStart + pStart - pBuffer, nStart + pSymbolStart - pBuffer, FALSE, RGB(0,0,0));
				SetFormatRange(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, ic.bBold, ic.clrColor);
				pStart = pPtr;
				pSymbolStart = 0;
			} 
			else if (*pPtr == 0)
				SetFormatRange(nStart + pStart - pBuffer, nStart + pPtr - pBuffer, FALSE, RGB(0,0,0));
		}
	} catch(...){}

	delete [] pBuffer;

	SetSel(crOldSel);
	HideSelection(FALSE, FALSE);
	UnlockWindowUpdate();
	//AfxGetApp()->DoWaitCursor(-1);

	m_bInForcedChange = FALSE;
}

void CTWScriptEdit::FormatTextLines(int nLineStart, int nLineEnd)
{
	long nStart = LineIndex(LineFromChar(nLineStart));
	long nEnd = LineIndex(LineFromChar(nLineEnd));
	nEnd += LineLength(nLineEnd);

	FormatTextRange(nStart, nEnd);
}


void CTWScriptEdit::FormatAll()
{
	FormatTextRange(0, GetTextLength());
}

LRESULT CTWScriptEdit::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT res = Default();
	FormatAll();
	return res;	
}

void CTWScriptEdit::OnChange() 
{
	if (m_bInForcedChange)
		return;

	CHARRANGE crCurSel; 
	GetSel(crCurSel);

	if (m_changeType == ctMove && crCurSel.cpMin == crCurSel.cpMax) {
		// cut was canceled, so this is paste operation
		m_changeType = ctPaste;
	}

	if (!m_bUndoLock)
	{
		CString sText;
		switch (m_changeType) {
		case ctReplSel:
		case ctKeyDown:
			{
				HideSelection(TRUE, FALSE);
				SetSel(m_crOldSel.cpMin, crCurSel.cpMax);
				m_sAfterText = GetSelText();
				SetSel(crCurSel);
				HideSelection(FALSE, FALSE);
				CUndoAction action, beforeAction;
				if (m_pUndoArr->GetSize())
				{
					beforeAction = m_pUndoArr->GetAt(m_pUndoArr->GetSize() - 1);
					if (beforeAction.nType == CUndoAction::eKeyPaste &&
						beforeAction.beforeRange.cpMax == m_crOldSel.cpMin - 1 &&
						beforeAction.sAddText.GetLength() &&
						isalnum(beforeAction.sAddText.GetAt(beforeAction.sAddText.GetLength() - 1)) &&
						m_sAfterText.GetLength() == 1 &&
						isalnum(m_sAfterText.GetAt(0)))
					{
						beforeAction.afterRange  = crCurSel;
						beforeAction.beforeRange.cpMax = m_crOldSel.cpMax;
						beforeAction.sAddText    += m_sAfterText;
						m_pUndoArr->SetAt(m_pUndoArr->GetSize() - 1, beforeAction);
						break;
					}
				}
				action.sRemoveText = m_sBeforeText;
				action.beforeRange = m_crOldSel;
				action.afterRange  = crCurSel;
				action.sAddText    = m_sAfterText;
				action.nType       = CUndoAction::eKeyPaste;
				m_pUndoArr->Add(action);
				m_pRedoArr->RemoveAll();
			}
			break;
		case ctPaste:
			{
				CUndoAction action;
				action.sRemoveText = m_sBeforeText;
				action.beforeRange = m_crOldSel;
				action.afterRange  = crCurSel;
				action.sAddText    = m_sAfterText;
				action.nType       = CUndoAction::ePaste;
				m_pUndoArr->Add(action);
				m_pRedoArr->RemoveAll();
			}
			break;
		case ctDelete:
		case ctBack:
		case ctCut:
			{
				CUndoAction action;
				action.sRemoveText = m_sBeforeText;
				action.beforeRange = m_crOldSel;
				action.afterRange  = crCurSel;
				action.nType       = CUndoAction::eCut;
				m_pUndoArr->Add(action);
				m_pRedoArr->RemoveAll();
			}
			break;
		case ctUndo:
			sText = "Undo";	break;
			break;
		case ctMove:
			if (crCurSel.cpMin > m_crOldSel.cpMin) // move after
				sText = "Move After";
			else // move before
				sText = "Move Before";
			break;
		default:
			sText = "Default";
			break;
		}
		
//		TRACE("Change = %s, %d, %d, (%d, %d)(%s)\n", sText, crCurSel.cpMin, crCurSel.cpMax, m_crOldSel.cpMin, m_crOldSel.cpMax, m_sBeforeText);
	}

	switch (m_changeType) {
	case ctReplSel:// old=(x,y) -> cur=(x+len,x+len)
		/*if (m_nMaxFmRange == 0)
		{
			NextFormat();
			SetTimer(101, 2000, NULL);
			break;
		}*/
	case ctPaste:  // old=(x,y) -> cur=(x+len,x+len)
		FormatTextLines(m_crOldSel.cpMin, crCurSel.cpMax);
		break;
	case ctDelete: // old=(x,y) -> cur=(x,x)
	case ctBack:   // old=(x,y) -> cur=(x,x), newline del => old=(x,x+1) -> cur=(x-1,x-1)
	case ctCut:    // old=(x,y) -> cur=(x,x)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctUndo:   // old=(?,?) -> cur=(x,y)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctMove:   // old=(x,x+len) -> cur=(y-len,y) | cur=(y,y+len)
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		if (crCurSel.cpMin > m_crOldSel.cpMin) // move after
			FormatTextLines(m_crOldSel.cpMin, m_crOldSel.cpMin);
		else // move before
			FormatTextLines(m_crOldSel.cpMax, m_crOldSel.cpMax);
		break;
	case ctKeyDown:
		FormatTextLines(m_crOldSel.cpMin, crCurSel.cpMax);
		break;
	default:
		m_rangeArr.RemoveAll();
		FormatAll();
		break;
	}
	
	//undo action does not call OnProtected, so make it default
	m_changeType = ctUndo;

	if (!m_bUndoLock)
		ProcessEmpty();
}

void CTWScriptEdit::OnProtected(NMHDR* pNMHDR, LRESULT* pResult)
{
	ENPROTECTED* pEP = (ENPROTECTED*)pNMHDR;

	// determine type of change will occur

	m_sAfterText.Empty();
	m_sBeforeText.Empty();
	switch (pEP->msg) {
	case WM_KEYDOWN:
		switch (pEP->wParam) {
		case VK_DELETE:
			m_changeType = ctDelete;
			if (pEP->chrg.cpMin == pEP->chrg.cpMax)
				SetSel(pEP->chrg.cpMin, pEP->chrg.cpMin + 1);

			break;
		case VK_BACK:
			m_changeType = ctBack;
			if (pEP->chrg.cpMin == pEP->chrg.cpMax)
				SetSel(pEP->chrg.cpMin - 1, pEP->chrg.cpMin);
			break;
		default:
			m_changeType = ctKeyDown;
			break;
		}
		break;
	case WM_SETTEXT:	// added for tab key (LWJ)
	case EM_REPLACESEL:
		//if (!m_bUndoLock)
			m_changeType = ctReplSel;
		break;
	case WM_CHAR:
		m_changeType = ctKeyDown;
		break;
	case WM_PASTE:
		m_changeType = (m_changeType == ctCut)?ctMove:ctPaste;
		{
			if (OpenClipboard())
			{
				if(::IsClipboardFormatAvailable(CF_TEXT))
				{
					HANDLE hMem;
					hMem = ::GetClipboardData(CF_TEXT);
					m_sAfterText = (LPCTSTR)(LPSTR)GlobalLock((HGLOBAL)hMem);
					GlobalUnlock((HGLOBAL)hMem);
				}
				CloseClipboard();
			}
		}
		break;
	case WM_CUT:
		m_changeType = ctCut;
		break;
	default:
		//m_changeType = ctUnknown;
		break;
	};

	if (/*!m_bUndoLock && */pEP->msg != EM_SETCHARFORMAT && m_changeType != ctMove)
	{
		m_crOldSel = pEP->chrg;
		m_sBeforeText = GetSelText();
	}

	*pResult = FALSE;
}

void CTWScriptEdit::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SELCHANGE* pSC = (SELCHANGE*)pNMHDR;

	*pResult = 0;
}

void CTWScriptEdit::Initialize(_mapH *mapH)
{
	m_mapH = mapH;
}

int CTWScriptEdit::IsSymbol(CString &sym)
{
	CFormItem	formItem(m_mapH);

	void *rtValue;
	sym.MakeUpper();
	if (m_ScriptStrMap.Lookup(sym, rtValue))
		return MAKELONG((int)rtValue, 0);

	CString tmp = m_mapH->publicR;
	do
	{
		CString temp = Parser(tmp, ";");
		if (!temp.IsEmpty() && !sym.CompareNoCase(temp))
		{
			void* addedID;
			if (m_publicStrMap.Lookup(sym, addedID))
				return MAKELONG((int)addedID, 0);
		}
	}while(!tmp.IsEmpty());
	
	for (int kk = 0; kk < formItem.getFormCount(); kk++)
	{
		CString str = formItem.getFormStr(kk, "name");
		if (atoi(str))
			str = "S" + str;
		if (!sym.CompareNoCase(str))
		{
			sym = str;
			m_sControlName.Empty();
			if (formItem.getFormPoint(kk)->kind == FM_CONTROL)
				m_sControlName = formItem.getFormStr(kk, "str");

			return MAKELONG(formItem.getFormPoint(kk)->kind, formItem.getFormPoint(kk)->type);
		}
	}
	
	return MAKELONG(IS_NOSYMBOL, 0);
}

void CTWScriptEdit::showList(int kind)
{
	CString info = "";
	CRect rc;
	CHARFORMAT cf;
	CHARRANGE curSel;
	int type = HIWORD(kind);
	kind = LOWORD(kind);

	m_pAutoDlg->Reset();
	if (kind == FM_BUTTON)
	{
		if (type == BTN_RADIO)
			kind = FM_CONTROL + 10;
		else if (type == BTN_CHECK)
			kind = FM_CONTROL + 11;
	}
	
	info = GetBaseControlInfo(kind);
	if (/*m_AutoListMap.Lookup(kind, info)*/!info.IsEmpty() || !m_sControlName.IsEmpty())
	{
		if (kind == FM_CONTROL && !m_sControlName.IsEmpty())
			info = GetControlInfo(m_sControlName);

		do
		{
			CString temp = Parser(info, "\t");
			CString text = Parser(temp, ":");
			int isProp = atoi(temp);
			if (!text.IsEmpty())
				m_pAutoDlg->AddString(text, isProp);
		}while(!info.IsEmpty());
	}
	
	GetSel(curSel);
	GetSelectionCharFormat(cf);

	rc.SetRect(GetCharPos(curSel.cpMin).x, GetCharPos(curSel.cpMin).y + cf.yHeight / 10, 0, 0);
	rc.right = rc.left + m_pAutoDlg->getWidth();
	rc.bottom = rc.top + 150;

	ClientToScreen(&rc);
	if (!m_pAutoDlg->IsEmpty())
		m_pAutoDlg->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
}

void CTWScriptEdit::showTip(CString sKey)
{
	CRect rc;
	CHARFORMAT cf;
	CHARRANGE curSel;

	GetSel(curSel);
	GetSelectionCharFormat(cf);

	CString sSym = Parser(sKey, "."), sTipText = "", info = "";
	if (sKey.IsEmpty())	// VB keyword
	{
		return;
	}
	else
	{
		sKey.TrimRight();
		int nPos = IsSymbol(sSym);
		if (LOWORD(nPos) < IS_NOSYMBOL) // symbol valid
		{
			// search keyword
			int type = HIWORD(nPos), kind = LOWORD(nPos);
			if (kind == FM_BUTTON)
			{
				if (type == BTN_RADIO)
					kind = FM_CONTROL + 10;
				else if (type == BTN_CHECK)
					kind = FM_CONTROL + 11;
			}
			info = GetBaseControlInfo(kind);
			if (/*m_AutoListMap.Lookup(kind, info)*/!info.IsEmpty())
			{
				if (kind == FM_CONTROL && !m_sControlName.IsEmpty())
					info = GetControlInfo(m_sControlName);
				do
				{
					CString temp = Parser(info, "\t");
					CString text = Parser(temp, ":");
					int isProp = atoi(Parser(temp, ":"));
					CString sTip = Parser(temp, ";");
					if (!text.IsEmpty() && !text.CompareNoCase(sKey))
					{
						if (!isProp)
							sTipText = sTip;
						break;
					}
				}while(!info.IsEmpty());
			}
		}
	}

	if (sTipText.IsEmpty())
		return;
	m_pAutoTip->SetWindowText(sTipText);
	m_pAutoTip->m_nStart = m_curPos;

	rc.SetRect(GetCharPos(curSel.cpMin).x, GetCharPos(curSel.cpMin).y - cf.yHeight / 10, 0, 0);
	rc.right = rc.left + m_pAutoTip->getWidth() + 10;
	rc.bottom = rc.top + 18;

	ClientToScreen(&rc);
	m_pAutoTip->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
	m_pAutoTip->RedrawWindow();
}

void CTWScriptEdit::OnKillfocus() 
{
	hidePopup();
}

LRESULT CTWScriptEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_KEYDOWN)
	{
		if (m_pAutoDlg->IsWindowVisible())
		{
			switch(wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_ESCAPE:
				m_pAutoDlg->ShowWindow(SW_HIDE);
				break;
			case VK_BACK:
				{
					long nStart, nEnd;
					GetSel(nStart, nEnd);
					SetSel(nStart - 1, nEnd);
					if (GetSelText().GetLength() && !GetSelText().Compare("."))
						m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				break;
			case VK_UP:
			case VK_DOWN:
			case VK_HOME:
			case VK_END:
			case VK_PRIOR:
			case VK_NEXT:
				m_pAutoDlg->m_aclist.SendMessage(WM_KEYDOWN, wParam, 0);
				return TRUE;
				break;
			case VK_RETURN:
				{
					HideSelection(TRUE, FALSE);
					long nStart, nEnd;
					if (!m_pAutoDlg->getSelString().IsEmpty())
					{
						MSG msg;
						PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
						do
						{
							GetSel(nStart, nEnd);
							if (GetSelText().GetLength() && GetSelText().Find(".") == 0)
							{
								SetSel(nStart + 1, nEnd);
								break;
							}
							else
								SetSel(nStart - 1, nEnd);
						}while(nStart >= 0);
						//m_bInForcedChange = TRUE;
						ReplaceSel(m_pAutoDlg->getSelString(), TRUE);
						//m_bInForcedChange = FALSE;
						HideSelection(FALSE, FALSE);
					}
					m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				return FALSE;
			default:
				if (wParam >= '!' && (!_istalpha(wParam) && (char)wParam != '_' && ((char)wParam != '9' && (::GetKeyState(VK_SHIFT) & 0x0800))))
					m_pAutoDlg->ShowWindow(SW_HIDE);
				else if ((char)wParam == ' ' || ((char)wParam == '9' && (::GetKeyState(VK_SHIFT) & 0x0800)))
				{
					HideSelection(TRUE, FALSE);
					long nStart, nEnd;
					if (!m_pAutoDlg->getSelString().IsEmpty())
					{
						do
						{
							GetSel(nStart, nEnd);
							if (GetSelText().GetLength() && GetSelText().Find(".") == 0)
							{
								SetSel(nStart + 1, nEnd);
								break;
							}
							else
								SetSel(nStart - 1, nEnd);
						}while(nStart >= 0);
						//m_bInForcedChange = TRUE;
						ReplaceSel(m_pAutoDlg->getSelString(), TRUE);
						//m_bInForcedChange = FALSE;
						HideSelection(FALSE, FALSE);
					}
					m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				else if (wParam >= '!')	// maybe ascii
				{
					HideSelection(TRUE, FALSE);
					long nStart, nEnd;
					CString txt;
					GetSel(nStart, nEnd);
					nStart = LineIndex(LineFromChar(nStart));
					SetSel(nStart, nEnd);
					txt = GetSelText();
					txt = txt.Mid(txt.ReverseFind('.') + 1);
					txt += (char)wParam;
					m_pAutoDlg->FindString(txt);
					SetSel(nEnd, nEnd);
					HideSelection(FALSE, FALSE);
				}
				break;
			}
		}
		if (m_pAutoTip->IsWindowVisible())
		{
			long nStart, nEnd;
			CString sTip;
			m_pAutoTip->GetWindowText(sTip);

			GetSel(nStart, nEnd);

			switch(wParam)
			{
			case VK_RETURN:
			case VK_UP:
			case VK_DOWN:
			case VK_ESCAPE:
			case VK_HOME:
			case VK_PRIOR:
			case VK_NEXT:
				m_pAutoTip->ShowWindow(SW_HIDE);	
				break;
			case VK_END:
				if (::GetKeyState(VK_CONTROL) & 0x0800)
					m_pAutoTip->ShowWindow(SW_HIDE);
				else
				{
					CString txt;
					int nLineLength = LineLength(-1) + 4;
					int nLen = GetLine(LineFromChar(-1), txt.GetBuffer(nLineLength), nLineLength);
					txt.ReleaseBuffer(nLineLength - 4);
					if (txt.Replace(",", ",") >= sTip.Replace(",", ","))
					{
						if (txt.ReverseFind(',') != -1)
						{
							txt = txt.Mid(txt.ReverseFind(',') + 1);
							txt.TrimLeft();
							if (!txt.IsEmpty() && txt.FindOneOf("\t ") != -1)
								m_pAutoTip->ShowWindow(SW_HIDE);
						}
						else
							m_pAutoTip->ShowWindow(SW_HIDE);
					}
				}
				break;
			case VK_LEFT:
			case VK_RIGHT:
				{
					HideSelection(TRUE, FALSE);
					
					CString txt;
					
					if (m_pAutoTip->m_nStart >= nEnd - (wParam==VK_LEFT?1:0))
					{
						m_pAutoTip->ShowWindow(SW_HIDE);
						HideSelection(FALSE, FALSE);
						break;
					}
					if (::GetKeyState(VK_SHIFT) & 0x0800)
					{
						HideSelection(FALSE, FALSE);
						break;
					}
					SetSel(m_pAutoTip->m_nStart, nEnd + (wParam==VK_RIGHT?1:0));
					txt = GetSelText();
					if (txt.Find(")") != -1)
					{
						m_pAutoTip->ShowWindow(SW_HIDE);
					}
					SetSel(nStart, nEnd);
					HideSelection(FALSE, FALSE);
				}
				break;
			case VK_DELETE:
			case VK_BACK:
				{
					long nStart, nEnd;
					GetSel(nStart, nEnd);
					if (m_pAutoTip->m_nStart >= nStart - (nStart == nEnd?1:0))
						m_pAutoTip->ShowWindow(SW_HIDE);
				}
				break;
			default:
				if ((char)wParam == '0' && (::GetKeyState(VK_SHIFT) & 0x0800))
					m_pAutoTip->ShowWindow(SW_HIDE);
				if ((char)wParam == ' ')
				{
					HideSelection(TRUE, FALSE);
					long nStart, nEnd;
					CString txt;
					GetSel(nStart, nEnd);
					SetSel(m_pAutoTip->m_nStart, nEnd + 1);
					txt = GetSelText();
					
					if (txt.Replace(",", ",") >= sTip.Replace(",", ",") || m_pAutoTip->m_nStart >= nEnd)
					{
						if (txt.ReverseFind(',') != -1)
						{
							txt = txt.Mid(txt.ReverseFind(',') + 1);
							txt.TrimLeft();
							if (!txt.IsEmpty())
								m_pAutoTip->ShowWindow(SW_HIDE);
						}
						else
							m_pAutoTip->ShowWindow(SW_HIDE);
					}
					SetSel(nStart, nEnd);
					HideSelection(FALSE, FALSE);
				}
				break;
			}
		}
	}
	else if (message == WM_LBUTTONDOWN)
	{
		if (m_pAutoDlg->IsWindowVisible())
			m_pAutoDlg->ShowWindow(SW_HIDE);
		if (m_pAutoTip->IsWindowVisible())
			m_pAutoTip->ShowWindow(SW_HIDE);
	}
	
	return CRichEditCtrl::WindowProc(message, wParam, lParam);
}

void CTWScriptEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CHARRANGE	cr;
	GetSel(cr);
	if (cr.cpMax == cr.cpMin)
		SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKEWPARAM(point.x, point.y));

	CRichEditCtrl::OnRButtonDown(nFlags, point);
	ShowMenu();
}

void CTWScriptEdit::ShowMenu()
{
	CPoint	point;
	CMenu	menu;
	CHARRANGE	cr;

	menu.CreatePopupMenu();
	GetCursorPos(&point);
	GetSel(cr);
	m_sRtn = GetSelText();

	menu.AppendMenu(MF_STRING, 1, "Cut\t(Ctrl+X)");
	menu.AppendMenu(MF_STRING, 2, "&Copy\t(Ctrl+C)");
	menu.AppendMenu(MF_STRING, 3, "&Paste\t(Ctrl+V)");
	if (m_sRtn.GetLength() == 8)
	{
		menu.AppendMenu(MF_STRING, 4, "Open Map \"" + m_sRtn + "\"");
		if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISOFFLINE))
			menu.EnableMenuItem(4, MF_GRAYED);
	}

	if (cr.cpMax <= cr.cpMin)
		menu.EnableMenuItem(1, MF_GRAYED);

	int rtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
				point.x, point.y, this);

	switch(rtn)
	{
	case 1: // Cut
		Cut();
		break;
	case 2: // Copy
		Copy();
		break;
	case 3: // Paste
		Paste();
		break;
	case 4:	// Open map
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, (long)m_sRtn.operator LPCTSTR());
		break;
	}
}

BOOL CTWScriptEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == 2)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}
	return CRichEditCtrl::OnSetCursor(pWnd, nHitTest, message);
}


void CTWScriptEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditCtrl::OnSetFocus(pOldWnd);

}

CString CTWScriptEdit::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

bool CTWScriptEdit::ReplaceComment()
{
	long nStart, nEnd, nStartLine, nEndLine;
	GetSel(nStart, nEnd);
	if (nStart == nEnd)
		return false;
	
	nStartLine = LineFromChar(nStart);
	nEndLine = LineFromChar(nEnd);

	nStart = LineIndex(nStartLine);
	if (nEnd <= LineIndex(nEndLine))
		nEnd = LineIndex(nEndLine - 1) + 1;
	else
		nEnd = LineIndex(nEndLine) + 1;

	HideSelection(TRUE, FALSE);
	SetSel(nStart, nEnd);
	CString txt = GetSelText();
	if (txt.GetLength() > 0)
	{
		if (txt.GetAt(0) != '\'')
		{
			txt.Replace("\n", "\n\'");
			txt = '\'' + txt;
		}
		else
		{
			txt = txt.Mid(1);
			txt.Replace("\n\'", "\n");
		}
		ReplaceSel(txt);
	}
	HideSelection(FALSE, FALSE);
	return true;
}

void CTWScriptEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_mousePnt = point;
	
	CRichEditCtrl::OnMouseMove(nFlags, point);
}

void CTWScriptEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CRichEditCtrl::OnLButtonDown(nFlags, point);
}

int CTWScriptEdit::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT && !m_bActivate)
	{
		CPoint pnt;
		GetCursorPos(&pnt);
		ScreenToClient(&pnt);
		m_bActivate = true;
		SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pnt.x, pnt.y));
	}
	return CRichEditCtrl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CTWScriptEdit::OnTimer(UINT nIDEvent) 
{
	/*if (nIDEvent == 101)
		NextFormat();
	if (nIDEvent == 100 && m_rangeArr.GetSize())
	{
		CHARRANGE range = m_rangeArr.GetAt(0);
		m_rangeArr.RemoveAt(0);
		FormatTextRange(range.cpMin, range.cpMax);
	}
	if (!m_rangeArr.GetSize())
		KillTimer(100);*/
	CRichEditCtrl::OnTimer(nIDEvent);
}

void CTWScriptEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CRichEditCtrl::OnKillFocus(pNewWnd);
	
	m_bActivate = false;	
}

long CTWScriptEdit::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CHARRANGE cr;

	GetSel(cr);
	CString tmp;
	if (cr.cpMin == cr.cpMax)
	{
		HideSelection(TRUE, FALSE);
		cr.cpMin--;
		SetSel(cr);
		tmp = GetSelText();
		if (!tmp.IsEmpty())
		{
			while(isalnum(tmp.GetAt(0)))
			{
				cr.cpMin--;
				if (cr.cpMin < 0)
				{
					cr.cpMin = 0;
					break;
				}
				SetSel(cr);
				tmp = GetSelText();
			}
		}
		else
			cr.cpMin = 0;

		cr.cpMax++;
		SetSel(cr);
		tmp = GetSelText();
		while(isalnum(tmp.GetAt(tmp.GetLength() - 1)))
		{
			cr.cpMax++;
			SetSel(cr);
			tmp = GetSelText();
			if (tmp.GetLength() < cr.cpMax - cr.cpMin)
				break;
		}
		if (cr.cpMin > 0)
			cr.cpMin++;
		cr.cpMax--;
		HideSelection(FALSE, FALSE);
		SetSel(cr);
		m_sRtn = GetSelText();
	}
	else
		m_sRtn = GetSelText();
	return (long)m_sRtn.operator LPCTSTR();
}

CString CTWScriptEdit::ParseWord(CString str, int &pos, CString sub)
{
	if (pos >= str.GetLength())
		return "";
	CString sStr = str.Mid(pos);
	int nPos = 0;

	if (sStr.FindOneOf(sub) == -1)
	{
		pos += sStr.GetLength() + 1;
		return sStr;
	}
	else
	{
		nPos = sStr.FindOneOf(sub);
		while(nPos == 0)
		{
			pos += nPos + 1;
			sStr = sStr.Mid(nPos + 1);
			nPos = sStr.FindOneOf(sub);
		}
		if (nPos < 0)
			return sStr;
		pos += nPos + 1;
		CString  temp = sStr.Left(nPos);
		return temp;
	}
	return "";
}

void CTWScriptEdit::ReplaceTab()
{
	long fPos, lPos;
	GetSel(fPos, lPos);
	if (fPos != lPos)
	{
		CString buf = GetSelText();
		int nFirstLine = LineFromChar(fPos), nLastLine = LineFromChar(lPos - 1);
		for (int ii = nFirstLine; ii <= nLastLine; ii++)
		{
			if (!(GetKeyState(VK_SHIFT) & 0x80))
			{
				SetSel(LineIndex(ii), LineIndex(ii));
				ReplaceSel("\t", TRUE);
				lPos++;
			}
			else
			{
				SetSel(LineIndex(ii), LineIndex(ii) + 1);
				if (GetSelText().GetAt(0) == '\t')
				{
					ReplaceSel("", TRUE);
					lPos--;
				}
			}
		}
		
		fPos = LineIndex(nFirstLine);
		lPos = LineIndex(nLastLine + 1) - 1;
		
		SetSel(fPos, lPos);
	}
	else if (!(GetKeyState(VK_SHIFT) & 0x80))
	{
		ReplaceSel("\t", TRUE);
		SetSel(fPos + 1, lPos + 1);
	}
}

void CTWScriptEdit::ReplaceFTab()
{
	long fPos, lPos;
	GetSel(fPos, lPos);
	CString sText = "\n";
	if (fPos != 0 && fPos == lPos)
	{
		HideSelection(TRUE, FALSE);
		int nFirstLine = LineFromChar(fPos) + 1, nTab = 0;
		if (nFirstLine)
		{
			SetSel(LineIndex(nFirstLine - 1), LineIndex(nFirstLine) - 1);
			sText = GetSelText();
			nTab = GetTabCount(sText);
			sText.Replace("\t", "");
			sText.TrimLeft();
			sText.MakeUpper();

			if (!sText.Find("SELECT ") || !sText.Find("IF ") || 
				!sText.Find("FOR ") || !sText.Find("WHILE "))
				nTab++;
			
			sText = "\n";
			for (int ii = 0; ii < nTab; ii++)
				sText += "\t";
			SetSel(fPos, lPos);
			CUndoAction action;
			action.sRemoveText = "";
			action.beforeRange.cpMin = action.beforeRange.cpMax = fPos + 1;
			action.afterRange.cpMax = action.afterRange.cpMin = lPos + nTab + 1;
			action.sAddText    = sText;
			action.nType       = CUndoAction::eKeyPaste;
			m_pUndoArr->Add(action);
			m_pRedoArr->RemoveAll();
			HideSelection(FALSE, FALSE);
		}
	}
	ReplaceSel(sText);
}

BOOL CTWScriptEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'c' || pMsg->wParam == 'C'))
		{
			Copy();
			return TRUE;
		}
		else if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'v' || pMsg->wParam == 'V'))
		{
			Paste();
			return TRUE;
		}
		else if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'x' || pMsg->wParam == 'X'))
		{
			Cut();
			return TRUE;
		}
		else if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'z' || pMsg->wParam == 'Z'))
		{
			UndoAction();
			return TRUE;
		}
		else if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'y' || pMsg->wParam == 'Y'))
		{
			RedoAction();
			return TRUE;
		}
		else if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'a' || pMsg->wParam == 'A'))
		{
			SetSel(0, -1);
			return TRUE;
		}		
	}	
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

CString CTWScriptEdit::GetControlInfo(CString str)
{
	if (str.IsEmpty())
		return "GetProperties:1\tSetProperties:1\t";
	ITypeLib *pTypeLib = NULL;
	ITypeInfo *pTypeInfo = NULL;
	TYPEATTR *pTypeAttr = NULL;

	BSTR bstr;
	HRESULT hr;

	CString sRoot = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR), sRtn = "";
	sRoot += "\\dev\\" + str + ".DLL";
	bstr = sRoot.AllocSysString();
	hr = LoadTypeLib(bstr, &pTypeLib);
	SysFreeString(bstr);

	if (FAILED(hr))
		return "GetProperties:0\tSetProperties:0\t";

	UINT iCount = pTypeLib->GetTypeInfoCount();

	for (int ii = 0; ii < (int)iCount; ii++)
	{
		if (FAILED(hr = (pTypeLib->GetTypeInfo(ii, &pTypeInfo))))
		{
			if (pTypeInfo)
				pTypeInfo->Release();
			if (pTypeLib)
				pTypeLib->Release();
			pTypeInfo = NULL;
			pTypeLib = NULL;
			if (sRtn.IsEmpty())
				sRtn = "GetProperties:0\tSetProperties:0\t";
			return sRtn;
		}
		
		if (FAILED(hr = (pTypeInfo->GetTypeAttr(&pTypeAttr))))
		{
			if (pTypeAttr)
				pTypeInfo->ReleaseTypeAttr(pTypeAttr);
			
			if (pTypeInfo)
				pTypeInfo->Release();
			if (pTypeLib)
				pTypeLib->Release();
			pTypeAttr = NULL;
			pTypeInfo = NULL;
			pTypeLib = NULL;
			if (sRtn.IsEmpty())
				sRtn = "GetProperties:0\tSetProperties:0\t";
			return sRtn;
		}

		sRtn += GetTypeInfoString(pTypeInfo, pTypeAttr);

		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeInfo->Release();
	}
	
	pTypeLib->Release();
	pTypeLib = NULL;
	return sRtn;
}


void CTWScriptEdit::hidePopup()
{
	if (m_pAutoDlg && m_pAutoDlg->IsWindowVisible())
		m_pAutoDlg->ShowWindow(SW_HIDE);
	if (m_pAutoTip && m_pAutoTip->IsWindowVisible())
		m_pAutoTip->ShowWindow(SW_HIDE);
}

int CTWScriptEdit::GetTabCount(CString sText)
{
	int nCnt = 0;
	for (int ii = 0; ii < sText.GetLength(); ii++)
		if (sText.GetAt(ii) == '\t')
			nCnt++;
		else
			break;
	return nCnt;
}


CString CTWScriptEdit::GetVtType(VARTYPE var)
{
	switch(var)
	{
	case VT_I4:
		return "Long";
	case VT_BSTR:
		return "String";
	case VT_BOOL:
		return "Boolean";
	case VT_I2:
		return "Integer";
	case VT_VARIANT:
		return "Variant";
	case VT_R4:
		return "Float";
	case VT_R8:
		return "Double";
	case VT_EMPTY:
	case VT_VOID:
		return "void";
	}
	return "";
}

CString CTWScriptEdit::GetTypeInfoString(ITypeInfo *pTypeInfo, TYPEATTR *pTypeAttr)
{
	CString sRtn = "";
	if (TKIND_DISPATCH == pTypeAttr->typekind)
	{
		int nCnt = pTypeAttr->cFuncs + pTypeAttr->cVars;
		
		for (int jj = 1; jj <= nCnt; jj++)
		{
			BSTR *pBstr = new BSTR[50];
			UINT      cNames;
			pTypeInfo->GetNames(jj, pBstr, -1, &cNames);
			
			CString sStr(*pBstr);
			sRtn += sStr;
			if (jj <= pTypeAttr->cVars)
			{
				sRtn += ":1:";	// properties
				
				VARDESC *pVarDesc;
				pTypeInfo->GetVarDesc(jj - 1, &pVarDesc);
				CString sTip = GetVtType(pVarDesc->elemdescVar.tdesc.vt) + " " + sStr;
				sRtn += sTip + "\t";
				pTypeInfo->ReleaseVarDesc(pVarDesc);
			}
			else
			{
				sRtn += ":0:";	// Methods
				
				CString sTip = sStr + "(";
				FUNCDESC *pFuncDesc;
				pTypeInfo->GetFuncDesc(jj - pTypeAttr->cVars - 1, &pFuncDesc);
				sTip = GetVtType(pFuncDesc->elemdescFunc.tdesc.vt) + " " + sTip;
				for (int nCount = 1; nCount < (int)cNames; nCount++)
				{
					CString sParam(pBstr[nCount]);
					sTip += GetVtType(pFuncDesc->lprgelemdescParam[nCount - 1].tdesc.vt) + " ";
					sTip += sParam;
					if (nCount != (int)cNames - 1)
						sTip += ", ";
					SysFreeString(pBstr[nCount]);
				}
				sRtn += sTip + ")\t";
				pTypeInfo->ReleaseFuncDesc(pFuncDesc);
			}
			SysFreeString(*pBstr);
			delete pBstr;
		}
	}
	return sRtn;
}

CString CTWScriptEdit::GetBaseControlInfo(int kind)
{
	CString sControlName = "", sRtn = "";
	if (m_AutoListMap.Lookup(kind, sRtn))
		return sRtn;
	switch(kind)
	{
	case FM_SYS:		sControlName = "IfmSysm";	break;
	case FM_LABEL:		sControlName = "IfmLabel";	break;
	case FM_BOX:		sControlName = "IfmBox";	break;
	case FM_GROUP:		sControlName = "IfmGroup";	break;
	case FM_PANEL:		sControlName = "IfmPanel";	break;
	case FM_BUTTON:		sControlName = "IfmButton";	break;
	case FM_EDIT:		sControlName = "IfmEdit";	break;
	case FM_COMBO:		sControlName = "IfmCombo";	break;
	case FM_OUT:		sControlName = "IfmOut";	break;
	case FM_MEMO:		sControlName = "IfmMemo";	break;
	case FM_GRID:		sControlName = "IfmGrid";	break;
	case FM_TABLE:		sControlName = "IfmTable";	break;
	case FM_TREE:		sControlName = "IfmTreeView";	break;
	case FM_AVI:		sControlName = "IfmAvi";	break;
	case FM_OBJECT:		sControlName = "IfmObject";	break;
	case FM_TAB:		sControlName = "IfmTab";	break;
	case FM_UTAB:		sControlName = "IfmUserTab";	break;
	case FM_SHEET:		sControlName = "IfmSheet";	break;
	case FM_BROWSER:	sControlName = "IfmBrowser";	break;
	case (FM_CONTROL + 10):	sControlName = "IfmRadio";	break;	// radio
	case (FM_CONTROL + 11):	sControlName = "IfmRadio";	break;	// check  Derived from radio
	}

	ITypeLib *pTypeLib = NULL;
	ITypeInfo *pTypeInfo = NULL;
	TYPEATTR *pTypeAttr = NULL;

	BSTR bstr;
	HRESULT hr;

	CString sRoot = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	sRoot += "\\exe\\axisform.dll";
	bstr = sRoot.AllocSysString();
	hr = LoadTypeLib(bstr, &pTypeLib);
	SysFreeString(bstr);

	if (FAILED(hr))
		return "";

	UINT iCount = pTypeLib->GetTypeInfoCount();

	for (int ii = 0; ii < (int)iCount; ii++)
	{
		pTypeLib->GetDocumentation(ii, &bstr, NULL, NULL, NULL);
		CString sName(bstr);
		//TRACE("name = %s\n", sName);
		SysFreeString(bstr);

		if (sName.CompareNoCase(sControlName))
			continue;

		if (FAILED(hr = (pTypeLib->GetTypeInfo(ii, &pTypeInfo))))
		{
			if (pTypeInfo)
				pTypeInfo->Release();
			if (pTypeLib)
				pTypeLib->Release();
			pTypeInfo = NULL;
			pTypeLib = NULL;
			if (sRtn.IsEmpty())
				sRtn = "";
			return sRtn;
		}
		
		if (FAILED(hr = (pTypeInfo->GetTypeAttr(&pTypeAttr))))
		{
			if (pTypeAttr)
				pTypeInfo->ReleaseTypeAttr(pTypeAttr);
			
			if (pTypeInfo)
				pTypeInfo->Release();
			if (pTypeLib)
				pTypeLib->Release();
			pTypeAttr = NULL;
			pTypeInfo = NULL;
			pTypeLib = NULL;
			if (sRtn.IsEmpty())
				sRtn = "";
			return sRtn;
		}

		sRtn += GetTypeInfoString(pTypeInfo, pTypeAttr);

		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeInfo->Release();
	}
	
	pTypeLib->Release();
	pTypeLib = NULL;
	return sRtn;
}

void CTWScriptEdit::UndoAction()
{
	if (!m_pUndoArr->GetSize())
		return;

	CUndoAction action = m_pUndoArr->GetAt(m_pUndoArr->GetSize() - 1);
	
	HideSelection(TRUE, FALSE);
	switch (action.nType)
	{
	case CUndoAction::eCut:
		SetSel(action.afterRange);
		ReplaceSel(action.sRemoveText);
		m_pUndoArr->RemoveAt(m_pUndoArr->GetSize() - 1);
		m_pRedoArr->Add(action);
		break;
	case CUndoAction::eKeyPaste:
	case CUndoAction::ePaste:
		SetSel(action.afterRange.cpMax, action.afterRange.cpMax - action.sAddText.GetLength());
		ReplaceSel(action.sRemoveText);
		m_pUndoArr->RemoveAt(m_pUndoArr->GetSize() - 1);
		m_pRedoArr->Add(action);
		break;
	}
	ProcessEmpty();
	
	HideSelection(FALSE, FALSE);
}

void CTWScriptEdit::RedoAction()
{
	if (!m_pRedoArr->GetSize())
		return;

	CUndoAction action = m_pRedoArr->GetAt(m_pRedoArr->GetSize() - 1);

	HideSelection(TRUE, FALSE);
	switch (action.nType)
	{
	case CUndoAction::eCut:
		SetSel(action.afterRange.cpMax, action.afterRange.cpMax + action.sRemoveText.GetLength());
		ReplaceSel("");
		m_pRedoArr->RemoveAt(m_pRedoArr->GetSize() - 1);
		m_pUndoArr->Add(action);
		break;
	case CUndoAction::eKeyPaste:
	case CUndoAction::ePaste:
		{
			int nFirstPos = action.afterRange.cpMax - action.sAddText.GetLength();
			SetSel(nFirstPos, nFirstPos + action.sRemoveText.GetLength());
			ReplaceSel(action.sAddText);
			m_pRedoArr->RemoveAt(m_pRedoArr->GetSize() - 1);
			m_pUndoArr->Add(action);
		}
		break;
	}
	ProcessEmpty();
	HideSelection(FALSE, FALSE);
}

void CTWScriptEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
{
	if (!bCanUndo)
		m_bUndoLock = true;
	CRichEditCtrl::ReplaceSel(lpszNewText, bCanUndo);
	if (!bCanUndo)
		m_bUndoLock = false;
}

void * CTWScriptEdit::SetUndoArray(void *pUndoArr)
{
	if (pUndoArr)
	{
		m_pUndoArr = (CArray <CUndoAction, CUndoAction> *)pUndoArr;
		return pUndoArr;
	}
	CArray <CUndoAction, CUndoAction> *pUndoArray = new CArray <CUndoAction, CUndoAction>;
	m_pUndoArr = pUndoArray;
	return (void*)pUndoArray;
}

void * CTWScriptEdit::SetRedoArray(void *pRedoArr)
{
	if (pRedoArr)
	{
		m_pRedoArr = (CArray <CUndoAction, CUndoAction> *)pRedoArr;
		return pRedoArr;
	}
	CArray <CUndoAction, CUndoAction> *pRedoArray = new CArray <CUndoAction, CUndoAction>;
	m_pRedoArr = pRedoArray;
	return (void*)pRedoArray;
}

void CTWScriptEdit::ProcessEmpty()
{
	CString sTmp;
	GetWindowText(sTmp);
	if (m_bEmpty && !sTmp.IsEmpty())
	{
		GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(1, 0), TRUE);	// In
		m_bEmpty = false;
	}
	else if (!m_bEmpty && sTmp.IsEmpty())
	{
		GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(1, 0), FALSE);	// Empty
		m_bEmpty = true;
	}
}

void CTWScriptEdit::OnVscroll() 
{
	return;
	if (m_nMaxFmRange < 0)
		return;
	int nStart = LineIndex(GetFirstVisibleLine()), nEnd = nStart;
	CRect rc;

	GetRect(&rc);
	nEnd = LineIndex(GetFirstVisibleLine() + rc.Height() / 15);
	if (nEnd < 0)
		nEnd = GetTextLength();
	else
		nEnd = min(GetTextLength(), nEnd);
	
	if (nEnd < m_nMaxFmRange)
		return;

	if (nStart <= m_scBeforeRange.cpMin && nEnd >= m_scBeforeRange.cpMin)
		nEnd = m_scBeforeRange.cpMin;

	if (nEnd >= m_scBeforeRange.cpMax && nStart <= m_scBeforeRange.cpMax)
		nStart = m_scBeforeRange.cpMax;


	if (nStart == nEnd)
		return;
	m_scBeforeRange.cpMin = nStart;
	m_scBeforeRange.cpMax = nEnd;
//TRACE("%d, %d\n", m_scBeforeRange.cpMin, m_scBeforeRange.cpMax);
	
	FormatTextRange(nStart, nEnd);

}

void CTWScriptEdit::NextFormat()
{
	return;
	if (m_nMaxFmRange < 0 || m_nMaxFmRange >= GetTextLength())
	{
		KillTimer(101);
		m_nMaxFmRange = -1;
	}
	int nStart = m_nMaxFmRange, nEnd = nStart;
	CRect rc;

	GetRect(&rc);
	nEnd = LineIndex(LineFromChar(nStart) + rc.Height() / 15);
	if (nEnd < 0)
		nEnd = GetTextLength();
	else
		nEnd = min(GetTextLength(), nEnd);
	
	m_nMaxFmRange = nEnd;

	FormatTextRange(nStart, nEnd);
}
