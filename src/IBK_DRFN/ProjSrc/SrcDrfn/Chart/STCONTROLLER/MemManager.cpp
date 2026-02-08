// MemManager.cpp: implementation of the CMemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemManager.h"
#include "../Common_ST/DataItem.h"
#include "SemiMemManager.h"
#include "../Include_ST/SubLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMemManager::CMemManager(CMemManager* pParent /*= NULL*/)
{
	m_pParent = pParent;
	m_lSubInPos = 0;

	m_pMemManager = NULL;
	m_pDataItem = NULL;
	m_pSubLoader = NULL;
	m_bExists = FALSE;

	m_lOffsetSubLoaderHolder = 0;
	m_bIsFirst = TRUE;
	m_lOffset = 0;	
	m_pCurrentPosTempMenMng = NULL;

	memset(m_parrayDataItemExchangerForChild,0x00,sizeof(m_parrayDataItemExchangerForChild));
	memset(m_parrayDataItemInputHolder,0x00,sizeof(m_parrayDataItemInputHolder));
	memset(m_parrayDataItemVarHolder,0x00,sizeof(m_parrayDataItemVarHolder));
	memset(m_parraySubLoaderHolder,0x00,sizeof(m_parraySubLoaderHolder));
	memset(m_parrayInnerFuncHolder,0x00,sizeof(m_parrayInnerFuncHolder));
	m_parrayDataItemFuncDataHolder = NULL;
	
	m_arrayMemManager.RemoveAll();
}

CMemManager::~CMemManager()
{
	CString strName;
	CSTDataItem *pDataItem = NULL;
	POSITION pos = m_mapInputDataItem.GetStartPosition();
	while(pos != NULL)
	{		
		try
		{
			m_mapInputDataItem.GetNextAssoc(pos, strName, pDataItem);
			delete pDataItem;
			pDataItem = NULL;
		}
		catch(...)
		{
			return;
		}
	}
	m_mapInputDataItem.RemoveAll();

	pos = m_mapVarDataItem.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapVarDataItem.GetNextAssoc(pos, strName, pDataItem);
		delete pDataItem;
		pDataItem = NULL;
	}
	m_mapVarDataItem.RemoveAll();

	CSubLoader *m_pSubLoader = NULL;
	pos = m_mapSubLoader.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapSubLoader.GetNextAssoc(pos, strName, m_pSubLoader);
		delete m_pSubLoader;
		m_pSubLoader = NULL;
	}
	m_mapSubLoader.RemoveAll();
	
	CMemManager* pChildMemManager = NULL;
	int nCnt = m_arrayMemManager.GetSize();
	for(int nPos=0;nPos<nCnt;nPos++)
	{
		if(pChildMemManager = m_arrayMemManager.GetAt(nPos))
		{
			delete pChildMemManager;
		}
		pChildMemManager = NULL;
	}
	m_arrayMemManager.RemoveAll();
}


BOOL CMemManager::SetInputs(long lPos, long lIndex, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData)
{
	CString strName = lpName;
	BOOL	bRetValue = FALSE;
	BOOL bAlreadyExists = FALSE;
	CSTDataItem *pDataItem = NULL;
	if(strName=="[SubInputs]")
	{
		CString strSubName;
		strSubName.Format("%s[%d,%d]",strName,m_lSubInPos,lIndex);
		bAlreadyExists = m_mapInputDataItem.Lookup(strSubName,pDataItem);
		if(!bAlreadyExists)
		{
			pDataItem = new CSTDataItem;
			if(pDataItem->Init(strSubName,nType,dInitValue,lCntOfAllData))
			{
				pDataItem->SetAt(lPos,dInitValue);
				m_mapInputDataItem.SetAt(strSubName,pDataItem);
				m_parrayDataItemExchangerForChild[lIndex] = pDataItem;
				bRetValue = TRUE;
			}
			else	// Initial Error..
			{
				delete pDataItem;
			}
		}
		else
		{
			pDataItem->SetAt(lPos,dInitValue);
			bRetValue = TRUE;
		}
	}
	else
	{
		BOOL bAlreadyExists = m_mapInputDataItem.Lookup(lpName,pDataItem);
		if(!bAlreadyExists&&m_pParent)
		{
			CSTDataItem *pDataItem = new CSTDataItem;
			CSTDataItem *pDataItemOfParent = m_pParent->m_parrayDataItemExchangerForChild[lIndex];
			if(pDataItem->Init(lpName,nType,dInitValue,lCntOfAllData,pDataItemOfParent))
			{
				m_mapInputDataItem.SetAt(lpName,pDataItem);
			}				
		}
		else
		{
			if(!bAlreadyExists)
			{
				pDataItem = new CSTDataItem;			
				if(pDataItem->Init(lpName,nType,dInitValue,lCntOfAllData))
				{
					m_mapInputDataItem.SetAt(lpName,pDataItem);
					bRetValue = TRUE;
				}
				else	// Initial Error..
				{
					delete pDataItem;
				}
			}
			else
			{
				pDataItem->SetAt(lPos,dInitValue);
				bRetValue = TRUE;
			}
		}
	}
	return bRetValue;
}

BOOL CMemManager::SetVariables(long lPos, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData)
{
	m_bExists = m_mapVarDataItem.Lookup(lpName,m_pDataItem);
	if(!m_bExists)
	{
		m_pDataItem = new CSTDataItem;			
		if(m_pDataItem->Init(lpName,nType,dInitValue,lCntOfAllData))
		{
			m_pDataItem->SetAt(lPos,dInitValue);
			m_mapVarDataItem.SetAt(lpName,m_pDataItem);
		}
		else	// Initial Error..
		{
			delete m_pDataItem;
			m_pDataItem = NULL;
			return FALSE;
		}
	}
	else
	{
		m_pDataItem->SetAt(lPos,dInitValue);
	}
	return TRUE;
}

BOOL CMemManager::SetArray(long lPos, LPCTSTR lpName, int nType, double dInitValue, long lCntOfAllData)
{
	m_bExists = m_mapArrayDataItem.Lookup(lpName,m_pDataItem);
	if(!m_bExists)
	{
		m_pDataItem = new CSTDataItem;			
		if(m_pDataItem->Init(lpName,nType,dInitValue,lCntOfAllData))
		{
			m_pDataItem->SetAt(lPos,dInitValue);
			m_mapArrayDataItem.SetAt(lpName,m_pDataItem);
		}
		else	// Initial Error..
		{
			delete m_pDataItem;
			m_pDataItem = NULL;
			return FALSE;
		}
	}
	else
	{
		m_pDataItem->SetAt(lPos,dInitValue);
	}
	return TRUE;
}

CMemManager* CMemManager::SubIn(long lPos, BOOL bSemi )
{

	//20081111 이문수 >>
	//return NULL;
	//20081111 이문수 <<

	m_pMemManager = NULL;
	if(m_arrayMemManager.GetSize()<=m_lSubInPos)
	{
		if(bSemi)
		{
			m_pMemManager = new CSemiMemManager(this);
		}
		else
		{
			m_pMemManager = new CMemManager(this);
		}
		m_arrayMemManager.Add(m_pMemManager);
	}
	else
	{
		m_pMemManager = m_arrayMemManager.GetAt(m_lSubInPos);		
	}
	m_pMemManager->m_pCurrentPosTempMenMng = &m_pMemManager->m_RootTempMenMng;
	return m_pMemManager;
}

CMemManager* CMemManager::SubOut(long lPos)
{	
	m_bIsFirst = FALSE;
	m_pMemManager = this->m_pParent;
	m_pMemManager->m_lSubInPos++;
	return m_pMemManager;
}

BOOL CMemManager::Next()
{
	int nCnt = m_arrayMemManager.GetSize();
	for(int nPos=0;nPos<nCnt;nPos++)
	{
		m_pMemManager = m_arrayMemManager.GetAt(nPos);
		m_pMemManager->Next();
	}
	m_lSubInPos = 0;	
	return TRUE;
}

inline BOOL CMemManager::GetInputData(LPCTSTR lpName, double dIndex, double* pdValue)
{
	m_bExists = m_mapInputDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		return m_pDataItem->GetAt((long)dIndex,pdValue);
	}
	return FALSE;	
}

inline BOOL CMemManager::SetInputData(LPCTSTR lpName, double dIndex, double dValue)
{
	m_bExists = m_mapInputDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		return m_pDataItem->SetAt((long)dIndex,dValue);
	}
	return FALSE;	
}

inline BOOL CMemManager::GetVarData(LPCTSTR lpName, double dIndex, double* pdValue)
{
	m_bExists = m_mapVarDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		return m_pDataItem->GetAt((long)dIndex,pdValue);
	}
	return FALSE;	
}

inline BOOL CMemManager::SetVarData(LPCTSTR lpName, double dIndex, double dValue)
{
	m_bExists = m_mapVarDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		return m_pDataItem->SetAt((long)dIndex,dValue);
	}
	return FALSE;	
}

inline BOOL CMemManager::GetVarHolderData(long lHolderIndex, double dIndex, double* pdValue)
{
	return m_parrayDataItemVarHolder[lHolderIndex]->GetAt((long)dIndex,pdValue);
}

inline double CMemManager::GetVarHolderData(long lHolderIndex, double dIndex)
{
	double dRetValue = 0;
	m_parrayDataItemVarHolder[lHolderIndex]->GetAt((long)dIndex,&dRetValue);
	return dRetValue;
}

inline BOOL CMemManager::SetVarHolderData(long lHolderIndex, double dIndex, double dValue)
{
	return m_parrayDataItemVarHolder[lHolderIndex]->SetAt((long)dIndex,dValue);
}

inline BOOL CMemManager::GetInputHolderData(long lHolderIndex, double dIndex, double* pdValue)
{
	return m_parrayDataItemInputHolder[lHolderIndex]->GetAt((long)dIndex,pdValue);
}

inline BOOL CMemManager::SetInputHolderData(long lHolderIndex, double dIndex, double dValue)
{
	return m_parrayDataItemInputHolder[lHolderIndex]->SetAt((long)dIndex,dValue);
}

inline BOOL CMemManager::GetFuncData(double dIndex, double* pdValue)
{
	return m_parrayDataItemFuncDataHolder->GetAt((long)dIndex,pdValue);
}

inline BOOL CMemManager::SetFuncData(double dIndex, double dValue)
{
	return m_parrayDataItemFuncDataHolder->SetAt((long)dIndex,dValue);
}

BOOL CMemManager::InitFuncData(long lCntOfAllData)
{
	if(SetVariables(0,"[FunctionData]",NumericSeries,0,lCntOfAllData))
	{
		m_mapVarDataItem.Lookup("[FunctionData]",m_pDataItem);		
		m_parrayDataItemFuncDataHolder = m_pDataItem;
		return TRUE;
	}
	else
	{
		m_parrayDataItemFuncDataHolder = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL CMemManager::SetVarHolder(long lHolderIndex,LPCTSTR lpName)
{
	if(lHolderIndex>=MAXCNTOFVARIABLEHOLDER) 
	{
		m_strError.Format("변수갯수가 허용치를 초과하였습니다.");
		return FALSE;
	}
	m_bExists = m_mapVarDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		m_parrayDataItemVarHolder[lHolderIndex] = m_pDataItem;
		return TRUE;
	}
	return FALSE;
}

long CMemManager::GetInputHolder(long lHolderIndex)
{
	return (long)m_parrayDataItemInputHolder[lHolderIndex];
}

BOOL CMemManager::SetInputHolder(long lHolderIndex,LPCTSTR lpName)
{
	if(lHolderIndex>=MAXCNTOFINPUTHOLDER) 
	{
		m_strError.Format("변수갯수가 %d개를 초과하였습니다.",MAXCNTOFINPUTHOLDER);
		return FALSE;
	}
	m_bExists = m_mapInputDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		m_parrayDataItemInputHolder[lHolderIndex] = m_pDataItem;
		return TRUE;
	}
	return FALSE;
}

long CMemManager::GetVarPointer(LPCTSTR lpName)
{
	m_bExists = m_mapVarDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		return (long)m_pDataItem;
	}
	return NULL;
}
long CMemManager::GetInputPointer(LPCTSTR lpName)
{
	m_bExists = m_mapInputDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		return (long)m_pDataItem;
	}
	return NULL;
}

void CMemManager::SetSizeDataItem(long lSize)
{
	CString strName;
	CSTDataItem *pDataItem = NULL;

	long lCntOfCurrentPacketData;
	long lNewCntOfPacketTimeData = lSize;
	long lCntUpdate;

	//POSITION pos = m_mapInputDataItem.GetStartPosition();
	//while(pos != NULL)
	//{		
	//	m_mapInputDataItem.GetNextAssoc(pos, strName, pDataItem);
	//	pDataItem->SetSize(lSize);
	//}

	//pos = m_mapVarDataItem.GetStartPosition();
	//while(pos != NULL)
	//{		
	//	m_mapVarDataItem.GetNextAssoc(pos, strName, pDataItem);
	//	pDataItem->SetSize(lSize);
	//}

	//CMemManager* pChildMemManager = NULL;
	//int nCnt = m_arrayMemManager.GetSize();
	//for(int nPos=0;nPos<nCnt;nPos++)
	//{
	//	pChildMemManager = m_arrayMemManager.GetAt(nPos);
	//	pChildMemManager->SetSizeDataItem(lSize);		
	//}

	POSITION pos = m_mapInputDataItem.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapInputDataItem.GetNextAssoc(pos, strName, pDataItem);
		lCntOfCurrentPacketData = pDataItem->GetSize();
		lCntUpdate = lNewCntOfPacketTimeData - lCntOfCurrentPacketData;
		if(lCntUpdate<0)
		{
			// 차트ocx에서 50개가 삭제되었으므로 똑같이 50개를 지운다.
			int nReverseCnt = ( lCntUpdate * (-1) ) + 1;
			pDataItem->m_arrayData.RemoveAt(0, nReverseCnt);

			// 리얼로 들어온 한 개를 위해 추가한다.
			pDataItem->SetSize(lSize);
		}
		else
			pDataItem->SetSize(lSize);
	}

	pos = m_mapVarDataItem.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapVarDataItem.GetNextAssoc(pos, strName, pDataItem);
		lCntOfCurrentPacketData = pDataItem->GetSize();
		lCntUpdate = lNewCntOfPacketTimeData - lCntOfCurrentPacketData;
		if(lCntUpdate<0)
		{
			// 차트ocx에서 50개가 삭제되었으므로 똑같이 50개를 지운다.
			int nReverseCnt = ( lCntUpdate * (-1) ) + 1;
			pDataItem->m_arrayData.RemoveAt(0, nReverseCnt);

			// 리얼로 들어온 한 개를 위해 추가한다.
			pDataItem->SetSize(lSize);
		}
		else
			pDataItem->SetSize(lSize);
	}

	CMemManager* pChildMemManager = NULL;
	int nCnt = m_arrayMemManager.GetSize();
	for(int nPos=0;nPos<nCnt;nPos++)
	{
		pChildMemManager = m_arrayMemManager.GetAt(nPos);
		pChildMemManager->SetSizeDataItem(lSize);		
	}
}

void CMemManager::SetSubFuncPath(LPCTSTR lpSubFunc)
{

	m_strSubFuncPath = lpSubFunc;
}

CString CMemManager::GetSubFuncPath()
{
	if(m_strSubFuncPath.GetLength())
	{
		return m_strSubFuncPath;
	}
	else if(m_pParent)
	{
		return m_pParent->GetSubFuncPath();
	}
	return "";	
}

double CMemManager::SubFunc(long lSTActor, long lIndexFunc, long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 )
{	
	return m_parraySubLoaderHolder[lIndexFunc]->SubInFuncST(lSTActor,lIndex, bTemp0, bTemp1, bTemp2,  bTemp3 , bTemp4 , bTemp5 ,bTemp6 , bTemp7 ,bTemp8 , bTemp9 );	
}

BOOL CMemManager::IncludeSubFunc(LPCTSTR lpFuncName)
{
	m_bExists = m_mapSubLoader.Lookup(lpFuncName,m_pSubLoader);
	if(!m_bExists)
	{
		m_pSubLoader = new CSubLoader;	
		CString strSubFuncFullPath = GetSubFuncPath();
		strSubFuncFullPath +=  "\\";
		strSubFuncFullPath +=  lpFuncName;
		strSubFuncFullPath +=  ".sdf";
		BOOL bRetValue = m_pSubLoader->LoadDLL(strSubFuncFullPath);
		if(!bRetValue)
		{
			m_strError = m_pSubLoader->GetErrorMessage();
			delete m_pSubLoader;
			m_pSubLoader = NULL;
			return FALSE;			
		}
		m_mapSubLoader.SetAt(lpFuncName,m_pSubLoader);		
	}
	return TRUE;	
}

BOOL CMemManager::SetSubFuncHolder(long lHolderIndex,LPCTSTR lpName)
{
	if(lHolderIndex>=MAXCNTOFSUBFUNCHOLDER) 
	{
		m_strError.Format("사용자함수 사용 갯수가 기준을 초과하였습니다.");
		return FALSE;
	}
	m_bExists = m_mapSubLoader.Lookup(lpName,m_pSubLoader);		
	if(m_bExists)
	{
		m_parraySubLoaderHolder[lHolderIndex] = m_pSubLoader;
		return TRUE;
	}
	return FALSE;
}

void CMemManager::SetValueInBuffer(long lIndex,double dValue)
{
	m_dBuffer[lIndex] = dValue;
}

double CMemManager::GetValueInBuffer(long lIndex)
{
	return m_dBuffer[lIndex];
}

BOOL CMemManager::IsFirst()
{
	return m_bIsFirst;
}

void CMemManager::SetCheckNotFirst()
{
	m_bIsFirst = FALSE;
}
BOOL CMemManager::SetInnerFuncHolder(long lHolderIndex,CInnerFunction *pInnerFunc)
{
	m_parrayInnerFuncHolder[lHolderIndex] = pInnerFunc;
	return TRUE;
}

CInnerFunction *CMemManager::GetInnerFuncFromHolder(long lHolderIndex)
{
	return m_parrayInnerFuncHolder[lHolderIndex];
}


BOOL CMemManager::GetArrayHolderData(long lHolderIndex, double dIndex, double* pdValue)
{
	return m_parrayDataItemArrayHolder[lHolderIndex]->GetAt((long)dIndex,pdValue);
}

BOOL CMemManager::SetArrayHolderData(long lHolderIndex, double dIndex, double dValue)
{
	return m_parrayDataItemArrayHolder[lHolderIndex]->SetAt((long)dIndex,dValue);
}

BOOL CMemManager::SetArrayHolder(long lHolderIndex,LPCTSTR lpName)
{
	if(lHolderIndex>=MAXCNTOFARRAYHOLDER) 
	{
		m_strError.Format("Array갯수가 최대기준을 초과하였습니다.");
		return FALSE;
	}
	m_bExists = m_mapArrayDataItem.Lookup(lpName,m_pDataItem);		
	if(m_bExists)
	{
		m_parrayDataItemArrayHolder[lHolderIndex] = m_pDataItem;
		return TRUE;
	}
	return FALSE;

}

void CMemManager::SetOffset(long lOffset)
{
	CMemManager *pMemManager = NULL;
	pMemManager = this->m_pParent;
	m_lOffset = 0;
	while(TRUE)
	{
		if(pMemManager)
		{
			m_lOffset += pMemManager->GetOffset();
			pMemManager = pMemManager->m_pParent;
		}
		else
		{
			break;
		}
	}
	m_lOffset += lOffset;
}

long CMemManager::GetOffset()
{
	return m_lOffset;
}


CSTDataItem* CMemManager::GetVarHolderPointer(long lHolderIndex)
{
	return m_parrayDataItemVarHolder[lHolderIndex];
}

CSTDataItem* CMemManager::GetArrayHolderPointer(long lHolderIndex)
{
	return m_parrayDataItemArrayHolder[lHolderIndex];
}

CITempMemMng *CMemManager::GetITempMemMng()
{
	CITempMemMng* pTempMemMng = m_pCurrentPosTempMenMng->GetSubTemp(0);
	m_pCurrentPosTempMenMng = pTempMemMng;
	return pTempMemMng;	
}

inline CSTDataItem* CMemManager::GetFuncHolderPointer()
{
	if(!m_parrayDataItemFuncDataHolder)
	{
		return m_pParent->GetFuncHolderPointer();
	}
	return m_parrayDataItemFuncDataHolder;
}

