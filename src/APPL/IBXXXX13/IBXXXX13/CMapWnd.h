#pragma once


// CMapWnd
//#include "../../../CONTROL/fx_misc/fxImgButton.h"

class CMapWnd : public CWnd
{
	DECLARE_DYNAMIC(CMapWnd)

public:
	CMapWnd();
	virtual ~CMapWnd();
	CString m_slog;
	CStringArray m_arrGroup;

	HWND m_hMain{};
	CWnd* m_pParent{};
	class CDlg* m_pdlg;

	BOOL	CreateMap(CWnd* pParent);

	

	void OnBtnInterList();
	void OnBtnInterCode(int igroup);
	void OnBtnDBBackup();
	void OnBtnNewGroup(CString strGroupName);
	void OnBtnGroupSave(int igroup);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnBtnInterList();
	//afx_msg void OnBtnInterCode();

	LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
	//LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


