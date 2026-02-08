#if !defined(AFX_SENDITEM_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_)
#define AFX_SENDITEM_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_

//#include "../../inc/"AUserCallback.h"
#include "../../inc/IAUGwanMng.h"

class CSendItem
{
public:
	CString m_szTR;
	long	m_pCallback;
	int		m_nKey,
			m_nLen;
	LPBYTE	m_pData;

public:
	void Copy(CSendItem* pItem)
	{
		m_szTR = pItem->m_szTR;
		m_pCallback = pItem->m_pCallback;
		m_nKey = pItem->m_nKey;
		m_nLen = pItem->m_nLen;
		m_pData = pItem->m_pData;
	}
};

typedef CList<CSendItem*, CSendItem*> List_CSendItem;

class CWinixGwanManager : public IWinixGwanManager
{
public:
	CWinixGwanManager() {m_pGwanMng=NULL; m_pGwandwRet=0;}
	void DoInit()
	{	m_pGwanMng = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager);	}
	
public:
	DECLARE_IWinixGwanManager_MAP()
};

// ->cpp에 아래의 루틴을 넣을 것.
/////////////////////////////////////////////////////////////////////////////
// CWinixGwanManager
// IMPLEMENT_IWinixGwanManager_MAP()
// <--cpp에 아래의 루틴을 넣을 것.

#endif	//AFX_SENDITEM_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_