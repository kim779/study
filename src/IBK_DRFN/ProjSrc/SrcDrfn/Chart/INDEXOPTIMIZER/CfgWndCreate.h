#if !defined(AFX_CFGWINDOWCREATE_H_22843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
#define AFX_CFGWINDOWCREATE_H_22843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
// #include "./CfgWndCreate.h"
// CfgWndCreate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCFGWndCreate 

typedef struct {
	CString szName;
	int		nDataType;
	union {
		int		nValue;
		UINT	uValue;
		double	dValue;
	};
} CFGWndItem;

typedef CList<CFGWndItem*, CFGWndItem*> CFGWndItemList;

class CCFGScanInfo
{
public:
	CCFGScanInfo()
	{
	}

	CCFGScanInfo(LPCSTR szData)
	{
		char aMin[21]={0,},
			 aMax[21]={0,},
			 aDefStart[21]={0,},
			 aDefEnd[21]={0,},
			 aDefStep[21]={0,};

		m_szTilte.GetBuffer(MAX_PATH);
		sscanf(szData,"%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,]", m_szTilte,
												&m_nDataType,
												aMin,
												aMax,
												aDefStart,
												aDefEnd,
												aDefStep);
		m_szTilte.ReleaseBuffer();

		m_fMin		= atof(aMin);
		m_fMax		= atof(aMax);
		m_fDefStart	= atof(aDefStart);
		m_fDefEnd	= atof(aDefEnd);
		m_fDefStep	= atof(aDefStep);

		m_szMin		= aMin;
		m_szMax		= aMax;
		m_szDefStart= aDefStart;
		m_szDefEnd	= aDefEnd;
		m_szDefStep = aDefStep;
	}

	CString m_szTilte;
	int		m_nDataType;
	double	m_fMin,
			m_fMax,
			m_fDefStart,
			m_fDefEnd,
			m_fDefStep;
	CString m_szMin,
			m_szMax,
			m_szDefStart,
			m_szDefEnd,
			m_szDefStep;

};

typedef CList<CCFGScanInfo*,CCFGScanInfo*> CCFGScanInfoList;

class CCFGSectionInfo
{
public:
	~CCFGSectionInfo()
	{
		DeleteAllItems();
	}

	void DeleteAllItems()
	{
		for(POSITION pos=m_ItemInfoList.GetHeadPosition(); pos;) 
			delete m_ItemInfoList.GetNext(pos);
		m_ItemInfoList.RemoveAll();
	}

	int GetCount()
	{
		return m_ItemInfoList.GetCount();
	}

public:
	CString				m_szTilte;
	CCFGScanInfoList	m_ItemInfoList;

};

class CCFGWndCreate
{
public:
	CCFGSectionInfo	m_ItemSectionInfo;

	CCFGWndCreate() 
	{
	}

	~CCFGWndCreate() 
	{
		DeleteAllItems();
	}

	int GetCount()
	{
		return m_ItemSectionInfo.GetCount();
	}

	void Reset()
	{
		DeleteAllItems();
	}

	void DeleteAllItems()
	{
		m_ItemSectionInfo.DeleteAllItems();
	}

	int CreateCtrl(LPCSTR szCFGName, LPCSTR szSection, CWnd* pParent, UINT nID)
	{
		int nRet = ReadCFG(szCFGName, szSection);

		return nRet;
	}

	int ReadCFG(LPCSTR szCFGName, LPCSTR szSection)
	{
		int nCount = ::GetPrivateProfileInt(szSection, "Count", 0, szCFGName);
		char aTmp[MAX_PATH]={0,};

		// 타이틀이 없으면 섹션키를 디폴트로 처리한다.
		::GetPrivateProfileString(szSection, "Title", szSection, aTmp, MAX_PATH, szCFGName);
		m_ItemSectionInfo.m_szTilte = aTmp;
		CString szSub;

		for(int i=0; i<nCount; i++)
		{
			szSub.Format("%d", i+1);
			int nSize = ::GetPrivateProfileString(szSection, szSub, "", aTmp, MAX_PATH, szCFGName);
			if(nSize>0)
			{
				CCFGScanInfo* pInfo = new CCFGScanInfo(aTmp);
				m_ItemSectionInfo.m_ItemInfoList.AddTail(pInfo);
			}
		}

		return nCount;
	}

	BOOL GetInputData(CFGWndItemList& list)
	{
		return TRUE;
	}

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFGWINDOWCREATE_H_22843D_469D_45B4_8BEB_6DC808FE3CA7__INCLUDED_)
