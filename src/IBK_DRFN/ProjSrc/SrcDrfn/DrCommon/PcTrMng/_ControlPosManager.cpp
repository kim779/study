#include "StdAfx.h"
#include "PcTrMng.h"

/******************************************************************************

					현재 작업중.... 06.07.26
					예상 작업일.... 06년 08월 중.

******************************************************************************/

#include "ControlPos.h"

BOOL CControlPosManager::Advise(CWnd* pParent)
{
	CControlPos* pNew = new CControlPos(pParent);
	m_pReservedForSelp =  pNew;
	return TRUE;
}

BOOL CControlPosManager::UnAdvise()
{
	if(m_pReservedForSelp) delete m_pReservedForSelp; m_pReservedForSelp=NULL;
	return TRUE;
}

void CControlPosManager::SetParent(CWnd* pParent, CSize* pSize)
{
}

BOOL CControlPosManager::AddControl(const UINT& unId, const DWORD& dwStyle, CString csVBName)
{
	return TRUE;
}

BOOL CControlPosManager::AddControl(CWnd* pControl, const DWORD& dwStyle, CString csVBName)
{
	return TRUE;
}


BOOL CControlPosManager::AddControl(CWnd* pControl, const DWORD& dwStyle /* = CP_MOVE_HORIZONTAL */)
{
	return TRUE;
}

BOOL CControlPosManager::AddControl(const UINT& unId, const DWORD& dwStyle /* = CP_MOVE_HORIZONTAL */)
{
	return TRUE;
}


BOOL CControlPosManager::RemoveControl(CWnd* pControl)
{
	return TRUE;
}

BOOL CControlPosManager::RemoveControl(const UINT& unId)
{
	return TRUE;
}

void CControlPosManager::ResetControls()
{
}

void CControlPosManager::ResizeControls(int oldwidth, int oldheight, int newwidth, int newheight)
{
}


//void CControlPosManager::MoveControls()
//{
//}

void CControlPosManager::SetNegativeMoves(const BOOL& fNegativeMoves /* = TRUE */)
{
}

BOOL CControlPosManager::GetNegativeMoves() const
{
	return TRUE;
}

//void CControlPosManager::UpdateParentSize()
//{
//}
