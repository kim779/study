// IDataItem.h: interface for the CIDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDATAITEM_H__34B12FF2_AEC5_4692_AC87_666C363CC990__INCLUDED_)
#define AFX_IDATAITEM_H__34B12FF2_AEC5_4692_AC87_666C363CC990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CIDataItem CIDataItem0001
class CIDataItem0001
{
public:
	virtual ~CIDataItem0001() {};
	virtual BOOL Init(LPCTSTR lpName,int nType,double dInitValue,long lCntOfAllData,CIDataItem0001 *pDataItemForConnection =NULL) = 0;
	virtual BOOL SetAt(long lIndex, double dValue) = 0;
	virtual BOOL GetAt(long lIndex, double* pdValue) = 0;
	virtual double GetAt(long lIndex) = 0;
	virtual CString GetDataItemName() = 0;
	virtual long GetSize() = 0;
	virtual void SetSize(long lSize) = 0;
	virtual void SetComment(LPCTSTR lpTitle,LPCTSTR lpComment) = 0;
	virtual CString GetComment(LPCTSTR lpTitle) = 0;
	virtual long GetFirstDataIndex() = 0;
};

#endif // !defined(AFX_IDATAITEM_H__34B12FF2_AEC5_4692_AC87_666C363CC990__INCLUDED_)
