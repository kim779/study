#pragma once


// CMainWnd
#include "ShMemory.h"

class CMainWnd : public CWnd
{
	DECLARE_DYNAMIC(CMainWnd)

public:
	CMainWnd(CWnd* pParent, void* pParam);
	virtual ~CMainWnd();

	std::unique_ptr<char[]> m_hkey;
	std::unique_ptr<char[]> m_mkey;

	std::unique_ptr<class CShMemory> m_ShMemory;
	std::unique_ptr<class CShMemory> m_SmMemory;

	CWnd* m_pWizard{};
	CString m_slog{};
	CString m_sRoot{};
	CString m_sName{};
	void   FileMove();
	CString Variant(int comm, CString data);
	CString Parser(CString& srcstr, CString substr);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
};


