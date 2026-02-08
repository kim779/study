// MemberMask.h: interface for the CMemberMask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERMASK_H__3CA7B088_295A_4B90_9184_5E3C4CE4D9E3__INCLUDED_)
#define AFX_MEMBERMASK_H__3CA7B088_295A_4B90_9184_5E3C4CE4D9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemberDraw.h"

class CMemberMask : public CMemberDraw 
{
public:
	CMemberMask(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberMask();

public:
	int			m_nStartChar;		// Current position of the first character in the current selection.
	int			m_nEndChar;			// Current position of the first non-selected character past the end of the current selection.
	BOOL		m_bUseMask;			// TRUE to use the edit mask.
	BOOL		m_bOverType;		// TRUE to over type the text, set with VK_INSERT key press.
	BOOL		m_bRedo;			// TRUE to redo, or FALSE to undo.
	BOOL		m_bModified;		// TRUE ifmask edit has been modified.
	TCHAR		m_chPrompt;			// Prompt character used to identify the text entry.
	CString		m_strMask;			// Buffer that holds the actual edit mask value.
	CString		m_strDefault;		// Contains the edit controls default display text.
	CString		m_strUndoBuffer;	// Holds the contents of the undo buffer.
	CString		m_strRedoBuffer;	// Holds the contents of the redo buffer.
	CString		m_strWindowText;	// Buffer that holds the actual edit text.
	CString		m_strLiteral;		// Literal format that restricts where the user can enter text.
};

#endif // !defined(AFX_MEMBERMASK_H__3CA7B088_295A_4B90_9184_5E3C4CE4D9E3__INCLUDED_)
