#pragma once

class CCxPrice : public CWnd
{
public:
	CCxPrice();
	virtual ~CCxPrice();
	
	//{{AFX_VIRTUAL(CCxPrice)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCxPrice)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void BtnClick();
	void SetValues(CString str);
	void CreatePrice();

public:
	CString		m_sRoot, m_rtnStr;
	HINSTANCE	m_hPriceDll{};
	CWnd* m_pParent{};
	CWnd* m_pPrice{};
};

