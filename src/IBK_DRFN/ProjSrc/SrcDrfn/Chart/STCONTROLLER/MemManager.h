// MemManager.h: interface for the CMemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMMANAGER_H__A9D6F090_38B4_4893_9F70_468893316959__INCLUDED_)
#define AFX_MEMMANAGER_H__A9D6F090_38B4_4893_9F70_468893316959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "../Common_ST/STControllerDef.h"
#include "TempMemMng.h"


class CSTDataItem;
class CSubLoader;
class CInnerFunction;
class CMemManager  
{
public:
	CMemManager(CMemManager* pParent = NULL);
	virtual ~CMemManager();
	virtual void SetSizeDataItem(long lSize);
	virtual BOOL SetInputs(long lPos, long lIndex, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData);
	virtual BOOL SetVariables(long lPos, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData);
	virtual BOOL SetArray(long lPos, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData);
	virtual BOOL Condition(BOOL bCondition) { return bCondition;}
	virtual CMemManager* SubIn(long lPos, BOOL bSemi );
	virtual CMemManager* SubOut(long lPos);		
	inline virtual BOOL GetVarData(LPCTSTR lpName, double dIndex, double* pdValue);
	inline virtual BOOL SetVarData(LPCTSTR lpName, double dIndex, double dValue);
	inline virtual BOOL GetInputData(LPCTSTR lpName, double dIndex, double* pdValue);
	inline virtual BOOL SetInputData(LPCTSTR lpName, double dIndex, double dValue);
	inline virtual BOOL GetVarHolderData(long lHolderIndex, double dIndex, double* pdValue);
	inline virtual double GetVarHolderData(long lHolderIndex, double dIndex);
	inline virtual BOOL GetInputHolderData(long lHolderIndex, double dIndex, double* pdValue);
	inline virtual BOOL GetArrayHolderData(long lHolderIndex, double dIndex, double* pdValue);
	inline virtual BOOL GetFuncData(double dIndex, double* pdValue);
	inline virtual BOOL SetVarHolderData(long lHolderIndex, double dIndex, double dValue);	
	inline virtual BOOL SetInputHolderData(long lHolderIndex, double dIndex, double dValue);
	inline virtual BOOL SetArrayHolderData(long lHolderIndex, double dIndex, double dValue);
	inline virtual BOOL SetFuncData(double dIndex, double dValue);	
	inline virtual CSTDataItem* GetVarHolderPointer(long lHolderIndex);
	inline virtual CSTDataItem* GetArrayHolderPointer(long lHolderIndex);
	inline virtual CSTDataItem* GetFuncHolderPointer();
	virtual BOOL InitFuncData(long lCntOfAllData);
	virtual BOOL SetVarHolder(long lHolderIndex,LPCTSTR lpName);
	virtual BOOL SetArrayHolder(long lHolderIndex,LPCTSTR lpName);
	virtual BOOL SetInputHolder(long lHolderIndex,LPCTSTR lpName);
	virtual long GetInputHolder(long lHolderIndex);
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
	virtual CITempMemMng *GetITempMemMng();

	BOOL IsFirst();
	void SetCheckNotFirst();
	CString GetSubFuncPath();
	BOOL Next();
	void SetSubFuncPath(LPCTSTR lpSubFunc);
	CString GetErrorMessage() { return m_strError;}
protected:
	long m_lSubInPos;
	CMemManager* m_pParent;
	CMap<CString, LPCSTR, CSTDataItem*, CSTDataItem*> m_mapVarDataItem;
	CMap<CString, LPCSTR, CSTDataItem*, CSTDataItem*> m_mapInputDataItem;
	CMap<CString, LPCSTR, CSTDataItem*, CSTDataItem*> m_mapArrayDataItem;
	CArray<CMemManager*,CMemManager*> m_arrayMemManager;	
	CSTDataItem* m_parrayDataItemExchangerForChild[MAXCNTOFINPUTEXCHANGER];
	CSTDataItem* m_parrayDataItemInputHolder[MAXCNTOFINPUTHOLDER];
	CSTDataItem* m_parrayDataItemVarHolder[MAXCNTOFVARIABLEHOLDER];
	CSTDataItem* m_parrayDataItemArrayHolder[MAXCNTOFARRAYHOLDER];
	CSTDataItem* m_parrayDataItemFuncDataHolder;
	double m_dBuffer[MAXCNTOFMEMBUFFER];

	CMap<CString, LPCSTR, CSubLoader*, CSubLoader*> m_mapSubLoader;
	CSubLoader* m_parraySubLoaderHolder[MAXCNTOFSUBFUNCHOLDER];
	CInnerFunction* m_parrayInnerFuncHolder[MAXCNTOFINNFUNCHOLDER];
	CString  m_strSubFuncPath;
	//LPCTSTR  m_lpSubFuncPath;
	long m_lOffsetSubLoaderHolder;
	BOOL m_bIsFirst;
	long m_lOffset;

	// TempBuffer Manager
	CTempMemMng m_RootTempMenMng;
	CITempMemMng* m_pCurrentPosTempMenMng;

	// temp variable
	CSubLoader *m_pSubLoader;
	CMemManager* m_pMemManager;
	CSTDataItem *m_pDataItem;
	BOOL m_bExists;

	CString m_strError;

};

#endif // !defined(AFX_MEMMANAGER_H__A9D6F090_38B4_4893_9F70_468893316959__INCLUDED_)
