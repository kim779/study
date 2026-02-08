#if !defined(AFX_DRAGNDROPDEFINE_H__0ED0F7C0_1C28_4C38_89E7_C86FEE5FAC73__INCLUDED_)
#define AFX_DRAGNDROPDEFINE_H__0ED0F7C0_1C28_4C38_89E7_C86FEE5FAC73__INCLUDED_

const UINT RMSG_DRAGSTART			= ::RegisterWindowMessage(_T("RMSG_DRAGSTART"));
const UINT RMSG_DRAGEND				= ::RegisterWindowMessage(_T("RMSG_DRAGEND"));

class CDragNDrop
{
public:
	CDragNDrop()
	{
		m_szKey = NULL;
		m_szValue = NULL;
		m_ptCursor = NULL;
		m_bDelete = TRUE;
	}

	~CDragNDrop()
	{
		if(m_szKey)
		{
			delete [] m_szKey;
			m_szKey = NULL;
		}

		if(m_szValue)
		{
			delete [] m_szValue;
			m_szValue = NULL;
		}

		if(m_ptCursor)
		{
			delete m_ptCursor;
			m_ptCursor = NULL;
		}
	}

	// DND 데이터 세팅
	void SetData(LPPOINT ptCursor, HWND hWnd, LPTSTR szKey, LPTSTR szValue, BOOL bDelete = TRUE)
	{
		if(m_ptCursor)
			delete m_ptCursor;

		m_ptCursor = new POINT;
		m_ptCursor->x = ptCursor->x;
		m_ptCursor->y = ptCursor->y;

		m_hWndSource = hWnd;

		if(m_szKey)
			delete [] m_szKey;

		m_szKey = new char[::strlen(szKey) + 1];
		::strcpy(m_szKey, szKey);

		if(m_szValue)
			delete [] m_szValue;

		m_szValue = new char[::strlen(szValue) + 1];
		::strcpy(m_szValue, szValue);

		m_bDelete = bDelete;
	}

	// 드랍시점에서 메인이 삭제 할 것인지
	BOOL IsDelete()
	{
		return m_bDelete;
	}

	// 공유키
	LPTSTR GetKey()
	{
		return m_szKey;
	}

	// 공유값
	LPTSTR GetValue()
	{
		return m_szValue;
	}

	// 드래그가 시작된 핸들
	HWND GetHWndStart()
	{
		return m_hWndSource;
	}

	// 마우스 커서 위치
	LPPOINT GetCursorPoint()
	{
		return m_ptCursor;
	}

protected:
	LPPOINT m_ptCursor;				// 커서위치
	HWND	m_hWndSource;			// 시작시점(핸들)
	LPTSTR	m_szKey;				// 공유키
	LPTSTR	m_szValue;				// 공유값
	BOOL	m_bDelete;				// 받은곳에서 메모리 삭제할지 여부
};

#endif // !defined(AFX_DRAGNDROPDEFINE_H__0ED0F7C0_1C28_4C38_89E7_C86FEE5FAC73__INCLUDED_)
