#pragma once
#include "CXedit.h"
#include "NoteDefine.h"

#define	XCOM_HEIGHT	18
#define	CTRL_SCREEN	1
#define	CTRL_CODE	2

class CSedit : public CWnd
{
	DECLARE_DYNAMIC(CSedit)

public:
	CSedit(){};
	CSedit(UINT id);
	virtual ~CSedit();

	std::unique_ptr<class CXedit> m_edit;

	void	init(bool (*callback)(int, WPARAM, LPARAM));
	bool	(*m_sticker_call)(int, WPARAM, LPARAM);

	void changepos();
protected:
	UINT		m_id{};
	CRect		m_bRc{}, m_RcGlass{};
	std::unique_ptr<CPen>m_pen{};
	CBrush		m_brush{};
	CBitmap* m_arr{};
	CBitmap* m_arr_dn{};
	CBitmap* m_bmpXCombBack{};
	bool		m_push{};
	//

	CFont	m_font;

	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


