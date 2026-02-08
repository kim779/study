// SemiMemManager.cpp: implementation of the CSemiMemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemiMemManager.h"
#include "../Common_ST/DataItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemiMemManager::CSemiMemManager(CMemManager* pParent /*= NULL*/)
:CMemManager(pParent)
{

}

CSemiMemManager::~CSemiMemManager()
{

}

BOOL CSemiMemManager::Condition(BOOL bCondition)
{
	if(!bCondition)
	{
		m_lSubInPos++;
	}
	return bCondition;
}

CMemManager* CSemiMemManager::SubOut(long lPos)
{
	CMemManager::Next();
	return CMemManager::SubOut(lPos);
}

inline BOOL CSemiMemManager::GetVarData(LPCTSTR lpName, double dIndex, double* pdValue)
{
	return this->m_pParent->GetVarData(lpName,dIndex,pdValue);
}

inline BOOL CSemiMemManager::SetVarData(LPCTSTR lpName, double dIndex, double dValue)
{
	return this->m_pParent->SetVarData(lpName,dIndex,dValue);
}

inline BOOL CSemiMemManager::GetInputData(LPCTSTR lpName, double dIndex, double* pdValue)
{
	return this->m_pParent->GetInputData(lpName,dIndex,pdValue);
}

inline BOOL CSemiMemManager::SetInputData(LPCTSTR lpName, double dIndex, double dValue)
{
	return this->m_pParent->SetInputData(lpName,dIndex,dValue);
}

inline BOOL CSemiMemManager::GetVarHolderData(long lHolderIndex, double dIndex, double* pdValue)
{
	return this->m_pParent->GetVarHolderData(lHolderIndex,dIndex,pdValue);
}

inline BOOL CSemiMemManager::SetVarHolderData(long lHolderIndex, double dIndex, double dValue)
{
	return this->m_pParent->SetVarHolderData(lHolderIndex,dIndex,dValue);
}

inline BOOL CSemiMemManager::GetInputHolderData(long lHolderIndex, double dIndex, double* pdValue)
{
	return this->m_pParent->GetInputHolderData(lHolderIndex,dIndex,pdValue);
}

inline BOOL CSemiMemManager::SetInputHolderData(long lHolderIndex, double dIndex, double dValue)
{
	return this->m_pParent->SetInputHolderData(lHolderIndex,dIndex,dValue);
}

inline BOOL CSemiMemManager::GetFuncData(double dIndex, double* pdValue)
{
	return this->m_pParent->GetFuncData(dIndex,pdValue);
}

inline BOOL CSemiMemManager::SetFuncData(double dIndex, double dValue)
{
	return this->m_pParent->SetFuncData(dIndex,dValue);
}

BOOL CSemiMemManager::SetVarHolder(long lHolderIndex,LPCTSTR lpName)
{
	return this->m_pParent->SetVarHolder(lHolderIndex,lpName);
}

BOOL CSemiMemManager::SetInputHolder(long lHolderIndex,LPCTSTR lpName)
{
	return this->m_pParent->SetInputHolder(lHolderIndex,lpName);
}

long CSemiMemManager::GetVarPointer(LPCTSTR lpName)
{
	return this->m_pParent->GetVarPointer(lpName);
}

long CSemiMemManager::GetInputPointer(LPCTSTR lpName)
{
	return this->m_pParent->GetInputPointer(lpName);
}

BOOL CSemiMemManager::IncludeSubFunc(LPCTSTR lpFuncName)
{
	return this->m_pParent->IncludeSubFunc(lpFuncName);
}

double CSemiMemManager::SubFunc( long lSTActor,long lIndexFunc, long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 )
{
	return this->m_pParent->SubFunc(lSTActor, lIndexFunc, lIndex, bTemp0, bTemp1,bTemp2 , bTemp3 ,bTemp4 , bTemp5 ,bTemp6 , bTemp7 ,bTemp8 , bTemp9 );
}

BOOL CSemiMemManager::SetSubFuncHolder(long lHolderIndex,LPCTSTR lpName)
{
	return this->m_pParent->SetSubFuncHolder(lHolderIndex,lpName);
}

void CSemiMemManager::SetValueInBuffer(long lIndex,double dValue)
{
	this->m_pParent->SetValueInBuffer(lIndex,dValue);
}

double CSemiMemManager::GetValueInBuffer(long lIndex)
{
	return this->m_pParent->GetValueInBuffer(lIndex);
}

long CSemiMemManager::GetInputHolder(long lHolderIndex)
{
	return this->m_pParent->GetInputHolder(lHolderIndex);
}

BOOL CSemiMemManager::SetInnerFuncHolder(long lHolderIndex,CInnerFunction *pInnerFunc)
{
	return this->m_pParent->SetInnerFuncHolder(lHolderIndex,pInnerFunc);
}

CInnerFunction *CSemiMemManager::GetInnerFuncFromHolder(long lHolderIndex)
{
	return this->m_pParent->GetInnerFuncFromHolder(lHolderIndex);
}

BOOL CSemiMemManager::GetArrayHolderData(long lHolderIndex, double dIndex, double* pdValue)
{
	return this->m_pParent->GetArrayHolderData(lHolderIndex,dIndex,pdValue);
}

BOOL CSemiMemManager::SetArrayHolderData(long lHolderIndex, double dIndex, double dValue)
{
	return this->m_pParent->SetArrayHolderData(lHolderIndex,dIndex,dValue);
}

BOOL CSemiMemManager::SetArrayHolder(long lHolderIndex,LPCTSTR lpName)
{
	return this->m_pParent->SetArrayHolder(lHolderIndex,lpName);
}

void CSemiMemManager::SetOffset(long lOffset)
{
}

long CSemiMemManager::GetOffset()
{
	return this->m_pParent->GetOffset();
}
