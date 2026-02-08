#pragma once
// ControlWnd.h : header file
//

#include <afxmt.h>

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{

public:
	CControlWnd();
	virtual ~CControlWnd();

public:
	CWnd*		m_pParent;
	CParam		m_Param;

private:
	CCriticalSection	m_cs;
	bool			m_bFilter;

	std::unique_ptr<class CShMemory>	m_ShMemory;
	CMapStringToString	m_CodeMap;
	CMapStringToString	m_AccnMap;
	CMapStringToString	m_AccnFilter;		// 계좌별 키관리
	CString			m_sCtrlData;
	bool			m_bAllAccn;
	bool			m_bFuture;
	CUIntArray		m_arField;

public:
	void	SetParam(struct _param *pParam);

private:
	bool	m_bLaw;

	CString OptionParser(CString sOption, CString sKey);
	CString	Variant(int comm, CString data);
	CString	getKey(CString codeS, CString dateS, CString sygbS, CString jggb);
	void	getDate(const CStringArray& arData, CString& dateS, CString& sygbS, CString& jggb);
	void	SendToMap(CString sData, bool bAll, CString sAccn = "");
	CString Parser(CString &srcstr, CString substr);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	CString m_setAccount;
	afx_msg void OnSetAccountChanged();
	CString m_flag;
	afx_msg void OnFlagChanged();
	CString m_dataMsg;
	afx_msg void OnDataMsgChanged();
	CString m_dataList;
	afx_msg void OnDataListChanged();
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR sProperties);
	afx_msg void Send(LPCTSTR sAccn, LPCTSTR sPswd ,LPCTSTR dfee,LPCTSTR dmass);
	afx_msg void SendEx(LPCTSTR sAccn, LPCTSTR sPswd ,LPCTSTR dfee,LPCTSTR dmass,LPCTSTR dCalcType=_T("0"));
	afx_msg void AllAccnSend(LPCTSTR sUserID, LPCTSTR sPswd, LPCTSTR sCode);
	afx_msg void Clear();
	afx_msg void AddReminData(LPCTSTR sAccn, LPCTSTR sCode, LPCTSTR sRowData);
	afx_msg void SendX(LPCTSTR sAccn, LPCTSTR sPswd);
	afx_msg void SendRaw(LPCTSTR sName);
	afx_msg void SetFilterAcc(LPCTSTR sAccList);

	//}}AFX_DISPATCH
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnRemainMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
