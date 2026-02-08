#ifndef __REALDATA_COUNTER__MANAGER__HEADER__
#define	__REALDATA_COUNTER__MANAGER__HEADER__

class CRealCounterItem
{
public:
	CRealCounterItem():m_nCount(0) {}

	CString m_szCode;
	int		m_nCount;
};
typedef CList<CRealCounterItem, CRealCounterItem&> List_CRealCounterItem;

class CRealCounterMng
{
public:
	CRealCounterMng();
	~CRealCounterMng();

	List_CRealCounterItem m_itemList;

	POSITION FindItem(LPCSTR szCode);
	int AddItem(LPCSTR szCode);
	int RemoveItem(LPCSTR szCode);
	void RemoveAll();
};

#endif //__REALDATA_COUNTER__MANAGER__HEADER__
