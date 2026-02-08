// ExEdit.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExEdit.h"
#include "../../inc/ExEdit.h"

#include "../../inc/ISkinColorMng.h"
#include "../../inc/IMainInfoMng.h"

#include "MemberDraw.h"
#include "MemberMask.h"
#include ".\exedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CExEditApp

BEGIN_MESSAGE_MAP(CExEditApp, CWinApp)
	//{{AFX_MSG_MAP(CExEditApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExEditApp construction

CExEditApp::CExEditApp()
{
	m_pCurMember = NULL;
}

long CExEditApp::ActionControl(LPVOID lpAction)
{
	if(!m_pCurMember)
		return 0L;

	LPST_ACTIONCONTROL lpActionControl = (LPST_ACTIONCONTROL)lpAction;

	return 0L;
}

void CExEditApp::ExitEdit(LPVOID lpPointer)
{
	m_pCurMember = (CMemberDraw*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

void CExEditApp::ExitEditMask(LPVOID lpPointer)
{
	m_pCurMember = (CMemberMask*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

LPVOID CExEditApp::InitEditMask(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);

	CMember* pMember = new CMemberMask(lpPointer, m_pISkinColorManager, bUseDefSkin);
	return pMember;
}

LPVOID CExEditApp::InitEdit(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	if(!m_pISkinColorManager)
		return NULL;

	ARR_COLOR_TABLE* pArray = (ARR_COLOR_TABLE*)m_pISkinColorManager->GetColorTableArray();
	if(pArray->GetSize() == 0)
	{
		if(!m_pIMainInfoManager)
		{
			HWND hMainWnd;
			AfxGetHanaroMainWnd(hMainWnd);

			m_pIMainInfoManager = (IMainInfoManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 21, 0);
		}

		char szPath[MAX_PATH] = {NULL, };
		::sprintf(szPath, _T("%s\\%s"), m_pIMainInfoManager->GetRootDir(), ISkinColor_default_File);

		m_pISkinColorManager->LoadColorTable(szPath);
	}

	CMember* pMember = new CMemberDraw(lpPointer, m_pISkinColorManager, bUseDefSkin);
	return pMember;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExEditApp object

//////////////////////////////////////////////////////////////////////////
// ExMaskEdit
void CExEditApp::SetUseMask(BOOL bUseMask)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	pMember->m_bUseMask = bUseMask;
}

BOOL CExEditApp::CanUseMask()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	return pMember->m_bUseMask;
}

void CExEditApp::SetOverType(BOOL bOverType) 
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	pMember->m_bOverType = bOverType;
}

BOOL CExEditApp::CanOverType() 
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	return pMember->m_bOverType;
}

BOOL CExEditApp::PosInRange(int nPos) 
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	return ((nPos >= 0) && (nPos < pMember->m_strLiteral.GetLength()));
}

TCHAR CExEditApp::GetPromptChar()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	return pMember->m_chPrompt;
}

void CExEditApp::SetPromptChar(TCHAR ch)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_strLiteral.Replace(pMember->m_chPrompt, ch);
	pMember->m_strWindowText.Replace(pMember->m_chPrompt, ch);
	
	pMember->m_chPrompt = ch;
	pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);		
}

void CExEditApp::MaskCut()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(!CanUseMask()) 
	{
		pMember->m_pEdit->CEdit::Cut();
	} 
	else
	{		
		MaskCopy();
		MaskClear();
		
		pMember->m_bRedo = FALSE; 
		pMember->m_bModified = TRUE;
		pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
		
		CorrectPosition(pMember->m_nStartChar);
		pMember->m_nEndChar = pMember->m_nStartChar;
		pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	}		
}

void CExEditApp::MaskCopy()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(!CanUseMask()) 
	{
		pMember->m_pEdit->CEdit::Copy();
	} 
	else
	{
		pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
		
		CString strBuffer;
		int i;
		for(i = pMember->m_nStartChar; i < pMember->m_nEndChar; ++i)
		{
			if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
			{
				if(pMember->m_strWindowText.GetAt(i) == pMember->m_chPrompt)
					continue;
				
				strBuffer += pMember->m_strWindowText.GetAt(i);
			}
		}
		
		CopyToClipboard(strBuffer);
	}		
}

void CExEditApp::MaskPaste()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(!CanUseMask()) 
	{
		pMember->m_pEdit->CEdit::Paste();
	} 
	else
	{
		pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
		MaskClear();
		
		if(!OpenClipboard(pMember->m_pEdit->GetSafeHwnd())) 
			return; 
		
		LPTSTR lptstrPaste = NULL;
		HGLOBAL hglbPaste = ::GetClipboardData(CF_TEXT); 
		if(hglbPaste != NULL) 
		{ 
			lptstrPaste = (TCHAR*)::GlobalLock(hglbPaste); 
			if(lptstrPaste != NULL) 
			{ 
				::GlobalUnlock(hglbPaste); 
			} 
		} 
		
		::CloseClipboard();
		
		int x = pMember->m_nStartChar, nLen = _tcslen(lptstrPaste);
		
		for(int i = 0; i < nLen; ++i)
		{
			UINT ch = lptstrPaste[ i ];
			
			if(CheckChar(ch, FALSE))
			{
				InsertCharAt(x, (TCHAR)ch);
				++x;
				
				if(x == pMember->m_strWindowText.GetLength())
					break;
			}
		}
		
		pMember->m_bRedo = FALSE; 
		pMember->m_bModified = TRUE;
		pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
		
		pMember->m_nEndChar = pMember->m_nStartChar;
		pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	}		
}

void CExEditApp::MaskClear()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	
	if(!CanUseMask()) 
	{
		pMember->m_pEdit->CEdit::Clear();
	} 
	else
	{
		if(pMember->m_nStartChar == pMember->m_nEndChar)
			return;
		
		int nLength = pMember->m_strWindowText.GetLength();
		int i = 0;
		for( i = pMember->m_nStartChar; i < nLength; ++i)
		{
			TCHAR ch1 = pMember->m_strLiteral.GetAt(i);
			TCHAR ch2 = pMember->m_strWindowText.GetAt(i);
			
			if((ch1 == pMember->m_chPrompt) && (ch2 != pMember->m_chPrompt))
				break;
		}
		
		if(i == nLength)
		{
			::MessageBeep((UINT)-1);
			return;
		}
		
		for(i = pMember->m_nStartChar; i < pMember->m_nEndChar; ++i)
		{
			if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
				DeleteCharAt(pMember->m_nStartChar);
		}
		
		pMember->m_bRedo = FALSE; 
		pMember->m_bModified = TRUE;
		pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
		
		pMember->m_nEndChar = pMember->m_nStartChar;
		pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	}		
}

void CExEditApp::MaskUndo()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(!CanUseMask()) 
	{
		pMember->m_pEdit->CEdit::Undo();
	} 
	else
	{
		pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
		
		if(pMember->m_bRedo)
		{
			pMember->m_bRedo = FALSE; 
			pMember->m_bModified = TRUE;
			pMember->m_pEdit->SetWindowText(pMember->m_strRedoBuffer);
		}
		else
		{
			pMember->m_bRedo = TRUE; 
			pMember->m_bModified = TRUE;
			pMember->m_pEdit->GetWindowText(pMember->m_strRedoBuffer);
			pMember->m_pEdit->SetWindowText(pMember->m_strUndoBuffer);
		}
		
		pMember->m_pEdit->GetWindowText(pMember->m_strWindowText);
		pMember->m_nEndChar = pMember->m_nStartChar;
		pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	}			
}

void CExEditApp::MaskSelectAll()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(!CanUseMask()) 
	{
		pMember->m_pEdit->CEdit::SetSel(0, -1);
//		pMember->m_pEdit->SetSel_Feedback(0, -1);
	} 
	else
	{
		pMember->m_nStartChar = 0;
		CorrectPosition(pMember->m_nStartChar);
		pMember->m_pEdit->SetSel(pMember->m_nStartChar, -1);
	}
}

BOOL CExEditApp::IsModified() 
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	return pMember->m_bModified;
}

//			0                 Numeric (0-9)
//			9                 Numeric (0-9) or space (' ')
//			#                 Numeric (0-9) or space (' ') or ('+') or ('-')
//			L                 Alpha (a-Z) 
//			?                 Alpha (a-Z) or space (' ') 
//			A                 Alpha numeric (0-9 and a-Z)
//			a                 Alpha numeric (0-9 and a-Z) or space (' ') 
//			&                 All print character only 
//			H                 Hex digit (0-9 and A-F)
//			X                 Hex digit (0-9 and A-F) and space (' ')
//			>                 Forces characters to upper case (A-Z)
//			<                 Forces characters to lower case (a-z)</pre>
BOOL CExEditApp::SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault/* = NULL*/)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_strMask    = lpszMask;
	pMember->m_strLiteral = lpszLiteral;

	if(lpszDefault == NULL)
	{
		pMember->m_strWindowText = lpszLiteral;
		pMember->m_strDefault    = lpszLiteral;
	}
	else
	{
		pMember->m_strWindowText = lpszDefault;
		pMember->m_strDefault    = lpszDefault;
	}
	
	pMember->m_bRedo = FALSE; 
	pMember->m_bModified = FALSE;
	pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
	
	pMember->m_strUndoBuffer = pMember->m_strWindowText;

	return TRUE;
}

BOOL CExEditApp::CheckChar(UINT& nChar, BOOL bBeep/* = TRUE*/)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;
	int i = 0;

	if(!CanUseMask())
		return FALSE;
	
	if(!_istprint((TCHAR)nChar))
		return TRUE;
	
	for( i = pMember->m_nStartChar; i < pMember->m_strLiteral.GetLength(); ++i)
	{
		if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
		{
			pMember->m_pEdit->SetSel(i, i);
			break;
		}
	}
	
	if(i >= pMember->m_strMask.GetLength())
	{
		if(bBeep)
			::MessageBeep((UINT)-1);

		pMember->m_pEdit->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(pMember->m_pEdit->GetDlgCtrlID(), EN_MAXTEXT), (LPARAM)pMember->m_pEdit->GetSafeHwnd());

		return FALSE;
	}
	
	if(!ProcessMask(nChar, i))
	{
		if(bBeep)
			::MessageBeep((UINT)-1);

		return FALSE;
	}
	
	return TRUE;		
}

BOOL CExEditApp::ProcessMask(UINT& nChar, int nEndPos)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	switch (pMember->m_strMask.GetAt(nEndPos))
	{
	case '0':		// digit only //completely changed this
		{
			if(_istdigit((TCHAR)nChar)) {
				return TRUE;
			}
			break;
		}
	case '9':		// digit or space
		{
			if(_istdigit((TCHAR)nChar)) {
				return TRUE;
			}
			if(nChar == ' ') {
				return TRUE;
			}
			break;
		}
	case '#':		// digit or space or '+' or '-'
		{
			if(_istdigit((TCHAR)nChar)) {
				return TRUE;
			}
			if(nChar == ' ' || nChar == '-' || nChar == '+') {
				return TRUE;
			}
			break;
		}
	case 'L':		// alpha only
		{
			if(_istalpha((TCHAR)nChar)) {
				return TRUE;
			}
			break;
		}
	case '?':		// alpha or space
		{
			if(_istalpha((TCHAR)nChar)) {
				return TRUE;
			}
			if(nChar == ' ') {
				return TRUE;
			}
			break;
		}
	case 'A':		// alpha numeric only
		{
			if(_istalnum((TCHAR)nChar)) {
				return TRUE;
			}
			break;
		}
	case 'a':		// alpha numeric or space
		{
			if(_istalnum((TCHAR)nChar)) {
				return TRUE;
			}
			if(nChar == ' ') {
				return TRUE;
			}
			break;
		}
	case '&':		// all print character only
		{
			if(_istprint((TCHAR)nChar)) {
				return TRUE;
			}
			break;
		}
	case 'H':		// hex digit
		{
			if(_istxdigit((TCHAR)nChar)) {
				return TRUE;
			}
			break;
		}
	case 'X':		// hex digit or space
		{
			if(_istxdigit((TCHAR)nChar)) {
				return TRUE;
			}
			if(nChar == ' ') {
				return TRUE;
			}
			break;
		}
	case '>':
		{
			if(_istalpha((TCHAR)nChar))
			{
				if(_istlower((TCHAR)nChar))
				{
					nChar = _toupper((TCHAR)nChar);
				}
				return TRUE;
			}
			break;
		}
	case '<':
		{
			if(_istalpha((TCHAR)nChar))
			{
				if(_istupper((TCHAR)nChar))
				{
					nChar = _tolower((TCHAR)nChar);
				}
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

BOOL CExEditApp::PreTranslateMessage(MSG* pMsg)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(CanUseMask() == FALSE)
		return pMember->m_pEdit->CEdit::PreTranslateMessage(pMsg);
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(::GetKeyState(VK_SUBTRACT) < 0)
		{
			OnChar_ExMaskEdit('-', 1, 1);
			return TRUE;
		}
		
		if(::GetKeyState(VK_ADD) < 0)
		{
			OnChar_ExMaskEdit('+', 1, 1);
			return TRUE;
		}
		
		if(::GetKeyState(VK_CONTROL) < 0)
		{
			switch (pMsg->wParam)
			{
			case 'X':
			case 'x':
				{
					MaskCut();
					return TRUE;
				}
				
			case 'C':
			case 'c':
				{
					MaskCopy();
					return TRUE;
				}
				
			case 'V':
			case 'v':
				{
					MaskPaste();
					return TRUE;
				}
				
			case 'Z':
			case 'z':
				{
					MaskUndo();
					return TRUE;
				}
			}
		}
		else
		{
			switch (pMsg->wParam)
			{
			case '-':
				{
					OnChar_ExMaskEdit('-', 1, 1);
					return TRUE;
				}
				
			case '+':
				{
					OnChar_ExMaskEdit('+', 1, 1);
					return TRUE;
				}
			}
		}
	}
	
	return pMember->m_pEdit->CEdit::PreTranslateMessage(pMsg);
}

BOOL CExEditApp::CorrectPosition(int& nPos, BOOL bForward/* = TRUE*/, BOOL bBeep/* = TRUE*/) 
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_pEdit->GetWindowText(pMember->m_strWindowText);
	int nLen = pMember->m_strWindowText.GetLength();
	
	if(nPos == nLen)
	{
		if(bBeep)
			::MessageBeep((UINT)-1);

		return FALSE;
	}
	
	if(PosInRange(nPos) == FALSE)
		return FALSE;
	
	if(pMember->m_strLiteral.GetAt(nPos) != pMember->m_chPrompt)
	{
		int i;
		if(bForward)
		{
			for(i = nPos; i < nLen; ++i)
			{
				if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
				{
					nPos = i;
					break;
				}
			}
		}
		else
		{
			for(i = nPos; i >= 0; --i)
			{
				if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
				{
					nPos = i;
					break;
				}
			}
			
			if(i == -1)
			{
				nPos++;
				
				if(bBeep)
					::MessageBeep((UINT)-1);

				return FALSE;
			}
		}
		
		if(i == nLen)
		{
			if(bBeep)
				::MessageBeep((UINT)-1);

			return FALSE;
		}
	}

	return TRUE;		
}

void CExEditApp::DeleteCharAt(int nPos)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(PosInRange(nPos) == FALSE)
		return;
	
	CString strBuffer = GetMaskedText(nPos +1 );
	strBuffer += pMember->m_chPrompt;
	
	int x = 0;
	int iLen = strBuffer.GetLength();
	int i;
	for(i = nPos; i < pMember->m_strLiteral.GetLength(); ++i)
	{
		if((pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt))
		{
			pMember->m_strWindowText.SetAt(i, strBuffer.GetAt(x));
			++x;
			
			if(x == iLen)
				break;
		}
	}		
}

void CExEditApp::InsertCharAt(int nPos, TCHAR nChar)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(PosInRange(nPos) == FALSE)
		return;
	
	UINT uChar = nChar;
	if(!CheckChar(uChar, FALSE))
		return;
	
	CString strBuffer = GetMaskedText(nPos);
	
	CString strTemp(nChar);
	int i, x = 0, iLen = strBuffer.GetLength();
	for(i = 1; i < strBuffer.GetLength(); ++i)
	{
		strTemp += strBuffer.GetAt(i-1);
	}
	strBuffer = strTemp;
	
	for(i = nPos; i < pMember->m_strWindowText.GetLength(); ++i)
	{
		if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
		{
			pMember->m_strWindowText.SetAt(i, strBuffer[ x ]);
			++x;
			
			if(x == iLen)
				break;
		}
	}
}

BOOL CExEditApp::CopyToClipboard(LPCTSTR lpszText)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(!OpenClipboard(pMember->m_pEdit->GetSafeHwnd())) 
		return FALSE;
	
	::EmptyClipboard(); 
	
	int iLen = _tcslen(lpszText) + 1;
	
	HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, iLen);
	
	if(hglbCopy == NULL)
	{ 
		::CloseClipboard();
		return FALSE;
	}
	
	LPTSTR lptstrCopy = (TCHAR*)::GlobalLock(hglbCopy);
	memcpy(lptstrCopy, lpszText, iLen);
	::GlobalUnlock(hglbCopy); 
	
	::SetClipboardData(CF_TEXT, hglbCopy); 
	
	if(!::CloseClipboard())
		return FALSE;
	
	return TRUE;		
}

CString CExEditApp::GetMaskedText(int nPos/* = 0*/)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	int nWndLen = pMember->m_strWindowText.GetLength();
	int nLitlen = pMember->m_strLiteral.GetLength();
	
	CString strBuffer;
	int i;
	for(i = nPos; i < nWndLen; ++i)
	{
		if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
			strBuffer += pMember->m_strWindowText.GetAt(i);
	}
	
	return strBuffer;		
}

BOOL CExEditApp::SelectionMade() 
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar); 

	return (pMember->m_nStartChar != pMember->m_nEndChar);
}

void CExEditApp::OnKeyDown_ExMaskEdit(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(CanUseMask() == FALSE)
	{
		pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
	if(!m_pCurMember)
		return;
	
	pMember->m_pEdit->GetWindowText(pMember->m_strWindowText); // refresh
	pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	
	if(pMember->m_strMask.IsEmpty())
	{
		pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
	BOOL bShift = (::GetKeyState(VK_SHIFT) < 0);
	
	switch (nChar)
	{
	case VK_UP:
	case VK_LEFT:
		{
			pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			
			int nStartChar = pMember->m_nStartChar;
			CorrectPosition(nStartChar, FALSE, FALSE);
			
			if(pMember->m_nStartChar < nStartChar)
			{
				pMember->m_nStartChar = nStartChar;
				
				if(!bShift)
					pMember->m_nEndChar = nStartChar;
				
				pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			}
		}
		return;
		
	case VK_DOWN:
	case VK_RIGHT:
		{
			pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			
			int nEndChar = pMember->m_nEndChar;
			CorrectPosition(nEndChar, TRUE, FALSE);
			
			if(pMember->m_nEndChar > nEndChar)
			{
				pMember->m_nEndChar = nEndChar;
				
				if(!bShift)
					pMember->m_nStartChar = nEndChar;
				
				pMember->m_pEdit->CEdit::SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			}
		}
		return;
		
	case VK_HOME:
		{
			pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			
			int nStartChar = pMember->m_nStartChar;
			CorrectPosition(nStartChar, TRUE, FALSE);
			
			if(pMember->m_nStartChar < nStartChar)
			{
				pMember->m_nStartChar = nStartChar;
				
				if(bShift == FALSE)
				{
					pMember->m_nEndChar = pMember->m_nStartChar;
				}
				pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			}
		}
		return;
		
	case VK_END:
		{
			pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			
			int nEndChar = pMember->m_nEndChar;
			CorrectPosition(nEndChar, TRUE, FALSE);
			
			if(pMember->m_nEndChar > nEndChar)
			{
				pMember->m_nEndChar = nEndChar;
				
				if(!bShift)
					pMember->m_nStartChar = nEndChar;
				
				pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			}
		}
		return;
		
	case VK_INSERT:
		{
			if(bShift)
			{
				MaskPaste();
			}
			else
			{
				pMember->m_bOverType = !pMember->m_bOverType; // set the type-over flag
			}
		}
		return;
		
	case VK_DELETE:
		{
			if(pMember->m_nStartChar == pMember->m_nEndChar)
			{
				if(CorrectPosition(pMember->m_nStartChar))
				{
					DeleteCharAt(pMember->m_nStartChar);
					
					pMember->m_bRedo = FALSE; 
					pMember->m_bModified = TRUE;
					pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
					
					pMember->m_nEndChar = pMember->m_nStartChar;
					pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
				}
			}
			else
			{
				MaskClear();
			}
		}
		return;
		
	case VK_SPACE:
		{
			pMember->m_pEdit->GetWindowText(pMember->m_strWindowText); // refresh
			
			if(CorrectPosition(pMember->m_nStartChar))
			{
				if(pMember->m_nStartChar < pMember->m_strLiteral.GetLength())
				{
					if(ProcessMask(nChar, pMember->m_nStartChar))
					{
						if(CanOverType())
						{
							pMember->m_strWindowText.SetAt(pMember->m_nStartChar, ' ');
						}
						else
						{
							InsertCharAt(pMember->m_nStartChar, (TCHAR)nChar);
						}
					}
					else
					{
						pMember->m_strWindowText.SetAt(pMember->m_nStartChar, pMember->m_chPrompt);
					}
					
					pMember->m_bRedo = FALSE; 
					pMember->m_bModified = TRUE;
					pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
					
					pMember->m_nStartChar++;
					pMember->m_nEndChar = pMember->m_nStartChar;
					pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
				}
				else
				{
					::MessageBeep((UINT)-1);
				}
			}
		}
		return;
		
	case VK_BACK:
		{
			pMember->m_pEdit->GetWindowText(pMember->m_strWindowText); // refresh
			
			if((pMember->m_nStartChar > 0) && 
				(pMember->m_nStartChar <= pMember->m_strLiteral.GetLength()))
			{
				pMember->m_nStartChar--;
				
				if(CorrectPosition(pMember->m_nStartChar, FALSE))
				{
					TCHAR ch = pMember->m_chPrompt;
					
					// get the masked literal representation.
					if(!pMember->m_strDefault.IsEmpty())
					{
						ch = pMember->m_strDefault.GetAt(pMember->m_nStartChar);
					}
					
					pMember->m_strWindowText.SetAt(pMember->m_nStartChar, ch);
					
					pMember->m_bRedo = FALSE; 
					pMember->m_bModified = TRUE;
					pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
					
					pMember->m_nEndChar = pMember->m_nStartChar;
					pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
				}
			}
			else 
			{
				::MessageBeep((UINT)-1);
			}
		}
		return;
	}

	pMember->m_pEdit->CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExEditApp::OnChar_ExMaskEdit(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(CanUseMask() == FALSE)
	{
		pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	
	switch (nChar)
	{
	case VK_UP:
	case VK_LEFT:
	case VK_DOWN:
	case VK_RIGHT:
	case VK_HOME:
	case VK_END:
	case VK_DELETE:
	case VK_SPACE:
	case VK_BACK:
		return; // handled in WM_KEYDOWN
	}
	
	if(CheckChar(nChar) == FALSE)
		return;
	
	if(CanUseMask())
	{
		if(_istprint((TCHAR)nChar))
		{
			int nLen = pMember->m_strLiteral.GetLength();
			if(pMember->m_nStartChar >= nLen)
			{
				::MessageBeep((UINT)-1);
				return;
			}
			
			if(pMember->m_nEndChar >= nLen)
				pMember->m_nEndChar = nLen;
			
			if(pMember->m_nStartChar != pMember->m_nEndChar)
			{
				for(int i = pMember->m_nStartChar; i <pMember->m_nEndChar; ++i)
				{
					if(pMember->m_strLiteral.GetAt(i) == pMember->m_chPrompt)
						DeleteCharAt(pMember->m_nStartChar);
				}
			}
			
			pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
			
			if(CorrectPosition(pMember->m_nStartChar))
			{
				if(CanOverType())
				{
					if(pMember->m_nStartChar != pMember->m_nEndChar)
					{
						InsertCharAt(pMember->m_nStartChar, (TCHAR)nChar);
					}
					else
					{
						pMember->m_strWindowText.SetAt(pMember->m_nStartChar, (TCHAR)nChar);
					}
				}
				else
				{
					InsertCharAt(pMember->m_nStartChar, (TCHAR)nChar);
				}
			}
			
			pMember->m_bRedo = FALSE; 
			pMember->m_bModified = TRUE;
			pMember->m_pEdit->SetWindowText(pMember->m_strWindowText);
			
			if(pMember->m_nStartChar < nLen)
				pMember->m_nStartChar++;
			
			if(pMember->m_nStartChar < nLen)
				CorrectPosition(pMember->m_nStartChar);
			
			pMember->m_nEndChar = pMember->m_nStartChar;
			pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
			
			return;
		}
	}
	
	pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
	pMember->m_pEdit->GetWindowText(pMember->m_strWindowText);		
}

void CExEditApp::OnLButtonDown_ExMaskEdit(UINT nFlags, CPoint point)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_pEdit->CEdit::OnLButtonDown(nFlags, point);
	
	if(CanUseMask() == FALSE)
		return;

	pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	CorrectPosition(pMember->m_nStartChar, TRUE, FALSE);
	
	if(::GetKeyState(VK_SHIFT) < 0)
		pMember->m_nEndChar = pMember->m_nStartChar;
	
	pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);		
}

void CExEditApp::OnSetFocus_ExMaskEdit(CWnd* pOldWnd)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_pEdit->CEdit::OnSetFocus(pOldWnd);
	
	if(CanUseMask() == FALSE)
		return;
	
	pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	CorrectPosition(pMember->m_nStartChar, TRUE, FALSE);
	
	pMember->m_nEndChar = pMember->m_nStartChar;
	pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);		
}

void CExEditApp::OnLButtonDblClk_ExMaskEdit(UINT nFlags, CPoint point)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	pMember->m_pEdit->CEdit::OnLButtonDblClk(nFlags, point);
	
	if(CanUseMask() == FALSE)
		return;
	
	pMember->m_pEdit->GetSel(pMember->m_nStartChar, pMember->m_nEndChar);
	CorrectPosition(pMember->m_nStartChar, TRUE, FALSE);
	CorrectPosition(pMember->m_nEndChar, TRUE, FALSE);
	
	pMember->m_pEdit->SetSel(pMember->m_nStartChar, pMember->m_nEndChar);
}

void CExEditApp::OnUpdateEditUndo_ExMaskEdit(CCmdUI* pCmdUI)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	if(CanUseMask() == FALSE)
		pCmdUI->Enable(pMember->m_pEdit->CEdit::CanUndo());
	else
		pCmdUI->Enable(pMember->m_bModified);
}

BOOL CExEditApp::OnCommand_ExMaskEdit(WPARAM wParam, LPARAM lParam)
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	switch (LOWORD(wParam))
	{		
		case ID_EDIT_CUT:
			if(!CanUseMask()) pMember->m_pEdit->CEdit::Cut(); else MaskCut();
			return TRUE;

		case ID_EDIT_COPY:
			if(!CanUseMask()) pMember->m_pEdit->CEdit::Copy(); else MaskCopy();
			return TRUE;
	
		case ID_EDIT_PASTE:
			if(!CanUseMask()) pMember->m_pEdit->CEdit::Paste(); else MaskPaste();
			return TRUE;

		case ID_EDIT_CLEAR:
			if(!CanUseMask()) pMember->m_pEdit->CEdit::Clear(); else MaskClear();
			return TRUE;

		case ID_EDIT_UNDO:
			if(!CanUseMask()) pMember->m_pEdit->CEdit::Undo(); else MaskUndo();
			return TRUE;

		case ID_EDIT_SELECT_ALL:
			MaskSelectAll();
			return TRUE;
	}

	return pMember->m_pEdit->CEdit::OnCommand(wParam, lParam);
}

void CExEditApp::OnPaint(LPVOID lpPointer)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	if(!pMember)
		return;
	
	CDC* pDC = pMember->m_pEdit->GetDC();
	CRect rcItem;
	pMember->m_pEdit->GetClientRect(&rcItem);

	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, pMember->m_clrBackground, pMember->m_clrBackground);
	rcItem.InflateRect(1,1,2,1);
	pDC->Draw3dRect(rcItem, RGB(116,117,117), RGB(116,117,117));

	pMember->m_pEdit->ReleaseDC(pDC);
}

void CExEditApp::OnPaintMaskEdit()
{
	CMemberMask* pMember = (CMemberMask*)m_pCurMember;

	CDC* pDC = pMember->m_pEdit->GetDC();
	CRect rcItem;
	pMember->m_pEdit->GetClientRect(&rcItem);

	pDC->Draw3dRect(rcItem, pMember->m_clrBackground, pMember->m_clrBackground);
	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, pMember->m_clrBackground, pMember->m_clrBackground);
	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, pMember->m_clrBackground, pMember->m_clrBackground);
	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, pMember->m_clrLine, pMember->m_clrLine);

	pMember->m_pEdit->ReleaseDC(pDC);
}

void CExEditApp::UseNumericType(BOOL bUse)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_bNumericType = bUse;
}

void CExEditApp::OnChar_Edit(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	if(!pMember->m_bNumericType)
	{
		pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	
	if (nChar == 8)
		pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);

	POINT caret;
	::GetCaretPos (&caret);
	caret.x = LOWORD (pMember->m_pEdit->CharFromPos (caret));

	CString text;
	pMember->m_pEdit->GetWindowText (text);

	if (isdigit(nChar)) 
	{
		pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
//		PutComma();
	}
	else if (nChar == '-')
	{
		if (!caret.x)
		{
			if (((text.GetLength() > 0) && (text[0]!='-')) || (text.GetLength()==0))
				pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			if ((text.GetAt(caret.x-1) == 'e') || (text.GetAt(caret.x-1) == 'E'))
				pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}

	else if ((nChar == 'e') || (nChar == 'E'))
	{
		if ((caret.x == 1) && (text[0] == '-'))
			return ;

		if (caret.x)
		{
			for (int i=0; i<text.GetLength(); i++)
			{
				if ((text[i] == 'e') ||(text[i] == 'E'))
					return ;
			}
			pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}

	else if (nChar == '.')
	{
		for (int i=0; i<text.GetLength(); i++)
		{
			if (text[i] == '.')
				return ;
		}

		for (int i=0; i<text.GetLength(); i++)
		{
			if (((text[i] == 'e') ||(text[i]=='E')) && (caret.x > i))
				return ;
		}

		pMember->m_pEdit->CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CExEditApp::PutComma()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	CString strText;
	pMember->m_pEdit->GetWindowText(strText);

	if(strText.IsEmpty())
		return;

	strText.Replace(_T(","), _T(""));
	
	int nLeft = 0, nRight = 0;
	int nLength = strText.GetLength();
	for(int nIndex = 0; nIndex < nLength; nIndex++)
	{
		if(isdigit(strText[nIndex]))
		{
			nLeft = nIndex;
			break;
		}
	}

	nRight = strText.Find(_T('.'));
	if(nRight < 0)
		nRight = nLength - 1;

	if(nLeft == nRight)
		return;

	CString strResult;
	strResult.GetBuffer(nLength + (nLength / 3) + 2);

	strResult = strText.Left(nLeft + 1);

	int nNumber = 0;
	for(int nIndex = nLeft; nIndex < nRight; nIndex++)
    {
		strResult += strText[nIndex];

		nNumber ++;
		nNumber %= 3;
		if(nNumber == 0)
			strResult += _T(",");
    }
	
	if(nLength > nRight + 1)
		strResult += strText.Right(nLength - nRight + 1);

	strResult.ReleaseBuffer();
	
	pMember->m_pEdit->SetWindowText(strResult);
}

BOOL CExEditApp::InitInstance()
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);

	m_pISkinColorManager = (ISkinColorManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 6, 0);

	return CWinApp::InitInstance();
}
//////////////////////////////////////////////////////////////////////////

CExEditApp theApp;

extern "C" __declspec(dllexport) LPVOID Ex_InitEdit(LPVOID lpPointer)
{
	return theApp.InitEdit(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitEdit2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitEdit(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) long Ex_ActionControl(LPVOID lpPointer, LPVOID lpAction)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	long lResult = theApp.ActionControl(lpAction);
	theApp.m_pCurMember = NULL;

	return lResult;
}

extern "C" __declspec(dllexport) LPVOID Ex_InitEditMask(LPVOID lpPointer)
{
	return theApp.InitEditMask(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitEditMask2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitEditMask(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) BOOL Ex_SetEditMask(LPVOID lpPointer, LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetEditMask(lpszMask, lpszLiteral, lpszDefault);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_SetPromptChar(LPVOID lpPointer, TCHAR ch)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetPromptChar(ch);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_ExitEdit(LPVOID lpPointer)
{
	theApp.ExitEdit(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_ExitEditMask(LPVOID lpPointer)
{
	theApp.ExitEditMask(lpPointer);
}

extern "C" __declspec(dllexport) BOOL Ex_CanOverType(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.CanOverType();
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_CanUseMask(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.CanUseMask();
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_IsModified(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.IsModified();
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_PosInRange(LPVOID lpPointer, int nPos)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.PosInRange(nPos);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) TCHAR Ex_GetPromptChar(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	TCHAR chResult = theApp.GetPromptChar();
//	theApp.m_pCurMember = NULL;

	return chResult;
}

extern "C" __declspec(dllexport) void Ex_MaskClear(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.MaskClear();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_MaskCopy(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.MaskCopy();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_MaskCut(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.MaskCut();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_MaskPaste(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.MaskPaste();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_MaskSelectAll(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.MaskSelectAll();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_MaskUndo(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.MaskUndo();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetOverType(LPVOID lpPointer, BOOL bOverType)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetOverType(bOverType);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetUseMask(LPVOID lpPointer, BOOL bUseMask)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetUseMask(bUseMask);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) BOOL Ex_CheckChar(LPVOID lpPointer, UINT& nChar, BOOL bBeep)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.CheckChar(nChar, bBeep);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_ProcessMask(LPVOID lpPointer, UINT& nChar, int nEndPos)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.CheckChar(nChar, nEndPos);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_PreTranslateMessage(LPVOID lpPointer, MSG* pMsg)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.PreTranslateMessage(pMsg);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_CorrectPosition(LPVOID lpPointer, int& nPos, BOOL bForward, BOOL bBeep)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.CorrectPosition(nPos, bForward, bBeep);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_DeleteCharAt(LPVOID lpPointer, int nPos)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.DeleteCharAt(nPos);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_InsertCharAt(LPVOID lpPointer, int nPos, TCHAR nChar)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.InsertCharAt(nPos, nChar);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) BOOL Ex_CopyToClipboard(LPVOID lpPointer, LPCTSTR lpszText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.CopyToClipboard(lpszText);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) LPCTSTR Ex_GetMaskedText(LPVOID lpPointer, int nPos)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	CString strResult(theApp.GetMaskedText(nPos));
//	theApp.m_pCurMember = NULL;

	return (LPCTSTR)strResult;
}

extern "C" __declspec(dllexport) BOOL Ex_SelectionMade(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SelectionMade();
//	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_OnKeyDown_MaskEdit(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnKeyDown_ExMaskEdit(nChar, nRepCnt, nFlags);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnChar_MaskEdit(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnChar_ExMaskEdit(nChar, nRepCnt, nFlags);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnChar_Edit(LPVOID lpPointer, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnChar_Edit(nChar, nRepCnt, nFlags);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnLButtonDown_MaskEdit(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnLButtonDown_ExMaskEdit(nFlags, point);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnSetFocus_MaskEdit(LPVOID lpPointer, CWnd* pOldWnd)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnSetFocus_ExMaskEdit(pOldWnd);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnLButtonDblClk_MaskEdit(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnLButtonDblClk_ExMaskEdit(nFlags, point);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnUpdateEditUndo_MaskEdit(LPVOID lpPointer, CCmdUI* pCmdUI)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnUpdateEditUndo_ExMaskEdit(pCmdUI);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnCommand_MaskEdit(LPVOID lpPointer, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnCommand_ExMaskEdit(wParam, lParam);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnPaintEdit(LPVOID lpPointer)
{
//	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnPaint(lpPointer);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnPaintMaskEdit(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnPaintMaskEdit();
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_UseNumericType(LPVOID lpPointer, BOOL bUse)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.UseNumericType(bUse);
//	theApp.m_pCurMember = NULL;
}

