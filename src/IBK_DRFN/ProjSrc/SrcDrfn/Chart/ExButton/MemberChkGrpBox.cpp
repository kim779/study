// MemberChkGrpBox.cpp: implementation of the CMemberChkGrpBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExButton.h"
#include "MemberChkGrpBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberChkGrpBox::CMemberChkGrpBox(LPVOID lpControl) : CMember(lpControl)
{
	m_pButton = (CButton_Friendly*)lpControl;
	m_nStyle = TCH_ALL;
}

CMemberChkGrpBox::~CMemberChkGrpBox()
{

}
