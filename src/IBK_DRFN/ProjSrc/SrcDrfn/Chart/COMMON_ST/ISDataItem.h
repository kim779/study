// ISDataItem.h: interface for the CISDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISDATAITEM_H__8A87074B_2D58_4AE0_8ACF_C721B325514D__INCLUDED_)
#define AFX_ISDATAITEM_H__8A87074B_2D58_4AE0_8ACF_C721B325514D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define CISDataItem	CISDataItem0001

class CISDataItem0001
{
public:
	virtual ~CISDataItem0001() {}
	virtual BOOL Init(LPCTSTR lpName,LPCTSTR lpData,long lCntOfAllData,CISDataItem *pDataItemForConnection =NULL) = 0;
	virtual BOOL SetAt(long lIndex, LPCTSTR lpValue) = 0;
	virtual BOOL GetAt(long lIndex, CString *pStrValue) = 0;
	virtual CString GetAt(long lIndex) = 0;
	virtual int Add(LPCTSTR lpValue) = 0;
	virtual CString GetDataItemName() = 0;
	virtual long GetSize() = 0;
	virtual void SetSize(long lSize) = 0;
	virtual void SetType(char chType) = 0;
	virtual char GetType() = 0;
};

#endif // !defined(AFX_ISDATAITEM_H__8A87074B_2D58_4AE0_8ACF_C721B325514D__INCLUDED_)
