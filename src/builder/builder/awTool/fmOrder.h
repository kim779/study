#pragma once

class CChildView;

class AFX_EXT_CLASS CfmOrder : public CWnd
{
public:
	CfmOrder() {};
	CfmOrder(CChildView* pView, CObArray* pAryProp);
	virtual ~CfmOrder();
public:
	CChildView*		m_pView;
	CObArray*		m_pAryProp;
	CPtrArray		m_AryWnd;
	int			m_curIdx;
	DWORD			m_dwOrderKind;
public:
	void DrawOrderNumber(DWORD dwOrderKind);
	void SortOrder(int iSelF, int iOldIdx);
	bool CheckOrder(CPoint point);
};