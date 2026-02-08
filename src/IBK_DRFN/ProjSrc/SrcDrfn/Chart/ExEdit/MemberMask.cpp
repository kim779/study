// MemberMask.cpp: implementation of the CMemberMask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExEdit.h"
#include "MemberMask.h"

#include "../../inc/ISkinColorMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberMask::CMemberMask(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMemberDraw(lpControl, pISkinColorManager, bUseDefSkin)
		, m_nStartChar(0)
		, m_nEndChar(0)
		, m_bOverType(TRUE)
		, m_bUseMask(TRUE)
		, m_bRedo(FALSE)
		, m_bModified(FALSE)
		, m_strWindowText(_T(""))
		, m_strMask(_T(""))
		, m_strLiteral(_T(""))
		, m_strDefault(_T(""))
		, m_strUndoBuffer(_T(""))
		, m_chPrompt(_T('_'))
{
}

CMemberMask::~CMemberMask()
{

}
