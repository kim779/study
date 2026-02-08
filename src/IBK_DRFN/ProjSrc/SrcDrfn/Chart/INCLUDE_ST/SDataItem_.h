// SDataItem.h: interface for the CSDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDATAITEM_H__1F179072_37DE_44DE_9338_1C8F169CE5C2__INCLUDED_)
#define AFX_SDATAITEM_H__1F179072_37DE_44DE_9338_1C8F169CE5C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// string array item
#include "../Include_ST/ISDataItem.h"
class CSDataItem  : public CISDataItem
{
public:
	CSDataItem();
	virtual ~CSDataItem();
	BOOL Init(LPCTSTR lpName,LPCTSTR lpData,long lCntOfAllData,CISDataItem *pDataItemForConnection =NULL);
	BOOL SetAt(long lIndex, LPCTSTR lpValue)
	{
		if(m_pSDataItemConnected) return FALSE;
		m_arraySData.SetAt(lIndex,lpValue);
		return TRUE;
	}
	BOOL GetAt(long lIndex, CString *pStrValue)
	{
		if(m_pSDataItemConnected) 
		{
			return m_pSDataItemConnected->GetAt(lIndex,pStrValue);
		}
		*pStrValue = m_arraySData.GetAt(lIndex);
		return TRUE;
	}
	CString GetAt(long lIndex)
	{
		if(m_pSDataItemConnected) 
		{
			return m_pSDataItemConnected->GetAt(lIndex);
		}
		return m_arraySData.GetAt(lIndex);
	}
	int Add(LPCTSTR lpValue)
	{
		return m_arraySData.Add(lpValue);
	}
	CString GetDataItemName() { return m_strName;}
	long GetSize() { return m_arraySData.GetSize(); }
	void SetSize(long lSize);
	void SetType(char chType) { m_chType = chType;} 
	char GetType() { 
		return m_chType;}
//private:
//protected:
public:
	CISDataItem* m_pSDataItemConnected;
	CString m_strName;
	CStringArray m_arraySData;
	char m_chType;
};


class CSDataItemEx : public CSDataItem
{
public:
	void RemoveFront(int nCnt)
	{
		m_arraySData.RemoveAt(0, nCnt);
	}
};

#endif // !defined(AFX_SDATAITEM_H__1F179072_37DE_44DE_9338_1C8F169CE5C2__INCLUDED_)
