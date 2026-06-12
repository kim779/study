// VBScriptEdit.cpp: implementation of the CVBScriptEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VBScriptEdit.h"

#include "h/mainvar.h"
#include "h/keywords.h"
#include "editcmd.h"
#include "h\mapform.H"
#include "awTool\awObjectLoad.h"
#include "h\formOb.h"

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
	m_AutoListCntObj.RemoveAll();
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
	return (int)lParam;
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
	strcpy_s(m_lf.lfFaceName, LF_FACESIZE, _T("����ü"));

	CDC *pDC = GetDC();
	FONTENUMPROC pFunc = (FONTENUMPROC) EnumFontFamProc;
	if (EnumFontFamilies(pDC->m_hDC, _T("����ü"), pFunc, 2) != 2)
	{
		m_lf.lfHeight         = 10;
		m_lf.lfCharSet        = ANSI_CHARSET;
		strcpy_s(m_lf.lfFaceName, LF_FACESIZE, _T("Courier"));
	}
	ReleaseDC(pDC);

	SetFont(m_lf);
	LoadColors();
	SendMessage(WM_INITIALUPDATE, 0, 0);

	return 0;
}

CCrystalTextBuffer* CVBScriptEdit::LocateTextBuffer()
{
	return &m_buf;
}

void CVBScriptEdit::SetPythonMode(bool bPython)
{
	m_bPythonMode = bPython;
	if (bPython) {
		m_strKeywords   = m_strKeywordsLower   = szPythonKeywords;
		m_strConstants  = m_strConstantsLower  = szPythonConstants;
	} else {
		void* pVal = (void *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_MEDIA);
		BYTE byteMedia = MEDIA_HTS;
		if (pVal) memcpy(&byteMedia, pVal, sizeof(BYTE));
		m_strKeywords = m_strKeywordsLower = (byteMedia == MEDIA_HTS) ? szKeywords : szLuaKeywords;
		m_strConstants = m_strConstantsLower = szConstants;
	}
	m_strKeywordsLower.MakeLower();
	m_strConstantsLower.MakeLower();
	InvalidateRect(NULL);
}

void CVBScriptEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	if (!m_bPythonMode) {
		void* pVal = (void *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_MEDIA);
		BYTE byteMedia;
		memcpy(&byteMedia, pVal, sizeof(BYTE));
		m_strKeywords = m_strKeywordsLower = (byteMedia == MEDIA_HTS) ? szKeywords : szLuaKeywords;
		m_strKeywordsLower.MakeLower();
	}
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

#define COOKIE_COMMENT		0x0001
#define COOKIE_SYMBOL		0x0002
#define COOKIE_EXT_COMMENT	0x0004
#define COOKIE_STRING		0x0008
#define COOKIE_CHAR		0x0010
#define COOKIE_FUNCTION		0x0020

DWORD CVBScriptEdit::ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems)
{
	int	nLength = GetLineLength(nLineIndex);
	if (nLength <= 0)
		return dwCookie & COOKIE_EXT_COMMENT;

	LPCTSTR	pszChars   = GetLineChars(nLineIndex);
	BOOL	bFirstChar = (dwCookie & ~COOKIE_EXT_COMMENT) == 0;
	BOOL	bRedefineBlock = TRUE;
	BOOL	bDecIndex  = FALSE;
	int	ii, nIdentBegin = -1;
	bool	bHangul;

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

		if (dwCookie & COOKIE_FUNCTION)
		{
			DEFINE_BLOCK(ii, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			break;
		}

		// String constant "...."
		if (dwCookie & COOKIE_STRING)
		{
			if (pszChars[ii] == _T('"'))
			{
				dwCookie &= ~COOKIE_STRING;
				bRedefineBlock = TRUE;
			}
			continue;
		}
		// Comment: ' for VBScript, # for Python
		if ((!m_bPythonMode && pszChars[ii] == _T('\'')) ||
		    ( m_bPythonMode && pszChars[ii] == _T('#')))
		{
			DEFINE_BLOCK(ii, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			break;
		}

		// See All Script Event Color 
		if (ii + 1 < nLength && pszChars[ii] == _T('@') && pszChars[ii + 1] == _T('@'))
		{
			DEFINE_BLOCK(ii, COLORINDEX_GRAYLINE);
			dwCookie |= COOKIE_FUNCTION;
			break;
		}

		// Normal text
		if (pszChars[ii] == _T('"'))
		{
			DEFINE_BLOCK(ii, COLORINDEX_STRING);
			dwCookie |= COOKIE_STRING;
			continue;
		}

		if (bFirstChar)
		{
			if (!(pszChars[ii] & 0x80) && !isspace(pszChars[ii]))
				bFirstChar = FALSE;
		}

		if (pBuf == NULL)
			continue;

		if (bHangul)
		{
			ii++;
			continue;
		}

		if (isalnum(pszChars[ii]) || pszChars[ii] == _T('_'))
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

				if (nIdentBegin > 1 && pszChars[nIdentBegin - 1] == _T('.'))
				{
					sSymbol = GetFrontWord(pszChars, nIdentBegin - 2) + _T(".") + sSymbol;
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

		if (nIdentBegin > 1 && pszChars[nIdentBegin - 1] == _T('.'))
		{
			sSymbol = GetFrontWord(pszChars, nIdentBegin - 2) + _T(".") + sSymbol;
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
	CString strSymbol;
	
	strSymbol.Format(_T(" %s "), CString(pszChars, nLength));
	strSymbol.MakeLower();
	return m_strKeywordsLower.Find(strSymbol);
}

int CVBScriptEdit::IsVBSNumber(LPCTSTR pszChars, int nLength)
{
	CString strSymbol;
	
	strSymbol.Format(_T(" %s "), CString(pszChars, nLength));
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
	if (nChar == _T('.'))
	{
		LPCTSTR pszChars = GetLineChars(GetCursorPos().y);

		CString sSymbol = GetFrontWord(pszChars, GetCursorPos().x - 1);
		int nPos = IsSymbol(sSymbol);
		showList(nPos);
	}

	if (nChar == _T('(') || nChar == _T(' '))
	{
		LPCTSTR pszChars = GetLineChars(GetCursorPos().y);

		CString sSymbol = GetFrontWord(pszChars, GetCursorPos().x - 1);
		int nPos = GetCursorPos().x - sSymbol.GetLength() - 1;
		if (nPos > 0 && pszChars[nPos] == _T('.'))
		{
			sSymbol = GetFrontWord(pszChars, nPos - 1) + _T(".") + sSymbol;
			showTip(sSymbol);
		}
	}

	if (nChar == _T('\''))
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
		return _T("");

	int nStartPos = nPos;
	while (nStartPos >= 0)
	{
		if (pszChars[nStartPos] != _T('\t') && (_istalnum(pszChars[nStartPos]) || pszChars[nStartPos] == _T('_')))
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
	return _T("");
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

void CVBScriptEdit::Initialize(mapForm *mapForm)
{
	m_mapH = mapForm;
}

CString CVBScriptEdit::GetLineString(int nLine)
{
	CString sStr = GetLineChars(nLine);
	sStr.SetAt(GetLineLength(nLine), _T('\0'));
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
	void *rtValue;
	sym.MakeUpper();
	if (m_ScriptStrMap.Lookup(sym, rtValue))
		return (int)rtValue;

	CString tmp = m_mapH->publicR;
	do
	{
		CString temp = Parser(tmp, _T(";"));
		if (!temp.IsEmpty() && !sym.CompareNoCase(temp))
		{
			void* addedID;
			if (m_publicStrMap.Lookup(sym, addedID))
				return (int)addedID;
		}
	} while(!tmp.IsEmpty());
	
	for (int kk = 0; kk < m_mapH->forms.GetCount(); kk++)
	{
		CformProp* Prop = ((CObjectLoad*)m_mapH->forms.GetAt(kk))->getProp();
		CString str = Prop->name;
		if (atoi(str))
			str = _T("S") + str;
		if (!sym.CompareNoCase(str))
		{
			sym = str;
			m_sControlName.Empty();
			if (Prop->kind == fmOBJECT)
				m_sControlName = Prop->strings;

				return MAKELONG(Prop->kind, Prop->charType);
		}
	}
	
	return MAKELONG(IS_NOSYMBOL, 0);
}

void CVBScriptEdit::showList(int kind)
{
	CString info;
	CRect	rc;
	int	type = HIWORD(kind);

	kind = LOWORD(kind);
	m_pAutoDlg->Reset();
	
	info = GetBaseControlInfo(kind);
	if (/*m_AutoListMap.Lookup(kind, info)*/!info.IsEmpty() || !m_sControlName.IsEmpty())
	{
		if (kind == fmOBJECT && !m_sControlName.IsEmpty())
			info = GetControlInfo(m_sControlName);

		do
		{
			CString temp = Parser(info, _T("\t"));
			CString text = Parser(temp, _T(":"));
			int isProp = atoi(temp);
			if (!text.IsEmpty())
				m_pAutoDlg->AddString(text, isProp);
		} while(!info.IsEmpty());
	}
	
	CPoint pos = GetCursorPos();
	pos = TextToClient(pos);

	rc.SetRect(pos.x, pos.y + 16, 0, 0);
	rc.right = rc.left + m_pAutoDlg->GetWidth();
	rc.bottom = rc.top + 150;

	ClientToScreen(&rc);
	if (!m_pAutoDlg->IsEmpty())
		m_pAutoDlg->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
}

void CVBScriptEdit::showTip(CString sKey)
{
	CRect rc;
	CString sSym = Parser(sKey, _T(".")), sTipText = _T(""), info = _T("");

	if (sKey.IsEmpty())	// VB keyword
		return;
	else
	{
		sKey.TrimRight();
		int nPos = IsSymbol(sSym);
		if (LOWORD(nPos) < IS_NOSYMBOL) // symbol valid
		{
			// search keyword
			int type = HIWORD(nPos), kind = LOWORD(nPos);
			info = GetBaseControlInfo(kind);
			if (/*m_AutoListMap.Lookup(kind, info)*/!info.IsEmpty())
			{
				if (kind == fmOBJECT && !m_sControlName.IsEmpty())
					info = GetControlInfo(m_sControlName);
				do
				{
					CString temp = Parser(info, _T("\t"));
					CString text = Parser(temp, _T(":"));
					int isProp = atoi(Parser(temp, _T(":")));
					CString sTip = Parser(temp, _T(";"));
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
	m_pAutoTip->m_start = pos.x;

	pos = TextToClient(pos);

	rc.SetRect(pos.x, pos.y - 18, 0, 0);
	rc.right = rc.left + m_pAutoTip->GetWidth() + 10;
	rc.bottom = rc.top + 18;

	ClientToScreen(&rc);
	m_pAutoTip->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
	m_pAutoTip->RedrawWindow();
}

CString CVBScriptEdit::GetBaseControlInfo(int kind, int index)
{
	CString sControlName, sRtn = _T("");

	if (kind == fmOBJECT)
	{
		CformProp* Prop = ((CObjectLoad*)m_mapH->forms.GetAt(index))->getProp();
		if (m_AutoListCntObj.Lookup(Prop->iCtmIdx, sRtn))
			return sRtn;
	}
	else
	{
		if (m_AutoListMap.Lookup(kind, sRtn))
			return sRtn;
	}

	switch (kind)
	{
	case fmLABEL:		sControlName = _T("IfmLabel");	break;
	case fmBOX:		sControlName = _T("IfmBox");	break;
	case fmGROUP:		sControlName = _T("IfmGroup");	break;
	case fmIMVIEW:		sControlName = _T("IfmPanel");	break;
	case fmBUTTON:		sControlName = _T("IfmButton");	break;
	case fmEDIT:		sControlName = _T("IfmEdit");	break;
	case fmCOMBO:		sControlName = _T("IfmCombo");	break;
	case fmOUTPUT:		sControlName = _T("IfmOut");	break;
	case fmMEMO:		sControlName = _T("IfmMemo");	break;
	case fmGRID:		sControlName = _T("IfmGrid");	break;
	case fmGRIDEX:		sControlName = _T("IfmGridEx");	break;
	case fmTABLE:		sControlName = _T("IfmTable");	break;
	case fmTREE:		sControlName = _T("IfmTreeView");break;
	case fmFORM:		sControlName = _T("IfmForm");	break;
	case fmTAB:		sControlName = _T("IfmTab");	break;
	case fmBROWSER:		sControlName = _T("IfmBrowser");break;
	case fmRADIO:		sControlName = _T("IfmRadio");	break;	// radio
	case fmCHECK:		sControlName = _T("IfmCheck");	break;	// check  Derived from radio
	default:		sControlName = _T("IfmSysm");	break;
	}

	ITypeLib *pTypeLib = NULL;
	ITypeInfo *pTypeInfo = NULL;
	TYPEATTR *pTypeAttr = NULL;

	BSTR bstr;
	HRESULT hr;

	CString sRoot = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	sRoot += _T("\\exe\\axisform.dll");
	bstr = sRoot.AllocSysString();
	hr = LoadTypeLib(bstr, &pTypeLib);
	SysFreeString(bstr);

	if (FAILED(hr))
		return _T("");

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
				sRtn = _T("");
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
				sRtn = _T("");
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
		return _T("GetProperties:1\tSetProperties:1\t");

	ITypeLib *pTypeLib = NULL;
	ITypeInfo *pTypeInfo = NULL;
	TYPEATTR *pTypeAttr = NULL;

	BSTR bstr;
	HRESULT hr;

	CString sRoot = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR), sRtn = _T("");
	sRoot += _T("\\dev\\") + str + _T(".DLL");
	bstr = sRoot.AllocSysString();
	hr = LoadTypeLib(bstr, &pTypeLib);
	SysFreeString(bstr);

	if (FAILED(hr))
		return _T("GetProperties:0\tSetProperties:0\t");

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
				sRtn = _T("GetProperties:0\tSetProperties:0\t");
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
				sRtn = _T("GetProperties:0\tSetProperties:0\t");
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
	CString sRtn = _T("");
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
				sRtn += _T(":1:");	// properties
				
				VARDESC *pVarDesc;
				pTypeInfo->GetVarDesc(jj - 1, &pVarDesc);
				CString sTip = GetVtType(pVarDesc->elemdescVar.tdesc.vt) + _T(" ") + sStr;
				sRtn += sTip + _T("\t");
				pTypeInfo->ReleaseVarDesc(pVarDesc);
			}
			else
			{
				sRtn += _T(":0:");	// Methods
				
				CString sTip = sStr + _T("(");
				FUNCDESC *pFuncDesc;
				pTypeInfo->GetFuncDesc(jj - pTypeAttr->cVars - 1, &pFuncDesc);
				sTip = GetVtType(pFuncDesc->elemdescFunc.tdesc.vt) + _T(" ") + sTip;

				if (pFuncDesc->lprgelemdescParam)
				{
					for (int nCount = 1; nCount < (int)cNames; nCount++)
					{
						CString sParam(pBstr[nCount]);
						sTip += GetVtType(pFuncDesc->lprgelemdescParam[nCount - 1].tdesc.vt) + _T(" ");
						sTip += sParam;
						if (nCount != (int)cNames - 1)
							sTip += _T(", ");
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
				sRtn += sTip + _T(")\t");
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
		return _T("Long");
	case VT_BSTR:
		return _T("String");
	case VT_BOOL:
		return _T("Boolean");
	case VT_I2:
		return _T("Integer");
	case VT_VARIANT:
		return _T("Variant");
	case VT_R4:
		return _T("Float");
	case VT_R8:
		return _T("Double");
	case VT_EMPTY:
	case VT_VOID:
		return _T("void");
	}
	return _T("");
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
					if (pszChars[pos.x - 1] == _T('.'))
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
					if (!m_pAutoDlg->GetSelString().IsEmpty())
					{
						MSG msg;
						PeekMessage(&msg, 0, 0, 0, PM_REMOVE);

						do
						{
							if (pszChars[pos.x] == _T('.'))
								break;
							pos.x--;
						} while(pos.x >= 0);
						
						if (pos.x > 0)
						{
							int nEndLine, nEndChar;
							if (pos.x + 1 < GetCursorPos().x)
								m_buf.DeleteText(this, pos.y, pos.x + 1, pos.y, GetCursorPos().x);
							m_buf.InsertText(this, pos.y, pos.x + 1, m_pAutoDlg->GetSelString(), nEndLine, nEndChar);
							SetCursorPos(CPoint(nEndChar, nEndLine));
						}
					}
					m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				return TRUE;
			default:
				if (pMsg->wParam >= _T('!') && (!_istalpha((unsigned int)pMsg->wParam) && (char)pMsg->wParam != _T('_') && ((char)pMsg->wParam != _T('9') && (::GetKeyState(VK_SHIFT) & 0x0800))))
					m_pAutoDlg->ShowWindow(SW_HIDE);
				else if ((char)pMsg->wParam == _T(' ') || ((char)pMsg->wParam == _T('9') && (::GetKeyState(VK_SHIFT) & 0x0800)))
				{
					CPoint pos = GetCursorPos();
					LPCTSTR pszChars = GetLineChars(pos.y);
					if (!m_pAutoDlg->GetSelString().IsEmpty())
					{
						do
						{
							if (pszChars[pos.x] == _T('.'))
								break;
							pos.x--;
						} while(pos.x >= 0);
						
						if (pos.x > 0)
						{
							int nEndLine, nEndChar;
							if (pos.x + 1 < GetCursorPos().x)
								m_buf.DeleteText(this, pos.y, pos.x + 1, pos.y, GetCursorPos().x);
							m_buf.InsertText(this, pos.y, pos.x + 1, m_pAutoDlg->GetSelString(), nEndLine, nEndChar);
							SetCursorPos(CPoint(nEndChar, nEndLine));
						}
					}
					m_pAutoDlg->ShowWindow(SW_HIDE);
				}
				else if (pMsg->wParam >= _T('!'))	// maybe ascii
				{
					CPoint pos = GetCursorPos();
					CString txt = GetLineChars(pos.y);
					txt = txt.Left(pos.x);
					txt = txt.Mid(txt.ReverseFind(_T('.')) + 1);
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

			switch (pMsg->wParam)
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
					
					if (txt.Replace(_T(","), _T(",")) >= sTip.Replace(_T(","), _T(",")))
					{
						if (txt.ReverseFind(_T(',')) != -1)
						{
							txt = txt.Mid(txt.ReverseFind(_T(',')) + 1);
							txt.TrimLeft();
							if (!txt.IsEmpty() && txt.FindOneOf(_T("\t ")) != -1)
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
					
					if (m_pAutoTip->m_start >= GetCursorPos().x - (pMsg->wParam==VK_LEFT?1:0))
					{
						m_pAutoTip->ShowWindow(SW_HIDE);
						break;
					}
					if (::GetKeyState(VK_SHIFT) & 0x0800)
					{
						break;
					}
				
					if (txt.GetLength() >= GetCursorPos().x + (pMsg->wParam==VK_RIGHT?1:0))
						txt = txt.Mid(m_pAutoTip->m_start, GetCursorPos().x + (pMsg->wParam==VK_RIGHT?1:0) - m_pAutoTip->m_start);
					else
						break;

					if (txt.Find(_T(")")) != -1)
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
				if ((char)pMsg->wParam == _T('0') && (::GetKeyState(VK_SHIFT) & 0x0800))
					m_pAutoTip->ShowWindow(SW_HIDE);
				if ((char)pMsg->wParam == _T(' '))
				{
					CString txt = GetLineChars(GetCursorPos().y);
					if (m_pAutoTip->m_start < GetCursorPos().x && txt.GetLength() >= GetCursorPos().x)
					txt = txt.Mid(m_pAutoTip->m_start, GetCursorPos().x - m_pAutoTip->m_start);
					
					if (txt.Replace(_T(","), _T(",")) >= sTip.Replace(_T(","), _T(",")) || m_pAutoTip->m_start >= GetCursorPos().x)
					{
						if (txt.ReverseFind(_T(',')) != -1)
						{
							txt = txt.Mid(txt.ReverseFind(_T(',')) + 1);
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

		// Shift selection to right
		m_bHorzScrollBarLocked = TRUE;
		static const TCHAR pszText[] = _T("\'");
		for (int L = nStartLine; L <= nEndLine; L ++)
		{
			int x, y;
			LPCTSTR pszChars = GetLineChars(L);
			if (pszChars[0] == _T('\''))
				m_pTextBuffer->DeleteText(this, L, 0, L, 1, CE_ACTION_INDENT);
			else
				m_pTextBuffer->InsertText(this, L, 0, pszText, y, x, CE_ACTION_INDENT); // [JRT]
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
	CString sSym = Parser(sKey, _T(".")), sTipText = _T(""), info = _T("");
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
			info = GetBaseControlInfo(kind);
			if (!info.IsEmpty())
			{
				if (kind == fmOBJECT && !m_sControlName.IsEmpty())
					info = GetControlInfo(m_sControlName);
				do
				{
					CString temp = Parser(info, _T("\t"));
					CString text = Parser(temp, _T(":"));
					int isProp = atoi(Parser(temp, _T(":")));
					CString sTip = Parser(temp, _T(";"));
					if (!text.IsEmpty() && !text.CompareNoCase(sKey))
					{
						bRet = TRUE;
						break;
					}
				} while(!info.IsEmpty());
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
	case COLORINDEX_LINENUMBER:		// KSI
		return RGB(43, 145, 175);
	case COLORINDEX_GRAYLINE:		// KSI
		return RGB(165, 165, 165);
	}
	return m_clString;
}

void CVBScriptEdit::LoadColors()
{
	CString currentPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	m_clBkGnd    = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRBKGND"), ::GetSysColor(COLOR_WINDOW), currentPath + _T("\\workshop.ini"));
	m_clText     = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRNORMAL"), ::GetSysColor(COLOR_WINDOWTEXT), currentPath + _T("\\workshop.ini"));
	m_clSymbol   = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRSYMBOL"), RGB(0, 150, 180), currentPath + _T("\\workshop.ini"));
	m_clComment  = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRCOMMENT"), RGB(0, 128, 0), currentPath + _T("\\workshop.ini"));
	m_clConstant = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRCONSTANT"), RGB(0x80, 0x00, 0x60), currentPath + _T("\\workshop.ini"));
	m_clKeyword  = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRKEYWORD"), RGB(0, 0, 255), currentPath + _T("\\workshop.ini"));
	m_clString   = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRSTRING"), RGB(255, 0, 0), currentPath + _T("\\workshop.ini"));
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
	
	menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("Copy"));
	menu.AppendMenu(MF_STRING, ID_EDIT_CUT, _T("Cut"));
	menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("Paste"));
	menu.AppendMenu(MF_STRING, ID_EDIT_LINENUMBER, _T("Line Number"));
	menu.AppendMenu(MF_STRING, ID_EDIT_FIND, _T("Search"));
	menu.AppendMenu(MF_STRING, ID_EDIT_REPLACE, _T("Replace"));
	
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

