#if !defined(AFX_SOCKCTL_H__4CF268A1_0011_4986_B212_5B6ADDAE2043__INCLUDED_)
#define AFX_SOCKCTL_H__4CF268A1_0011_4986_B212_5B6ADDAE2043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SockCtl.h : Declaration of the CSockCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CSockCtrl : See SockCtl.cpp for implementation.

class CSockCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSockCtrl)

// Constructor
public:
	CSockCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSockCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

private:
	class CDgram*	m_dgram;
	class CStream*	m_stream;
	class CWsh*	m_rsm;
	class CWsh*	m_wsh;

// Implementation
protected:
	~CSockCtrl();

	DECLARE_OLECREATE_EX(CSockCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSockCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSockCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSockCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSockCtrl)
	afx_msg LRESULT OnFireEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireAlert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFirePush(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireAxis(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireStat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireWrite(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireVersion(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSockCtrl)
	afx_msg BOOL OpenSock(LPCTSTR home, LPCTSTR ip, long port);
	afx_msg void CloseSock();
	afx_msg BOOL WriteStream(LONG_PTR pBytes, long nBytes);
	afx_msg BOOL OpenDgram(long port);
	afx_msg void CloseDgram();
	afx_msg BOOL WriteDgram(LPCTSTR ip, LONG_PTR pBytes, long nBytes, BOOL route);
	afx_msg BOOL StartAxis(LONG_PTR key, long mode);
	afx_msg long GetEnum(LONG_PTR pBytes, long nBytes, long mode);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CSockCtrl)
	void OnStream(long type, LONG_PTR pBytes, long nBytes)
		{FireEvent(eventidOnStream,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), type, pBytes, nBytes);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSockCtrl)
	dispidOpenSock = 1L,
	dispidCloseSock = 2L,
	dispidWriteStream = 3L,
	dispidOpenDgram = 4L,
	dispidCloseDgram = 5L,
	dispidWriteDgram = 6L,
	dispidStartAxis = 7L,
	dispidGetEnum = 8L,
	eventidOnStream = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKCTL_H__4CF268A1_0011_4986_B212_5B6ADDAE2043__INCLUDED)

