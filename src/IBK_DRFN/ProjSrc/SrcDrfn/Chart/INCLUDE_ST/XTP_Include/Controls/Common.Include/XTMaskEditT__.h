// XTMaskEditT.h interface for the CXTMaskEditT class.
//
// This file is a part of the XTREME TOOLKIT MFC class library.
// ?004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTMASKEDITEX_H__)
#define __XTMASKEDITEX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// Summary: CXTMaskEditT is a template class. It allows text masking to be
//			applied to the control to format it for special editing restrictions.
template<class T>
class CXTMaskEditT : public T
{
	
public:
    // Summary: Constructs a CXTMaskEditT object
	CXTMaskEditT()
		: m_nStartChar( 0 )
		, m_nEndChar( 0 )
		, m_bOverType( true )
		, m_bUseMask( true )
		, m_bRedo( false )
		, m_bModified( false )
		, m_strWindowText( _T( "" ) )
		, m_strMask( _T( "" ) )
		, m_strLiteral( _T( "" ) )
		, m_strDefault( _T( "" ) )
		, m_strUndoBuffer( _T( "" ) )
		, m_chPrompt( _T('_') )
	{
	}
	
public:
	
	// Input:	bUseMask - true to enable the mask. false to disable the mask.
	// Summary:	Call this member function to enable or disable the mask for the mask
	//			edit control.
	void SetUseMask(bool bUseMask) {
		m_bUseMask = bUseMask;
	}
	
	// Returns: true if the mask can be used, otherwise returns false.
	// Summary:	This member function is called to determine if the mask for the edit
	//			control can be used. 
	bool CanUseMask() {
		return m_bUseMask;
	}
	
	// Input:	bOverType - true to enable type over.
	// Summary:	This member function is called to enable or disable type over, also
	//			known as insert mode.
	void SetOverType(bool bOverType) {
		m_bOverType = bOverType;
	}
	
	// Returns: true if type over is enabled, otherwise returns false.
	// Summary:	This member function is called to determine if type over has been enabled.
	bool CanOverType() {
		return m_bOverType;
	}
	
	// Input:	iPos - Index of the character to check.
	// Returns: true if the index is valid, otherwise returns false.
	// Summary:	This member function is called to determine if the index specified
	//			by 'iPos' is a valid index for the currently displayed edit text. 
	bool PosInRange(int iPos) {
		return ( ( iPos >= 0 ) && ( iPos < m_strLiteral.GetLength( ) ) );
	}
	
	// Returns: A TCHAR data type.
	// Summary:	This member function retrieves the character that is currently used as
	//			the mask prompt. The mask prompt indicates that the field is editable.
	TCHAR GetPromptChar() {
		return m_chPrompt;
	}
	
	// Input:	ch - A TCHAR data type.
	// Summary:	This member function is called to set the prompt character that is
	//			displayed to the user that indicates the field can be edited.
	void SetPromptChar(TCHAR ch)
	{
		m_strLiteral.Replace( m_chPrompt, ch );
		m_strWindowText.Replace( m_chPrompt, ch );
		
		m_chPrompt = ch;
		SetWindowText( m_strWindowText );
		
	}
	
	// Summary: This member function is called to perform a cut operation using the
	//			currently selected text.
	void MaskCut()
	{
		if ( !CanUseMask( )) 
		{
			CEdit::Cut();
		} 
		else
		{		
			MaskCopy( );
			MaskClear( );
			
			m_bRedo = false; 
			m_bModified = true;
			SetWindowText( m_strWindowText );
			
			CorrectPosition( m_nStartChar );
			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}
		
	}
	
	// Summary: This member function is called to perform a copy operation using the
	//			currently selected text.
	void MaskCopy()
	{
		if ( !CanUseMask( )) 
		{
			CEdit::Copy();
		} 
		else
		{
			GetSel( m_nStartChar, m_nEndChar );
			
			CString strBuffer;
			int i;
			for ( i = m_nStartChar; i < m_nEndChar; ++i )
			{
				if ( m_strLiteral.GetAt( i ) == m_chPrompt )
				{
					if ( m_strWindowText.GetAt( i ) == m_chPrompt )
						continue;
					
					strBuffer += m_strWindowText.GetAt( i );
				}
			}
			
			CopyToClipboard( strBuffer );
		}
		
	}
	
	// Summary: This member function is called to perform a paste operation using the
	//			current clipboard text.
	void MaskPaste()
	{
		if ( !CanUseMask( )) 
		{
			CEdit::Paste();
		} 
		else
		{
			GetSel( m_nStartChar, m_nEndChar );
			MaskClear( );
			
			if ( !OpenClipboard( ) ) 
				return; 
			
			LPTSTR lptstrPaste = NULL;
			HGLOBAL hglbPaste = ::GetClipboardData( CF_TEXT ); 
			if ( hglbPaste != NULL ) 
			{ 
				lptstrPaste = ( TCHAR* )::GlobalLock( hglbPaste ); 
				if ( lptstrPaste != NULL ) 
				{ 
					::GlobalUnlock( hglbPaste ); 
				} 
			} 
			
			::CloseClipboard();
			
			int i, x = m_nStartChar, iLen = _tcslen( lptstrPaste );
			
			for ( i = 0; i < iLen; ++i )
			{
				UINT ch = lptstrPaste[ i ];
				
				if ( CheckChar( ch, false ) )
				{
					InsertCharAt( x, ( TCHAR )ch );
					++x;
					
					if ( x == m_strWindowText.GetLength( ) )
						break;
				}
			}
			
			m_bRedo = false; 
			m_bModified = true;
			SetWindowText( m_strWindowText );
			
			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}
		
	}
	
	// Summary: This member function is called to clear the current text selection.
	void MaskClear()
	{
		if ( !CanUseMask( )) 
		{
			CEdit::Clear();
		} 
		else
		{
			if ( m_nStartChar == m_nEndChar )
				return;
			
			// check to see if there is anything left to delete
			int iLength = m_strWindowText.GetLength( );
			
			int i;
			for ( i = m_nStartChar; i < iLength; ++i )
			{
				TCHAR ch1 = m_strLiteral.GetAt( i );
				TCHAR ch2 = m_strWindowText.GetAt( i );
				
				if ( ( ch1 == m_chPrompt ) && ( ch2 != m_chPrompt ) )
				{
					break;
				}
			}
			
			if ( i == iLength )
			{
				::MessageBeep( ( UINT )-1 );
				return;
			}
			
			for ( i = m_nStartChar; i < m_nEndChar; ++i )
			{
				if ( m_strLiteral.GetAt( i ) == m_chPrompt )
				{
					DeleteCharAt( m_nStartChar );
				}
			}
			
			m_bRedo = false; 
			m_bModified = true;
			SetWindowText( m_strWindowText );
			
			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}
		
	}
	
	// Summary: This member function is called to undo the previous action.
	void MaskUndo()
	{
		if ( !CanUseMask( )) 
		{
			CEdit::Undo();
		} 
		else
		{
			GetSel( m_nStartChar, m_nEndChar );
			
			if ( m_bRedo )
			{
				m_bRedo = false; 
				m_bModified = true;
				SetWindowText( m_strRedoBuffer );
			}
			else
			{
				m_bRedo = true; 
				m_bModified = true;
				GetWindowText( m_strRedoBuffer );
				SetWindowText( m_strUndoBuffer );
			}
			
			GetWindowText( m_strWindowText );
			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}
			
	}
	
	// Summary: This member function is called to select all text in the mask edit
	//			control.
	void MaskSelectAll()
	{
		if ( !CanUseMask( )) 
		{
			CEdit::SetSel(0, -1);
		} 
		else
		{
			m_nStartChar = 0;
			CorrectPosition( m_nStartChar );
			SetSel(m_nStartChar, -1);
		}
	}
	// Returns: true if the text has changed, otherwise returns false.
	// Summary:	This member function is called to determine if the text has been modified.
	bool IsModified() {
		return m_bModified;
	}
	
	
	// Input:	lpszMask - The format for the mask field. For example, if you wanted to set
	//			the mask for a phone number, and you only wanted digits to be entered,
	//			your mask might look like this; _T("(000) 000-0000"). 
	//			lpszLiteral - The literal format is entered here. Wherever you place an underscore
	//			('_') is where the user will be allowed to enter data only. Using
	//			the phone number example; _T("(___) ___-____"). 
	//			lpszDefault - Text that is to be displayed when the control is initialized. For
	//			example; _T("(800) 555-1212"). If NULL, 'lpszLiteral' is used to initialize
	//			the edit text.
	// Summary:	This member function will set the mask for the edit control. The values 
	//			that can be set are:
	//
	//			<pre>
	//			<b>Mask Character</b>    <b>Description</b>
	//			-------------------------------------------------
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
	virtual void SetEditMask(LPCTSTR lpszMask,LPCTSTR lpszLiteral,LPCTSTR lpszDefault=NULL)
	{
		ASSERT(lpszMask);
		ASSERT(lpszLiteral);
		
		// initialize the mask for the control.
		m_strMask    = lpszMask;
		m_strLiteral = lpszLiteral;
		
		if ( lpszDefault == NULL )
		{
			m_strWindowText = lpszLiteral;
			m_strDefault    = lpszLiteral;
		}
		else
		{
			m_strWindowText = lpszDefault;
			m_strDefault    = lpszDefault;
		}
		
		// set the window text for the control.
		m_bRedo = false; 
		m_bModified = false;
		SetWindowText( m_strWindowText );
		
		m_strUndoBuffer = m_strWindowText;
		
		// initialize the font used by the control.
		//SetFont( &xtAfxData.font );
		
	}
	
	// Input:	nChar - Contains the character code value of the key.
	//			bBeep - true to enable beep. false to disable beep.
	// Returns: true if successful, otherwise returns false.
	// Summary:	This member function is used internally to validate the character indicated
	//			by 'nChar'. 
	virtual bool CheckChar(UINT& nChar,bool bBeep=true)
	{
		// do not use mask
		if ( !CanUseMask( ) )
		{
			return false;
		}
		
		// control character, OK
		if ( !_istprint( ( TCHAR )nChar ) )
		{
			return true;
		}
		
		// advance to the first character input position.
		int i;
		for ( i = m_nStartChar; i < m_strLiteral.GetLength( ); ++i )
		{
			if ( m_strLiteral.GetAt( i ) == m_chPrompt )
			{
				SetSel( i, i );
				break;
			}
		}
		
		// make sure the string is not longer than the mask
		if ( i >= m_strMask.GetLength( ) )
		{
			if ( bBeep )
			{
				:: MessageBeep( ( UINT )-1 );
			}
			return false;
		}
		
		if ( !ProcessMask( nChar, i ) )
		{
			if ( bBeep )
			{
				:: MessageBeep( ( UINT )-1 );
			}
			return false;
		}
		
		return true;
		
	}
	
	// Input:	nChar - Contains the character code value of the key.
	//			nEndPos - Index of character in display string.
	// Returns: true if successful, otherwise returns false.
	// Summary:	This member function is used internally to process the character passed
	//			in by 'nChar' whose index is specified by 'nEndPos'. 
	virtual bool ProcessMask(UINT& nChar,int nEndPos)
	{
		// check the key against the mask
		switch ( m_strMask.GetAt( nEndPos ) )
		{
		case '0':		// digit only //completely changed this
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case '9':		// digit or space
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case '#':		// digit or space or '+' or '-'
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' || nChar == '-' || nChar == '+' ) {
					return true;
				}
				break;
			}
		case 'L':		// alpha only
			{
				if ( _istalpha( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case '?':		// alpha or space
			{
				if ( _istalpha( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case 'A':		// alpha numeric only
			{
				if ( _istalnum( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case 'a':		// alpha numeric or space
			{
				if ( _istalnum( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case '&':		// all print character only
			{
				if ( _istprint( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case 'H':		// hex digit
			{
				if ( _istxdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case 'X':		// hex digit or space
			{
				if ( _istxdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case '>':
			{
				if ( _istalpha( ( TCHAR )nChar ) )
				{
					if ( _istlower( ( TCHAR )nChar ) )
					{
						nChar = _toupper( ( TCHAR )nChar );
					}
					return true;
				}
				break;
			}
		case '<':
			{
				if ( _istalpha( ( TCHAR )nChar ) )
				{
					if ( _istupper( ( TCHAR )nChar ) )
					{
						nChar = _tolower( ( TCHAR )nChar );
					}
					return true;
				}
				break;
			}
		}
	
		return false;
	}
	
public:
//:Ignore
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ( CanUseMask( ) == false )
		{
			return T::PreTranslateMessage(pMsg);
		}
		
		// intercept Ctrl+C (copy), Ctrl+V (paste), Ctrl+X (cut) and Ctrl+Z (undo)
		// before CEdit base class gets ahold of them.
		
		if ( pMsg->message == WM_KEYDOWN )
		{
			if ( ::GetKeyState( VK_SUBTRACT ) < 0 )
			{
				OnChar( '-', 1, 1 );
				return TRUE;
			}
			
			if ( ::GetKeyState( VK_ADD ) < 0 )
			{
				OnChar( '+', 1, 1 );
				return TRUE;
			}
			
			if ( ::GetKeyState( VK_CONTROL ) < 0 )
			{
				switch ( pMsg->wParam )
				{
				case 'X':
				case 'x':
					{
						MaskCut( );
						return TRUE;
					}
					
				case 'C':
				case 'c':
					{
						MaskCopy( );
						return TRUE;
					}
					
				case 'V':
				case 'v':
					{
						MaskPaste( );
						return TRUE;
					}
					
				case 'Z':
				case 'z':
					{
						MaskUndo( );
						return TRUE;
					}
				}
			}
			else
			{
				switch ( pMsg->wParam )
				{
				case '-':
					{
						OnChar( '-', 1, 1 );
						return TRUE;
					}
					
				case '+':
					{
						OnChar( '+', 1, 1 );
						return TRUE;
					}
				}
			}
		}
			
		return T::PreTranslateMessage(pMsg);
		
	}
	virtual bool CorrectPosition(int& iPos, bool bForward=true, bool bBeep=true) 
	{
		GetWindowText( m_strWindowText );
		int iLen = m_strWindowText.GetLength( );
		
		if ( iPos == iLen )
		{
			if ( bBeep )
			{
				::MessageBeep( ( UINT )-1 );
			}
			return false;
		}
		
		if ( PosInRange( iPos ) == false )
			return false;
		
		if ( m_strLiteral.GetAt( iPos ) != m_chPrompt )
		{
			int i;
			if ( bForward )
			{
				for ( i = iPos; i < iLen; ++i )
				{
					if ( m_strLiteral.GetAt( i ) == m_chPrompt )
					{
						iPos = i;
						break;
					}
				}
			}
			else
			{
				for ( i = iPos; i >= 0; --i )
				{
					if ( m_strLiteral.GetAt( i ) == m_chPrompt )
					{
						iPos = i;
						break;
					}
				}
				
				if ( i == -1 )
				{
					iPos++;
					
					if ( bBeep )
					{
						::MessageBeep( ( UINT )-1 );
					}
					return false;
				}
			}
			
			if ( i == iLen )
			{
				if ( bBeep )
				{
					::MessageBeep( ( UINT )-1 );
				}
				return false;
			}
		}
		return true;
		
	}
	
	virtual void DeleteCharAt(int iPos)
	{
		if ( PosInRange( iPos ) == false )
			return;
		
		CString strBuffer = GetMaskedText( iPos+1 );
		strBuffer += m_chPrompt;
		
		int x = 0;
		int iLen = strBuffer.GetLength( );
		int i;
		for (i = iPos; i < m_strLiteral.GetLength( ); ++i )
		{
			if ( ( m_strLiteral.GetAt( i ) == m_chPrompt ) )
			{
				m_strWindowText.SetAt( i, strBuffer.GetAt( x ) );
				++x;
				
				if ( x == iLen )
					break;
			}
		}
		
		
	}
	virtual void InsertCharAt(int iPos, TCHAR nChar)
	{
		if ( PosInRange( iPos ) == false )
		{
			return;
		}
		
		UINT uChar = nChar;
		if ( !CheckChar( uChar, false ) )
		{
			return;
		}
		
		CString strBuffer = GetMaskedText( iPos );
		
		CString strTemp(nChar);
		int i, x = 0, iLen = strBuffer.GetLength( );
		for ( i = 1; i < strBuffer.GetLength(); ++i )
		{
			strTemp += strBuffer.GetAt( i-1 );
		}
		strBuffer = strTemp;
		
		for ( i = iPos; i < m_strWindowText.GetLength( ); ++i )
		{
			if ( m_strLiteral.GetAt( i ) == m_chPrompt )
			{
				m_strWindowText.SetAt( i, strBuffer[ x ] );
				++x;
				
				if ( x == iLen )
					break;
			}
		}
	}
	virtual bool CopyToClipboard(LPCTSTR lpszText)
	{
		if ( !OpenClipboard( ) ) 
			return false;
		
		::EmptyClipboard( ); 
		
		int iLen = _tcslen( lpszText ) + 1;
		
		HGLOBAL hglbCopy = ::GlobalAlloc( GMEM_MOVEABLE, iLen );
		
		if ( hglbCopy == NULL )
		{ 
			::CloseClipboard( );
			return false;
		}
		
		LPTSTR lptstrCopy = ( TCHAR* )::GlobalLock( hglbCopy );
		memcpy( lptstrCopy, lpszText, iLen );
		::GlobalUnlock( hglbCopy ); 
		
		::SetClipboardData( CF_TEXT, hglbCopy ); 
		
		if ( !::CloseClipboard( ) )
			return false;
		
		return true;
		
	}
	virtual CString GetMaskedText(int iPos=0)
	{
		int iWndLen = m_strWindowText.GetLength( );
		int iLitlen = m_strLiteral.GetLength( );
		
		ASSERT( iWndLen ==iLitlen ); // must be equal in length
		
		CString strBuffer;
		int i;
		for ( i = iPos; i < iWndLen; ++i )
		{
			if ( m_strLiteral.GetAt( i ) == m_chPrompt )
			{
				strBuffer += m_strWindowText.GetAt( i );
			}
		}
		
		return strBuffer;
		
	}
//:End Ignore
	
	
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Call this member function to determine if a selection has been made
	virtual BOOL SelectionMade() {
		GetSel( m_nStartChar, m_nEndChar ); return ( m_nStartChar != m_nEndChar );
	}
		
	
protected: 
		
	
//:Ignore
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if ( CanUseMask( ) == false )
		{
			T::OnKeyDown( nChar, nRepCnt, nFlags ); // default processing.
			return;
		}
		
		GetWindowText( m_strWindowText ); // refresh
		GetSel( m_nStartChar, m_nEndChar );
		
		if ( m_strMask.IsEmpty( ) )
		{
			T::OnKeyDown( nChar, nRepCnt, nFlags ); // default processing.
			return;
		}
		
		bool bShift = (::GetKeyState(VK_SHIFT) < 0);
		
		switch ( nChar )
		{
		case VK_UP:
		case VK_LEFT:
			{
				T::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );
				
				int iStartChar = m_nStartChar;
				CorrectPosition( iStartChar, false, false );
				
				if ( m_nStartChar < iStartChar )
				{
					m_nStartChar = iStartChar;
					
					if ( !bShift )
						m_nEndChar = iStartChar;
					
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;
			
		case VK_DOWN:
		case VK_RIGHT:
			{
				T::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );
				
				int iEndChar = m_nEndChar;
				CorrectPosition( iEndChar, true, false );
				
				if ( m_nEndChar > iEndChar )
				{
					m_nEndChar = iEndChar;
					
					if ( !bShift )
						m_nStartChar = iEndChar;
					
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;
			
		case VK_HOME:
			{
				T::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );
				
				int iStartChar = m_nStartChar;
				CorrectPosition( iStartChar, true, false );
				
				if ( m_nStartChar < iStartChar )
				{
					m_nStartChar = iStartChar;
					
					if ( bShift == false )
					{
						m_nEndChar = m_nStartChar;
					}
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;
			
		case VK_END:
			{
				T::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );
				
				int iEndChar = m_nEndChar;
				CorrectPosition( iEndChar, true, false );
				
				if ( m_nEndChar > iEndChar )
				{
					m_nEndChar = iEndChar;
					
					if ( !bShift )
						m_nStartChar = iEndChar;
					
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;
			
		case VK_INSERT:
			{
				if ( bShift )
				{
					MaskPaste( );
				}
				else
				{
					m_bOverType = !m_bOverType; // set the type-over flag
				}
			}
			return;
			
		case VK_DELETE:
			{
				if ( m_nStartChar == m_nEndChar )
				{
					if ( CorrectPosition( m_nStartChar ) )
					{
						DeleteCharAt( m_nStartChar );
						
						m_bRedo = false; 
						m_bModified = true;
						SetWindowText( m_strWindowText );
						
						m_nEndChar = m_nStartChar;
						SetSel( m_nStartChar, m_nEndChar );
					}
				}
				else
				{
					MaskClear( );
				}
			}
			return;
			
		case VK_SPACE:
			{
				GetWindowText( m_strWindowText ); // refresh
				
				if ( CorrectPosition( m_nStartChar ) )
				{
					if ( m_nStartChar < m_strLiteral.GetLength( ) )
					{
						if ( ProcessMask( nChar, m_nStartChar ) )
						{
							if ( CanOverType( ) )
							{
								m_strWindowText.SetAt( m_nStartChar, ' ' );
							}
							else
							{
								InsertCharAt( m_nStartChar, ( TCHAR )nChar );
							}
						}
						else
						{
							m_strWindowText.SetAt( m_nStartChar, m_chPrompt );
						}
						
						m_bRedo = false; 
						m_bModified = true;
						SetWindowText( m_strWindowText );
						
						m_nStartChar++;
						m_nEndChar = m_nStartChar;
						SetSel( m_nStartChar, m_nEndChar );
					}
					else
					{
						::MessageBeep( ( UINT )-1 );
					}
				}
			}
			return;
			
		case VK_BACK:
			{
				GetWindowText( m_strWindowText ); // refresh
				
				if ( ( m_nStartChar > 0 ) && 
					( m_nStartChar <= m_strLiteral.GetLength( ) ) )
				{
					m_nStartChar--;
					
					if ( CorrectPosition( m_nStartChar, false ) )
					{
						TCHAR ch = m_chPrompt;
						
						// get the masked literal representation.
						if ( !m_strDefault.IsEmpty( ) )
						{
							ch = m_strDefault.GetAt( m_nStartChar );
						}
						
						m_strWindowText.SetAt( m_nStartChar, ch );
						
						m_bRedo = false; 
						m_bModified = true;
						SetWindowText( m_strWindowText );
						
						m_nEndChar = m_nStartChar;
						SetSel( m_nStartChar, m_nEndChar );
					}
				}
				else 
				{
					::MessageBeep( ( UINT )-1 );
				}
			}
			return;
		}
	
		T::OnKeyDown( nChar, nRepCnt, nFlags );
	
	}
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if ( CanUseMask( ) == false )
		{
			T::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
		
		switch ( nChar )
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
		
		if ( CheckChar( nChar ) == false )
		{
			return;
		}
		
		if ( CanUseMask( ) )
		{
			if ( _istprint( ( TCHAR )nChar ) )
			{
				int iLen = m_strLiteral.GetLength( );
				if ( m_nStartChar >= iLen )
				{
					::MessageBeep( ( UINT )-1 );
					return;
				}
				
				if ( m_nEndChar >= iLen )
					m_nEndChar = iLen-1;
				
				if ( m_nStartChar != m_nEndChar )
				{
					int i;
					for ( i = m_nStartChar; i < m_nEndChar; ++i )
					{
						if ( m_strLiteral.GetAt( i ) == m_chPrompt )
						{
							DeleteCharAt( m_nStartChar );
						}
					}
				}
				
				SetWindowText( m_strWindowText );
				
				if ( CorrectPosition( m_nStartChar ) )
				{
					if ( CanOverType( ) )
					{
						if ( m_nStartChar != m_nEndChar )
						{
							InsertCharAt( m_nStartChar, ( TCHAR )nChar );
						}
						else
						{
							m_strWindowText.SetAt( m_nStartChar, ( TCHAR )nChar );
						}
					}
					else
					{
						InsertCharAt( m_nStartChar, ( TCHAR )nChar );
					}
				}
				
				m_bRedo = false; 
				m_bModified = true;
				SetWindowText( m_strWindowText );
				
				if (m_nStartChar < iLen )
					m_nStartChar++;
				
				if ( m_nStartChar < iLen )
					CorrectPosition( m_nStartChar );
				
				m_nEndChar = m_nStartChar;
				SetSel( m_nStartChar, m_nEndChar );
				
				return;
			}
		}
		
		T::OnChar(nChar, nRepCnt, nFlags);
		
		// update the window text string.
		GetWindowText( m_strWindowText );
		
	}
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
	{
		T::OnLButtonDown(nFlags, point);
		
		if ( CanUseMask( ) == false )
		{
			return;
		}
		
		GetSel( m_nStartChar, m_nEndChar );
		CorrectPosition( m_nStartChar, true, false );
		
		if ( ::GetKeyState( VK_SHIFT ) < 0 )
			m_nEndChar = m_nStartChar;
		
		SetSel( m_nStartChar, m_nEndChar );
		
	}
	afx_msg void OnSetFocus(CWnd* pOldWnd)
	{
		T::OnSetFocus(pOldWnd);
		
		if ( CanUseMask( ) == false )
		{
			return;
		}
		
		GetSel( m_nStartChar, m_nEndChar );
		CorrectPosition( m_nStartChar, true, false );
		
		m_nEndChar = m_nStartChar;
		SetSel( m_nStartChar, m_nEndChar );
		
	}
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		T::OnLButtonDblClk(nFlags, point);
		
		if ( CanUseMask( ) == false )
		{
			return;
		}
		
		GetSel( m_nStartChar, m_nEndChar );
		CorrectPosition( m_nStartChar, true, false );
		CorrectPosition( m_nEndChar, true, false );
		
		SetSel( m_nStartChar, m_nEndChar );
		
	}
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI)
	{
		if ( CanUseMask( ) == false )
		{
			pCmdUI->Enable( CanUndo( ) );
		}
		else
		{
			pCmdUI->Enable( m_bModified );
		}
		
	}
	
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		switch (LOWORD(wParam))
		{		
			case ID_EDIT_CUT:
				if ( !CanUseMask( )) CEdit::Cut(); else MaskCut( );
				return TRUE;

			case ID_EDIT_COPY:
				if ( !CanUseMask( )) CEdit::Copy(); else MaskCopy( );
				return TRUE;
		
			case ID_EDIT_PASTE:
				if ( !CanUseMask()) CEdit::Paste(); else MaskPaste( );
				return TRUE;

			case ID_EDIT_CLEAR:
				if ( !CanUseMask()) CEdit::Clear(); else MaskClear( );
				return TRUE;
	
			case ID_EDIT_UNDO:
				if ( !CanUseMask()) CEdit::Undo(); else MaskUndo( );
				return TRUE;

			case ID_EDIT_SELECT_ALL:
				MaskSelectAll( );
				return TRUE;
		}
		return T::OnCommand(wParam, lParam);		
	}
//:End Ignore
	
protected:
	
	int			m_nStartChar;		// Current position of the first character in the current selection.
	int			m_nEndChar;			// Current position of the first non-selected character past the end of the current selection.
	bool		m_bUseMask;			// true to use the edit mask.
	bool		m_bOverType;		// true to over type the text, set with VK_INSERT key press.
	bool		m_bRedo;			// true to redo, or false to undo.
	bool		m_bModified;		// true if mask edit has been modified.
	TCHAR		m_chPrompt;			// Prompt character used to identify the text entry.
	CString		m_strMask;			// Buffer that holds the actual edit mask value.
	CString		m_strDefault;		// Contains the edit controls default display text.
	CString		m_strUndoBuffer;	// Holds the contents of the undo buffer.
	CString		m_strRedoBuffer;	// Holds the contents of the redo buffer.
	CXTString	m_strWindowText;	// Buffer that holds the actual edit text.
	CXTString	m_strLiteral;		// Literal format that restricts where the user can enter text.
	
};

//////////////////////////////////////////////////////////////////////


#endif // #if !defined(__XTMASKEDITEX_H__)
