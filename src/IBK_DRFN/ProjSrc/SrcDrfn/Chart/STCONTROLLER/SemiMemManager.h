// SemiMemManager.h: interface for the CSemiMemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMIMEMMANAGER_H__7B3B57EE_962D_4710_B7AA_CCC6B617A3EB__INCLUDED_)
#define AFX_SEMIMEMMANAGER_H__7B3B57EE_962D_4710_B7AA_CCC6B617A3EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MemManager.h"

class CSemiMemManager  : public CMemManager
{
public:
	CSemiMemManager(CMemManager* pParent = NULL);
	virtual ~CSemiMemManager();
	virtual BOOL SetInputs(long lPos, long lIndex, LPCTSTR lpName, LPCTSTR lpType, double dInitValue) { return FALSE;}
	virtual BOOL SetVariables(long lPos, LPCTSTR lpName, LPCTSTR lpType, double dInitValue) { return FALSE;}
	virtual BOOL SetArrays(long lPos, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData) { return FALSE;}
	virtual BOOL Condition(BOOL bCondition);
	inline virtual BOOL GetVarData(LPCTSTR lpName, double dIndex, double* pdValue);
	inline virtual BOOL SetVarData(LPCTSTR lpName, double dIndex, double dValue);	
	inline virtual BOOL GetInputData(LPCTSTR lpName, double dIndex, double* pdValue);
	inline virtual BOOL SetInputData(LPCTSTR lpName, double dIndex, double dValue);
	virtual CMemManager* SubOut(long lPos);
	inline virtual BOOL GetVarHolderData(long lHolderIndex, double dIndex, double* pdValue);
	inline virtual BOOL SetVarHolderData(long lHolderIndex, double dIndex, double dValue);
	inline virtual BOOL GetInputHolderData(long lHolderIndex, double dIndex, double* pdValue);
	inline virtual BOOL SetInputHolderData(long lHolderIndex, double dIndex, double dValue);
	inline virtual BOOL GetArrayHolderData(long lHolderIndex, double dIndex, double* pdValue);
	inline virtual BOOL SetArrayHolderData(long lHolderIndex, double dIndex, double dValue);
	inline virtual BOOL GetFuncData(double dIndex, double* pdValue);
	inline virtual BOOL SetFuncData(double dIndex, double dValue);
	virtual BOOL SetVarHolder(long lHolderIndex,LPCTSTR lpName);
	virtual BOOL SetArrayHolder(long lHolderIndex,LPCTSTR lpName);
	virtual long GetInputHolder(long lHolderIndex);
	virtual BOOL SetInputHolder(long lHolderIndex,LPCTSTR lpName);
	virtual long GetVarPointer(LPCTSTR lpName);
	virtual long GetInputPointer(LPCTSTR lpName);
	virtual double SubFunc(long lSTActor, long lIndexFunc, long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 );
	virtual BOOL IncludeSubFunc(LPCTSTR lpFuncName);
	virtual BOOL SetSubFuncHolder(long lHolderIndex,LPCTSTR lpName);
	virtual void SetValueInBuffer(long lIndex,double dValue);
	virtual double GetValueInBuffer(long lIndex);
	virtual BOOL SetInnerFuncHolder(long lHolderIndex,CInnerFunction *pInnerFunc);
	virtual CInnerFunction *GetInnerFuncFromHolder(long lHolderIndex);
	virtual void SetOffset(long lOffset);
	virtual long GetOffset();

};

#endif // !defined(AFX_SEMIMEMMANAGER_H__7B3B57EE_962D_4710_B7AA_CCC6B617A3EB__INCLUDED_)
