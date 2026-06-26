#pragma once

#include "DefineData.h"
#include "fxIGrid.h"

class CThreadWnd : public CWnd
{
	DECLARE_DYNAMIC(CThreadWnd)

public:
	CThreadWnd();
	virtual ~CThreadWnd();

	CfxIGrid* m_pGrid{};
	void initGrid();
	BOOL GetCIColumnData(CString colname, CIColumn& column);

	bool m_bThreadUSE{};
	COLORREF m_col;
	int  m_id;
	void endThread();
public:
	int m_icnt;
	CString m_strText;
	CString m_slog;
	CCriticalSection m_cs;
	CObArray	m_que;
	void Dispatch(char*& pdata, int ilen = 0);
	void MsgFromThread(CString str);
public:
	class CProducer_TD* m_tdProducer;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
