#pragma once

class AFX_EXT_CLASS COrderWnd : public CWnd
{
public:
	COrderWnd(class CformProp* pProp, DWORD dwOrderKind);
	virtual ~COrderWnd();
public:
	DWORD		m_dwOrderKind;
	class CformProp* m_pProp;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};