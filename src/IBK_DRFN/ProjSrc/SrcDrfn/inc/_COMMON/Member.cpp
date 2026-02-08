// Member.cpp: implementation of the CMember class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Member.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMember::CMember(LPVOID lpControl, BOOL bUseDefSkin/* = FALSE*/) :
	m_pControl(lpControl),
	m_pISkinColorManager(NULL),
	m_bUseOnlyDefaultSkinTable(bUseDefSkin)
{
}

CMember::~CMember()
{
}
