// DataItem.h: interface for the CSTDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAITEM_H__A3204D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_)
#define AFX_DATAITEM_H__A3204D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_
#include <afxtempl.h>
#include <math.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/IDataItem.h"
class CSTDataItem  : public CIDataItem
{
public:
	CSTDataItem();
	virtual ~CSTDataItem();
	BOOL Init(LPCTSTR lpName,int nType,double dInitValue,long lCntOfAllData,CIDataItem *pDataItemForConnection =NULL);
	BOOL SetAt(long lIndex, double dValue)
	{
		if(m_pDataItemConnected) return FALSE;
		if(dValue==HUGE_VAL||dValue==-HUGE_VAL)
		{
			if(m_lFirstIndex == lIndex)
			{
				m_lFirstIndex = lIndex + 1;
				m_bSetValue = FALSE;
			}
			else if(m_bSetValue)
			{
				dValue = 0.;
			}
		}
		else
		{
			if(!m_bSetValue)
			{
				m_lFirstIndex = lIndex;
				m_bSetValue = TRUE;
			}			
		}
		m_arrayData.SetAt(lIndex,dValue);
		return TRUE;
	}
	BOOL GetAt(long lIndex, double* pdValue)
	{
		if(m_pDataItemConnected) 
		{
			return m_pDataItemConnected->GetAt(lIndex,pdValue);
		}
		*pdValue = m_arrayData.GetAt(lIndex);
		return TRUE;
	}
	double GetAt(long lIndex)
	{
		if(m_pDataItemConnected) 
		{
			return m_pDataItemConnected->GetAt(lIndex);
		}
		return m_arrayData.GetAt(lIndex);
	}
	CString GetDataItemName();
	long GetSize();
	void SetSize(long lSize) {	m_arrayData.SetSize(lSize);	}
	void SetComment(LPCTSTR lpTitle,LPCTSTR lpComment);
	CString GetComment(LPCTSTR lpTitle);
	long GetFirstDataIndex();

	int Add(double dValue)
	{
		return m_arrayData.Add(dValue);
	}

//private:
public:
	CIDataItem* m_pDataItemConnected;
	CString m_strName;
	int		m_nType;
	double  m_dInitValue;
	CArray<double,double> m_arrayData;
	CMapStringToString m_mapComment;
	CString m_strComment;
	long m_lFirstIndex;
	BOOL m_bSetValue;

};

#endif // !defined(AFX_DATAITEM_H__A3204D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_)
