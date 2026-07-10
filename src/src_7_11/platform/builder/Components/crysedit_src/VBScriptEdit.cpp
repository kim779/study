// VBScriptEdit.cpp: implementation of the CVBScriptEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VBScriptEdit.h"
#include "editcmd.h"

#include "../../mainvar.h"
#include "../../keywords.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	IS_NOSYMBOL	9999

BEGIN_MESSAGE_MAP(CVBScriptEdit, CCrystalEditView)
	//{{AFX_MSG_MAP(CVBScriptEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_TAB, OnEditTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVBScriptEdit::CVBScriptEdit()
{
	m_buf.InitNew();
	m_buf.SetReadOnly(FALSE);

	m_strKeywords = m_strKeywordsLower = szKeywords;
	
	m_strConstants = m_strConstantsLower = szConstants;

	m_strKeywordsLower.MakeLower();
	m_strConstantsLower.MakeLower();

	m_pAutoDlg  = NULL;
	m_pAutoTip  = NULL;

	m_bEmpty      = TRUE;
	m_bPythonMode = false;

	m_sControlName.Empty();
}

CVBScriptEdit::~CVBScriptEdit()
{
	m_buf.FreeAll();

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

int CALLBACK EnumFontFamProc(
  ENUMLOGFONT FAR *lpelf,  // pointer to logical-font data
  NEWTEXTMETRIC FAR *lpntm,  // pointer to physical-font data
  int FontType,            // type of font
  LPARAM lParam            // pointer to application-defined data
)
{
    return lParam;
}

int CVBScriptEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCrystalEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	memset(&m_lf, 0, sizeof(m_lf));
	m_lf.lfHeight         = 14;
	m_lf.lfWeight         = FW_NORMAL;
	m_lf.lfCharSet        = HANGUL_CHARSET;
	m_lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	m_lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	m_lf.lfQuality        = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(m_lf.lfFaceName, _T("����ü"));

	CDC *pDC = GetDC();
	FONTENUMPROC pFunc = (FONTENUMPROC) EnumFontFamProc;
	if (EnumFontFamilies(pDC->m_hDC, "����ü", pFunc, 2) != 2)
	{
		m_lf.lfHeight         = 10;
		m_lf.lfCharSet        = ANSI_CHARSET;
		strcpy_s(m_lf.lfFaceName, "Courier");
	}
	ReleaseDC(pDC);

	SetFont(m_lf);

	LoadColors();
	
	SendMessage(WM_INITIALUPDATE, 0, 0);

	return 0;
}

CCrystalTextBuffer * CVBScriptEdit::LocateTextBuffer()
{
	return &m_buf;
}

// Python indentation is syntactic, so the Tab key must insert spaces here
// instead of a literal tab char (mixing tabs/spaces breaks Python parsing).
// VBScript mode is untouched - it still falls through to the base tab-char behavior.
void CVBScriptEdit::OnEditTab()
{
	if (!m_bPythonMode)
	{
		CCrystalEditView::OnEditTab();
		return;
	}

	if (! QueryEditable() || m_pTextBuffer == NULL)
		return;

	BOOL bTabify = FALSE;
	CPoint ptSelStart, ptSelEnd;
	if (IsSelection())
	{
		GetSelection(ptSelStart, ptSelEnd);
		bTabify = ptSelStart.y != ptSelEnd.y;
	}

	static const TCHAR pszText[] = _T("    ");

	if (bTabify)
	{
		m_pTextBuffer->BeginUndoGroup();

		int nStartLine = ptSelStart.y;
		int nEndLine = ptSelEnd.y;
		ptSelStart.x = 0;
		if (ptSelEnd.x > 0)
		{
			if (ptSelEnd.y == GetLineCount() - 1)
			{
				ptSelEnd.x = GetLineLength(ptSelEnd.y);
			}
			else
			{
				ptSelEnd.x = 0;
				ptSelEnd.y ++;
			}
		}
		else
			nEndLine --;
		SetSelection(ptSelStart, ptSelEnd);
		SetCursorPos(ptSelEnd);
		EnsureVisible(ptSelEnd);

		m_bHorzScrollBarLocked = TRUE;
		for (int L = nStartLine; L <= nEndLine; L ++)
		{
			int x, y;
			m_pTextBuffer->InsertText(this, L, 0, pszText, y, x, CE_ACTION_INDENT);
		}
		m_bHorzScrollBarLocked = FALSE;
		RecalcHorzScrollBar();

		m_pTextBuffer->FlushUndoGroup(this);
		return;
	}

	if (GetOverwriteMode())
	{
		CCrystalEditView::OnEditTab();
		return;
	}

	m_pTextBuffer->BeginUndoGroup();

	if (IsSelection())
	{
		ReplaceSelection(pszText);
	}
	else
	{
		CPoint ptCursorPos = GetCursorPos();
		ASSERT_VALIDTEXTPOS(ptCursorPos);

		int x, y;
		m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, pszText, y, x, CE_ACTION_TYPING);
		ptCursorPos.x = x;
		ptCursorPos.y = y;
		ASSERT_VALIDTEXTPOS(ptCursorPos);
		SetSelection(ptCursorPos, ptCursorPos);
		SetAnchor(ptCursorPos);
		SetCursorPos(ptCursorPos);
		EnsureVisible(ptCursorPos);
	}

	m_pTextBuffer->FlushUndoGroup(this);
}

void CVBScriptEdit::SetPythonMode(bool bPython)
{
	m_bPythonMode = bPython;
	if (bPython)
	{
		m_strKeywords  = m_strKeywordsLower  = szPythonKeywords;
		m_strConstants = m_strConstantsLower = szPythonConstants;
	}
	else
	{
		m_strKeywords  = m_strKeywordsLower  = szKeywords;
		m_strConstants = m_strConstantsLower = szConstants;
	}
	m_strKeywordsLower.MakeLower();
	m_strConstantsLower.MakeLower();
	InvalidateRect(NULL);
}

void CVBScriptEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();	
	CCrystalEditView::OnLButtonDown(nFlags, point);
}

void CVBScriptEdit::PostNcDestroy()
{

}

#define DEFINE_BLOCK(pos, colorindex)	\
	ASSERT((pos) >= 0 && (pos) <= nLength);\
	if (pBuf != NULL)\
	{\
		if (nActualItems == 0 || pBuf[nActualItems - 1].m_nCharPos <= (pos))\
		{\
			pBuf[nActualItems].m_nCharPos = (pos);\
			pBuf[nActualItems].m_nColorIndex = (colorindex);\
			nActualItems ++;\
		}\
	}

#define COOKIE_COMMENT			0x0001
#define COOKIE_SYMBOL			0x0002
#define COOKIE_EXT_COMMENT		0x0004
#define COOKIE_STRING			0x0008
#define COOKIE_CHAR			0x0010

DWORD CVBScriptEdit::ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems)
{
	int nLength = GetLineLength(nLineIndex);
	if (nLength <= 0)
		return dwCookie & COOKIE_EXT_COMMENT;

	LPCTSTR pszChars    = GetLineChars(nLineIndex);
	BOOL bFirstChar     = (dwCookie & ~COOKIE_EXT_COMMENT) == 0;
	BOOL bRedefineBlock = TRUE;
	BOOL bDecIndex  = FALSE;
	int nIdentBegin = -1;
	bool bHangul;

	int ii = 0;
	for (ii = 0; ; ii++)
	{
		bHangul = false;
		if ((BYTE)pszChars[ii] >= 0x80)
			bHangul = true;
		if (bRedefineBlock)
		{
			int nPos = ii;
			if (bDecIndex)
				nPos--;

			if (dwCookie & (COOKIE_COMMENT | COOKIE_EXT_COMMENT))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_COMMENT);
			}
			else if (dwCookie & (COOKIE_CHAR | COOKIE_STRING))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_STRING);
			}
			else
			{
				DEFINE_BLOCK(nPos, COLORINDEX_NORMALTEXT);
			}

			bRedefineBlock = FALSE;
			bDecIndex      = FALSE;
		}

		if (ii == nLength)
			break;

		if (dwCookie & COOKIE_COMMENT)
		{
			DEFINE_BLOCK(ii, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			break;
		}

		//	String constant "...."
		if (dwCookie & COOKIE_STRING)
		{
			if (pszChars[ii] == '"')
			{
				dwCookie &= ~COOKIE_STRING;
				bRedefineBlock = TRUE;
			}
			continue;
		}
		//	Comment: ' for VBScript, # for Python
		if ((!m_bPythonMode && pszChars[ii] == '\'') ||
		    ( m_bPythonMode && pszChars[ii] == '#'))
		{
			DEFINE_BLOCK(ii, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			break;
		}

		//	Normal text
		if (pszChars[ii] == '"')
		{
			DEFINE_BLOCK(ii, COLORINDEX_STRING);
			dwCookie |= COOKIE_STRING;
			continue;
		}

		if (bFirstChar)
		{
			if (! isspace(pszChars[ii]))
				bFirstChar = FALSE;
		}

		if (pBuf == NULL)
			continue;

		if (bHangul)
		{
			ii++;
			continue;
		}

		if (isalnum(pszChars[ii]) || pszChars[ii] == '_')
		{
			if (nIdentBegin == -1)
				nIdentBegin = ii;
		}
		else
		{
			if (nIdentBegin >= 0)
			{
				int nPos = IsVBSKeyword(pszChars + nIdentBegin, ii - nIdentBegin);
				if (nPos >= 0)
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
					LPTSTR str = m_buf.GetLineChars(nLineIndex);
					CopyMemory(str + nIdentBegin, m_strKeywords.Mid(nPos + 1, ii - nIdentBegin), ii - nIdentBegin);
				}

				nPos = IsVBSNumber(pszChars + nIdentBegin, ii - nIdentBegin);
				if (nPos >= 0)
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_CONSTANT);
					LPTSTR str = m_buf.GetLineChars(nLineIndex);
					CopyMemory(str + nIdentBegin, m_strConstants.Mid(nPos + 1, ii - nIdentBegin), ii - nIdentBegin);
				}

				CString sSymbol = CString(pszChars + nIdentBegin, ii - nIdentBegin);
				nPos = IsSymbol(sSymbol);
				if (LOWORD(nPos) < IS_NOSYMBOL)
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_SYMBOL);
					LPTSTR str = m_buf.GetLineChars(nLineIndex);
					CopyMemory(str + nIdentBegin, sSymbol, ii - nIdentBegin);
				}

				if (nIdentBegin > 1 && pszChars[nIdentBegin - 1] == '.')
				{
					sSymbol = GetFrontWord(pszChars, nIdentBegin - 2) + "." + sSymbol;
					if (IsInSymbol(sSymbol))
					{
						DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
					}
				}

				bRedefineBlock = TRUE;
				bDecIndex      = TRUE;
				nIdentBegin    = -1;
			}
		}
	}

	if (nIdentBegin >= 0)
	{
		int nPos = IsVBSKeyword(pszChars + nIdentBegin, ii - nIdentBegin);
		if (nPos >= 0)
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
			LPTSTR str = m_buf.GetLineChars(nLineIndex);
			CopyMemory(str + nIdentBegin, m_strKeywords.Mid(nPos + 1, ii - nIdentBegin), ii - nIdentBegin);
		}
		
		nPos = IsVBSNumber(pszChars + nIdentBegin, ii - nIdentBegin);
		if (nPos >= 0)
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_CONSTANT);
			LPTSTR str = m_buf.GetLineChars(nLineIndex);
			CopyMemory(str + nIdentBegin, m_strConstants.Mid(nPos + 1, ii - nIdentBegin), ii - nIdentBegin);
		}

		CString sSymbol = CString(pszChars + nIdentBegin, ii - nIdentBegin);
		nPos = IsSymbol(sSymbol);
		if (LOWORD(nPos) < IS_NOSYMBOL)
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_SYMBOL);
			LPTSTR str = m_buf.GetLineChars(nLineIndex);
			CopyMemory(str + nIdentBegin, sSymbol, ii - nIdentBegin);
		}

		if (nIdentBegin > 1 && pszChars[nIdentBegin - 1] == '.')
		{
			sSymbol = GetFrontWord(pszChars, nIdentBegin - 2) + "." + sSymbol;
			if (IsInSymbol(sSymbol))
			{
				DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
			}
		}
	}

	dwCookie &= COOKIE_EXT_COMMENT;
	return dwCookie;
}

int CVBScriptEdit::IsVBSKeyword(LPCTSTR pszChars, int nLength)
{
	CString strSymbol; strSymbol.Format(" %s ", CString(pszChars, nLength));
	strSymbol.MakeLower();
	return m_strKeywordsLower.Find(strSymbol);
}

int CVBScriptEdit::IsVBSNumber(LPCTSTR pszChars, int nLength)
{
	CString strSymbol; strSymbol.Format(" %s ", CString(pszChars, nLength));
	strSymbol.MakeLower();
	return m_strConstantsLower.Find(strSymbol);
}

CString CVBScriptEdit::GetScriptText()
{
	CString strResult = m_buf.ReadText();
	return strResult;
}

void CVBScriptEdit::SetScriptText(CString sText)
{
	m_buf.WriteText(sText);
	if (sText.IsEmpty())
		m_bEmpty = TRUE;
	ProcessEmpty();
	Invalidate();
}

void CVBScriptEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == '.')
	{
		LPCTSTR pszChars = GetLineChars(GetCursorPos().y);

		CString sSymbol = GetFrontWord(pszChars, GetCursorPos().x - 1);
		int nPos = IsSymbol(sSymbol);
		showList(nPos);
	}

	if (nChar == '(' || nChar == ' ')
	{
		LPCTSTR pszChars = GetLineChars(GetCursorPos().y);

		CString sSymbol = GetFrontWord(pszChars, GetCursorPos().x - 1);
		int nPos = GetCursorPos().x - sSymbol.GetLength() - 1;
		if (nPos > 0 && pszChars[nPos] == '.')
		{
			sSymbol = GetFrontWord(pszChars, nPos - 1) + "." + sSymbol;
			showTip(sSymbol);
		}
	}

	if (nChar == '\'')
	{
		if (OnComment())
			return;
	}
	CCrystalEditView::OnChar(nChar, nRepCnt, nFlags);
	ProcessEmpty();
}

CString CVBScriptEdit::GetFrontWord(LPCTSTR pszChars, int nPos)
{
	if (nPos <= 0)
		return "";
	int nStartPos = nPos;
	while (nStartPos >= 0)
	{
		if (pszChars[nStartPos] != '\t' && (_istalnum(pszChars[nStartPos]) || pszChars[nStartPos] == '_'))
		{
			if (nStartPos == 0)
				break;
			nStartPos--;
		}
		else
		{
			nStartPos++;
			break;
		}
	}
	return CString(pszChars + nStartPos, nPos - nStartPos + 1);
}

CString CVBScriptEdit::Parser(CString &srcstr, CString substr)
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


void CVBScriptEdit::Initialize()
{
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

void CVBScriptEdit::Initialize(_mapH *mapH)
{
	m_mapH = mapH;
}

CString CVBScriptEdit::GetLineString(int nLine)
{
	CString sStr = GetLineChars(nLine);
	sStr.SetAt(GetLineLength(nLine), '\0');
	sStr.ReleaseBuffer();
	return sStr;
}

int CVBScriptEdit::GetLineCnt()
{
	return GetLineCount();
}

void CVBScriptEdit::hidePopup()
{
	if (m_pAutoDlg && m_pAutoDlg->IsWindowVisible())
		m_pAutoDlg->ShowWindow(SW_HIDE);
	if (m_pAutoTip && m_pAutoTip->IsWindowVisible())
		m_pAutoTip->ShowWindow(SW_HIDE);
}

BOOL CVBScriptEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta < 0)
	{
		SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
		SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
	}
	else
	{
		SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
		SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
	}
	return CCrystalEditView::OnMouseWheel(nFlags, zDelta, pt);
}

int CVBScriptEdit::IsSymbol(CString &sym)
{
	CFormItem	formItem(m_mapH);

	void *rtValue;
	sym.MakeUpper();
	if (m_ScriptStrMap.Lookup(sym, rtValue))
	{
		// PUBLIC.INI registers global engine objects (Screen, System, ...) in all-caps,
		// which is harmless for case-insensitive VBScript but breaks Python's case-sensitive
		// globals - Screen.cpp's AddObject() registers these exact mixed-case names, so in
		// Python mode recase to match instead of forcing all-caps.
		if (m_bPythonMode)
		{
			static const struct { LPCTSTR upper; LPCTSTR exact; } pyGlobals[] = {
				{ "SCREEN", "Screen" }, { "SYSTEM", "System" }, { "LOGIN", "Login" }, { "LEDGER", "Ledger" },
			};
			for (int gg = 0; gg < sizeof(pyGlobals) / sizeof(pyGlobals[0]); gg++)
			{
				if (sym == pyGlobals[gg].upper)
				{
					sym = pyGlobals[gg].exact;
					break;
				}
			}
		}
		return MAKELONG((int)rtValue, 0);
	}

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
	} while(!tmp.IsEmpty());
	
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

void CVBScriptEdit::showList(int kind)
{
	CString info = "";
	CRect rc;
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
	
	CPoint pos = GetCursorPos();
	pos = TextToClient(pos);

	rc.SetRect(pos.x, pos.y + 16, 0, 0);
	rc.right = rc.left + m_pAutoDlg->getWidth();
	rc.bottom = rc.top + 150;

	ClientToScreen(&rc);
	if (!m_pAutoDlg->IsEmpty())
		m_pAutoDlg->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
}

void CVBScriptEdit::showTip(CString sKey)
{
	CRect rc;
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

	CPoint pos = GetCursorPos();
	m_pAutoTip->SetWindowText(sTipText);
	m_pAutoTip->m_nStart = pos.x;

	pos = TextToClient(pos);

	rc.SetRect(pos.x, pos.y - 18, 0, 0);
	rc.right = rc.left + m_pAutoTip->getWidth() + 10;
	rc.bottom = rc.top + 18;

	ClientToScreen(&rc);
	m_pAutoTip->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
	m_pAutoTip->RedrawWindow();
}

CString CVBScriptEdit::GetBaseControlInfo(int kind)
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

CString CVBScriptEdit::GetControlInfo(CString str)
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

CString CVBScriptEdit::GetTypeInfoString(ITypeInfo *pTypeInfo, TYPEATTR *pTypeAttr)
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

				if (pFuncDesc->lprgelemdescParam)
				{
					for (int nCount = 1; nCount < (int)cNames; nCount++)
					{
						CString sParam(pBstr[nCount]);
						sTip += GetVtType(pFuncDesc->lprgelemdescParam[nCount - 1].tdesc.vt) + " ";
						sTip += sParam;
						if (nCount != (int)cNames - 1)
							sTip += ", ";
						SysFreeString(pBstr[nCount]);
					}
				}
/*				else if (cNames > 1)
				{
					pTypeInfo->GetNames(jj, pBstr, 1, &cNames);
					CString	str(*pBstr);
					TRACE("%s\n", str);
				}
*/
				sRtn += sTip + ")\t";
				pTypeInfo->ReleaseFuncDesc(pFuncDesc);
			}
			SysFreeString(*pBstr);
			delete pBstr;
		}
	}
	return sRtn;
}

CString CVBScriptEdit::GetVtType(VARTYPE var)
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

BOOL CVBScriptEdit::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (m_pAutoDlg->IsWindowVisible())
		{
			switch (pMsg->wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_ESCAPE:
				m_pAutoDlg->ShowWindow(SW_HIDE);
				break;
			case VK_BACK:
				{
					CPoint pos = GetCursorPos();
					LPCTSTR pszChars = GetLineChars(pos.y);
					if (pszChars[pos.x - 1] == '.')
						m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				break;
			case VK_UP:
			case VK_DOWN:
			case VK_HOME:
			case VK_END:
			case VK_PRIOR:
			case VK_NEXT:
				m_pAutoDlg->m_aclist.SendMessage(WM_KEYDOWN, pMsg->wParam, 0);
				return TRUE;
				break;
			case VK_RETURN:
				{
					CPoint pos = GetCursorPos();
					LPCTSTR pszChars = GetLineChars(pos.y);
					if (!m_pAutoDlg->getSelString().IsEmpty())
					{
						MSG msg;
						PeekMessage(&msg, 0, 0, 0, PM_REMOVE);

						do
						{
							if (pszChars[pos.x] == '.')
								break;
							pos.x--;
						} while(pos.x >= 0);
						
						if (pos.x > 0)
						{
							int nEndLine, nEndChar;
							if (pos.x + 1 < GetCursorPos().x)
								m_buf.DeleteText(this, pos.y, pos.x + 1, pos.y, GetCursorPos().x);
							m_buf.InsertText(this, pos.y, pos.x + 1, m_pAutoDlg->getSelString(), nEndLine, nEndChar);
							SetCursorPos(CPoint(nEndChar, nEndLine));
						}
					}
					m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				return TRUE;
			default:
				if (pMsg->wParam >= '!' && (!_istalpha(pMsg->wParam) && (char)pMsg->wParam != '_' && ((char)pMsg->wParam != '9' && (::GetKeyState(VK_SHIFT) & 0x0800))))
					m_pAutoDlg->ShowWindow(SW_HIDE);
				else if ((char)pMsg->wParam == ' ' || ((char)pMsg->wParam == '9' && (::GetKeyState(VK_SHIFT) & 0x0800)))
				{
					CPoint pos = GetCursorPos();
					LPCTSTR pszChars = GetLineChars(pos.y);
					if (!m_pAutoDlg->getSelString().IsEmpty())
					{
						do
						{
							if (pszChars[pos.x] == '.')
								break;
							pos.x--;
						} while(pos.x >= 0);
						
						if (pos.x > 0)
						{
							int nEndLine, nEndChar;
							if (pos.x + 1 < GetCursorPos().x)
								m_buf.DeleteText(this, pos.y, pos.x + 1, pos.y, GetCursorPos().x);
							m_buf.InsertText(this, pos.y, pos.x + 1, m_pAutoDlg->getSelString(), nEndLine, nEndChar);
							SetCursorPos(CPoint(nEndChar, nEndLine));
						}
					}
					m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				else if (pMsg->wParam >= '!')	// maybe ascii
				{
					CPoint pos = GetCursorPos();
					CString txt = GetLineChars(pos.y);
					txt = txt.Left(pos.x);
					txt = txt.Mid(txt.ReverseFind('.') + 1);
					txt += (char)pMsg->wParam;
					m_pAutoDlg->FindString(txt);
				}
				break;
			}
		}
		if (m_pAutoTip->IsWindowVisible())
		{
			CString sTip;
			m_pAutoTip->GetWindowText(sTip);

			switch(pMsg->wParam)
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
					CString txt = GetLineChars(GetCursorPos().y);
					
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
					CString txt = GetLineChars(GetCursorPos().y);
					
					if (m_pAutoTip->m_nStart >= GetCursorPos().x - (pMsg->wParam==VK_LEFT?1:0))
					{
						m_pAutoTip->ShowWindow(SW_HIDE);
						break;
					}
					if (::GetKeyState(VK_SHIFT) & 0x0800)
					{
						break;
					}
				
					if (txt.GetLength() >= GetCursorPos().x + (pMsg->wParam==VK_RIGHT?1:0))
						txt = txt.Mid(m_pAutoTip->m_nStart, GetCursorPos().x + (pMsg->wParam==VK_RIGHT?1:0) - m_pAutoTip->m_nStart);
					else
						break;

					if (txt.Find(")") != -1)
					{
						m_pAutoTip->ShowWindow(SW_HIDE);
					}
				}
				break;
			case VK_DELETE:
			case VK_BACK:
				{
					/*CPoint start, end;
					GetSelection(start, end);
					long nStart = start.x, nEnd = end.x;
					
					if (m_pAutoTip->m_nStart >= nStart - (nStart == nEnd?1:0))
						m_pAutoTip->ShowWindow(SW_HIDE);*/
				}
				break;
			default:
				if ((char)pMsg->wParam == '0' && (::GetKeyState(VK_SHIFT) & 0x0800))
					m_pAutoTip->ShowWindow(SW_HIDE);
				if ((char)pMsg->wParam == ' ')
				{
					CString txt = GetLineChars(GetCursorPos().y);
					if (m_pAutoTip->m_nStart < GetCursorPos().x && txt.GetLength() >= GetCursorPos().x)
					txt = txt.Mid(m_pAutoTip->m_nStart, GetCursorPos().x - m_pAutoTip->m_nStart);
					
					if (txt.Replace(",", ",") >= sTip.Replace(",", ",") || m_pAutoTip->m_nStart >= GetCursorPos().x)
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
				}
				break;
			}
		}
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (m_pAutoDlg->IsWindowVisible())
			m_pAutoDlg->ShowWindow(SW_HIDE);
		if (m_pAutoTip->IsWindowVisible())
			m_pAutoTip->ShowWindow(SW_HIDE);
	}
	
	BOOL bRet =  CCrystalEditView::PreTranslateMessage(pMsg);
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_BACK || pMsg->wParam == VK_DELETE)
			ProcessEmpty();
	return bRet;
}

BOOL CVBScriptEdit::OnComment()
{
	if (! QueryEditable() || m_pTextBuffer == NULL)
		return FALSE;

	BOOL bTabify = FALSE;
	CPoint ptSelStart, ptSelEnd;
	if (IsSelection())
	{
		GetSelection(ptSelStart, ptSelEnd);
		bTabify = ptSelStart.y != ptSelEnd.y;
	}

	if (bTabify)
	{
		m_pTextBuffer->BeginUndoGroup();

		int nStartLine = ptSelStart.y;
		int nEndLine = ptSelEnd.y;
		ptSelStart.x = 0;
		if (ptSelEnd.x > 0)
		{
			if (ptSelEnd.y == GetLineCount() - 1)
			{
				ptSelEnd.x = GetLineLength(ptSelEnd.y);
			}
			else
			{
				ptSelEnd.x = 0;
				ptSelEnd.y ++;
			}
		}
		else
			nEndLine --;
		SetSelection(ptSelStart, ptSelEnd);
		SetCursorPos(ptSelEnd);
		EnsureVisible(ptSelEnd);

		//	Shift selection to right
		m_bHorzScrollBarLocked = TRUE;
		static const TCHAR pszText[] = _T("\'");
		for (int L = nStartLine; L <= nEndLine; L ++)
		{
			int x, y;
			LPCTSTR pszChars = GetLineChars(L);
			if (pszChars[0] == '\'')
				m_pTextBuffer->DeleteText(this, L, 0, L, 1, CE_ACTION_INDENT);
			else
				m_pTextBuffer->InsertText(this, L, 0, pszText, y, x, CE_ACTION_INDENT); //	[JRT]
		}
		m_bHorzScrollBarLocked = FALSE;
		RecalcHorzScrollBar();

		m_pTextBuffer->FlushUndoGroup(this);
		return TRUE;
	}
	return FALSE;
}

void CVBScriptEdit::ProcessEmpty()
{
	if (m_mapH && m_mapH->mapK == MK_PROCEDURES)
		return;
	if (m_bEmpty && (GetLineCount() != 1 || GetLineLength(0) != 0))
	{
		GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(1, 0), TRUE);	// In
		m_bEmpty = false;
	}
	else if (!m_bEmpty && GetLineCount() == 1 && GetLineLength(0) == 0)
	{
		GetParent()->SendMessage(WM_USER + 100, MAKEWPARAM(1, 0), FALSE);	// Empty
		m_bEmpty = true;
	}
}

BOOL CVBScriptEdit::IsInSymbol(CString sKey)
{
	CRect rc;
	CString sSym = Parser(sKey, "."), sTipText = "", info = "";
	BOOL	bRet = FALSE;

	if (sKey.IsEmpty())	// VB keyword
	{
		return FALSE;
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
			if (!info.IsEmpty())
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
						bRet = TRUE;
						break;
					}
				}while(!info.IsEmpty());
			}
		}
	}
	return bRet;
}

COLORREF CVBScriptEdit::GetColor(int nColorIndex)
{
	switch (nColorIndex)
	{
	case COLORINDEX_WHITESPACE:
	case COLORINDEX_BKGND:
		if (IsWindowEnabled())
			return m_clBkGnd;
		else
			return ::GetSysColor(COLOR_INACTIVEBORDER);
	case COLORINDEX_NORMALTEXT:
		return m_clText;
	case COLORINDEX_SELMARGIN:
		return ::GetSysColor(COLOR_SCROLLBAR);
	case COLORINDEX_SYMBOL:
		return m_clSymbol;
	case COLORINDEX_COMMENT:
		return m_clComment;
	case COLORINDEX_CONSTANT:
		return m_clConstant;
	case COLORINDEX_OPERATOR:
		return RGB(0x00, 0x00, 0x00);
	case COLORINDEX_KEYWORD:
		return m_clKeyword;
	case COLORINDEX_SELBKGND:
		return RGB(0, 0, 0);
	case COLORINDEX_SELTEXT:
		return RGB(255, 255, 255);
	}
	return m_clString;
}

void CVBScriptEdit::LoadColors()
{
	CString currentPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	m_clBkGnd    = GetPrivateProfileInt("SCRIPT", "CLRBKGND", ::GetSysColor(COLOR_WINDOW), currentPath + "\\workshop.ini");
	m_clText     = GetPrivateProfileInt("SCRIPT", "CLRNORMAL", ::GetSysColor(COLOR_WINDOWTEXT), currentPath + "\\workshop.ini");
	m_clSymbol   = GetPrivateProfileInt("SCRIPT", "CLRSYMBOL", RGB(0, 150, 180), currentPath + "\\workshop.ini");
	m_clComment  = GetPrivateProfileInt("SCRIPT", "CLRCOMMENT", RGB(0, 128, 0), currentPath + "\\workshop.ini");
	m_clConstant = GetPrivateProfileInt("SCRIPT", "CLRCONSTANT", RGB(0x80, 0x00, 0x60), currentPath + "\\workshop.ini");
	m_clKeyword  = GetPrivateProfileInt("SCRIPT", "CLRKEYWORD", RGB(0, 0, 255), currentPath + "\\workshop.ini");
	m_clString   = GetPrivateProfileInt("SCRIPT", "CLRSTRING", RGB(255, 0, 0), currentPath + "\\workshop.ini");
}

void CVBScriptEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CCrystalEditView::OnKillFocus(pNewWnd);
	hidePopup();	
}

void CVBScriptEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CCrystalEditView::OnRButtonDown(nFlags, point);

	CMenu menu;
	menu.CreatePopupMenu();
	::GetCursorPos(&point);
	
	menu.AppendMenu(MF_STRING, ID_EDIT_COPY, "Copy");
	menu.AppendMenu(MF_STRING, ID_EDIT_CUT, "Cut");
	menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, "Paste");
	
	if (!IsSelection())
		menu.EnableMenuItem(MF_BYCOMMAND | ID_EDIT_CUT, MF_GRAYED);
	
	if (!TextInClipboard())
		menu.EnableMenuItem(MF_BYCOMMAND | ID_EDIT_PASTE, MF_GRAYED);
	
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, this);
}

void CVBScriptEdit::SetSel(CPoint startPnt, CPoint endPnt)
{
	SetSelection(startPnt, endPnt);
	SetCursorPos(endPnt);
}

